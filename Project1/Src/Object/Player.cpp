#include <string>
#include "../Application.h"
#include "../common/Flame.h"
#include "../Utility/AsoUtility.h"
#include "../Input/GamePad.h"
#include "../Input/Keybord.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/GravityManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"
#include "Common/Transform.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Stage.h"
#include "Player.h"

#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

Player::Player(Stage* stage) 
	: resourceManager_(ResourceManager::GetInstance()),
	gravityManager_(GravityManager::GetInstance())
{
	animationController_ = nullptr;
	capsule_ = nullptr;
	moveDiff_ = VECTOR();
	moveDir_ = VECTOR();
	movePow_ = VECTOR();
	movedPos_ = VECTOR();
	speed_ = 0.0f;			
	playerRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRot_ = 0.0f;			
	isJump_ = false;
	isDance_ = false;
	stepJump_ = 0.0f;		
	jumpPow_ = VECTOR();
	hitPos_ = VECTOR();
	slopeDir_ = VECTOR();
	slopeAngleDeg_ = 0.0f;	
	slopePow_ = VECTOR();
	hitNormal_ = VECTOR();
	imgShadow_ = -1;
	
	stage_ = stage;

}

Player::~Player(void)
{
}

void Player::Init(void)
{
	//コントローラーの情報取得
	if (GetJoypadNum())
	{
		//パッドが接続されていた時
		controller_ = std::make_unique<GamePad>();
	}
	else
	{
		controller_ = std::make_unique<Keybord>();
	}

	rectangular_ = std::make_unique<Rectangular>();

	//モデルの設定
	ModelInit();

	// アニメーションの設定
	AnimationInit();
}

void Player::Update(void)
{	
	controller_->Update();

	//行動制御
	BehaviorControll();

	MV1RefreshCollInfo(transform_.modelId, -1);

	transform_.Update();
	animationController_->Update();

}

void Player::Draw(void)
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);

	//デバッグ表示
	//DrawDebug();
}

void Player::Release(void)
{
	//モデルの解放
	MV1DeleteModel(transform_.modelId);
}

Transform* Player::GetTransform(void)
{
	//モデル基本情報の取得
	return &transform_;
}

void Player::DrawDebug(void)
{
	//デバッグ表示
	DrawFormatString(0, 38, 0xffffff, "プレイヤー座標:%f,%f,%f",
		transform_.pos.x, transform_.pos.y, transform_.pos.z);

	//AABB衝突判定用
	//DrawSphere3D(playerAABB_.max, 10.0f, 10, 0xff0000, 0xff0000, true);
	//DrawString(0, 0, "AABB衝突判定Max値", 0xff0000);
	//DrawSphere3D(playerAABB_.min, 10.0f, 10, 0x00ff00, 0x00ff00, true);
	//DrawString(0, 16, "AABB衝突判定Min値", 0x00ff00);

	//足元の衝突判定用球体
	DrawSphere3D(transform_.pos, COL_RADIUS, 20, 0x0000ff, 0x0000ff, true);

	//足元の衝突判定用線分
	DrawLine3D(gravityHitUp_, gravityHitDown_, 0x0000ff);

	//カプセルコライダの表示
	//capsule_->Draw();
}

void Player::ModelInit(void)
{
	//モデル情報を格納
	transform_.SetModel(
		resourceManager_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));

	//モデルの設定
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { 0.0f, -900.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	//回転の設定
	playerRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRot_ = 0.0f;

	//ジャンプ処理関連
	jumpPow_ = AsoUtility::VECTOR_ZERO;
	isJump_ = false;
	stepJump_ = 0.0f;

	//丸影画像
	imgShadow_ = LoadGraph((Application::PATH_IMAGE + "Shadow.png").c_str());

	//カプセルコライダ
	capsule_ = new Capsule(&transform_);
	capsule_->SetRelativePosTop({ 0.0f,110.0f,0.0f });
	capsule_->SetRelativePosDown({ 0.0f,30.0f,0.0f });
	capsule_->SetRadius(20.0f);

	//傾斜関連
	hitPos_ = AsoUtility::VECTOR_ZERO;
	slopeDir_ = AsoUtility::VECTOR_ZERO;
	slopeAngleDeg_ = 0.0f;
	slopePow_ = AsoUtility::VECTOR_ZERO;
	hitNormal_ = AsoUtility::VECTOR_ZERO;
}

void Player::AnimationInit(void)
{
	std::string path = Application::PATH_MODEL + "Player/";

	animationController_ = new AnimationController(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::WALK, path + "Walk.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::DANCE, path + "Dance.mv1", 20.0f);

	animationController_->Play((int)ANIM_TYPE::IDLE);
}

void Player::BehaviorControll(void)
{
	//移動
	ProcessMove();

	//傾斜計算
	CalcSlope();

	//重力による移動量を計算
	CalcGravityPow();

	//衝突判定と移動処理
	Collision();

	//回転
	transform_.quaRot = gravityManager_.GetTransform().quaRot;
	transform_.quaRot = transform_.quaRot.Mult(playerRotY_);

}

void Player::ProcessMove(void)
{
	//移動量ゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;

	//X軸回転を除いた、重力方向に垂直なカメラ角度（XZ平面）を取得
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	//回転したい角度
	float rotRad = 0.0f;

	//移動方向
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	auto contData = controller_->GetInputData();
	//移動方向・回転量を決める
	if (contData[NowFlame][static_cast<int>(InputID::FrontSide)])
	{
		dir = cameraRot.GetForward();
		rotRad = AsoUtility::Deg2RadF(0.0f);
	}
	if (contData[NowFlame][static_cast<int>(InputID::BackSide)])
	{
		dir = cameraRot.GetBack();
		rotRad = AsoUtility::Deg2RadF(180.0f);
	}
	if (contData[NowFlame][static_cast<int>(InputID::RightSide)])
	{
		dir = cameraRot.GetRight();
		rotRad = AsoUtility::Deg2RadF(90.0f);
	}
	if (contData[NowFlame][static_cast<int>(InputID::LeftSide)])
	{
		dir = cameraRot.GetLeft();
		rotRad = AsoUtility::Deg2RadF(270.0f);
	}

	//ジャンプ処理
	//Zキーを押したとき、かつジャンプしていないとき
	//あとからスペースキーに変更
	//if (contData[NowFlame][static_cast<int>(InputID::Btn2)] &&
	//	contData[OldFlame][static_cast<int>(InputID::Btn2)] &&
	//	(isJump_ || IsEndLanding()))
	//{
	//	ProcessJump();
	//}

	//地形変動処理
	//上昇
	//if (contData[NowFlame][static_cast<int>(InputID::Btn1)] &&
	//	(!isJump_ && IsEndLanding()))
	//{
	//	ProcessTerrainUp();
	//}
	//else 
	//{
	//	isDance_ = false;
	//}
	//下降
	if (contData[NowFlame][static_cast<int>(InputID::Btn2)] &&
		(!isJump_ && IsEndLanding()))
	{
		ProcessTerrainDown();
	}
	else 
	{
		isDance_ = false;
	}

	//移動キーが入力されていたら
	if (!AsoUtility::EqualsVZero(dir) && (!isJump_ || IsEndLanding()))
	{
		//回転処理
		SetGoalRotate(rotRad);
		ANIM_TYPE type = ANIM_TYPE::WALK;
		speed_ = SPEED_MOVE;

		moveDir_ = dir;
		movePow_ = VScale(dir, speed_);
		if (!isJump_ && !isDance_ && IsEndLanding())
		{
			//アニメーション再生
			animationController_->Play(static_cast<int>(type));
		}
	}
	else
	{
		if (!isJump_ && !isDance_ && IsEndLanding())
		{
			//アニメーション再生
			animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
		}
	}

	//回転（ゴールに対して、球面補完を行う）
	Rotate();
}

void Player::ProcessJump(void)
{
	isJump_ = true;

	//ジャンプの入力受付時間を減らす
	stepJump_ += SceneManager::GetInstance().GetDeltaTime();
	if (stepJump_ < TIME_JUMP_IN)
	{
		jumpPow_ = VScale(gravityManager_.GetDirUpGravity(), POW_JUMP);
	}

	//アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP));

}

void Player::ProcessTerrainUp(void)
{
	isDance_ = true;

	//プレイヤーの前方向座標
	VECTOR pos;

	stage_->SetUpPolygon(transform_.pos);

	//アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::DANCE));
}

void Player::ProcessTerrainDown(void)
{
	isDance_ = true;

	//プレイヤーの前方向座標
	VECTOR pos;

	//stage_->SetDownPolygon(transform_.pos);
	stage_->SetDownPolygonRock(transform_.pos);

	//アニメーション再生
	animationController_->Play(static_cast<int>(ANIM_TYPE::DANCE));
}

bool Player::IsEndLanding(void)
{
	//着地モーションが終わっている
	bool ret = true;

	if (animationController_->GetPlayType() != static_cast<int>(ANIM_TYPE::JUMP))
	{
		//ジャンプモーションしない
		return ret;
	}

	if (animationController_->IsEnd())
	{
		//ジャンプモーションが終了している
		return ret;
	}

	return ret;
}

void Player::Rotate(void)
{
	stepRot_ -= SceneManager::GetInstance().GetDeltaTime();

	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, 0.2f
	);
}

void Player::SetGoalRotate(float rad)
{
	//カメラの角度を取得
	VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();

	//カメラが向いている方向＋回転量
	Quaternion axis = Quaternion::AngleAxis(cameraAngles.y + rad, AsoUtility::AXIS_Y);

	//現在設定されている回転との角度差を求める
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	//しきい値
	if (angleDiff > 0.1)
	{
		stepRot_ = TIME_ROT;
	}

	//到達したい回転を設定
	goalQuaRot_ = axis;
}

void Player::DrawShadow(void)
{
	float PLAYER_SHADOW_HEIGHT = 300.0f;
	float PLAYER_SHADOW_SIZE = 30.0f;

	int i;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3];
	VECTOR SlideVec;
	int ModelHandle;

	// ライティングを無効にする
	SetUseLighting(FALSE);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// 影を落とすモデルの数だけ繰り返し
	for (auto c : colliders_)
	{
		// チェックするモデルは、jが0の時はステージモデル、1以上の場合はコリジョンモデル
		ModelHandle = c->modelId_;

		// プレイヤーの直下に存在する地面のポリゴンを取得
		HitResDim = MV1CollCheck_Capsule(
			ModelHandle, -1,
			transform_.pos,
			VAdd(transform_.pos, 
				VGet(0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f)), PLAYER_SHADOW_SIZE);

		// 頂点データで変化が無い部分をセット
		Vertex[0].dif = GetColorU8(255, 255, 255, 255);
		Vertex[0].spc = GetColorU8(0, 0, 0, 0);
		Vertex[0].su = 0.0f;
		Vertex[0].sv = 0.0f;
		Vertex[1] = Vertex[0];
		Vertex[2] = Vertex[0];

		// 球の直下に存在するポリゴンの数だけ繰り返し
		HitRes = HitResDim.Dim;
		for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
		{
			// ポリゴンの座標は地面ポリゴンの座標
			Vertex[0].pos = HitRes->Position[0];
			Vertex[1].pos = HitRes->Position[1];
			Vertex[2].pos = HitRes->Position[2];

			// ちょっと持ち上げて重ならないようにする
			SlideVec = VScale(HitRes->Normal, 0.5f);
			Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
			Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
			Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

			// ポリゴンの不透明度を設定する
			Vertex[0].dif.a = 0;
			Vertex[1].dif.a = 0;
			Vertex[2].dif.a = 0;
			if (HitRes->Position[0].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[0].dif.a = 128 * (1.0f - fabs(HitRes->Position[0].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[1].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[1].dif.a = 128 * (1.0f - fabs(HitRes->Position[1].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[2].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[2].dif.a = 128 * (1.0f - fabs(HitRes->Position[2].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
			Vertex[0].u = (HitRes->Position[0].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[0].v = (HitRes->Position[0].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].u = (HitRes->Position[1].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].v = (HitRes->Position[1].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].u = (HitRes->Position[2].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].v = (HitRes->Position[2].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

			// 影ポリゴンを描画
			DrawPolygon3D(Vertex, 1, imgShadow_, TRUE);
		}

		// 検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(HitResDim);
	}

	// ライティングを有効にする
	SetUseLighting(TRUE);

	// Ｚバッファを無効にする
	SetUseZBuffer3D(FALSE);
}

void Player::CalcSlope(void)
{
	//重力の反対方向
	VECTOR gravityUp = gravityManager_.GetDirUpGravity();

	//重力の反対方向から地面の法線方向に向けた回転量
	Quaternion upNorQua = Quaternion::FromToRotation(gravityUp, hitNormal_);

	//取得した回転軸と角度を取得する
	float angle = 0.0f;
	VECTOR axis;
	upNorQua.ToAngleAxis(&angle, &axis);

	//90度を足して、傾斜ベクトルへの回転を取得する
	Quaternion slopeQua = Quaternion::AngleAxis(
		angle + AsoUtility::Deg2RadF(90.0f), axis
	);

	//地面の法線方向を傾斜方向に回転させる
	slopeDir_ = slopeQua.PosAxis(gravityUp);

	//傾斜の角度
	slopeAngleDeg_ =
		static_cast<float>(AsoUtility::AngleDeg(gravityUp, slopeDir_));

	//傾斜による移動
	if (AsoUtility::SqrMagnitude(jumpPow_) == 0.0f)
	{
		float CHK_ANGLE = 120.0f;

		if (slopeAngleDeg_ >= CHK_ANGLE)
		{
			float diff = abs(slopeAngleDeg_ - CHK_ANGLE);
			slopePow_ = VScale(slopeDir_, diff / 3.0f);
			movePow_ = VAdd(movePow_, slopePow_);
		}
	}

}

void Player::CalcGravityPow(void)
{
	//重力方向
	VECTOR dirGravity = gravityManager_.GetDirGravity();

	//重力の強さ
	float gravityPow = gravityManager_.GetPower();

	//重力
	VECTOR gravity = VScale(dirGravity, gravityPow);

	//上下の移動量に重量を加える
	jumpPow_ = VAdd(jumpPow_, gravity);

	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		//重力方向と逆でなければジャンプ力を打ち消す
		jumpPow_ = gravity;
	}
}

void Player::Collision(void)
{
	//現在座標を起点に移動量を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	//衝突（カプセル）
	//CollisionCapsule();

	//衝突（三角形と線分）
	CollisionTriangle();

	//フレーム間の移動量
	moveDiff_ = VSub(movedPos_, transform_.pos);

	//移動
	transform_.pos = movedPos_;
}

void Player::CollisionCapsule(void)
{
	//カプセルを移動する
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = capsule_->Copy(&trans);

	//カプセルとステージモデルの衝突判定
	for (auto c : colliders_)
	{
		auto hits = MV1CollCheck_Capsule(
			c->modelId_, -1,
			cap.GetPosTop(),
			cap.GetPosDown(),
			cap.GetRadius()
		);

		//衝突処理
		for (int i = 0; i < hits.HitNum; i++)
		{
			//一つ一つの衝突情報を取得
			auto hit = hits.Dim[i];

			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{
				//処理負荷を減らすために１ポリゴンと
				//カプセルの衝突判定にする
				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(),
					cap.GetPosDown(),
					cap.GetRadius(),
					hit.Position[0],
					hit.Position[1],
					hit.Position[2]
				);

				if (pHit)
				{
					//衝突している
					float pow = 1.0f;	//ちょっとだけ動かす
					hit.Normal;			//衝突したポリゴンの法線	

					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, pow));

					//カプセルを移動させる
					trans.pos = movedPos_;
					trans.Update();
					cap = capsule_->Copy(&trans);

					continue;
				}

				break;
			}
		}

		//検出したポリゴンとの衝突情報を消す
		MV1CollResultPolyDimTerminate(hits);
	}
}

void Player::CollisionTriangle(void)
{
	//ジャンプ量を加算
	movedPos_ = VAdd(movedPos_, jumpPow_);

	//重力方向
	VECTOR dirGravity = gravityManager_.GetDirGravity();

	//重力の反対方向
	VECTOR dirUpGravity = gravityManager_.GetDirUpGravity();

	//重力の強さ
	float gravityPow = gravityManager_.GetPower();

	//プレイヤーの衝突判定立方体
	auto playerPos_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	playerAABB_.min = VAdd(playerPos_, VECTOR{ -50,0,-15 });
	playerAABB_.max = VAdd(playerPos_, VECTOR{ 50,200,15 });
	auto playerOldPos = playerPos_;

	//線分
	float checkPow = 10.0f;

	//足元のちょっと上の座標
	gravityHitUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravityHitUp_ = VAdd(gravityHitUp_, VScale(dirUpGravity, checkPow * 2.0f));

	//足元のちょっと上の座標
	gravityHitDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));

	auto& polygons = stage_->GetPolygons();
	for (auto& p : polygons)
	{
		//AABB衝突判定
		if (!(
			playerAABB_.min.x > p.aabb.max.x ||
			playerAABB_.min.y > p.aabb.max.y ||
			playerAABB_.min.z > p.aabb.max.z ||
			playerAABB_.max.x < p.aabb.min.x ||
			playerAABB_.max.y < p.aabb.min.y ||
			playerAABB_.max.z < p.aabb.min.z))
		{
			//ポリゴンと線分の衝突判定
			auto hit = HitCheck_Line_Triangle(
				gravityHitUp_, gravityHitDown_, 
				p.vertexs[0].pos, p.vertexs[1].pos, p.vertexs[2].pos);

			//衝突している　かつ　落下している
			if (hit.HitFlag && VDot(dirGravity, jumpPow_) > 0)
			{
				float dis = 0.5f;
				movedPos_ = VAdd(hit.Position,
					VScale(lpGravityMng.GetDirUpGravity(), dis));

				//抵抗力の代わりに
				jumpPow_ = AsoUtility::VECTOR_ZERO;

				//ジャンプ終了
				isJump_ = false;
				stepJump_ = 0.0f;
			}
		}
	}
}


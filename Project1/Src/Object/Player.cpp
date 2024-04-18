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
	//�R���g���[���[�̏��擾
	if (GetJoypadNum())
	{
		//�p�b�h���ڑ�����Ă�����
		controller_ = std::make_unique<GamePad>();
	}
	else
	{
		controller_ = std::make_unique<Keybord>();
	}

	rectangular_ = std::make_unique<Rectangular>();

	//���f���̐ݒ�
	ModelInit();

	// �A�j���[�V�����̐ݒ�
	AnimationInit();
}

void Player::Update(void)
{	
	controller_->Update();

	//�s������
	BehaviorControll();

	MV1RefreshCollInfo(transform_.modelId, -1);

	transform_.Update();
	animationController_->Update();

}

void Player::Draw(void)
{
	// ���f���̕`��
	MV1DrawModel(transform_.modelId);

	//�f�o�b�O�\��
	//DrawDebug();
}

void Player::Release(void)
{
	//���f���̉��
	MV1DeleteModel(transform_.modelId);
}

Transform* Player::GetTransform(void)
{
	//���f����{���̎擾
	return &transform_;
}

void Player::DrawDebug(void)
{
	//�f�o�b�O�\��
	DrawFormatString(0, 38, 0xffffff, "�v���C���[���W:%f,%f,%f",
		transform_.pos.x, transform_.pos.y, transform_.pos.z);

	//AABB�Փ˔���p
	//DrawSphere3D(playerAABB_.max, 10.0f, 10, 0xff0000, 0xff0000, true);
	//DrawString(0, 0, "AABB�Փ˔���Max�l", 0xff0000);
	//DrawSphere3D(playerAABB_.min, 10.0f, 10, 0x00ff00, 0x00ff00, true);
	//DrawString(0, 16, "AABB�Փ˔���Min�l", 0x00ff00);

	//�����̏Փ˔���p����
	DrawSphere3D(transform_.pos, COL_RADIUS, 20, 0x0000ff, 0x0000ff, true);

	//�����̏Փ˔���p����
	DrawLine3D(gravityHitUp_, gravityHitDown_, 0x0000ff);

	//�J�v�Z���R���C�_�̕\��
	//capsule_->Draw();
}

void Player::ModelInit(void)
{
	//���f�������i�[
	transform_.SetModel(
		resourceManager_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));

	//���f���̐ݒ�
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { 0.0f, -900.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	//��]�̐ݒ�
	playerRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRot_ = 0.0f;

	//�W�����v�����֘A
	jumpPow_ = AsoUtility::VECTOR_ZERO;
	isJump_ = false;
	stepJump_ = 0.0f;

	//�ۉe�摜
	imgShadow_ = LoadGraph((Application::PATH_IMAGE + "Shadow.png").c_str());

	//�J�v�Z���R���C�_
	capsule_ = new Capsule(&transform_);
	capsule_->SetRelativePosTop({ 0.0f,110.0f,0.0f });
	capsule_->SetRelativePosDown({ 0.0f,30.0f,0.0f });
	capsule_->SetRadius(20.0f);

	//�X�Ί֘A
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
	//�ړ�
	ProcessMove();

	//�X�Όv�Z
	CalcSlope();

	//�d�͂ɂ��ړ��ʂ��v�Z
	CalcGravityPow();

	//�Փ˔���ƈړ�����
	Collision();

	//��]
	transform_.quaRot = gravityManager_.GetTransform().quaRot;
	transform_.quaRot = transform_.quaRot.Mult(playerRotY_);

}

void Player::ProcessMove(void)
{
	//�ړ��ʃ[��
	movePow_ = AsoUtility::VECTOR_ZERO;

	//X����]���������A�d�͕����ɐ����ȃJ�����p�x�iXZ���ʁj���擾
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	//��]�������p�x
	float rotRad = 0.0f;

	//�ړ�����
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	auto contData = controller_->GetInputData();
	//�ړ������E��]�ʂ����߂�
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

	//�W�����v����
	//Z�L�[���������Ƃ��A���W�����v���Ă��Ȃ��Ƃ�
	//���Ƃ���X�y�[�X�L�[�ɕύX
	//if (contData[NowFlame][static_cast<int>(InputID::Btn2)] &&
	//	contData[OldFlame][static_cast<int>(InputID::Btn2)] &&
	//	(isJump_ || IsEndLanding()))
	//{
	//	ProcessJump();
	//}

	//�n�`�ϓ�����
	//�㏸
	//if (contData[NowFlame][static_cast<int>(InputID::Btn1)] &&
	//	(!isJump_ && IsEndLanding()))
	//{
	//	ProcessTerrainUp();
	//}
	//else 
	//{
	//	isDance_ = false;
	//}
	//���~
	if (contData[NowFlame][static_cast<int>(InputID::Btn2)] &&
		(!isJump_ && IsEndLanding()))
	{
		ProcessTerrainDown();
	}
	else 
	{
		isDance_ = false;
	}

	//�ړ��L�[�����͂���Ă�����
	if (!AsoUtility::EqualsVZero(dir) && (!isJump_ || IsEndLanding()))
	{
		//��]����
		SetGoalRotate(rotRad);
		ANIM_TYPE type = ANIM_TYPE::WALK;
		speed_ = SPEED_MOVE;

		moveDir_ = dir;
		movePow_ = VScale(dir, speed_);
		if (!isJump_ && !isDance_ && IsEndLanding())
		{
			//�A�j���[�V�����Đ�
			animationController_->Play(static_cast<int>(type));
		}
	}
	else
	{
		if (!isJump_ && !isDance_ && IsEndLanding())
		{
			//�A�j���[�V�����Đ�
			animationController_->Play(static_cast<int>(ANIM_TYPE::IDLE));
		}
	}

	//��]�i�S�[���ɑ΂��āA���ʕ⊮���s���j
	Rotate();
}

void Player::ProcessJump(void)
{
	isJump_ = true;

	//�W�����v�̓��͎�t���Ԃ����炷
	stepJump_ += SceneManager::GetInstance().GetDeltaTime();
	if (stepJump_ < TIME_JUMP_IN)
	{
		jumpPow_ = VScale(gravityManager_.GetDirUpGravity(), POW_JUMP);
	}

	//�A�j���[�V�����Đ�
	animationController_->Play(static_cast<int>(ANIM_TYPE::JUMP));

}

void Player::ProcessTerrainUp(void)
{
	isDance_ = true;

	//�v���C���[�̑O�������W
	VECTOR pos;

	stage_->SetUpPolygon(transform_.pos);

	//�A�j���[�V�����Đ�
	animationController_->Play(static_cast<int>(ANIM_TYPE::DANCE));
}

void Player::ProcessTerrainDown(void)
{
	isDance_ = true;

	//�v���C���[�̑O�������W
	VECTOR pos;

	//stage_->SetDownPolygon(transform_.pos);
	stage_->SetDownPolygonRock(transform_.pos);

	//�A�j���[�V�����Đ�
	animationController_->Play(static_cast<int>(ANIM_TYPE::DANCE));
}

bool Player::IsEndLanding(void)
{
	//���n���[�V�������I����Ă���
	bool ret = true;

	if (animationController_->GetPlayType() != static_cast<int>(ANIM_TYPE::JUMP))
	{
		//�W�����v���[�V�������Ȃ�
		return ret;
	}

	if (animationController_->IsEnd())
	{
		//�W�����v���[�V�������I�����Ă���
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
	//�J�����̊p�x���擾
	VECTOR cameraAngles = SceneManager::GetInstance().GetCamera()->GetAngles();

	//�J�����������Ă�������{��]��
	Quaternion axis = Quaternion::AngleAxis(cameraAngles.y + rad, AsoUtility::AXIS_Y);

	//���ݐݒ肳��Ă����]�Ƃ̊p�x�������߂�
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	//�������l
	if (angleDiff > 0.1)
	{
		stepRot_ = TIME_ROT;
	}

	//���B��������]��ݒ�
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

	// ���C�e�B���O�𖳌��ɂ���
	SetUseLighting(FALSE);

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// �e�𗎂Ƃ����f���̐������J��Ԃ�
	for (auto c : colliders_)
	{
		// �`�F�b�N���郂�f���́Aj��0�̎��̓X�e�[�W���f���A1�ȏ�̏ꍇ�̓R���W�������f��
		ModelHandle = c->modelId_;

		// �v���C���[�̒����ɑ��݂���n�ʂ̃|���S�����擾
		HitResDim = MV1CollCheck_Capsule(
			ModelHandle, -1,
			transform_.pos,
			VAdd(transform_.pos, 
				VGet(0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f)), PLAYER_SHADOW_SIZE);

		// ���_�f�[�^�ŕω��������������Z�b�g
		Vertex[0].dif = GetColorU8(255, 255, 255, 255);
		Vertex[0].spc = GetColorU8(0, 0, 0, 0);
		Vertex[0].su = 0.0f;
		Vertex[0].sv = 0.0f;
		Vertex[1] = Vertex[0];
		Vertex[2] = Vertex[0];

		// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
		HitRes = HitResDim.Dim;
		for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
		{
			// �|���S���̍��W�͒n�ʃ|���S���̍��W
			Vertex[0].pos = HitRes->Position[0];
			Vertex[1].pos = HitRes->Position[1];
			Vertex[2].pos = HitRes->Position[2];

			// ������Ǝ����グ�ďd�Ȃ�Ȃ��悤�ɂ���
			SlideVec = VScale(HitRes->Normal, 0.5f);
			Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
			Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
			Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

			// �|���S���̕s�����x��ݒ肷��
			Vertex[0].dif.a = 0;
			Vertex[1].dif.a = 0;
			Vertex[2].dif.a = 0;
			if (HitRes->Position[0].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[0].dif.a = 128 * (1.0f - fabs(HitRes->Position[0].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[1].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[1].dif.a = 128 * (1.0f - fabs(HitRes->Position[1].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			if (HitRes->Position[2].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[2].dif.a = 128 * (1.0f - fabs(HitRes->Position[2].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT);

			// �t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
			Vertex[0].u = (HitRes->Position[0].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[0].v = (HitRes->Position[0].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].u = (HitRes->Position[1].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].v = (HitRes->Position[1].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].u = (HitRes->Position[2].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].v = (HitRes->Position[2].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

			// �e�|���S����`��
			DrawPolygon3D(Vertex, 1, imgShadow_, TRUE);
		}

		// ���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(HitResDim);
	}

	// ���C�e�B���O��L���ɂ���
	SetUseLighting(TRUE);

	// �y�o�b�t�@�𖳌��ɂ���
	SetUseZBuffer3D(FALSE);
}

void Player::CalcSlope(void)
{
	//�d�͂̔��Ε���
	VECTOR gravityUp = gravityManager_.GetDirUpGravity();

	//�d�͂̔��Ε�������n�ʂ̖@�������Ɍ�������]��
	Quaternion upNorQua = Quaternion::FromToRotation(gravityUp, hitNormal_);

	//�擾������]���Ɗp�x���擾����
	float angle = 0.0f;
	VECTOR axis;
	upNorQua.ToAngleAxis(&angle, &axis);

	//90�x�𑫂��āA�X�΃x�N�g���ւ̉�]���擾����
	Quaternion slopeQua = Quaternion::AngleAxis(
		angle + AsoUtility::Deg2RadF(90.0f), axis
	);

	//�n�ʂ̖@���������X�Ε����ɉ�]������
	slopeDir_ = slopeQua.PosAxis(gravityUp);

	//�X�΂̊p�x
	slopeAngleDeg_ =
		static_cast<float>(AsoUtility::AngleDeg(gravityUp, slopeDir_));

	//�X�΂ɂ��ړ�
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
	//�d�͕���
	VECTOR dirGravity = gravityManager_.GetDirGravity();

	//�d�͂̋���
	float gravityPow = gravityManager_.GetPower();

	//�d��
	VECTOR gravity = VScale(dirGravity, gravityPow);

	//�㉺�̈ړ��ʂɏd�ʂ�������
	jumpPow_ = VAdd(jumpPow_, gravity);

	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		//�d�͕����Ƌt�łȂ���΃W�����v�͂�ł�����
		jumpPow_ = gravity;
	}
}

void Player::Collision(void)
{
	//���ݍ��W���N�_�Ɉړ��ʂ����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

	//�Փˁi�J�v�Z���j
	//CollisionCapsule();

	//�Փˁi�O�p�`�Ɛ����j
	CollisionTriangle();

	//�t���[���Ԃ̈ړ���
	moveDiff_ = VSub(movedPos_, transform_.pos);

	//�ړ�
	transform_.pos = movedPos_;
}

void Player::CollisionCapsule(void)
{
	//�J�v�Z�����ړ�����
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = capsule_->Copy(&trans);

	//�J�v�Z���ƃX�e�[�W���f���̏Փ˔���
	for (auto c : colliders_)
	{
		auto hits = MV1CollCheck_Capsule(
			c->modelId_, -1,
			cap.GetPosTop(),
			cap.GetPosDown(),
			cap.GetRadius()
		);

		//�Փˏ���
		for (int i = 0; i < hits.HitNum; i++)
		{
			//���̏Փˏ����擾
			auto hit = hits.Dim[i];

			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{
				//�������ׂ����炷���߂ɂP�|���S����
				//�J�v�Z���̏Փ˔���ɂ���
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
					//�Փ˂��Ă���
					float pow = 1.0f;	//������Ƃ���������
					hit.Normal;			//�Փ˂����|���S���̖@��	

					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, pow));

					//�J�v�Z�����ړ�������
					trans.pos = movedPos_;
					trans.Update();
					cap = capsule_->Copy(&trans);

					continue;
				}

				break;
			}
		}

		//���o�����|���S���Ƃ̏Փˏ�������
		MV1CollResultPolyDimTerminate(hits);
	}
}

void Player::CollisionTriangle(void)
{
	//�W�����v�ʂ����Z
	movedPos_ = VAdd(movedPos_, jumpPow_);

	//�d�͕���
	VECTOR dirGravity = gravityManager_.GetDirGravity();

	//�d�͂̔��Ε���
	VECTOR dirUpGravity = gravityManager_.GetDirUpGravity();

	//�d�͂̋���
	float gravityPow = gravityManager_.GetPower();

	//�v���C���[�̏Փ˔��藧����
	auto playerPos_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	playerAABB_.min = VAdd(playerPos_, VECTOR{ -50,0,-15 });
	playerAABB_.max = VAdd(playerPos_, VECTOR{ 50,200,15 });
	auto playerOldPos = playerPos_;

	//����
	float checkPow = 10.0f;

	//�����̂�����Ə�̍��W
	gravityHitUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravityHitUp_ = VAdd(gravityHitUp_, VScale(dirUpGravity, checkPow * 2.0f));

	//�����̂�����Ə�̍��W
	gravityHitDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));

	auto& polygons = stage_->GetPolygons();
	for (auto& p : polygons)
	{
		//AABB�Փ˔���
		if (!(
			playerAABB_.min.x > p.aabb.max.x ||
			playerAABB_.min.y > p.aabb.max.y ||
			playerAABB_.min.z > p.aabb.max.z ||
			playerAABB_.max.x < p.aabb.min.x ||
			playerAABB_.max.y < p.aabb.min.y ||
			playerAABB_.max.z < p.aabb.min.z))
		{
			//�|���S���Ɛ����̏Փ˔���
			auto hit = HitCheck_Line_Triangle(
				gravityHitUp_, gravityHitDown_, 
				p.vertexs[0].pos, p.vertexs[1].pos, p.vertexs[2].pos);

			//�Փ˂��Ă���@���@�������Ă���
			if (hit.HitFlag && VDot(dirGravity, jumpPow_) > 0)
			{
				float dis = 0.5f;
				movedPos_ = VAdd(hit.Position,
					VScale(lpGravityMng.GetDirUpGravity(), dis));

				//��R�͂̑����
				jumpPow_ = AsoUtility::VECTOR_ZERO;

				//�W�����v�I��
				isJump_ = false;
				stepJump_ = 0.0f;
			}
		}
	}
}


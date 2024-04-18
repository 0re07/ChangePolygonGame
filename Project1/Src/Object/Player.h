#pragma once
#include <vector>
#include <memory>
#include <DxLib.h>
#include "Common/Rectangular.h"
#include "Common/Transform.h"

class AnimationController;
class ResourceManager;
class GravityManager;
class Quaternion;
class Collider;
class Capsule;
class Controller;
class Stage;

class Player
{
public:
	//移動スピード
	static constexpr float SPEED_MOVE = 5.0f;	//通常移動

	//回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	//ジャンプ量
	static constexpr float POW_JUMP = 35.0f;

	//ジャンプ受付時間
	static constexpr float TIME_JUMP_IN = 0.5f;

	//煙エフェクトの発生間隔
	static constexpr float TIME_SMOKE = 0.3f;

	//煙エフェクトの発生間隔
	static constexpr float COL_RADIUS = 10.0f;

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		WALK,
		JUMP,
		DANCE
	};

	Player(Stage* stage);
	~Player(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	Transform* GetTransform(void);

private:
	//モデルの初期設定
	void ModelInit(void);

	//アニメーション読み込み
	void AnimationInit(void);

	//行動制御
	void BehaviorControll(void);

	//移動処理
	void ProcessMove(void);

	//ジャンプ処理
	void ProcessJump(void);

	//地形変動手続き処理
	void ProcessTerrainUp(void);
	void ProcessTerrainDown(void);

	//着地処理
	bool IsEndLanding(void);

	//回転制御
	//ゴールに対して、球面補完を行う
	void Rotate(void);
	//振り向きたい角度を決める
	void SetGoalRotate(float rad);

	//影の描画
	void DrawShadow(void);

	//デバッグ表示
	void DrawDebug(void);

	//傾斜の計算
	void CalcSlope(void);

	//重力による移動量を計算
	void CalcGravityPow(void);

	//衝突判定
	void Collision(void);
	void CollisionCapsule(void);	//カプセル
	void CollisionTriangle(void);	//三角形と線分

	//操作
	std::unique_ptr<Controller> controller_;

	//ステージ
	Stage* stage_;

	//プレイヤーのモデル制御
	Transform transform_;

	//３Dモデルの読み込み
	ResourceManager& resourceManager_;

	//重力制御
	GravityManager& gravityManager_;

	//プレイヤーのアニメーション制御
	AnimationController* animationController_;

	//矩形の当たり判定
	std::unique_ptr<Rectangular> rectangular_;

	//プレイヤー移動関係
	VECTOR moveDiff_;		//フレーム間の移動値
	VECTOR moveDir_;		//移動方向
	VECTOR movePow_;		//移動量
	VECTOR movedPos_;		//移動後の座標
	float speed_;			//移動スピード

	//回転関連
	Quaternion playerRotY_;	//現在のプレイヤーの向き
	Quaternion goalQuaRot_;	//到達したい向き=ゴール
	float stepRot_;			//時間計測用

	//ジャンプ関連
	bool isJump_;			//ジャンプ状態判定
	float stepJump_;		//ジャンプの入力受付時間
	VECTOR jumpPow_;		//ジャンプ量

	//地形変更（ダンス）関連
	bool isDance_;			//状態判定

	//衝突判定関連
	//コライダ
	Capsule* capsule_;
	std::vector<Collider*> colliders_;

	VECTOR hitPos_;			//接地点
	VECTOR slopeDir_;		//傾斜方向
	float slopeAngleDeg_;	//傾斜角
	VECTOR slopePow_;		//傾斜の移動量
	VECTOR hitNormal_;		//プレイヤーが設置しているポリゴンの法線

	//影画像
	int imgShadow_;

	//プレイヤーのAABB衝突判定
	AABB playerAABB_;

	//足元の線分（衝突判定用）座標
	VECTOR gravityHitUp_;	//足元の少し上の座標
	VECTOR gravityHitDown_; //足元の少し下の座標

};


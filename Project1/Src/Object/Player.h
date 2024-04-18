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
	//�ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;	//�ʏ�ړ�

	//��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	//�W�����v��
	static constexpr float POW_JUMP = 35.0f;

	//�W�����v��t����
	static constexpr float TIME_JUMP_IN = 0.5f;

	//���G�t�F�N�g�̔����Ԋu
	static constexpr float TIME_SMOKE = 0.3f;

	//���G�t�F�N�g�̔����Ԋu
	static constexpr float COL_RADIUS = 10.0f;

	// �A�j���[�V�������
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
	//���f���̏����ݒ�
	void ModelInit(void);

	//�A�j���[�V�����ǂݍ���
	void AnimationInit(void);

	//�s������
	void BehaviorControll(void);

	//�ړ�����
	void ProcessMove(void);

	//�W�����v����
	void ProcessJump(void);

	//�n�`�ϓ��葱������
	void ProcessTerrainUp(void);
	void ProcessTerrainDown(void);

	//���n����
	bool IsEndLanding(void);

	//��]����
	//�S�[���ɑ΂��āA���ʕ⊮���s��
	void Rotate(void);
	//�U����������p�x�����߂�
	void SetGoalRotate(float rad);

	//�e�̕`��
	void DrawShadow(void);

	//�f�o�b�O�\��
	void DrawDebug(void);

	//�X�΂̌v�Z
	void CalcSlope(void);

	//�d�͂ɂ��ړ��ʂ��v�Z
	void CalcGravityPow(void);

	//�Փ˔���
	void Collision(void);
	void CollisionCapsule(void);	//�J�v�Z��
	void CollisionTriangle(void);	//�O�p�`�Ɛ���

	//����
	std::unique_ptr<Controller> controller_;

	//�X�e�[�W
	Stage* stage_;

	//�v���C���[�̃��f������
	Transform transform_;

	//�RD���f���̓ǂݍ���
	ResourceManager& resourceManager_;

	//�d�͐���
	GravityManager& gravityManager_;

	//�v���C���[�̃A�j���[�V��������
	AnimationController* animationController_;

	//��`�̓����蔻��
	std::unique_ptr<Rectangular> rectangular_;

	//�v���C���[�ړ��֌W
	VECTOR moveDiff_;		//�t���[���Ԃ̈ړ��l
	VECTOR moveDir_;		//�ړ�����
	VECTOR movePow_;		//�ړ���
	VECTOR movedPos_;		//�ړ���̍��W
	float speed_;			//�ړ��X�s�[�h

	//��]�֘A
	Quaternion playerRotY_;	//���݂̃v���C���[�̌���
	Quaternion goalQuaRot_;	//���B����������=�S�[��
	float stepRot_;			//���Ԍv���p

	//�W�����v�֘A
	bool isJump_;			//�W�����v��Ԕ���
	float stepJump_;		//�W�����v�̓��͎�t����
	VECTOR jumpPow_;		//�W�����v��

	//�n�`�ύX�i�_���X�j�֘A
	bool isDance_;			//��Ԕ���

	//�Փ˔���֘A
	//�R���C�_
	Capsule* capsule_;
	std::vector<Collider*> colliders_;

	VECTOR hitPos_;			//�ڒn�_
	VECTOR slopeDir_;		//�X�Ε���
	float slopeAngleDeg_;	//�X�Ίp
	VECTOR slopePow_;		//�X�΂̈ړ���
	VECTOR hitNormal_;		//�v���C���[���ݒu���Ă���|���S���̖@��

	//�e�摜
	int imgShadow_;

	//�v���C���[��AABB�Փ˔���
	AABB playerAABB_;

	//�����̐����i�Փ˔���p�j���W
	VECTOR gravityHitUp_;	//�����̏�����̍��W
	VECTOR gravityHitDown_; //�����̏������̍��W

};


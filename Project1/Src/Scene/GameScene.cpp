#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Common/Flame.h"
#include "../Input/Controller.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/Player.h"
#include "../Object/Coin.h"
#include "GameScene.h"

GameScene::GameScene(void)
	:SceneBase()
{
}

void GameScene::Init(void)
{
	// �X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	//�X�e�[�W
	stage_ = new Stage();
	stage_->Init();

	//�v���C���[
	player_ = new Player(stage_);
	player_->Init();

	//�R�C��
	MakeCoin();
	for (const auto c : coins_)
	{
		c->Init();
	}

	isHitNum_ = 0;

	//�J�����̐ݒ�
	lpSceneMng.GetCamera()->SetTransform(player_->GetTransform());
	lpSceneMng.GetCamera()->ChangeMode(Camera::MODE::FOLLOW);
}

void GameScene::Update(void)
{
	controller_->Update();

	// �V�[���J��
	if(isHitNum_ >= 2)
	{
		lpSceneMng.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	player_->Update();
	stage_->Update();
	skyDome_->Update();

	// �R�C��
	for (const auto c : coins_)
	{
		c->Update();
	}

	IsHitCoinPlayer();


}

void GameScene::Draw(void)
{
	skyDome_->Draw();
	stage_->Draw();
	player_->Draw();

	// �R�C��
	for (const auto c : coins_)
	{
		c->Draw();
	}

	// �f�o�b�O�\��
	//DrawDebug();

}

void GameScene::DrawDebug(void)
{
	DrawString(0, 0, "GameScene", 0xffffff);
}

void GameScene::Release(void)
{	
	player_->Release();
	delete player_;

	skyDome_->Release();

	stage_->Release();
	delete stage_;

	//�R�C��
	for (const auto c : coins_)
	{
		c->Release();
		delete c;
	}
	coins_.clear();

}

void GameScene::MakeCoin(void)
{
	Transform trans;
	Coin* coin;

	trans.pos = { 0.0f, -800.0f, 100.0f };
	trans.scl = { 1.0f,1.0f, 1.0f };
	trans.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(-90.0f),
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	coin = new Coin(trans);
	coin->Init();
	coins_.push_back(coin);

	trans.pos = { -1000.0f, -1200.0f, 1600.0f };
	trans.scl = {1.0f,1.0f, 1.0f };
	trans.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(-90.0f),
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	coin = new Coin(trans);
	coin->Init();
	coins_.push_back(coin);
}

void GameScene::IsHitCoinPlayer(void)
{
	// �R�C���ƃv���C���[�̏Փ˔���
	auto p = player_->GetTransform();

	auto coin = coins_;
	for (const auto c : coin)
	{
		//�\������Ă���Ƃ�
		if (c->GetHit())
		{
			//�Փ˔���
			auto hit = MV1CollCheck_Sphere(p->modelId, -1, c->GetPos(), c->COL_RADIUS);

			//�ЂƂł��Փ˂��Ă�����
			if (hit.HitNum > 0) 
			{
				//�Փ˂��Ă�����
				if (hit.Dim->HitFlag > 0) 
				{
					c->SetHit(false);
					isHitNum_++;
					break;
				}
			}
		}
	}
}

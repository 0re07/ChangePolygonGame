#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Common/Flame.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/GravityManager.h"
#include "../Manager/Camera.h"
#include "../Input/Controller.h"
#include "../Object/Common/Grid.h"
#include "../Object/SkyDome.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
	:SceneBase()
{
	imgPush_ = -1;
	imgTitle_ = -1;
	skyDome_ = nullptr;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	lpGravityMng.Init();

	ResourceManager& rm = lpResourceMng;

	// �摜�ǂݍ���
	imgTitle_ = rm.Load(ResourceManager::SRC::TITLE).handleId_;
	imgPush_ = rm.Load(ResourceManager::SRC::PUSH_SPACE).handleId_;

	// �O���b�h��
	grid_ = std::make_unique<Grid>();
	grid_->Init();

	// �w�i
	//skyDome_ = std::make_unique<SkyDome>();
	//skyDome_->Init();

	// ��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	count_ = 0;
}

void TitleScene::Update(void)
{
	controller_->Update();

	count_++;

	grid_->Update();

	// �V�[���J��
	auto contData = controller_->GetInputData();
	if (contData[NowFlame][static_cast<int>(InputID::Btn3)] &&
		!contData[OldFlame][static_cast<int>(InputID::Btn3)])
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}
	//skyDome_->Update();

}

void TitleScene::Draw(void)
{
	//skyDome_->Draw();

	//grid_->Draw();

	//�^�C�g���摜
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 300, 1.0, 0.0, imgTitle_, true);

	if ((count_ / 30) % 2 != 0)
	{
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 300, 1.0, 0.0, imgPush_, true);
	}

	//�f�o�b�O�p
	//DrawString(0, 0, "TitleScene", 0xffffff);
}

void TitleScene::Release(void)
{
	//skyDome_->Release();
	grid_->Release();
}

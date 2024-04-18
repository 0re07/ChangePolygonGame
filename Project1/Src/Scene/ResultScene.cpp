#include "../Application.h"
#include "../Input/Controller.h"
#include "../Common/Flame.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "ResultScene.h"

ResultScene::ResultScene(void)
	:SceneBase()
{
}

ResultScene::~ResultScene(void)
{
}

void ResultScene::Init(void)
{
	ResourceManager& rm = lpResourceMng;

	// 画像読み込み
	imgResult_ = rm.Load(ResourceManager::SRC::TITLE).handleId_;
	imgSpace_ = rm.Load(ResourceManager::SRC::PUSH_SPACE).handleId_;

	count_ = 0;
}

void ResultScene::Update(void)
{
	count_++;

	controller_->Update();

	// シーン遷移
	auto contData = controller_->GetInputData();
	if (contData[NowFlame][static_cast<int>(InputID::Btn3)] &&
		!contData[OldFlame][static_cast<int>(InputID::Btn3)])
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void ResultScene::Draw(void)
{
	//タイトル画像
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 250, 0.35, 0.0, imgResult_, true);

	if ((count_ / 30) % 2 != 0)
	{
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 500, 1.0, 0.0, imgSpace_, true);
	}

}

void ResultScene::Release(void)
{
}

#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{
	Resource res;

	// タイトル画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Title.png");
	resouceMap_.emplace(SRC::TITLE, res);

	// PushSpace画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "push_space.png");
	resouceMap_.emplace(SRC::PUSH_SPACE, res);

	// PushSpace画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Result.png");
	resouceMap_.emplace(SRC::RESULT, res);

	// スカイドーム
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "SkyDome/SkyDome.mv1");
	resouceMap_.emplace(SRC::SKYDOME, res);

	//ステージ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage.mv1");
	resouceMap_.emplace(SRC::STAGE, res);

	//プレイヤー
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Player.mv1");
	resouceMap_.emplace(SRC::PLAYER, res);
	
	//プレイヤー影
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Shadow.png");
	resouceMap_.emplace(SRC::PLAYER_SHADOW, res);

	//コイン
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Star.mv1");
	resouceMap_.emplace(SRC::COIN, res);

	//コインの狼煙
	res = Resource(Resource::TYPE::EFFEKSEER,Application::PATH_EFFECT + "CoinSmoke/CoinSmoke.efkefc");
	resouceMap_.emplace(SRC::COIN_SMOKE, res);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

ResourceManager::~ResourceManager(void)
{
	delete instance_;
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resouceMap_.find(src);
	if (rPair == resouceMap_.end())
	{
		// 登録されていない
		return nullptr;
	}

	rPair->second.Load();

	// 念のためコピーコンストラクタ
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}

#include "../Input/Controller.h"
#include "../Input/GamePad.h"
#include "../Input/KeyBord.h"
#include "SceneBase.h"

SceneBase::SceneBase(void)
{
	Init();
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init(void)
{
	if (GetJoypadNum())		//パッドが接続されていた時
	{
		controller_ = std::make_unique<GamePad>();
	}
	else
	{
		controller_ = std::make_unique<Keybord>();
	}
}

void SceneBase::Update(void)
{
}

void SceneBase::Draw(void)
{
}

void SceneBase::Release(void)
{
}

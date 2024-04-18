#include <DxLib.h>
#include "../../Input/GamePad.h"
#include "../../Input/Keybord.h"

#include "Fluctuation.h"

Fluctuation::Fluctuation()
{
}

Fluctuation::~Fluctuation()
{
}

void Fluctuation::Init(void)
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
}

void Fluctuation::Update(void)
{
	controller_->Update();

}

void Fluctuation::Release(void)
{
}

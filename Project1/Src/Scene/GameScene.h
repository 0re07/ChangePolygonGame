#pragma once
#include "SceneBase.h"

class SceneManager;
class Controller;
class Stage;
class SkyDome;
class WaveSea;
class Player;
class Coin;

class GameScene 
	: public SceneBase
{

public:

	GameScene(void);
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void DrawDebug(void);
	void Release(void) override;

private:
	//コインの情報生成
	void MakeCoin(void);

	//コインとプレイヤーの衝突判定
	void IsHitCoinPlayer(void);

	//ステージ
	Stage* stage_;

	//プレイヤー
	Player* player_;

	//背景
	std::unique_ptr<SkyDome> skyDome_;

	// コイン
	std::vector<Coin*> coins_;

	int isHitNum_;
};

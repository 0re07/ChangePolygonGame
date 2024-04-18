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
	//�R�C���̏�񐶐�
	void MakeCoin(void);

	//�R�C���ƃv���C���[�̏Փ˔���
	void IsHitCoinPlayer(void);

	//�X�e�[�W
	Stage* stage_;

	//�v���C���[
	Player* player_;

	//�w�i
	std::unique_ptr<SkyDome> skyDome_;

	// �R�C��
	std::vector<Coin*> coins_;

	int isHitNum_;
};

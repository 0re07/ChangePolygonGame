#pragma once
#include "SceneBase.h"

class SceneManager;
class Grid;
class SkyDome;

class TitleScene 
	: public SceneBase
{

public:
	TitleScene(void);
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	int count_;

	// �摜
	int imgTitle_;
	int imgPush_;

	//�O���b�h��
	std::unique_ptr<Grid> grid_;
	
	// �w�i
	std::unique_ptr<SkyDome> skyDome_;

};

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

	// ‰æ‘œ
	int imgTitle_;
	int imgPush_;

	//ƒOƒŠƒbƒhü
	std::unique_ptr<Grid> grid_;
	
	// ”wŒi
	std::unique_ptr<SkyDome> skyDome_;

};

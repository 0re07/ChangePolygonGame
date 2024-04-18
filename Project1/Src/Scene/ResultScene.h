#pragma once
#include "SceneBase.h"

class SceneManager;

class ResultScene :
    public SceneBase
{
public:
	ResultScene(void);
	~ResultScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	int count_;

	int imgResult_;
	int imgSpace_;

};


#pragma once
#include "AABBs.h"

class Rectangular
{
public:
	Rectangular(void);
	~Rectangular(void);

	void Init(void);
	void Update(VECTOR pPos);
	void Draw(VECTOR pos);
	void Relase(void);

	//�����蔻��
	bool HitAABB(AABB player, AABB stage);

private:

};


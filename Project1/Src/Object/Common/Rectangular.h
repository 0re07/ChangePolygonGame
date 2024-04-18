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

	//“–‚½‚è”»’è
	bool HitAABB(AABB player, AABB stage);

private:

};


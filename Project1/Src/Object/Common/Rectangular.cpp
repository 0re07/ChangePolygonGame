#include "Rectangular.h"

Rectangular::Rectangular(void)
{
}

Rectangular::~Rectangular(void)
{
}

void Rectangular::Init(void)
{

}

void Rectangular::Update(VECTOR pPos)
{
}

void Rectangular::Draw(VECTOR pos)
{
}

void Rectangular::Relase(void)
{
}

bool Rectangular::HitAABB(AABB player, AABB stage)
{
	if (player.min.x > stage.max.x) return false;
	if (player.max.x < stage.min.x) return false;
	if (player.min.y > stage.max.y) return false;
	if (player.max.y < stage.min.y) return false;
	if (player.min.z > stage.max.z) return false;
	if (player.max.z < stage.min.z) return false;
	return true;
}

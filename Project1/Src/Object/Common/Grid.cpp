#include <DxLib.h>
#include "Grid.h"

Grid::Grid(void)
{
}

Grid::~Grid(void)
{
}

void Grid::Init(void)
{
}

void Grid::Update(void)
{
}

void Grid::Draw(void)
{
	for (int i = 0; i < 15; i++) {

		//‚˜Ž² Â
		DrawLine3D(
			VGet(-HLEN, 0.0f, -HLEN + i * 80),
			VGet(HLEN, 0.0f, -HLEN + i * 80),
			0x0000ff);
		DrawSphere3D(
			VGet(HLEN, 0.0f, -HLEN + i * 80),
			30.0f, 2, 0x0000ff, 0x0000ff, true);

		//‚šŽ²@Ô
		DrawLine3D(
			VGet(-HLEN + i * 80, 0.0f, -HLEN),
			VGet(-HLEN + i * 80, 0.0f, HLEN),
			0xff0000);
		DrawSphere3D(
			VGet(-HLEN + i * 80, 0.0f, HLEN),
			30.0f, 2, 0xff0000, 0xff0000, true);

	}

	//‚™Ž²@—Î
	DrawLine3D(VGet(0.0f, 0.0f, 0.0f),
		VGet(0.0f, 300.0f, 0.0f),
		0x00ff00);
}

void Grid::Release(void)
{
}

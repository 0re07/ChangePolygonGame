#pragma once
#include <vector>
#include "Common/Transform.h"

class ResourceManager;

class Stage 
{
public:
	struct ONI_POLYGON
	{
		VERTEX3D vertexs[3];
		int idx[3];
		AABB aabb;
	};

	Stage(void);
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void DrawDebug(void);
	void Release(void);

	//î•ñæ“¾
	MV1_REF_POLYGONLIST GetPolygon(void) const;
	const std::vector<VERTEX3D>& GetVertex(void) const;

	const std::vector<ONI_POLYGON>& GetPolygons(void) const;

	//î•ñŠi”[
	void SetDownPolygon(VECTOR pos);
	void SetDownPolygonRock(VECTOR pos);
	void SetUpPolygon(VECTOR pos);


private:	
	//ƒ|ƒŠƒSƒ“î•ñŠi”[
	void CreateMesh(void);
	void CreateOniPolygon(void);

	//ƒ‚ƒfƒ‹‚Ì“Ç‚İ‚İ
	ResourceManager& resourceManager_;

	// ƒ‚ƒfƒ‹§Œä‚ÌŠî–{î•ñ
	Transform transform_;

	//ƒ|ƒŠƒSƒ“î•ñŠi”[æ
	MV1_REF_POLYGONLIST polygonList_;

	//’¸“_î•ñŠÇ—
	std::vector<VERTEX3D> vertexes_;

	//ƒ|ƒŠƒSƒ“î•ñŠÇ—
	std::vector<ONI_POLYGON> polygons_;
};


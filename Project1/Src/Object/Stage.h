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

	//情報取得
	MV1_REF_POLYGONLIST GetPolygon(void) const;
	const std::vector<VERTEX3D>& GetVertex(void) const;

	const std::vector<ONI_POLYGON>& GetPolygons(void) const;

	//情報格納
	void SetDownPolygon(VECTOR pos);
	void SetDownPolygonRock(VECTOR pos);
	void SetUpPolygon(VECTOR pos);


private:	
	//ポリゴン情報格納
	void CreateMesh(void);
	void CreateOniPolygon(void);

	//モデルの読み込み
	ResourceManager& resourceManager_;

	// モデル制御の基本情報
	Transform transform_;

	//ポリゴン情報格納先
	MV1_REF_POLYGONLIST polygonList_;

	//頂点情報管理
	std::vector<VERTEX3D> vertexes_;

	//ポリゴン情報管理
	std::vector<ONI_POLYGON> polygons_;
};


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

	//���擾
	MV1_REF_POLYGONLIST GetPolygon(void) const;
	const std::vector<VERTEX3D>& GetVertex(void) const;

	const std::vector<ONI_POLYGON>& GetPolygons(void) const;

	//���i�[
	void SetDownPolygon(VECTOR pos);
	void SetDownPolygonRock(VECTOR pos);
	void SetUpPolygon(VECTOR pos);


private:	
	//�|���S�����i�[
	void CreateMesh(void);
	void CreateOniPolygon(void);

	//���f���̓ǂݍ���
	ResourceManager& resourceManager_;

	// ���f������̊�{���
	Transform transform_;

	//�|���S�����i�[��
	MV1_REF_POLYGONLIST polygonList_;

	//���_���Ǘ�
	std::vector<VERTEX3D> vertexes_;

	//�|���S�����Ǘ�
	std::vector<ONI_POLYGON> polygons_;
};


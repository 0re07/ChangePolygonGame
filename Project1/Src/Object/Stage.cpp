#include <algorithm>
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "Stage.h"

Stage::Stage()
	:resourceManager_(ResourceManager::GetInstance())
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	//���f���̃��[�h
	transform_.SetModel(
		resourceManager_.LoadModelDuplicate(ResourceManager::SRC::STAGE));

	//���f���̐ݒ�
	transform_.scl = { 50.0f, 50.0f, 50.0f };
	transform_.pos = { 0.0f, -2000.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, DX_PI_F, 0.0f });
	transform_.Update();

	// ���f���S�̂̎Q�Ɨp���b�V�����\�z
	MV1SetupReferenceMesh(transform_.modelId, -1, TRUE);

	//�|���S�������\�z
	//CreateMesh();

	CreateOniPolygon();

}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	//�|���S���`��
	//DrawPolygon3D(polygons_.data()->vertexs, polygonList_.PolygonNum, DX_NONE_GRAPH, FALSE);
	
	// �f���ɕ`��(�A���x��)
	//for (auto& p : polygons_)
	//{
	//	DrawPolygon3D(p.vertexs, 1, DX_NONE_GRAPH, FALSE);
	//}

	// ���_��������ĕ`��(�܂��x��)
	//std::vector<VERTEX3D> vs;
	//for (auto& p : polygons_)
	//{
	//	vs.emplace_back(p.vertexs[0]);
	//	vs.emplace_back(p.vertexs[1]);
	//	vs.emplace_back(p.vertexs[2]);
	//}
	//DrawPolygon3D(vs.data(), polygons_.size(), DX_NONE_GRAPH, FALSE);

	// �ő�
	DrawPolygon3D(vertexes_.data(), polygonList_.PolygonNum, DX_NONE_GRAPH, FALSE);

	//�f�o�b�O�`��
	//DrawDebug();
}

void Stage::DrawDebug(void)
{
	// ���b�V����`��
	// �|���S���̐������J��Ԃ�
	int i;
	for (i = 0; i < polygonList_.PolygonNum; i++)
	{
		// �|���S�����`������O���_���g�p���ă��C���[�t���[����`�悷��
		DrawLine3D(
			polygonList_.Vertexs[polygonList_.Polygons[i].VIndex[0]].Position,
			polygonList_.Vertexs[polygonList_.Polygons[i].VIndex[1]].Position,
			GetColor(255, 255, 0));

		DrawLine3D(
			polygonList_.Vertexs[polygonList_.Polygons[i].VIndex[1]].Position,
			polygonList_.Vertexs[polygonList_.Polygons[i].VIndex[2]].Position,
			GetColor(255, 255, 0));

		DrawLine3D(
			polygonList_.Vertexs[polygonList_.Polygons[i].VIndex[2]].Position,
			polygonList_.Vertexs[polygonList_.Polygons[i].VIndex[0]].Position,
			GetColor(255, 255, 0));
	}

	//���f���̒��S���W
	//DrawSphere3D(transform_.pos, 1000.0f, 20, 0xffff00, 0xffff00, true);
}

void Stage::Release(void) 
{
	MV1DeleteModel(transform_.modelId);
}

MV1_REF_POLYGONLIST Stage::GetPolygon(void) const
{
	return polygonList_;
}

const std::vector<VERTEX3D>& Stage::GetVertex(void) const
{
	return vertexes_;
}

const std::vector<Stage::ONI_POLYGON>& Stage::GetPolygons(void) const
{
	return polygons_;
}

void Stage::SetDownPolygon(VECTOR pos)
{
	VECTOR down = { 0.0f,-2.0f,0.0f };

	for (auto& p : polygons_) 
	{
		for (int i = 0; i < 3; i++)
		{
			if (AsoUtility::IsHitSphere(pos, 100.0f, p.vertexs[i].pos))
			{
				p.vertexs[i].pos = VAdd(p.vertexs[i].pos, down);
				vertexes_[p.idx[i]].pos = p.vertexs[i].pos;
			}
		}
	}
}

void Stage::SetDownPolygonRock(VECTOR pos)
{
	VECTOR down = { 0.0f,-2.0f,0.0f };
	VECTOR front = { 0.0f,0.0f,10.0f };

	for (auto& p : polygons_)
	{
		for (int i = 0; i < 3; i++)
		{
			if (AsoUtility::IsHitSphere(pos, 100.0f, p.vertexs[i].pos))
			{
				p.vertexs[i].pos = VAdd(p.vertexs[i].pos, down);
				vertexes_[p.idx[i]].pos = p.vertexs[i].pos;

				//VECTOR frontPos = VAdd(p.vertexs[i].pos, front);
				//frontPos = VSub(frontPos, down);
				//vertexes_[p.idx[i]].pos = frontPos;
			}
		}
	}
}

void Stage::SetUpPolygon(VECTOR pos)
{
	VECTOR down = { 0.0f,2.0f,0.0f };

	for (auto& p : polygons_)
	{
		for (int i = 0; i < 3; i++)
		{
			if (AsoUtility::IsHitSphere(pos, 100.0f, p.vertexs[i].pos))
			{
				p.vertexs[i].pos = VAdd(p.vertexs[i].pos, down);
				vertexes_[p.idx[i]].pos = p.vertexs[i].pos;
			}
		}
	}
}

void Stage::CreateMesh(void)
{
	// �Q�Ɨp���b�V�����̎擾
	polygonList_ = MV1GetReferenceMesh(transform_.modelId, -1, TRUE);

	int num = polygonList_.PolygonNum;
	for (int i = 0; i < num; i++)
	{
		auto poly = polygonList_.Polygons[i];

		for (int p = 0; p < 3; p++)
		{
			//���_�̗v�f�ԍ����擾
			int vIndex = poly.VIndex[p];

			//�擾�������_�̗v�f�ԍ����g���āA���_�����擾
			auto verRef = polygonList_.Vertexs[vIndex];

			//���_���֕ϊ�
			VERTEX3D v;
			v.pos = verRef.Position;
			v.dif = verRef.DiffuseColor;
			v.spc = verRef.SpecularColor;
			v.norm = verRef.Normal;
			v.u = verRef.TexCoord[0].u;
			v.v = verRef.TexCoord[0].v;

			//���I�z��Ɋi�[
			vertexes_.emplace_back(v);
		}
	}
}

void Stage::CreateOniPolygon(void)
{
	// �Q�Ɨp���b�V�����̎擾
	polygonList_ = MV1GetReferenceMesh(transform_.modelId, -1, TRUE);

	int num = polygonList_.PolygonNum;
	for (int i = 0; i < num; i++)
	{
		auto poly = polygonList_.Polygons[i];

		ONI_POLYGON polygon;

		for (int p = 0; p < 3; p++)
		{
			//���_�̗v�f�ԍ����擾
			int vIndex = poly.VIndex[p];

			//�擾�������_�̗v�f�ԍ����g���āA���_�����擾
			auto verRef = polygonList_.Vertexs[vIndex];

			//���_���֕ϊ�
			VERTEX3D v;
			v.pos = verRef.Position;
			v.dif = verRef.DiffuseColor;
			v.spc = verRef.SpecularColor;
			v.norm = verRef.Normal;
			v.u = verRef.TexCoord[0].u;
			v.v = verRef.TexCoord[0].v;

			polygon.vertexs[p] = v;

			//���I�z��Ɋi�[
			vertexes_.emplace_back(v);
		}

		polygon.aabb = AsoUtility::GetAABBTriangle(
			polygon.vertexs[0].pos,
			polygon.vertexs[1].pos,
			polygon.vertexs[2].pos
		);

		polygon.idx[0] = vertexes_.size() - 3;
		polygon.idx[1] = vertexes_.size() - 2;
		polygon.idx[2] = vertexes_.size() - 1;

		//���I�z��Ɋi�[
		polygons_.emplace_back(polygon);
	}
}

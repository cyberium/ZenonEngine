#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBXInterfaces.h"

class ZN_API CFBXModel
	: public ModelProxie
	, public IFBXModel
{
public:
	struct FBXVertex
	{
		FBXVertex()
		{
			memset(weights, 0x00, 16);
			memset(indexes, 0x00, 16);
		}

		// Render part
		glm::vec3 pos;
		glm::vec2 uv;
		glm::vec3 normal;
		//glm::vec3 tangent;
		//glm::vec3 binormal;
		float weights[4];
		uint32 indexes[4];

		// Engine part
		uint32 controlPointIndex;
		//glm::vec3 _unused;
	};

public:
	CFBXModel(const IBaseManager& BaseManager, const IFBXNode& FBXNode);
	virtual ~CFBXModel();

	bool Load(fbxsdk::FbxMesh* NativeMesh);

	// IFBXModel
	const IFBXNode& GetOwner() const override;
	std::shared_ptr<IModel> GetModel() override;
	

private:
	void MaterialLoad(fbxsdk::FbxMesh* NativeMesh);
	void SkeletonLoad(fbxsdk::FbxMesh* NativeMesh);
	FBXVertex& GetVertexByControlPointIndex(int Index);
	//void DisplayMaterialMapping(fbxsdk::FbxGeometryElementMaterial* materialElement);

private:
	std::vector<FBXVertex> m_Vertices;
	std::shared_ptr<IGeometry> m_Geometry;

	const IBaseManager& m_BaseManager;
	const IFBXNode& m_FBXNode;
};

#endif

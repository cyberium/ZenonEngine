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
		// Render part
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 uv;
		glm::vec3 tangent;
		glm::vec3 binormal;
		//std::vector<std::pair<float, uint32>> weightIndexes;

		// Engine part
		int32 controlPointIndex;
	};

public:
	CFBXModel(const IBaseManager& BaseManager, const IFBXNode& FBXNode);
	virtual ~CFBXModel();

	void Load(fbxsdk::FbxMesh* NativeMesh);

	// IFBXModel
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

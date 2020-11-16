#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBXInterfaces.h"

class ZN_API CFBXNode
	: public IFBXNode
	, public std::enable_shared_from_this<CFBXNode>
{
public:
	CFBXNode(const IBaseManager& BaseManager, IFBXScene& FBXScene);
	virtual ~CFBXNode();

	void LoadNode(fbxsdk::FbxNode * NativeNode, int pDepth = 0);
	
	// IFBXNode
	const IFBXScene& GetFBXScene() const override;
	glm::mat4 GetTransform() const override;
	glm::mat4 GetParentWorldTransform() const override;
	std::weak_ptr<IFBXNode> GetParent() const override;
	const std::vector<std::shared_ptr<IFBXNode>>& GetChilds() const override;
	std::shared_ptr<IFBXMaterial> GetFBXMaterial(int Index) const override;
	std::shared_ptr<IFBXModel> GetFBXModel() const override;
	std::shared_ptr<IFBXLight> GetFBXLight() const override;

protected:
	void LoadParams(fbxsdk::FbxNode * NativeNode);
	void LoadChilds(fbxsdk::FbxNode * NativeNode);
	void LoadMaterials(fbxsdk::FbxNode * NativeNode);
	void LoadAttributes(fbxsdk::FbxNode * NativeNode);
	void LoadModel(fbxsdk::FbxNode * NativeNode);
	void LoadLight(fbxsdk::FbxNode * NativeNode);

private:
	glm::mat4 m_Transform;
	std::weak_ptr<CFBXNode> m_Parent;
	std::vector<std::shared_ptr<IFBXNode>> m_Childs;

	std::vector<std::shared_ptr<IFBXMaterial>> m_MaterialsArray;
	std::shared_ptr<IFBXModel> m_Model;
	std::shared_ptr<IFBXLight> m_Light;

private:
	const IBaseManager& m_BaseManager;
	IFBXScene& m_FBXScene;
};

#endif

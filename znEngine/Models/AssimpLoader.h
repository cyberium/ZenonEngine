#pragma once

// FORWARD BEGIN
struct aiMaterial;
struct aiMesh;
struct aiNode;
// FORWARD END

class ZN_API CAssimpLoader
{
public:
	CAssimpLoader(std::shared_ptr<ISceneNode> ParentSceneNode, std::shared_ptr<IRenderDevice> RenderDevice);
	virtual ~CAssimpLoader();

	std::shared_ptr<ISceneNode> LoadFromFile(std::shared_ptr<IFile> File);

private:
	MaterialNameMap m_MaterialMap;
	MaterialList m_Materials;
	MeshList m_Meshes;

	void ImportMaterial(const aiMaterial& material, std::string parentPath);
	void ImportMesh(const aiMesh& mesh);
	std::shared_ptr<ISceneNode> ImportSceneNode(std::shared_ptr<ISceneNode> parent, aiNode* aiNode);

private:
	std::shared_ptr<ISceneNode> m_SceneNode;
	std::shared_ptr<IRenderDevice> m_RenderDevice;
};
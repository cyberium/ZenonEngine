#include "stdafx.h"

// General
#include "MeshComponent3D.h"

CMeshComponent3D::CMeshComponent3D(const ISceneNode3D& OwnerNode)
    : CComponentBase(OwnerNode)
{}

CMeshComponent3D::~CMeshComponent3D()
{}


void CMeshComponent3D::AddMesh(std::shared_ptr<IModel> mesh)
{
	//CSceneLocker locker(GetOwnerNode().GetScene());

    MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
    if (iter == m_Meshes.end())
        m_Meshes.push_back(mesh);
}

void CMeshComponent3D::RemoveMesh(IModel* mesh)
{
	//CSceneLocker locker(GetOwnerNode().GetScene());

    //MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
    //if (iter != m_Meshes.end())
    //    m_Meshes.erase(iter);
}

const CMeshComponent3D::MeshList CMeshComponent3D::GetMeshes()
{
	//CSceneLocker locker(GetOwnerNode().GetScene());
    return m_Meshes;
}



//
// ISceneNodeComponent
//
void CMeshComponent3D::Accept(IVisitor* visitor)
{
	const auto& meshes = GetMeshes();
	std::for_each(meshes.begin(), meshes.end(), [&visitor](const auto& Mesh) {
		Mesh->Accept(visitor);
	});
}

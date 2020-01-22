#include "stdafx.h"

// General
#include "MeshComponent3D.h"

CMeshComponent3D::CMeshComponent3D(std::shared_ptr<ISceneNode3D> OwnerNode)
    : CComponentBase(OwnerNode)
{}

CMeshComponent3D::~CMeshComponent3D()
{}


void CMeshComponent3D::AddMesh(IMesh* mesh)
{
    assert(mesh);

    MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
    if (iter == m_Meshes.end())
        m_Meshes.push_back(mesh);
}

void CMeshComponent3D::RemoveMesh(IMesh* mesh)
{
    assert(mesh);

    MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
    if (iter != m_Meshes.end())
        m_Meshes.erase(iter);
}

const CMeshComponent3D::MeshList& CMeshComponent3D::GetMeshes()
{
    return m_Meshes;
}



//
// ISceneNodeComponent
//
bool CMeshComponent3D::Accept(IVisitor* visitor)
{
    bool acceptResult = true;

	const auto& meshes = GetMeshes();
	std::for_each(meshes.begin(), meshes.end(), [&acceptResult, &visitor](IMesh* Mesh)
	{
		acceptResult = Mesh->Accept(visitor);
	});

    return acceptResult;
}

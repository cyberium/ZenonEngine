#include "stdafx.h"

// Include
#include "SceneFunctional/Base/SceneNode.h"

// General
#include "MeshComponent3D.h"

CMeshComponent3D::CMeshComponent3D(std::shared_ptr<SceneNode> OwnerNode)
    : CComponentBase(OwnerNode)
{
}

CMeshComponent3D::~CMeshComponent3D()
{
}


void CMeshComponent3D::AddMesh(std::shared_ptr<IMesh> mesh)
{
    assert(mesh);

    MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
    if (iter == m_Meshes.end())
        m_Meshes.push_back(mesh);
}

void CMeshComponent3D::RemoveMesh(std::shared_ptr<IMesh> mesh)
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
bool CMeshComponent3D::Accept(IVisitor& visitor)
{
    bool acceptResult = true;

    for (auto mesh : GetMeshes())
    {
        acceptResult = mesh->Accept(visitor);
    }

    return acceptResult;
}

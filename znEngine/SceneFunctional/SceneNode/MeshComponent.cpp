#include "stdafx.h"

// Include
#include "SceneFunctional\\SceneNode3D.h"

// General
#include "MeshComponent.h"

CMeshComponent::CMeshComponent(std::shared_ptr<SceneNode3D> OwnerNode)
    : CComponentBase(OwnerNode)
{
}

CMeshComponent::~CMeshComponent()
{
}


void CMeshComponent::AddMesh(std::shared_ptr<IMesh> mesh)
{
    assert(mesh);
    std::lock_guard<std::mutex> lg(m_MeshMutex);

    MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
    if (iter == m_Meshes.end())
        m_Meshes.push_back(mesh);
}

void CMeshComponent::RemoveMesh(std::shared_ptr<IMesh> mesh)
{
    assert(mesh);
    std::lock_guard<std::mutex> lg(m_MeshMutex);

    MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
    if (iter != m_Meshes.end())
        m_Meshes.erase(iter);
}

const CMeshComponent::MeshList& CMeshComponent::GetMeshes()
{
    std::lock_guard<std::mutex> lg(m_MeshMutex);

    return m_Meshes;
}



//
// ISceneNodeComponent
//
bool CMeshComponent::Accept(IVisitor& visitor)
{
    bool acceptResult = true;

    for (auto mesh : GetMeshes())
    {
        acceptResult = mesh->Accept(visitor);
    }

    return acceptResult;
}

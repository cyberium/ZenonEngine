#pragma once

#include "ComponentBase.h"

class 
    __declspec(uuid("403E886D-7BD7-438B-868D-AC4380830716"))
    CMeshComponent : public CComponentBase
{
public:
    typedef std::vector<std::shared_ptr<IMesh>> MeshList;

public:
    CMeshComponent(std::shared_ptr<SceneNode3D> OwnerNode);
    virtual ~CMeshComponent();

    /**
     * Add a mesh to this scene node.
     * The scene node does not take ownership of a mesh that is set on a mesh
     * as it is possible that the same mesh is added to multiple scene nodes.
     * Deleting the scene node does not delete the meshes associated with it.
     */
    virtual void AddMesh(std::shared_ptr<IMesh> mesh);
    virtual void RemoveMesh(std::shared_ptr<IMesh> mesh);
    virtual const MeshList&  GetMeshes();

    // ISceneNodeComponent
    virtual bool Accept(IVisitor& visitor) override;

private:
    MeshList          m_Meshes;
    std::mutex        m_MeshMutex;

};
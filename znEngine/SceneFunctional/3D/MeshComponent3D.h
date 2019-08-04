#pragma once

#include "SceneFunctional/Base/ComponentBase.h"

class 
    __declspec(uuid("403E886D-7BD7-438B-868D-AC4380830716"))
    OW_ENGINE_API
    CMeshComponent3D : public CComponentBase
{
public:
    typedef std::vector<std::shared_ptr<IMesh>> MeshList;

public:
    CMeshComponent3D(std::shared_ptr<SceneNode> OwnerNode);
    virtual ~CMeshComponent3D();

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
    virtual bool Accept(std::shared_ptr<IVisitor> visitor) override;

private:
    MeshList          m_Meshes;
    std::mutex        m_MeshMutex;

};
#pragma once

#include "../ComponentBase.h"

class __declspec(UUID_MeshComponent) ZN_API CMeshComponent3D
	: public IMeshComponent3D
	, public CComponentBase
{
public:
    typedef std::vector<std::shared_ptr<IMesh>> MeshList;

public:
    CMeshComponent3D(std::shared_ptr<ISceneNode> OwnerNode);
    virtual ~CMeshComponent3D();

	// IMeshComponent3D
    virtual void AddMesh(std::shared_ptr<IMesh> mesh);
    virtual void RemoveMesh(std::shared_ptr<IMesh> mesh);
    virtual const MeshList&  GetMeshes();

    // CComponentBase
    virtual bool Accept(IVisitor* visitor) override;

private:
    MeshList m_Meshes;
    std::mutex m_MeshMutex;
};
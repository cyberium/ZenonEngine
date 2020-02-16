#pragma once

#include "../ComponentBase.h"

class __declspec(UUID_MeshComponent) ZN_API CMeshComponent3D
	: public IMeshComponent3D
	, public CComponentBase
{
public:
    CMeshComponent3D(const ISceneNode3D& OwnerNode);
    virtual ~CMeshComponent3D();

	// IMeshComponent3D
    virtual void AddMesh(std::shared_ptr<IModel> mesh);
    virtual void RemoveMesh(IModel* mesh);
    const MeshList GetMeshes();

    // CComponentBase
    virtual void Accept(IVisitor* visitor) override;

private:
    MeshList m_Meshes;
};
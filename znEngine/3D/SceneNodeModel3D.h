#pragma once

#include "SceneNode3D.h"
#include "Light3D.h"

class SceneNodeModel3D : public SceneNode3D
{
	typedef SceneNode3D base;
public:
	typedef std::vector<std::shared_ptr<IMesh>> MeshList;
	typedef std::vector<std::shared_ptr<CLight3D>> LightList;

public:
	explicit SceneNodeModel3D(cmat4 localTransform = mat4(1.0f));
	virtual ~SceneNodeModel3D();

	std::shared_ptr<SceneNodeModel3D> shared_from_this()
	{
		return std::dynamic_pointer_cast<SceneNodeModel3D>(SceneNode3D::shared_from_this());
	}

	std::weak_ptr<SceneNodeModel3D> weak_from_this()
	{
		return std::dynamic_pointer_cast<SceneNodeModel3D>(std::shared_ptr<SceneNode3D>(SceneNode3D::weak_from_this()));
	}

	// Bounds
	void SetBounds(BoundingBox _bbox);
	cbbox GetBounds() const;

	/**
	 * Add a mesh to this scene node.
	 * The scene node does not take ownership of a mesh that is set on a mesh
	 * as it is possible that the same mesh is added to multiple scene nodes.
	 * Deleting the scene node does not delete the meshes associated with it.
	 */
	virtual void AddMesh(std::shared_ptr<IMesh> mesh);
	virtual void RemoveMesh(std::shared_ptr<IMesh> mesh);
	virtual const MeshList&  GetMeshes();

	/**
	 * Add a light to this scene node.
	 */
	virtual void AddLight(std::shared_ptr<CLight3D> _light);
	virtual void RemoveLight(std::shared_ptr<CLight3D> _light);
	virtual const LightList& GetLights();

	/**
	 * Allow a visitor to visit this node.
	 */
	virtual bool Accept(IVisitor& visitor) override;

	/**
	 * Useful for culling
	 */
	bool checkFrustum(const Camera* _camera) const;
	bool checkDistance2D(cvec3 _camPos, float _distance) const;
	bool checkDistance(cvec3 _camPos, float _distance) const;

protected:
	virtual void UpdateWorldTransform() override;
	virtual void UpdateBounds();

protected:
	BoundingBox       m_Bounds;
	
	MeshList          m_Meshes;
	std::mutex        m_MeshMutex;
	LightList         m_Lights;

};
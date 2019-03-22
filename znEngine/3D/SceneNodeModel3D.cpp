#include "stdafx.h"

// Include
#include "Light.h"

// General
#include "SceneNodeModel3D.h"

// Additional
#include "Loader.h"

SceneNodeModel3D::SceneNodeModel3D(cmat4 localTransform)
	: SceneNode3D(localTransform)
{
}

SceneNodeModel3D::~SceneNodeModel3D()
{
}

// Bounds
void SceneNodeModel3D::SetBounds(BoundingBox _bbox)
{
	m_Bounds = _bbox;
}
cbbox SceneNodeModel3D::GetBounds() const
{
	return m_Bounds;
}

void SceneNodeModel3D::UpdateWorldTransform()
{
	base::UpdateWorldTransform();

	UpdateBounds();
}

void SceneNodeModel3D::UpdateBounds()
{
	// do nothing
}

void SceneNodeModel3D::AddMesh(std::shared_ptr<IMesh> mesh)
{
	assert(mesh);
	std::lock_guard<std::mutex> lg(m_MeshMutex);

	MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
	if (iter == m_Meshes.end())
		m_Meshes.push_back(mesh);
}

void SceneNodeModel3D::RemoveMesh(std::shared_ptr<IMesh> mesh)
{
	assert(mesh);
	std::lock_guard<std::mutex> lg(m_MeshMutex);

	MeshList::iterator iter = std::find(m_Meshes.begin(), m_Meshes.end(), mesh);
	if (iter != m_Meshes.end())
		m_Meshes.erase(iter);
}

const SceneNodeModel3D::MeshList& SceneNodeModel3D::GetMeshes()
{
	std::lock_guard<std::mutex> lg(m_MeshMutex);

	return m_Meshes;
}

void SceneNodeModel3D::AddLight(std::shared_ptr<CLight3D> _light)
{
	LightList::iterator iter = std::find(m_Lights.begin(), m_Lights.end(), _light);
	if (iter == m_Lights.end())
		m_Lights.push_back(_light);
}

void SceneNodeModel3D::RemoveLight(std::shared_ptr<CLight3D> _light)
{
	LightList::iterator iter = std::find(m_Lights.begin(), m_Lights.end(), _light);
	if (iter != m_Lights.end())
		m_Lights.erase(iter);
}

const SceneNodeModel3D::LightList& SceneNodeModel3D::GetLights()
{
	return m_Lights;
}


bool SceneNodeModel3D::Accept(IVisitor& visitor)
{
	bool visitResult = visitor.Visit(*this);
	if (!visitResult)
		return false;

	// Visit meshes
	for (auto mesh : GetMeshes())
	{
		mesh->Accept(visitor);
	}

	// Visit lights
	for (auto light : GetLights())
	{
		light->Accept(visitor);
	}

	// Now visit children
	for (auto child : GetChilds())
	{
#ifdef LOADER_ENABLED
		std::shared_ptr<ILoadable> loadable = std::dynamic_pointer_cast<ILoadable, SceneNodeModel3D>(child);
		if (loadable != nullptr && ! loadable->isLoaded())
			continue;
#endif

		child->Accept(visitor);
	}

	return visitResult;
}

bool SceneNodeModel3D::checkFrustum(const Camera* _camera) const
{
	assert1(_camera != nullptr);
	return !_camera->GetFrustum().cullBox(GetBounds());
}

bool SceneNodeModel3D::checkDistance2D(cvec3 _camPos, float _distance) const
{
	// Check distance to camera
	float distToCamera2D = glm::length(Fix_X0Z(_camPos) - Fix_X0Z(GetBounds().getCenter())) - GetBounds().getRadius();
	return distToCamera2D < _distance;
}

bool SceneNodeModel3D::checkDistance(cvec3 _camPos, float _distance) const
{
	// Check distance to camera
	float distToCamera = glm::length(_camPos - GetBounds().getCenter()) - GetBounds().getRadius();
	return distToCamera < _distance;
}

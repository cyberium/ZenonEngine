#include "stdafx.h"

// General
#include "Editor3DToolMoverRTS.h"

CEditor3DToolMoverRTS::CEditor3DToolMoverRTS(IEditor3DFrame & EditorFrame)
	: CEditor3DToolBase(EditorFrame)
	, m_MoverValue(1.0f)
	, m_MoverNuber(-1)
	, m_IsMovingNow(false)
{
}

CEditor3DToolMoverRTS::~CEditor3DToolMoverRTS()
{
}

void CEditor3DToolMoverRTS::Initialize()
{
	m_MoverRoot = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), GetScene()->GetRootNode3D());
	m_MoverRoot->SetName("MoverRTS");

	std::shared_ptr<IFBXScene> fbxScene = GetBaseManager().GetManager<IFBXManager>()->LoadFBX("C:\\_engine\\ZenonEngine_gamedata\\arrow.FBX");

	auto fbxModels = fbxScene->GetFBXModels();
	_ASSERT(fbxModels.size() == 1);
	auto model = (*fbxModels.begin())->GetModel();
	auto geom = model->GetConnections().begin()->Geometry;
	if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
	{
		std::shared_ptr<IFile> file = std::make_shared<CFile>("C:\\_engine\\ZenonEngine_gamedata\\arrow.znmdl");
		loadable->Save(file);

		GetBaseManager().GetManager<IFilesManager>()->GetFilesStorage("PCEveryFileAccess")->SaveFile(file);
	}

	auto materialX = std::make_shared<MaterialDebug>(GetRenderDevice());
	materialX->SetDiffuseColor(glm::vec4(1.0f, 0.2f, 0.1f, 1.0f));
	auto modelX = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelX->AddConnection(materialX, geom);

	auto materialY = std::make_shared<MaterialDebug>(GetRenderDevice());
	materialY->SetDiffuseColor(glm::vec4(0.1f, 1.0f, 0.1f, 1.0f));
	auto modelY = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelY->AddConnection(materialY, geom);

	auto materialZ = std::make_shared<MaterialDebug>(GetRenderDevice());
	materialZ->SetDiffuseColor(glm::vec4(0.1f, 0.2f, 1.0f, 1.0f));
	auto modelZ = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelZ->AddConnection(materialZ, geom);


	m_MoverX = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_MoverRoot);
	m_MoverX->SetName("Mover_X");
	m_MoverX->SetRotation(glm::vec3(0.0f, 0.0f, -glm::half_pi<float>()));
	//moverX->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
	m_MoverX->GetModelsComponent()->SetModel(modelX);
	m_MoverX->GetColliderComponent()->SetBounds(model->GetBounds());

	m_MoverZ = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_MoverRoot);
	m_MoverZ->SetName("Mover_Z");
	//moverZ->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
	m_MoverZ->SetRotation(glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
	m_MoverZ->GetModelsComponent()->SetModel(modelZ);
	m_MoverZ->GetColliderComponent()->SetBounds(model->GetBounds());
}

void CEditor3DToolMoverRTS::Finalize()
{
}

void CEditor3DToolMoverRTS::Enable()
{
	CEditor3DToolBase::Enable();

	if (auto node = GetEditor3DFrame().GetNodesSelector()->GetFirstSelectedNode())
	{
		m_MovingNode = node;
		m_MoverRoot->SetTranslate(node->GetTranslation());
		m_MoverRoot->SetScale(glm::vec3(m_MovingNode->GetColliderComponent()->GetBounds().getRadius() * 1.0f / 10.0f));
	}
}

void CEditor3DToolMoverRTS::Disable()
{
	CEditor3DToolBase::Disable();

	m_MoverRoot->SetTranslate(glm::vec3(-1000000.0, -10000000.0f, -10000000.0f));

	Clear();
}

bool CEditor3DToolMoverRTS::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (!IsEnabled())
		return false;

	auto nodes = GetScene()->FindIntersection(RayToWorld, m_MoverRoot);
	if (nodes.empty())
		return false;

	if (m_MovingNode != nullptr && !IsChildOf(m_MoverRoot, m_MovingNode))
	{
		{
			auto pos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), m_MovingNode->GetTranslation().y));
			auto cameraPosX0Z = GetScene()->GetCameraController()->GetCamera()->GetTranslation();
			cameraPosX0Z = glm::vec3(cameraPosX0Z.x, 0.0f, cameraPosX0Z.z);
			auto movedObjectPosX0Z = glm::vec3(m_MovingNode->GetTranslation().x, 0.0f, m_MovingNode->GetTranslation().z);
			auto planeNormal = glm::normalize(movedObjectPosX0Z - cameraPosX0Z);
			auto posYYY = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(planeNormal, 0.0f));
			m_MoverOffset = m_MoverRoot->GetTranslation() - glm::vec3(pos.x, posYYY.y, pos.z);
		}

		for (const auto& it : nodes)
		{
			if (it.second == m_MoverX)
			{
				m_MoverNuber = 1;
			}
			else if (it.second == m_MoverZ)
			{
				m_MoverNuber = 3;
			}
		}

		if (m_MoverNuber > 0)
		{
			m_MovingObjectPos = m_MovingNode->GetTranslation();
			m_IsMovingNow = true;
			return true;
		}
	}

	return false;
}

void CEditor3DToolMoverRTS::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (!IsEnabled())
		return;

	if (m_IsMovingNow)
		Clear();
}

void CEditor3DToolMoverRTS::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	if (!IsEnabled())
		return;

	if (m_IsMovingNow)
	{
		glm::vec3 oldPos = m_MovingNode->GetTranslation();
		glm::vec3 newPos = glm::vec3(0.0f);

		if (m_MoverNuber == 1)
		{
			auto mousePos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), oldPos.y));
			newPos = glm::vec3(mousePos.x + m_MoverOffset.x, oldPos.y, oldPos.z);
		}
		else if (m_MoverNuber == 3)
		{
			auto mousePos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), oldPos.y));
			newPos = glm::vec3(oldPos.x, oldPos.y, mousePos.z + m_MoverOffset.z);
		}

		m_MovingNode->SetTranslate(FixBoxCoords(newPos));
		m_MoverRoot->SetTranslate(m_MovingNode->GetTranslation());

		return;
	}
}


glm::ivec3 CEditor3DToolMoverRTS::ToBoxCoords(const glm::vec3 & Position)
{
	return glm::round(Position / m_MoverValue);
}

glm::vec3 CEditor3DToolMoverRTS::FixBoxCoords(const glm::vec3 & Position)
{
	glm::vec3 newPosition = Position;
	newPosition /= m_MoverValue;
	newPosition = glm::round(newPosition);
	newPosition *= m_MoverValue;
	return newPosition;
}

void CEditor3DToolMoverRTS::SetMoverValue(float Value)
{
	m_MoverValue = Value;
}

void CEditor3DToolMoverRTS::Clear()
{
	m_MoverNuber = 0;
	m_IsMovingNow = false;
	m_MovingObjectPos = glm::vec3(0.0f);
}

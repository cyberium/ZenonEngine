#include "stdafx.h"

// General
#include "SceneXMLFunctional.h"

/*
bool SceneBase::Load(std::shared_ptr<IXMLReader> Reader)
{
	return false;
}

bool SceneBase::Save(std::shared_ptr<IXMLWriter> Writer)
{
	std::shared_ptr<IXMLWriter> writer = Writer->CreateChild("Scene");
	writer->AddAttribute("IsScene", "true");

	for (const auto& rootChild : GetRootNode3D()->GetChilds())
		rootChild->Save(writer);

	return false;
}*/

/*
bool CComponentBase::Load(std::shared_ptr<IXMLReader> Reader)
{
	_ASSERT_EXPR(false, L"Not implemented.");
	return false;
}

bool CComponentBase::Save(std::shared_ptr<IXMLWriter> Writer)
{
	//_ASSERT_EXPR(false, L"Not implemented.");
	return false;
}*/

	/*
	SCENENODE3D
	std::shared_ptr<IXMLWriter> componentWriter = Writer->CreateChild("TransformComponent3D");

	std::shared_ptr<IXMLWriter> translate = componentWriter->CreateChild("Translate");
	translate->AddAttribute("X", std::to_string(GetTranslation().x));
	translate->AddAttribute("Y", std::to_string(GetTranslation().y));
	translate->AddAttribute("Z", std::to_string(GetTranslation().z));

	std::shared_ptr<IXMLWriter> rotate = componentWriter->CreateChild("Rotate");
	rotate->AddAttribute("X", std::to_string(GetRotation().x));
	rotate->AddAttribute("Y", std::to_string(GetRotation().y));
	rotate->AddAttribute("Z", std::to_string(GetRotation().z));

	std::shared_ptr<IXMLWriter> scale = componentWriter->CreateChild("Scale");
	scale->AddAttribute("X", std::to_string(GetScale().x));
	scale->AddAttribute("Y", std::to_string(GetScale().y));
	scale->AddAttribute("Z", std::to_string(GetScale().z));



	std::shared_ptr<IXMLWriter> thisSceneNodeWriter = Writer->CreateChild("SceneNode");
	thisSceneNodeWriter->AddAttribute("Name", GetName());

	if (GetComponents().size() > 0)
	{
		std::shared_ptr<IXMLWriter> componentsWriter = thisSceneNodeWriter->CreateChild("Components");
		for (const auto& component : GetComponents())
			component.second->Save(componentsWriter);
	}

	if (GetChilds().size() > 0)
	{
		std::shared_ptr<IXMLWriter> childsWriter = thisSceneNodeWriter->CreateChild("Childs");
		for (const auto& child : GetChilds())
			child->Save(childsWriter);
	}*/

EVisitResult CSceneXMLFucntional::Visit(const ISceneNode3D * node)
{
	return EVisitResult::Block;
}

EVisitResult CSceneXMLFucntional::Visit(const ISceneNodeUI * node)
{
	return EVisitResult::Block;
}

EVisitResult CSceneXMLFucntional::Visit(const IModel * Model)
{
	return EVisitResult::Block;
}

EVisitResult CSceneXMLFucntional::Visit(const IGeometry * Geometry, const IMaterial * Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return EVisitResult::Block;
}

EVisitResult CSceneXMLFucntional::Visit(const ISceneNodeComponent * Component)
{
	return EVisitResult::Block;
}

EVisitResult CSceneXMLFucntional::Visit(const ILight3D * light)
{
	return EVisitResult::Block;
}

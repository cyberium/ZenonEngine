#include "stdafx.h"

// General
#include "SceneFinder.h"



namespace
{
	void FillIntersectedModelsMap(const std::shared_ptr<ISceneNode>& Owner, const Ray& Ray, std::map<float, std::shared_ptr<IModel>> * intersectedModels)
	{
		auto modelsComponent = Owner->GetComponentT<IModelsComponent3D>();
		if (modelsComponent == nullptr)
			return;

		auto model = modelsComponent->GetModel();
		if (model == nullptr)
			return;

		auto bounds = model->GetBounds();
		if (bounds.IsInfinite())
			return;

		BoundingBox worldBounds = bounds;
		worldBounds.transform(Owner->GetWorldTransfom());

		if (HitBoundingBox(worldBounds.getMin(), worldBounds.getMax(), Ray.GetOrigin(), Ray.GetDirection()))
		{
			float dist = nearestDistToAABB(Ray.GetOrigin(), worldBounds);
			intersectedModels->insert(std::make_pair(dist, model));
		}
	}

	void FillIntersectedModelsList(const std::shared_ptr<ISceneNode>& Owner, const Frustum& Frustum, std::vector<std::shared_ptr<IModel>> * intersectedModels)
	{
		auto modelsComponent = Owner->GetComponentT<IModelsComponent3D>();
		if (modelsComponent == nullptr)
			return;

		auto bounds = modelsComponent->GetModel()->GetBounds();
		if (modelsComponent->GetModel()->GetBounds().IsInfinite())
			return;

		auto worldMatrix = Owner->GetWorldTransfom();

		BoundingBox worldBounds = bounds;
		worldBounds.transform(worldMatrix);

		if (false == Frustum.cullBox(worldBounds))
			intersectedModels->push_back(modelsComponent->GetModel());
	}

	void FillIntersectedSceneNodesList(const std::shared_ptr<ISceneNode>& Parent, const Frustum& Frustum, std::vector<std::shared_ptr<ISceneNode>> * intersectedNodes)
	{
		const auto& childs = Parent->GetChilds();
		for (const auto& it : childs)
		{
			FillIntersectedSceneNodesList(it, Frustum, intersectedNodes);

			if (auto collider = it->GetComponentT<IColliderComponent3D>())
			{
				if (collider->GetBounds().IsInfinite())
					continue;

				if (false == Frustum.cullBox(collider->GetWorldBounds()))
					intersectedNodes->push_back(it);
			}
		}
	}

	void FillNearestSceneNodesMap(const std::shared_ptr<ISceneNode>& Parent, glm::vec3 Position, float Distance, std::map<float, std::shared_ptr<ISceneNode>> * NearestSceneNodes)
	{
		const auto& childs = Parent->GetChilds();
		for (const auto& it : childs)
		{
			FillNearestSceneNodesMap(it, Position, Distance, NearestSceneNodes);

			glm::vec3 nodeTranslate = it->GetTranslation();
			nodeTranslate = (Parent->GetWorldTransfom() * glm::vec4(nodeTranslate, 1.0f)).xyz;

			const float distanceToPoint = glm::length(Position - nodeTranslate);
			if (distanceToPoint >= Distance)
				continue;
			NearestSceneNodes->insert(std::make_pair(distanceToPoint, it));
		}
	}
	void FillNearestSceneNodesMap2D(const std::shared_ptr<ISceneNode>& Parent, glm::vec3 Position, float Distance, std::map<float, std::shared_ptr<ISceneNode>> * NearestSceneNodes)
	{
		const auto& childs = Parent->GetChilds();
		for (const auto& it : childs)
		{
			FillNearestSceneNodesMap(it, Position, Distance, NearestSceneNodes);

			glm::vec3 nodeTranslate = it->GetTranslation();
			nodeTranslate = (Parent->GetWorldTransfom() * glm::vec4(nodeTranslate, 1.0f)).xyz;

			const float distanceToPoint = glm::length(glm::vec2(Position.x, Position.z) - glm::vec2(nodeTranslate.x, nodeTranslate.z));
			if (distanceToPoint >= Distance)
				continue;
			NearestSceneNodes->insert(std::make_pair(distanceToPoint, it));
		}
	}

	void FillIntersectedSceneNodesMap(const std::shared_ptr<ISceneNode>& Parent, const Ray & Ray, std::map<float, std::shared_ptr<ISceneNode>> * intersectedNodes)
	{
		const auto& childs = Parent->GetChilds();
		for (const auto& it : childs)
		{
			FillIntersectedSceneNodesMap(it, Ray, intersectedNodes);

			if (auto collider = it->GetComponentT<IColliderComponent3D>())
			{
				if (collider->GetBounds().IsInfinite())
					continue;

				if (false == collider->IsRayIntersects(Ray))
					continue;

				float dist = nearestDistToAABB(Ray.GetOrigin(), collider->GetWorldBounds());
				intersectedNodes->insert(std::make_pair(dist, it));
			}
		}
	}
}

CSceneFinder::CSceneFinder(const IScene& Scene)
	: m_Scene(Scene)
{
}

CSceneFinder::~CSceneFinder()
{
}

std::map<float, std::shared_ptr<ISceneNode>> CSceneFinder::FindNearestNodes(glm::vec3 Position, float Distance, std::function<bool(std::shared_ptr<ISceneNode>)> Filter, std::shared_ptr<ISceneNode> RootForFinder) const
{
	std::map<float, std::shared_ptr<ISceneNode>> intersectedNodes;
	FillNearestSceneNodesMap2D(RootForFinder ? RootForFinder : m_Scene.GetRootSceneNode(), Position, Distance, &intersectedNodes);
	//FillNearestSceneNodesMap(RootForFinder ? RootForFinder : m_Scene.GetRootSceneNode(), Position, Distance, &intersectedNodes);

	if (Filter)
	{
		auto it = intersectedNodes.begin();
		while (it != intersectedNodes.end())
		{
			if (Filter(it->second))
				it++;
			else
				it = intersectedNodes.erase(it);
		}
	}

	return intersectedNodes;
}

std::map<float, std::shared_ptr<ISceneNode>> CSceneFinder::FindIntersection(const Ray& Ray, std::function<bool(std::shared_ptr<ISceneNode>)> Filter, std::shared_ptr<ISceneNode> RootForFinder) const
{
	std::map<float, std::shared_ptr<ISceneNode>> intersectedNodes;
	FillIntersectedSceneNodesMap(RootForFinder ? RootForFinder : m_Scene.GetRootSceneNode(), Ray, &intersectedNodes);

	if (Filter)
	{
		auto it = intersectedNodes.begin();
		while (it != intersectedNodes.end())
		{
			if (Filter(it->second))
				it++;
			else
				it = intersectedNodes.erase(it);
		}
	}

	return intersectedNodes;
}

std::vector<std::shared_ptr<ISceneNode>> CSceneFinder::FindIntersections(const Frustum & Frustum, std::function<bool(std::shared_ptr<ISceneNode>)> Filter, std::shared_ptr<ISceneNode> RootForFinder) const
{
	std::vector<std::shared_ptr<ISceneNode>> intersectedNodes;
	FillIntersectedSceneNodesList(RootForFinder ? RootForFinder : m_Scene.GetRootSceneNode(), Frustum, &intersectedNodes);
	
	if (Filter)
	{
		auto it = intersectedNodes.begin();
		while (it != intersectedNodes.end())
		{
			if (Filter(*it))
				it++;
			else
				it = intersectedNodes.erase(it);
		}
	}
	return intersectedNodes;
}

std::map<float, std::shared_ptr<IModel>> CSceneFinder::FindIntersectionWithModel(const Ray & Ray, std::function<bool(std::shared_ptr<ISceneNode>)> FilterForSceneNodes, std::function<bool(std::shared_ptr<IModel>)> FilterForModels, std::shared_ptr<ISceneNode> RootForFinder) const
{
	auto intersectedNodes = FindIntersection(Ray, FilterForSceneNodes, RootForFinder);
	if (intersectedNodes.empty())
		return std::map<float, std::shared_ptr<IModel>>();

	std::map<float, std::shared_ptr<IModel>> intersectedModels;
	for (const auto& node : intersectedNodes)
		FillIntersectedModelsMap(node.second, Ray, &intersectedModels);
	
	if (FilterForModels)
	{
		auto it = intersectedModels.begin();
		while (it != intersectedModels.end())
		{
			if (FilterForModels(it->second))
				it++;
			else
				it = intersectedModels.erase(it);
		}
	}

	return intersectedModels;
}

std::vector<std::shared_ptr<IModel>> CSceneFinder::FindIntersectionsWithModel(const Frustum & Frustum, std::function<bool(std::shared_ptr<ISceneNode>)> FilterForSceneNodes, std::function<bool(std::shared_ptr<IModel>)> FilterForModels, std::shared_ptr<ISceneNode> RootForFinder) const
{
	auto intersectedNodes = FindIntersections(Frustum, FilterForSceneNodes, RootForFinder);
	if (intersectedNodes.empty())
		return std::vector<std::shared_ptr<IModel>>();

	std::vector<std::shared_ptr<IModel>> intersectedModels;
	for (const auto& node : intersectedNodes)
		FillIntersectedModelsList(node, Frustum, &intersectedModels);

	if (FilterForModels)
	{
		auto it = intersectedModels.begin();
		while (it != intersectedModels.end())
		{
			if (FilterForModels(*it))
				it++;
			else
				it = intersectedModels.erase(it);
		}
	}

	return intersectedModels;
}

#include "stdafx.h"

// General
#include "SceneFinder.h"



namespace
{
	void FillIntersectedModelsMap(const std::shared_ptr<ISceneNode>& Owner, const Ray& Ray, std::map<float, std::shared_ptr<IModel>> * intersectedModels)
	{
		auto modelsComponent = Owner->GetComponent<IModelsComponent3D>();
		if (modelsComponent == nullptr)
			return;

		auto model = modelsComponent->GetModel();
		if (model == nullptr)
			return;

		auto bounds = model->GetBounds();
		if (bounds.IsInfinite())
			return;

		auto worldMatrix = Owner->GetWorldTransfom();

		BoundingBox worldBounds = bounds;
		bounds.transform(worldMatrix);

		if (HitBoundingBox(worldBounds.getMin(), worldBounds.getMax(), Ray.GetOrigin(), Ray.GetDirection()))
		{
			float dist = nearestDistToAABB(Ray.GetOrigin(), worldBounds);
			intersectedModels->insert(std::make_pair(dist, model));
		}
	}

	void FillIntersectedModelsList(const std::shared_ptr<ISceneNode>& Owner, const Frustum& Frustum, std::vector<std::shared_ptr<IModel>> * intersectedModels)
	{
		auto modelsComponent = Owner->GetComponent<IModelsComponent3D>();
		if (modelsComponent == nullptr)
			return;

		auto bounds = modelsComponent->GetModel()->GetBounds();
		if (modelsComponent->GetModel()->GetBounds().IsInfinite())
			return;

		auto worldMatrix = Owner->GetWorldTransfom();

		BoundingBox worldBounds = bounds;
		bounds.transform(worldMatrix);

		if (!Frustum.cullBox(worldBounds))
			intersectedModels->push_back(modelsComponent->GetModel());
	}

	void FillIntersectedSceneNodesList(const std::shared_ptr<ISceneNode>& Parent, const Frustum& Frustum, std::vector<std::shared_ptr<ISceneNode>> * intersectedNodes)
	{
		const auto& childs = Parent->GetChilds();
		for (const auto& it : childs)
		{
			FillIntersectedSceneNodesList(it, Frustum, intersectedNodes);

			if (auto collider = it->GetComponent<IColliderComponent3D>())
			{
				if (collider->GetBounds().IsInfinite())
					continue;

				if (!Frustum.cullBox(collider->GetWorldBounds()))
					intersectedNodes->push_back(it);
			}
		}
	}

	void FillIntersectedSceneNodesMap(const std::shared_ptr<ISceneNode>& Parent, const Ray & Ray, std::map<float, std::shared_ptr<ISceneNode>> * intersectedNodes)
	{
		const auto& childs = Parent->GetChilds();
		for (const auto& it : childs)
		{
			FillIntersectedSceneNodesMap(it, Ray, intersectedNodes);

			if (auto collider = it->GetComponent<IColliderComponent3D>())
			{
				if (collider->GetBounds().IsInfinite())
					continue;

				if (!collider->IsRayIntersects(Ray))
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

std::map<float, std::shared_ptr<ISceneNode>> CSceneFinder::FindIntersection(const Ray& Ray, std::function<bool(std::shared_ptr<ISceneNode>)> Filter, std::shared_ptr<ISceneNode> RootForFinder) const
{
	std::map<float, std::shared_ptr<ISceneNode>> intersectedNodes;
	FillIntersectedSceneNodesMap(RootForFinder ? RootForFinder : m_Scene.GetRootNode3D(), Ray, &intersectedNodes);

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
	FillIntersectedSceneNodesList(RootForFinder ? RootForFinder : m_Scene.GetRootNode3D(), Frustum, &intersectedNodes);
	
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

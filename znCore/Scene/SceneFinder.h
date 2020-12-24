#pragma once

class ZN_API CSceneFinder
	: public ISceneFinder
{
public:
	CSceneFinder(const IScene& Scene);
	virtual ~CSceneFinder();

	// ISceneFinder
	std::shared_ptr<ISceneNode> FindNodeByGuid(
		Guid Guid,
		std::shared_ptr<ISceneNode> RootForFinder
	) const override;

	std::map<float, std::shared_ptr<ISceneNode>> FindNearestNodes(
		glm::vec3 Position, 
		float Distance,
		std::function<bool(std::shared_ptr<ISceneNode>)> Filter,
		std::shared_ptr<ISceneNode> RootForFinder
	) const override;

	std::map<float, std::shared_ptr<ISceneNode>> FindIntersection(
		const Ray& Ray, 
		std::function<bool(std::shared_ptr<ISceneNode>)> Filter, 
		std::shared_ptr<ISceneNode> RootForFinder
	) const override;
	std::vector<std::shared_ptr<ISceneNode>> FindIntersections(
		const Frustum& Frustum, 
		std::function<bool(std::shared_ptr<ISceneNode>)> Filter, 
		std::shared_ptr<ISceneNode> RootForFinder
	) const override;

	std::map<float, std::shared_ptr<IModel>> FindIntersectionWithModel(
		const Ray& Ray,
		std::function<bool(std::shared_ptr<ISceneNode>)> FilterForSceneNodes = nullptr,
		std::function<bool(std::shared_ptr<IModel>)> FilterForModels = nullptr,
		std::shared_ptr<ISceneNode> RootForFinder = nullptr
	) const override;
	std::vector<std::shared_ptr<IModel>> FindIntersectionsWithModel(
		const Frustum& Frustum,
		std::function<bool(std::shared_ptr<ISceneNode>)> FilterForSceneNodes = nullptr,
		std::function<bool(std::shared_ptr<IModel>)> FilterForModels = nullptr,
		std::shared_ptr<ISceneNode> RootForFinder = nullptr
	) const override;

private:
	const IScene& m_Scene;
};
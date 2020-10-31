#pragma once

class ZN_API CSceneFinder
	: public ISceneFinder
{
public:
	CSceneFinder(const IScene& Scene);
	virtual ~CSceneFinder();

	// ISceneFinder
	std::map<float, std::shared_ptr<ISceneNode3D>> FindIntersection(
		const Ray& Ray, 
		std::function<bool(std::shared_ptr<ISceneNode3D>)> Filter, 
		std::shared_ptr<ISceneNode3D> RootForFinder
	) const override;
	std::vector<std::shared_ptr<ISceneNode3D>> FindIntersections(
		const Frustum& Frustum, 
		std::function<bool(std::shared_ptr<ISceneNode3D>)> Filter, 
		std::shared_ptr<ISceneNode3D> RootForFinder
	) const override;

	std::map<float, std::shared_ptr<IModel>> FindIntersectionWithModel(
		const Ray& Ray,
		std::function<bool(std::shared_ptr<ISceneNode3D>)> FilterForSceneNodes = nullptr,
		std::function<bool(std::shared_ptr<IModel>)> FilterForModels = nullptr,
		std::shared_ptr<ISceneNode3D> RootForFinder = nullptr
	) const override;
	std::vector<std::shared_ptr<IModel>> FindIntersectionsWithModel(
		const Frustum& Frustum,
		std::function<bool(std::shared_ptr<ISceneNode3D>)> FilterForSceneNodes = nullptr,
		std::function<bool(std::shared_ptr<IModel>)> FilterForModels = nullptr,
		std::shared_ptr<ISceneNode3D> RootForFinder = nullptr
	) const override;

private:
	const IScene& m_Scene;
};
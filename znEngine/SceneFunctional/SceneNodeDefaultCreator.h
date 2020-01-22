#pragma once

class CSceneNodeDefaultCreator
	: public ISceneNodeCreator
{
public:
	CSceneNodeDefaultCreator(const IBaseManager* BaseManager);
	virtual ~CSceneNodeDefaultCreator();

	// ISceneNodeCreator
	size_t GetSceneNodesCount() const override;
	std::string GetSceneNodeTypeName(size_t Index) const override;
	std::shared_ptr<ISceneNode3D> CreateSceneNode(std::weak_ptr<ISceneNode3D> Parent, size_t Index) const override;

private:
	const IBaseManager* m_BaseManager;
};
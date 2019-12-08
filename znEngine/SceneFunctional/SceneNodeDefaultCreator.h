#pragma once

class CSceneNodeDefaultCreator
	: public ISceneNodeCreator
{
public:
	CSceneNodeDefaultCreator(IBaseManager* BaseManager);
	virtual ~CSceneNodeDefaultCreator();

	// ISceneNodeCreator
	size_t GetSceneNodesCount() const override;
	std::string GetSceneNodeTypeName(size_t Index) const override;
	std::shared_ptr<ISceneNode> CreateSceneNode(std::weak_ptr<ISceneNode> Parent, size_t Index) const override;

private:
	IBaseManager* m_BaseManager;
};
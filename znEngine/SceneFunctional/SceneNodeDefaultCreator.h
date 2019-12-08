#pragma once

class CSceneNodeDefaultCreator
	: public ISceneNodeCreator
{
public:
	CSceneNodeDefaultCreator();
	virtual ~CSceneNodeDefaultCreator();

	// ISceneNodeCreator
	size_t GetSceneNodesCount() const;
	std::string GetSceneNodeTypeName(size_t Index) const;
	std::shared_ptr<ISceneNode> CreateSceneNode(std::shared_ptr<ISceneNode> Parent, size_t Index) const;

private:

};
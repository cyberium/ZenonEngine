#pragma once

//#define ZN_FBX_SDK_ENABLE

class CSceneNodeDefaultCreator
	: public ISceneNodeCreator
{
public:
	CSceneNodeDefaultCreator(const IBaseManager* BaseManager);
	virtual ~CSceneNodeDefaultCreator();

	// ISceneNodeCreator
	size_t GetSceneNodesCount() const override;
	std::string GetSceneNodeTypeName(size_t Index) const override;
	ISceneNode3D* CreateSceneNode3D(ISceneNode3D* Parent, size_t Index) const override;
	ISceneNodeUI* CreateSceneNodeUI(ISceneNodeUI* Parent, size_t Index) const override;

private:
	const IBaseManager* m_BaseManager;
};
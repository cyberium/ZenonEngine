#pragma once


enum class ETreeViewItemType
{
	Root = 0,

	VirtualFolder,
	
	Texture,
	Material,
	Geometry,
	Model,
	CSceneNode,

	SceneNodeProto
};


ZN_INTERFACE IznTreeViewItemSource
{
	virtual ~IznTreeViewItemSource() {}

	virtual ETreeViewItemType                      GetType() const = 0;
	virtual std::string                            GetName() const = 0;
	virtual size_t                                 GetChildsCount() const = 0;
	virtual std::shared_ptr<IznTreeViewItemSource> GetChild(size_t Index) const = 0;
	virtual std::shared_ptr<IObject>               Object() const = 0;
};

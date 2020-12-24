#pragma once


enum class ETreeViewItemType
{
	Root = 0,

	VirtualFolder,
	
	Texture,
	Material,
	Geometry,
	Model,
	ParticleSystem,

	SceneNode,

	SceneNodeProto
};


ZN_INTERFACE IznTreeViewItem
{
	virtual ~IznTreeViewItem() {}

	virtual ETreeViewItemType                      GetType() const = 0;
	virtual std::string                            GetText() const = 0;
	virtual std::string                            GetIconName() const = 0;
	virtual size_t                                 GetChildsCount() const = 0;
	virtual std::shared_ptr<IznTreeViewItem>       GetChild(size_t Index) const = 0;
	virtual const IznTreeViewItem*                 GetParent() const = 0;
	virtual size_t                                 GetMyIndexInParent() const = 0;
	virtual std::shared_ptr<IObject>               GetObject_() const = 0;
};


ZN_INTERFACE IznTreeViewItemFolder
{
	virtual ~IznTreeViewItemFolder() {}

	virtual void AddChild(std::shared_ptr<IznTreeViewItem> Child) = 0;
	virtual void ClearChilds() = 0;
};


ZN_INTERFACE IznTreeViewItemInternal
{
	virtual ~IznTreeViewItemInternal() {}

	virtual void                                   SetParent(IznTreeViewItem * Parent) = 0;
	virtual void                                   ClearCache() = 0;
};

#pragma once

#include "EditorInterfaces.h"
#include "ZenonWidgets/ZenonTreeView/TreeViewIntfs.h"

class CznModel3DTreeViewItemSource
	: public IznTreeViewItemSource
	, public CLoadableObject
{
public:
	CznModel3DTreeViewItemSource(IBaseManager& BaseManager, std::string FileName);
	virtual ~CznModel3DTreeViewItemSource();

	// IznTreeViewItemSource
	ETreeViewItemType GetType() const;
	std::string GetName() const override;
	size_t GetChildsCount() const;
	std::shared_ptr<IznTreeViewItemSource> GetChild(size_t Index) const;
	std::shared_ptr<IObject> Object() const;

	// ILoadableObject
	bool Load() override;
	bool Delete() override;

protected:
	std::shared_ptr<IFile> ConvertFBXToZNMDL(const std::string& FbxFilename);

protected:
	IBaseManager& GetBaseManager() const;

private:
	std::string m_FileName;
	std::shared_ptr<IModel> m_Model;
	IBaseManager& m_BaseManager;
};
#pragma once

#include "ZenonWidgets/ZenonTreeView/TreeViewItem.h"

class CTextureTreeViewItem
	: public CznTreeViewItem
	, public CLoadableObject
{
public:
	CTextureTreeViewItem(IBaseManager& BaseManager, std::string TextureFileName);
	virtual ~CTextureTreeViewItem();

	// IznTreeViewItem
	ETreeViewItemType GetType() const override;
	std::string GetText() const override;
	std::string GetIconName() const override;
	std::shared_ptr<IObject> GetObject_() const override;

	// ILoadableObject
	bool Load() override;
	bool Delete() override;

private:
	Utils::SFileName m_TextureFileName;
	std::shared_ptr<ITexture> m_Texture;
	IBaseManager& m_BaseManager;
};
#include "stdafx.h"

// General
#include "MaterialTreeViewItem.h"

CMaterialTreeViewItem::CMaterialTreeViewItem(const std::shared_ptr<IMaterial>& Material)
	: m_Material(Material)
{
}

CMaterialTreeViewItem::~CMaterialTreeViewItem()
{
}



//
// IznTreeViewItem
//
ETreeViewItemType CMaterialTreeViewItem::GetType() const
{
	return ETreeViewItemType::Material;
}

std::string CMaterialTreeViewItem::GetText() const
{
	return Utils::SplitFilename(m_Material->GetName()).NameWithoutExtension;
}

std::string CMaterialTreeViewItem::GetIconName() const
{
	return "node";
}

std::shared_ptr<IObject> CMaterialTreeViewItem::GetObject_() const
{
	return m_Material;
}

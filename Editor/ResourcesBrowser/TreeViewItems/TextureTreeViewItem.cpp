#include "stdafx.h"

// General
#include "TextureTreeViewItem.h"

CTextureTreeViewItem::CTextureTreeViewItem(IBaseManager& BaseManager, std::string TextureFileName)
	: m_TextureFileName(Utils::SplitFilename(TextureFileName))
	, m_BaseManager(BaseManager)
{
}

CTextureTreeViewItem::~CTextureTreeViewItem()
{
}



//
// IznTreeViewItem
//
ETreeViewItemType CTextureTreeViewItem::GetType() const
{
	return ETreeViewItemType::Texture;
}

std::string CTextureTreeViewItem::GetText() const
{
	if (GetState() != ILoadable::ELoadableState::Loaded)
		return m_TextureFileName.ToString();
	if (m_Texture == nullptr)
		return "Error. " + m_TextureFileName.NameWithoutExtension;
	return m_TextureFileName.NameWithoutExtension;
}

std::string CTextureTreeViewItem::GetIconName() const
{
	if (m_TextureFileName.Extension == "dds")
		return "file-texture-dds";
	else if (m_TextureFileName.Extension == "png")
		return "file-texture-png";
	return "";
}

std::shared_ptr<IObject> CTextureTreeViewItem::GetObject_() const
{
	if (GetState() != ILoadable::ELoadableState::Loaded)
		return nullptr;
	return m_Texture;
}



//
// ILoadable
//
bool CTextureTreeViewItem::Load()
{
	m_Texture = m_BaseManager.GetManager<IznTexturesFactory>()->LoadTexture2D(m_TextureFileName.ToString());
	return true;
}

bool CTextureTreeViewItem::Delete()
{
	return false;
}

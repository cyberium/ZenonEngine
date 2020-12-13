#include "stdafx.h"

// General
#include "TextureTreeViewItem.h"

CTextureTreeViewItem::CTextureTreeViewItem(const std::shared_ptr<ITexture>& Texture)
	: m_Texture(Texture)
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
	return Utils::SplitFilename(m_Texture->GetFilename()).NameWithoutExtension;
}

std::string CTextureTreeViewItem::GetIconName() const
{
	Utils::SFileName fileNameStruct = Utils::SplitFilename(m_Texture->GetFilename());
	if (fileNameStruct.Extension == "dds")
		return "file-texture-dds";
	else if (fileNameStruct.Extension == "png")
		return "file-texture-png";
	return "";
}

std::shared_ptr<IObject> CTextureTreeViewItem::GetObject_() const
{
	return m_Texture;
}

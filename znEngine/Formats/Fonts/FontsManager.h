#pragma once

#include "Font.h"

class ZN_API FontsManager 
	: public IznFontsManager
{
public:
	FontsManager(IRenderDevice& RenderDevice, IBaseManager& BaseManager);
	virtual ~FontsManager();

	// IFontsManager
	std::shared_ptr<IznFont> GetMainFont() const override;
	std::shared_ptr<IznFont> Add(IRenderDevice& RenderDevice, const std::string& _fontFileName, uint32 _fontSize) override;

private:
	std::shared_ptr<IznFont> m_MainFont;

private:
	IBaseManager& m_BaseManager;
};

#pragma once

#include "Font.h"

class ZN_API FontsManager 
	: public IFontsManager
{
public:
	FontsManager(IRenderDevice& RenderDevice, IBaseManager& BaseManager);
	virtual ~FontsManager();

	// IFontsManager
	std::shared_ptr<CFont> GetMainFont() const override;
	std::shared_ptr<CFont> Add(IRenderDevice& RenderDevice, const std::string& _fontFileName, uint32 _fontSize) override;
	bool Exists(const std::string& name) const { return false; }
	void Delete(const std::string& name) {  }
	void Delete(std::shared_ptr<CFont> item) { }

private:
	std::shared_ptr<CFont> m_MainFont;

private:
	IBaseManager& m_BaseManager;
};

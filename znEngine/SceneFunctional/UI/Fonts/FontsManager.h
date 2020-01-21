#pragma once

#include "Font.h"

class ZN_API FontsManager : public CRefManager1Dim<CFontMesh>, public IFontsManager
{
public:
	FontsManager(IRenderDevice* RenderDevice, IBaseManager* BaseManager);
	virtual ~FontsManager();

	// IFontsManager
	std::shared_ptr<CFontMesh> GetMainFont() const override;
	std::shared_ptr<CFontMesh> Add(const std::string& _fontFileName, uint32 _fontSize);

	// CRefManager1Dim
	std::shared_ptr<CFontMesh> CreateAction(const std::string& name) override;
	bool DeleteAction(const std::string& name) override;

	// IFontsManager
	std::shared_ptr<CFontMesh> Add(const std::string& name) { return CRefManager1Dim::Add(name); }
	bool Exists(const std::string& name) const { return CRefManager1Dim::Exists(name); }
	void Delete(const std::string& name) { CRefManager1Dim::Delete(name); }
	void Delete(std::shared_ptr<CFontMesh> item) { CRefManager1Dim::Delete(item); }

private:
	std::shared_ptr<CFontMesh> m_MainFont;

private:
	IRenderDevice* m_RenderDevice;
	IBaseManager* m_BaseManager;
};

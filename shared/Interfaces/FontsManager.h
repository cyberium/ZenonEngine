#pragma once

#include "BaseManager.h"

// FORWARD BEGIN
ZN_INTERFACE ITexture;
ZN_INTERFACE IGeometry;
// FORWARD END


ZN_INTERFACE ZN_API IznFont
{
	virtual ~IznFont() {}

	virtual std::shared_ptr<ITexture> GetTexture() const = 0;
	virtual std::shared_ptr<IGeometry> GetGeometry() const = 0;
	virtual uint32 GetCharWidth(char Char) const = 0;
	virtual uint32 GetStringWidth(const std::string& String) const = 0;
	virtual uint32 GetHeight() const = 0;
};


ZN_INTERFACE ZN_API
	__declspec(uuid("1427E242-CCB8-4AEC-ABC8-17DE58A96B05"))
	IznFontsManager : public IManager
{
	virtual ~IznFontsManager() {};

	virtual std::shared_ptr<IznFont> GetMainFont() const = 0;
	virtual std::shared_ptr<IznFont> Add(IRenderDevice& RenderDevice, const std::string& FontFileName, uint32 FontSize) = 0;
};

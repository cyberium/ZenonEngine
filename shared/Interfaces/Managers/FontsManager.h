#pragma once

// FORWARD BEGIN
ZN_INTERFACE IManager;
ZN_INTERFACE ITexture;
ZN_INTERFACE IGeometry;
// FORWARD END


ZN_INTERFACE ZN_API IznFont
{
	virtual ~IznFont() {}

	virtual std::shared_ptr<ITexture> GetTexture() const = 0;
	virtual std::shared_ptr<IGeometry> GetGeometry() const = 0;
	virtual uint32 GetWidth(char Char) const = 0;
	virtual uint32 GetWidth(const std::string& String) const = 0;
	virtual uint32 GetHeight() const = 0;
};


ZN_INTERFACE ZN_API	IznFontsManager 
	: public IManager
{
	ZN_OBJECTCLASS(cFontsManager)

	virtual ~IznFontsManager() {};

	virtual std::shared_ptr<IznFont> GetMainFont() const = 0;
	virtual std::shared_ptr<IznFont> Add(IRenderDevice& RenderDevice, const std::string& FontFileName, uint32 FontSize) = 0;
};

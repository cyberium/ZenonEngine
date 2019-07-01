#pragma once

class OW_ENGINE_API MaterialDX11 : public MaterialImpl
{
public:
	MaterialDX11(IRenderDevice* renderDevice);
	virtual ~MaterialDX11();
};

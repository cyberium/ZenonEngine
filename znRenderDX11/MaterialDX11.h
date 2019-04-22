#pragma once

class MaterialDX11 : public MaterialImpl
{
public:
	MaterialDX11(IRenderDevice* renderDevice);
	virtual ~MaterialDX11();
};

#pragma once


// A material class is used to wrap the shaders and to manage the shader parameters.
class MaterialDX11 : public MaterialImpl
{
public:
	MaterialDX11(IRenderDevice* renderDevice);
	virtual ~MaterialDX11();
};
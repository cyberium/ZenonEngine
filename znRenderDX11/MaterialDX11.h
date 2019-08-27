#pragma once

class OW_ENGINE_API MaterialDX11 : public MaterialImpl
{
public:
	MaterialDX11(IRenderDevice* renderDevice, size_t Size);
	virtual ~MaterialDX11();

    // MaterialImpl
    void Bind() const override final;
    void Unbind() const override final;
};

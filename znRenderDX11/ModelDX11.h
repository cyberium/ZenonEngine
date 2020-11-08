#pragma once

class ZN_API ModelDX11
	: public ModelBase
{
public:
	ModelDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual	~ModelDX11();

	// IModel
	bool Render() const override final;
};
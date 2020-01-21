#pragma once

class ZN_API GeometryDX11 
	: public GeometryBase
{
public:
	GeometryDX11(IRenderDeviceDX11* RenderDeviceD3D11);
	virtual	~GeometryDX11();

	virtual void                                    SetPrimitiveTopology(PrimitiveTopology Topology) override;

	virtual bool                                    Render(const RenderEventArgs* renderArgs, const IConstantBuffer* PerObject, const std::unordered_map<EShaderType, IShader*>& ShadersMap, const IMaterial* Material, const SGeometryPartParams& GeometryPartParams) const override;
	virtual bool                                    RenderInstanced(const RenderEventArgs* renderArgs, const IStructuredBuffer* InstancesBuffer, const std::unordered_map<EShaderType, IShader*>& ShadersMap, const IMaterial* Material, SGeometryPartParams GeometryPartParams) const override;

private:
	D3D11_PRIMITIVE_TOPOLOGY                        m_PrimitiveTopology;

private: // Link to parent d3d11 device
	IRenderDeviceDX11* m_RenderDeviceD3D11;
};
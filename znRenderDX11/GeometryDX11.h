#pragma once

class ZN_API GeometryDX11 
	: public GeometryBase
{
public:
	                                                GeometryDX11(ID3D11Device2 * pDevice);
	virtual                                         ~GeometryDX11();

	virtual void                                    SetPrimitiveTopology(PrimitiveTopology Topology) override;

	virtual bool                                    Render(const RenderEventArgs* renderArgs, const IConstantBuffer* PerObject, const std::unordered_map<SShaderType, std::shared_ptr<IShader>>& ShadersMap, const IMaterial* Material, const SGeometryPartParams& GeometryPartParams) const override;
	virtual bool                                    RenderInstanced(const RenderEventArgs* renderArgs, const IStructuredBuffer* InstancesBuffer, const std::unordered_map<SShaderType, std::shared_ptr<IShader>>& ShadersMap, const IMaterial* Material, SGeometryPartParams GeometryPartParams) const override;

private:
	D3D11_PRIMITIVE_TOPOLOGY                        m_PrimitiveTopology;

	// DirectX
	ATL::CComPtr<ID3D11Device2>                     m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2>              m_pDeviceContext;
};
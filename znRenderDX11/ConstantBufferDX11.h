#pragma once

class ConstantBufferDX11 : public ConstantBuffer
{
public:
	ConstantBufferDX11(ID3D11Device2* pDevice, size_t size);
	virtual ~ConstantBufferDX11();

	virtual bool Bind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType);
	virtual void UnBind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType);

	virtual void Copy(std::shared_ptr<ConstantBuffer> other) override;

protected:
	virtual void Copy(std::shared_ptr<IBuffer> other) override;
	void Set(const void* data, size_t size) override;

private:
	ATL::CComPtr<ID3D11Device2> m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2> m_pDeviceContext;
	ATL::CComPtr<ID3D11Buffer> m_pBuffer;

	size_t  m_BufferSize;
};
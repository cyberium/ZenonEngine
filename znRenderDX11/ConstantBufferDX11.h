#pragma once

class ConstantBufferDX11 : public IConstantBuffer
{
public:
	ConstantBufferDX11(IRenderDeviceDX11* RenderDeviceD3D11, size_t size);
	virtual ~ConstantBufferDX11();

	virtual bool Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;
	virtual void UnBind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;

	virtual BufferType GetType() const override;
	virtual uint32 GetElementCount() const override;
	virtual uint32 GetElementStride() const override;
	virtual uint32 GetElementOffset() const override;

	virtual void Copy(IConstantBuffer* other) const override;

protected:
	virtual void Copy(IBuffer* other) const override;
	void Set(const void* data, size_t size) override;

private:
	ATL::CComPtr<ID3D11Buffer> m_pBuffer;
	size_t m_BufferSize;

private: // Link to parent
	IRenderDeviceDX11* m_RenderDeviceD3D11;
};
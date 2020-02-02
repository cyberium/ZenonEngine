#pragma once

class ConstantBufferDX11 
	: public IConstantBuffer
{
public:
	ConstantBufferDX11(IRenderDeviceDX11& RenderDeviceDX11, size_t size);
	virtual ~ConstantBufferDX11();

	// IBuffer
	bool Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;
	void UnBind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;
	void Copy(const IBuffer* other) const override;
	BufferType GetType() const override;
	uint32 GetElementCount() const override;
	uint32 GetElementStride() const override;
	uint32 GetElementOffset() const override;

	// IConstantBuffer
	void Copy(const IConstantBuffer* other) const override;
	void Set(const void* data, size_t size) override;

private:
	ATL::CComPtr<ID3D11Buffer> m_pBuffer;
	size_t m_BufferSize;

private: // Link to parent
	IRenderDeviceDX11& m_RenderDeviceDX11;
};
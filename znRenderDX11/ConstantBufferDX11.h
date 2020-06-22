#pragma once

class ZN_API ConstantBufferDX11
	: public CBufferBase
	, public IConstantBuffer
{
public:
	ConstantBufferDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual ~ConstantBufferDX11();

	// IBuffer
	bool Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;
	void UnBind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;
	void Copy(const IBuffer* other) const override;
	BufferType GetType() const override { return CBufferBase::GetType(); }
	uint32 GetElementCount() const override { return CBufferBase::GetElementCount(); }
	uint32 GetElementStride() const override { return CBufferBase::GetElementStride(); };
	uint32 GetElementOffset() const override { return CBufferBase::GetElementOffset(); };

	// IConstantBuffer
	void Copy(const IConstantBuffer* other) const override;
	void Set(const void* data, size_t size) override;

protected:
	void DoInitializeBuffer() override;

private:
	ATL::CComPtr<ID3D11Buffer> m_pBuffer;

private: // Link to parent
	IRenderDeviceDX11& m_RenderDeviceDX11;
};
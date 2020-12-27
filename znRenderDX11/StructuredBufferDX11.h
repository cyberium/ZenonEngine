#pragma once

class ZN_API StructuredBufferDX11
	: public CStructuredBufferBase
{
public:
	StructuredBufferDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual ~StructuredBufferDX11();

	// IBuffer
	bool Bind(uint32 id, const IShader* shader, IShaderParameter::EType parameterType) const override;
	void UnBind(uint32 id, const IShader* shader, IShaderParameter::EType parameterType) const override;
	void Copy(const IBuffer* other) const override;

	// IStructuredBuffer
	virtual void Copy(const IStructuredBuffer* other) override;
	virtual void Set(const void* data, size_t elementSize, size_t numElements) override;
	virtual void Clear() override;

	// IStructuredBufferDX11
	ID3D11UnorderedAccessView* GetUnorderedAccessView() const;

	// IStructuredBufferPrivate
	void DoInitializeStructuredBuffer() override;

private:
	void Commit() const;

private:
	ATL::CComPtr<ID3D11Buffer> m_pBuffer;
	ATL::CComPtr<ID3D11ShaderResourceView> m_pSRV;
	ATL::CComPtr<ID3D11UnorderedAccessView> m_pUAV;
	mutable bool m_bIsDirty;

private: // Link to parent d3d11 device

	IRenderDeviceDX11& m_RenderDeviceDX11;
};

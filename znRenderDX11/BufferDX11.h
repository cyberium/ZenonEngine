#pragma once

class ZN_API BufferDX11 
	: public CBufferBase
{
public:
	BufferDX11(IRenderDeviceDX11& RenderDeviceDX11, IBuffer::BufferType BufferType);
	virtual ~BufferDX11();

	// IBuffer
	bool Bind(uint32 id, const IShader* shader, IShaderParameter::EType parameterType) const override;
	void UnBind(uint32 id, const IShader* shader, IShaderParameter::EType parameterType) const override;
	void Copy(const IBuffer* other) const override;

	// IBufferPrivate
	void DoInitializeBuffer() override;

private:
	ATL::CComPtr<ID3D11Buffer> m_pBuffer;

private: // Link to parent
	IRenderDeviceDX11& m_RenderDeviceDX11;
};

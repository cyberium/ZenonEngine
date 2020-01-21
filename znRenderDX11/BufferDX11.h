#pragma once

class BufferDX11 : public IBuffer
{
public:
	BufferDX11(IRenderDeviceDX11* RenderDeviceD3D11, UINT bindFlags, const void* data, size_t count, UINT offset, UINT stride);
	~BufferDX11();

	virtual bool Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;
	virtual void UnBind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;

	virtual void Copy(IBuffer* other) const override;

	virtual BufferType GetType() const override;
	virtual uint32 GetElementCount() const override;
	virtual uint32 GetElementStride() const override;
    virtual uint32 GetElementOffset() const override;

private:
	ATL::CComPtr<ID3D11Buffer> m_pBuffer;

	UINT m_uiOffset; // The offset of the vertex buffer in bytes.
	UINT m_uiStride; // The stride of the vertex buffer in bytes.
	UINT m_BindFlags; // How this buffer should be bound.
	UINT m_uiCount; // The number of elements in this buffer.

private: // Link to parent
	IRenderDeviceDX11* m_RenderDeviceD3D11;
};

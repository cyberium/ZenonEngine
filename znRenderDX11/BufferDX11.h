#pragma once

class BufferDX11 : public IBuffer
{
public:
	BufferDX11(ID3D11Device2* pDevice, UINT bindFlags, const void* data, size_t count, UINT offset, UINT stride);
	~BufferDX11();

	virtual bool Bind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType);
	virtual void UnBind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType);

	virtual void Copy(std::shared_ptr<IBuffer> other);

	virtual BufferType GetType() const;
	virtual uint32 GetElementCount() const;
	virtual uint32 GetElementStride() const;

private:
	ATL::CComPtr<ID3D11Device2> m_pDevice;
	ATL::CComPtr<ID3D11DeviceContext2> m_pDeviceContext;

	ATL::CComPtr<ID3D11Buffer> m_pBuffer;

	// The offset of the vertex buffer in bytes.
	UINT m_uiOffset;

	// The stride of the vertex buffer in bytes.
	UINT m_uiStride;

	// How this buffer should be bound.
	UINT m_BindFlags;

	// The number of elements in this buffer.
	UINT m_uiCount;

	bool m_bIsBound;
};

#pragma once

class BufferOGL : public IBuffer
{
public:
	BufferOGL(UINT bindFlags, const void* data, size_t count, UINT offset, UINT stride);
	~BufferOGL();

	virtual bool Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;
	virtual void UnBind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;

	virtual void Copy(std::shared_ptr<IBuffer> other);

	virtual BufferType GetType() const;
	virtual uint32 GetElementCount() const;
	virtual uint32 GetElementStride() const;
    virtual uint32 GetElementOffset() const;

private:
	uint32  m_GLObj;

private:
	UINT m_BindFlags; // How this buffer should be bound.
	UINT m_uiCount; // The number of elements in this buffer.
 
    UINT m_uiOffset;       // The offset of the vertex buffer in bytes.
	UINT m_uiStride; // The stride of the vertex buffer in bytes.

	bool m_bIsBound;
};

#pragma once

class BufferOGL : public IBuffer
{
public:
	BufferOGL(UINT bindFlags, const void* data, size_t count, UINT stride);
	~BufferOGL();

	// Bind the buffer to a particular attribute ID or slot
	virtual bool Bind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType);
	virtual void UnBind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType);

	// Copy the contents of another buffer into this one. Buffers must be the same size (in bytes).
	virtual void Copy(std::shared_ptr<IBuffer> other);

	virtual BufferType GetType() const;
	virtual uint32 GetElementCount() const;
	virtual uint32 GetElementStride() const;

private:
	uint32  m_GLObj;

private:
	UINT m_BindFlags; // How this buffer should be bound.
	UINT m_uiCount; // The number of elements in this buffer.
	UINT m_uiStride; // The stride of the vertex buffer in bytes.

	bool m_bIsBound;
};

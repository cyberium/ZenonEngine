#pragma once

class StructuredBufferOGL : public StructuredBuffer
{
public:
	StructuredBufferOGL(UINT bindFlags, const void* data, size_t count, UINT stride, CPUAccess cpuAccess = CPUAccess::None, bool bUAV = false);
	virtual ~StructuredBufferOGL();

	virtual bool Bind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType);
	virtual void UnBind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType);

	virtual BufferType GetType() const;
	virtual uint32 GetElementCount() const;
	virtual uint32 GetElementStride() const;

	virtual void Copy(std::shared_ptr<StructuredBuffer> other);

	virtual void Clear(); // Clear the contents of the buffer.

protected:
	virtual void Copy(std::shared_ptr<IBuffer> other);
	virtual void SetData(void* data, size_t elementSize, size_t offset, size_t numElements);

	void Commit(); // Commit the data from system memory to device memory.

private:
	GLuint  m_GLObj;

private:
	// The system data buffer.
	typedef std::vector<uint8_t> BufferType;
	BufferType m_Data;

	UINT m_BindFlags; // How this buffer should be bound.
	UINT m_uiCount;  // The number of elements in this buffer.
	UINT m_uiStride; // The stride of the vertex buffer in bytes.
	UINT m_uiSlot; // The last slot the UAV was bound to.

	bool m_bIsDirty; // Marked dirty if the contents of the buffer differ from what is stored on the GPU.
	//bool m_bUAV; // Does this buffer require GPU write access  If so, it must be bound as a UAV instead of an SRV.

	bool m_bDynamic; // Requires CPU read/write access.
	CPUAccess m_CPUAccess;
};

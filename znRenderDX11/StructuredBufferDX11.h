#pragma once

class StructuredBufferDX11 
	: public IStructuredBuffer
{
public:
	StructuredBufferDX11(IRenderDeviceDX11& RenderDeviceDX11, UINT bindFlags, const void* data, size_t count, UINT stride, CPUAccess cpuAccess = CPUAccess::None, bool bUAV = false);
	virtual ~StructuredBufferDX11();

	// IBuffer
	bool Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;
	void UnBind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;
	void Copy(const IBuffer* other) const override;
	BufferType GetType() const override;
	uint32 GetElementCount() const override;
	uint32 GetElementStride() const override;
    uint32 GetElementOffset() const override;

	// IStructuredBuffer
	virtual void Copy(const IStructuredBuffer* other) override;
	virtual void Set(void* data, size_t elementSize, size_t numElements) override;
	virtual void Clear() override;

	// IStructuredBufferDX11
	ID3D11UnorderedAccessView* GetUnorderedAccessView() const;

protected:
	void Commit() const;

private:
	ATL::CComPtr<ID3D11Buffer> m_pBuffer;
	ATL::CComPtr<ID3D11ShaderResourceView> m_pSRV;
	ATL::CComPtr<ID3D11UnorderedAccessView> m_pUAV;

	// The system data buffer.
	typedef std::vector<uint8_t> BufferType;
	BufferType m_Data;

	// The stride of the vertex buffer in bytes.
	UINT m_uiStride;

	// How this buffer should be bound.
	UINT m_BindFlags;

	// The number of elements in this buffer.
	UINT m_uiCount;

	// The last slot the UAV was bound to.
	UINT m_uiSlot;

	// Marked dirty if the contents of the buffer differ from what is stored on the GPU.
	mutable bool m_bIsDirty;
	// Does this buffer require GPU write access If so, it must be bound as a UAV instead of an SRV.
	bool m_bUAV;

	bool m_bDynamic; // Requires CPU read/write access.
	CPUAccess m_CPUAccess;

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};

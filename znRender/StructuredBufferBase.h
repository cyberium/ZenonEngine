#pragma once

#include "BufferBase.h"

ZN_INTERFACE IStructuredBufferPrivate
{
	virtual void InitializeStructuredBufferBase(EAccess EAccess) = 0;
	virtual void DoInitializeStructuredBuffer() = 0;
};

class ZN_API CStructuredBufferBase
	: public CBufferBase
	, public IStructuredBuffer
	, public IStructuredBufferPrivate
{
public:
	CStructuredBufferBase(IRenderDevice& RenderDevice);
	virtual ~CStructuredBufferBase();

protected:
	// IBuffer
	BufferType GetType() const override { return CBufferBase::GetType(); }
	uint32 GetElementCount() const override { return CBufferBase::GetElementCount(); }
	uint32 GetElementStride() const override { return CBufferBase::GetElementStride(); };
	uint32 GetElementOffset() const override { return CBufferBase::GetElementOffset(); };

	// IObjectLoadSave
	void Load(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;

	// IBufferPrivate
	void DoInitializeBuffer() override;

	// IStructuredBufferPrivate
	void InitializeStructuredBufferBase(EAccess EAccess) override final;
	void DoInitializeStructuredBuffer() = 0;

protected:
	EAccess GetAccess() const;
	bool IsDynamic() const;

private:
	EAccess m_Access;
};
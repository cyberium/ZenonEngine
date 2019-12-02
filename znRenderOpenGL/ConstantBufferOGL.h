#pragma once

class ConstantBufferOGL : public IConstantBuffer
{
public:
	ConstantBufferOGL(size_t size);
	virtual ~ConstantBufferOGL();

	virtual bool Bind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;
	virtual void UnBind(uint32 id, const IShader* shader, IShaderParameter::Type parameterType) const override;

	IBuffer::BufferType GetType() const;
	uint32 GetElementCount() const;
	uint32 GetElementStride() const;
	uint32 GetElementOffset() const;

	virtual void Copy(std::shared_ptr<IConstantBuffer> other) override;

protected:
	virtual void Copy(std::shared_ptr<IBuffer> other) override;
	void Set(const void* data, size_t size) override;

private:
	uint32  m_GLObj;

private:
	size_t  m_BufferSize;
};
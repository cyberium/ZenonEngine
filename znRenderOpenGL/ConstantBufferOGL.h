#pragma once

class ConstantBufferOGL : public ConstantBuffer
{
public:
	ConstantBufferOGL(size_t size);
	virtual ~ConstantBufferOGL();

	virtual bool Bind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType);
	virtual void UnBind(uint32 id, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType);

	virtual void Copy(std::shared_ptr<ConstantBuffer> other) override;

protected:
	virtual void Copy(std::shared_ptr<IBuffer> other) override;
	void Set(const void* data, size_t size) override;

private:
	uint32  m_GLObj;

private:
	size_t  m_BufferSize;
};
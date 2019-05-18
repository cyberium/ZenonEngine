#pragma once

class MaterialOGL : public MaterialImpl
{
public:
	MaterialOGL(IRenderDevice* renderDevice);
	virtual ~MaterialOGL();

	virtual void Bind() const;
	virtual void Unbind() const;

private: // OpenGL
	uint32 m_GLProgramPipeline;
};
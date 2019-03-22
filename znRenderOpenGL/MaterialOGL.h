#pragma once


// A material class is used to wrap the shaders and to manage the shader parameters.
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
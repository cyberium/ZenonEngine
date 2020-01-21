#pragma once

#include "BlendStateOGL.h"
#include "RasterizerStateOGL.h"
#include "DepthStencilStateOGL.h"

class PipelineStateOGL 
	: public PipelineStateBase
{
public:
	PipelineStateOGL();
	virtual ~PipelineStateOGL();

	// IPipelineState
	void SetShader(EShaderType type, IShader* pShader) override;

	void Bind() override;
	void UnBind() override;

private:
	uint32 m_GLProgramPipeline;
};
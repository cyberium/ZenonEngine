#pragma once

#include "ShaderInputLayoutOGL.h"

class ShaderOGL : public ShaderBase, public std::enable_shared_from_this<ShaderOGL>
{
public:
	ShaderOGL(IRenderDevice* RenderDevice);
	virtual ~ShaderOGL();

	// Shader loading
	bool LoadShaderFromString(EShaderType shaderType, const std::string& fileName, const std::string& source, const ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, IShaderInputLayout* _customLayout);
	bool LoadShaderFromFile(EShaderType shaderType, const std::string& fileName, const ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, IShaderInputLayout* _customLayout);

    bool LoadInputLayoutFromReflector() override final;
    bool LoadInputLayoutFromCustomElements(const std::vector<SCustomVertexElement>& declIn) override final;

	void Bind() const;
	void UnBind() const;

	void Dispatch(const glm::uvec3& numGroups);

public:
	uint32 GetGLObject() const;

protected:
	virtual void Destroy();

private:
	GLuint m_GLObj;
	bool GetShaderProgramLog(GLuint _obj, std::string * _errMsg);

	IRenderDevice* m_RenderDevice;
};

GLbitfield GLTranslateShaderBitType(EShaderType _type);

#pragma once

#include "ShaderInputLayoutOGL.h"

class ShaderOGL : public Shader, public std::enable_shared_from_this<ShaderOGL>
{
public:
	ShaderOGL();
	virtual ~ShaderOGL();

	// Shader loading
	bool LoadShaderFromString(ShaderType shaderType, const std::string& fileName, const std::string& source, const ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, std::shared_ptr<IShaderInputLayout> _customLayout);
	bool LoadShaderFromFile(ShaderType shaderType, const std::string& fileName, const ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, std::shared_ptr<IShaderInputLayout> _customLayout);

    bool LoadInputLayoutFromReflector() override final;
    bool LoadInputLayoutFromD3DElement(const std::vector<D3DVERTEXELEMENT9>& declIn) override final;

	void Bind();
	void UnBind();

	void Dispatch(const glm::uvec3& numGroups);

public:
	uint32 GetGLObject();

protected:
	virtual void Destroy();

private:
	GLuint m_GLObj;
	bool GetShaderProgramLog(GLuint _obj, std::string * _errMsg);
};

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

    std::shared_ptr<IShaderInputLayout> GetInputLayout() const override final;
	ShaderParameter& GetShaderParameterByName(const std::string& name) const;

	void Bind();
	void UnBind();

	void Dispatch(const glm::uvec3& numGroups);

public:
	uint32 GetGLObject();

protected:
	// Destroy the contents of this shader (in case we are loading a new shader).
	virtual void Destroy();

private:
	GLuint m_GLObj;
	bool GetShaderProgramLog(GLuint _obj, std::string * _errMsg);

private:
    std::shared_ptr<ShaderInputLayoutOGL> m_InputLayout;

	// Parameters necessary to reload the shader at runtime if it is modified on disc.
	ShaderMacros m_ShaderMacros;
	std::string m_EntryPoint;
	std::string m_Profile;
	std::string m_ShaderFileName;
};

#pragma once

class OW_ENGINE_API ShaderBase : public IShader, public Object
{
public:
	ShaderBase();
	virtual ~ShaderBase();

	// IShader
	ShaderType GetType() const override final;
	std::shared_ptr<IShaderInputLayout> GetInputLayout() const override final;
	std::shared_ptr<IShaderParameter> GetShaderParameterByName(const std::string& name) const override final;
	
protected:
    // Parameters necessary to reload the shader at runtime if it is modified on disc.
    ShaderMacros                        m_ShaderMacros;
    std::string                         m_EntryPoint;
    std::string                         m_Profile;
    std::string                         m_ShaderFileName;
	ShaderType	                        m_ShaderType;

    std::shared_ptr<IShaderInputLayout> m_InputLayout;

	ParameterMap                        m_ShaderParameters;
};

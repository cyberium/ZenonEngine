#pragma once

class ZN_API ShaderBase 
	: public IShader
{
public:
	ShaderBase();
	virtual ~ShaderBase();

	// IShader
	EShaderType GetShaderType() const override final;
	IShaderInputLayout& GetInputLayout() const override final;
	IShaderParameter& GetShaderParameterByName(const std::string& name) const override final;
	
protected:
    ShaderMacros                        m_ShaderMacros;
    std::string                         m_EntryPoint;
    std::string                         m_Profile;
    std::string                         m_ShaderFileName;
	EShaderType	                        m_ShaderType;

    std::shared_ptr<IShaderInputLayout> m_InputLayout;

	ParameterMap                        m_ShaderParameters;
};

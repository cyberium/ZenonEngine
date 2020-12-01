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
	IShaderParameter* GetShaderParameterByName(const std::string& name) const override final;
	
protected:
	EShaderType	                        m_Type;
	std::string                         m_FileName;
	std::string                         m_EntryPoint;
    ShaderMacros                        m_Macros;

    std::shared_ptr<IShaderInputLayout> m_InputLayout;
	ParameterMap                        m_ShaderParameters;
};

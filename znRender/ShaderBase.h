#pragma once

class ZN_API ShaderBase : public IShader, public Object
{
public:
	ShaderBase();
	virtual ~ShaderBase();

	// IShader
	EShaderType GetType() const override final;
	std::shared_ptr<IShaderInputLayout> GetInputLayout() const override final;
	const std::shared_ptr<IShaderParameter>& GetShaderParameterByName(const std::string& name) const override final;
	
protected:
    ShaderMacros                        m_ShaderMacros;
    std::string                         m_EntryPoint;
    std::string                         m_Profile;
    std::string                         m_ShaderFileName;
	EShaderType	                        m_ShaderType;

    std::shared_ptr<IShaderInputLayout> m_InputLayout;

	ParameterMap                        m_ShaderParameters;
};

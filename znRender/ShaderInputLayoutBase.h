#pragma once

class ZN_API ShaderInputLayoutBase : public IShaderInputLayout, public Object
{
public:
    ShaderInputLayoutBase();
	virtual ~ShaderInputLayoutBase();

    // IShaderInputLayout
    const SemanticMap& GetSemantics() const override final;

protected:
	SemanticMap m_InputSemantics;
};
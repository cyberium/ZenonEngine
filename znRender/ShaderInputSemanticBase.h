#pragma once

class ZN_API ShaderInputSemanticBase
	: virtual public IShaderInputSemantic
{
public:
	ShaderInputSemanticBase(const std::string& Name, uint32 Index);
    virtual ~ShaderInputSemanticBase();

	// IShaderInputSemantic
	const std::string& GetName() const override;
	uint32 GetIndex() const override;
	bool IsEqual(const IShaderInputSemantic* Other) const override;

private:
	std::string             m_Name;
	uint32                  m_Index;
};

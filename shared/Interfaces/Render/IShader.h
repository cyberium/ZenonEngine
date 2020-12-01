#pragma once

// FORWARD BEGIN
ZN_INTERFACE IShaderParameter;
ZN_INTERFACE IShaderInputLayout;
// FORWARD END

struct ZN_API SShaderMacro
{
	const char* MacrosName;
	const char* ValueStr;
};

ZN_INTERFACE ZN_API IShader
{
	typedef std::map<std::string, std::shared_ptr<IShaderParameter>> ParameterMap;
	typedef std::vector<SShaderMacro> ShaderMacros;

	virtual ~IShader() {}

	virtual EShaderType GetShaderType() const = 0;

	virtual bool LoadFromFile(EShaderType type, std::string fileName, ShaderMacros shaderMacros, std::string entryPoint) = 0;

	virtual bool LoadInputLayoutFromReflector() = 0;
	virtual bool LoadInputLayoutFromCustomElements(const std::vector<SCustomInputElement>& declIn) = 0;

	virtual IShaderInputLayout& GetInputLayout() const = 0;
	virtual IShaderParameter* GetShaderParameterByName(const std::string& name) const = 0;
	
	virtual void Bind() const = 0;
	virtual void UnBind() const = 0;

	virtual void Dispatch(const glm::uvec3& numGroups) = 0;
};

typedef std::vector<std::shared_ptr<IShader>> ShaderList;
typedef std::map<EShaderType, std::shared_ptr<IShader>> ShaderMap;
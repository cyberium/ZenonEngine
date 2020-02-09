#pragma once

// FORWARD BEGIN
ZN_INTERFACE IShaderParameter;
ZN_INTERFACE IShaderInputLayout;
// FORWARD END

ZN_INTERFACE ZN_API __declspec(novtable) IShader
{
	typedef std::map<std::string, std::shared_ptr<IShaderParameter>> ParameterMap;

	virtual ~IShader() {}

	/**
	 * What type of shader is this?
	 */
	virtual EShaderType GetType() const = 0;

	/**
	 * A shader macro consists of a macro name and a definition.
	 * Use this to pass macro definitions to the shader compiler.
	 */
	typedef std::map< std::string, std::string > ShaderMacros;


	/**
	 * Load a shader file from a std::string.
	 * @param type: The type of shader to load.
	 * @param source: The Shader source code in std::string format.
	 * @param sourceFileName: The file path of the original file if it exists. This is used to determine include paths.
	 * @param entryPoint: The name of the entry-point function to be used by this shader.
	 * @param profile: The shader profile to use to compile this shader.
	 * To use the latest supported profile, specify "latest" here.
	 * @return True if the shader was loaded correctly, or False otherwise.
	 */
	virtual bool LoadShaderFromString(EShaderType type, const std::string& fileName, const std::string& source, const ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, IShaderInputLayout* _customLayout) = 0;

	/**
	 * Load a shader from a file.
	 * @param type: The type of shader to load.
	 * @param fileName: The path to the shader file to load.
	 * @param entryPoint: The name of the entry-point function to be used by this shader.
	 * @param profile: The shader profile to use to compile this shader.
	 * To use the latest supported profile, specify "latest" here.
	 * @return True if the shader was loaded correctly, or False otherwise.
	 */
	virtual bool LoadShaderFromFile(EShaderType type, const std::string& fileName, const ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, IShaderInputLayout* _customLayout) = 0;

	/**
	 * Calculate shader input layout
	 */
	virtual bool LoadInputLayoutFromReflector() = 0;
	virtual bool LoadInputLayoutFromCustomElements(const std::vector<SCustomVertexElement>& declIn) = 0;

	/**
	 * Get a shader input layout description
	 */
	virtual IShaderInputLayout& GetInputLayout() const = 0;

	/**
	 * Get a reference to a parameter defined in the shader.
	 * @param name: The name of the parameter as defined in the shader file.
	 * @return A reference to the ShaderParameter. If the parameter with the specified name
	 * is not found in the shader, then this function will return an invalid shader parameter.
	 * You can check for validity using the IShaderParameter::IsValid method.
	 */
	virtual IShaderParameter& GetShaderParameterByName(const std::string& name) const = 0;

	/**
	 * Bind this shader for use in the rendering pipeline.
	 */
	virtual void Bind() const = 0;

	/**
	 * Unbind the shader from the rendering pipeline.
	 */
	virtual void UnBind() const = 0;

	/**
	 * Dispatch a compute shader. If this shader does not refer to a compute
	 * shader, this function does nothing.
	 * TODO: Refactor this into a Command (and CommandBuffer).
	 */
	virtual void Dispatch(const glm::uvec3& numGroups) = 0;
};

typedef std::vector<std::shared_ptr<IShader>> ShaderList;
typedef std::map<EShaderType, std::shared_ptr<IShader>> ShaderMap;
typedef std::map<std::string, std::shared_ptr<IShader>> ShaderNameMap;
#pragma once

class ShaderOGL : public Shader, public std::enable_shared_from_this<ShaderOGL>
{
public:
	ShaderOGL();
	virtual ~ShaderOGL();

	// Shader loading
	bool LoadShaderFromString(ShaderType shaderType, cstring fileName, cstring source, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile);
	bool LoadShaderFromFile(ShaderType shaderType, cstring fileName, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile);

	ShaderParameter& GetShaderParameterByName(cstring name) const;

	virtual bool                 HasSemantic(const BufferBinding& binding) const;
	virtual const InputSemantic& GetSemantic(const BufferBinding& binding) const;
	virtual UINT                 GetSemanticSlot(const BufferBinding& binding) const;

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
	// Parameters necessary to reload the shader at runtime if it is modified on disc.
	ShaderMacros m_ShaderMacros;
	std::string m_EntryPoint;
	std::string m_Profile;
	std::string m_ShaderFileName;

	typedef std::unique_lock<std::recursive_mutex> MutexLock;
	std::recursive_mutex m_Mutex;
};

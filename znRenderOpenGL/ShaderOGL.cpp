#include <stdafx.h>

// General
#include "ShaderOGL.h"

std::string RecursionInclude(std::shared_ptr<IFile> f)
{
	if (f == nullptr)
	{
		Log::Error("Error open shader [%s].", f->Path_Name().c_str());
		return "";
	}

	std::string data = "";

	while (!f->isEof())
	{
		std::string line;
		if (false == f->readLine(&line))
		{
			break;
		}

		// Skip empty lines
		if (line.length() == 0)
		{
			continue;
		}

		// Find directive
		if (line[0] == '#' && line[1] == 'i' && line[2] == 'n' && line[3] == 'c' && line[4] == 'l')
		{
			size_t firstBracketPosition = line.find('"');
			assert1(firstBracketPosition != std::string::npos);

			size_t lastBracketPosition = line.find_last_of('"');
			assert1(firstBracketPosition != lastBracketPosition);

			std::string inludeFileName = line.substr(firstBracketPosition + 1, lastBracketPosition - firstBracketPosition - 1);
			CFile::FixFilePath(inludeFileName);
			data += RecursionInclude(GetManager<IFilesManager>()->Open(f->Path() + inludeFileName)) + '\n';

			continue;
		}

		data += line + '\n';
	}

	return data;
}

// This parameter will be returned if an invalid shader parameter is requested.
static ShaderParameter gs_InvalidShaderParameter;
static InputSemantic gs_InvalidShaderSemantic;

GLenum GLTranslateShaderType(Shader::ShaderType _type)
{
	switch (_type)
	{
	case Shader::ShaderType::VertexShader:
		return GL_VERTEX_SHADER;
	case Shader::ShaderType::PixelShader:
		return GL_FRAGMENT_SHADER;
	case Shader::ShaderType::GeometryShader:
		return GL_GEOMETRY_SHADER;
	case Shader::ShaderType::TessellationControlShader:
		return GL_TESS_CONTROL_SHADER;
	case Shader::ShaderType::TessellationEvaluationShader:
		return GL_TESS_EVALUATION_SHADER;
	case Shader::ShaderType::ComputeShader:
		return GL_COMPUTE_SHADER;
	default:
		assert1(false);
	}
}

void GLTranslateAttribType(GLenum _type, GLint _size, GLenum * _newType, GLint * _newSize)
{
	/*
		GL_FLOAT,
		GL_FLOAT_VEC2, GL_FLOAT_VEC3, GL_FLOAT_VEC4,
		GL_FLOAT_MAT2, GL_FLOAT_MAT3, GL_FLOAT_MAT4,
		GL_FLOAT_MAT2x3, GL_FLOAT_MAT2x4,
		GL_FLOAT_MAT3x2, GL_FLOAT_MAT3x4,
		GL_FLOAT_MAT4x2, GL_FLOAT_MAT4x3,

		GL_INT,
		GL_INT_VEC2, GL_INT_VEC3, GL_INT_VEC4,

		GL_UNSIGNED_INT,
		GL_UNSIGNED_INT_VEC2, GL_UNSIGNED_INT_VEC3, GL_UNSIGNED_INT_VEC4,

		GL_DOUBLE,
		GL_DOUBLE_VEC2, GL_DOUBLE_VEC3, GL_DOUBLE_VEC4,
		GL_DOUBLE_MAT2, GL_DOUBLE_MAT3, GL_DOUBLE_MAT4,
		GL_DOUBLE_MAT2x3, GL_DOUBLE_MAT2x4,
		GL_DOUBLE_MAT3x2, GL_DOUBLE_MAT3x4,
		GL_DOUBLE_MAT4x2, or GL_DOUBLE_MAT4x3 
	*/

	switch (_type)
	{
	case GL_FLOAT:
		(*_newType) = GL_FLOAT;
		(*_newSize) = _size * 1;
		break;
	case GL_FLOAT_VEC2:
		(*_newType) = GL_FLOAT;
		(*_newSize) = _size * 2;
		break;
	case GL_FLOAT_VEC3:
		(*_newType) = GL_FLOAT;
		(*_newSize) = _size * 3;
		break;
	case GL_FLOAT_VEC4:
		(*_newType) = GL_FLOAT;
		(*_newSize) = _size * 4;
		break;
	default:
		assert1(false);
	}
}

ShaderOGL::ShaderOGL()
{
}

ShaderOGL::~ShaderOGL()
{
	Destroy();
}

bool ShaderOGL::GetShaderProgramLog(GLuint _obj, std::string * _errMsg)
{
	GLsizei infologLength = 0;
	glGetProgramiv(_obj, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 1)
	{
		GLsizei charsWritten = 0;
		_errMsg->resize(infologLength + 1);
		glGetProgramInfoLog(_obj, infologLength, &charsWritten, &(*_errMsg)[0]);
		return false;
	}

	GLint status;
	glGetProgramiv(_obj, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		return false;
	}

	glGetProgramiv(_obj, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
	{
		return false;
	}

	return true;
}

void ShaderOGL::Destroy()
{
	m_ShaderParameters.clear();
	m_InputSemantics.clear();
}



bool ShaderOGL::LoadShaderFromString(ShaderType shaderType, cstring fileName, cstring source, const ShaderMacros & shaderMacros, cstring entryPoint, cstring profile)
{
	m_ShaderType = shaderType;
	m_ShaderFileName = fileName;

	return false;
}

bool ShaderOGL::LoadShaderFromFile(ShaderType shaderType, cstring fileName, const ShaderMacros& shaderMacros, cstring entryPoint, cstring profile)
{
	m_ShaderType = shaderType;
	m_ShaderFileName = fileName;

	std::shared_ptr<IFile> file = GetManager<IFilesManager>()->Open(fileName);
	std::string fileSource = RecursionInclude(file);
	const GLchar *source = (const GLchar *)fileSource.c_str();

	m_GLObj = glCreateShaderProgramv(GLTranslateShaderType(shaderType), 1, &source);
	assert1(m_GLObj != 0);
	OGLCheckError();

	std::string errMsg;
	if (false == GetShaderProgramLog(m_GLObj, &errMsg))
	{
		fail2(errMsg.c_str());
		return false;
	}

	GLint attribCount;
	glGetProgramiv(m_GLObj, GL_ACTIVE_ATTRIBUTES, &attribCount);

	GLint attribNameMaxLenght;
	glGetProgramiv(m_GLObj, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attribNameMaxLenght);

	for (GLint j = 0; j < attribCount; ++j)
	{
		char name[32];
		GLsizei length, size;
		GLenum type;
		glGetActiveAttrib(m_GLObj, j, attribNameMaxLenght, &length, &size, &type, name);
		OGLCheckError();

		GLenum newType;
		GLint newSize;
		GLTranslateAttribType(type, size, &newType, &newSize);
		OGLCheckError();

		GLint slot = glGetAttribLocation(m_GLObj, name);
		OGLCheckError();

		m_InputSemantics.insert(SemanticMap::value_type(InputSemantic(name, slot, newType, newSize), slot));
	}

	GLint uniformsCount;
	glGetProgramiv(m_GLObj, GL_ACTIVE_UNIFORMS, &uniformsCount);

	GLint uniformsNameMaxLenght;
	glGetProgramiv(m_GLObj, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformsNameMaxLenght);

	for (GLint j = 0; j < uniformsCount; ++j)
	{
		char name[32];
		GLsizei length, size;
		GLenum type;
		glGetActiveUniform(m_GLObj, j, uniformsNameMaxLenght, &length, &size, &type, name);
		OGLCheckError();
		// Types https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetActiveUniform.xhtml

		ShaderParameter::Type parameterType = ShaderParameter::Type::Invalid;
		switch (type)
		{
		case GL_IMAGE_2D:
			parameterType = ShaderParameter::Type::Texture;
			break;
		case GL_SAMPLER_2D:
			parameterType = ShaderParameter::Type::Sampler;
			break;
		case GL_SAMPLER_BUFFER:
		case GL_IMAGE_BUFFER:
			parameterType = ShaderParameter::Type::Buffer;
			break;
		default:
			Log::Warn("Unknown shader parameter type [%s] [%s]", name, fileName.c_str());
			break;
		}

		GLint loc = glGetUniformLocation(m_GLObj, name);

		std::shared_ptr<ShaderParameter> shaderParameter = std::make_shared<ShaderParameter>(name, loc, shared_from_this(), parameterType);
		m_ShaderParameters.insert(ParameterMap::value_type(name, shaderParameter));
	}

	

	GLint uniformsBlocksCount;
	glGetProgramiv(m_GLObj, GL_ACTIVE_UNIFORM_BLOCKS, &uniformsBlocksCount);

	GLint uniformsBlocksNameMaxLenght;
	glGetProgramiv(m_GLObj, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &uniformsBlocksNameMaxLenght);

	for (GLint j = 0; j < uniformsBlocksCount; ++j)
	{
		char name[32];
		GLsizei length;

		//GLint param;
		// (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
		//glGetActiveUniformBlockiv(m_GLObj, j, GL_UNIFORM_BLOCK_BINDING, &param);
		//OGLCheckError();

		//glGetActiveUniformBlockiv(m_GLObj, j, GL_UNIFORM_BLOCK_DATA_SIZE, &param);
		//OGLCheckError();

		// (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
		glGetActiveUniformBlockName(m_GLObj, j, uniformsBlocksNameMaxLenght, &length, name);
		OGLCheckError();

		GLuint index = glGetUniformBlockIndex(m_GLObj, name);
		OGLCheckError();

		std::shared_ptr<ShaderParameter> shaderParameter = std::make_shared<ShaderParameter>(name, index, shared_from_this(), ShaderParameter::Type::Buffer);
		m_ShaderParameters.insert(ParameterMap::value_type(name, shaderParameter));
	}

	return true;
}

ShaderParameter& ShaderOGL::GetShaderParameterByName(cstring name) const
{
	ParameterMap::const_iterator iter = m_ShaderParameters.find(name);
	if (iter != m_ShaderParameters.end())
	{
		return *(iter->second);
	}

	//assert1(false);
	return gs_InvalidShaderParameter;
}

bool ShaderOGL::HasSemantic(const BufferBinding& binding) const
{
	for (auto& it : m_InputSemantics)
	{
		std::string newName = binding.Name;
		if (binding.Name != "POSITION")
			newName += std::to_string(binding.Index);
		if (it.first.Name == newName)
		{
			return true;
		}
	}

	return false;
}

const InputSemantic& ShaderOGL::GetSemantic(const BufferBinding& binding) const
{
	for (auto& it : m_InputSemantics)
	{
		std::string newName = binding.Name;
		if (binding.Name != "POSITION")
			newName += std::to_string(binding.Index);
		if (it.first.Name == newName)
		{
			return it.first;
		}
	}

	assert1(false);
	return gs_InvalidShaderSemantic;
}

UINT ShaderOGL::GetSemanticSlot(const BufferBinding& binding) const
{
	for (auto& it : m_InputSemantics)
	{
		std::string newName = binding.Name;
		if (binding.Name != "POSITION")
			newName += std::to_string(binding.Index);
		if (it.first.Name == newName)
		{
			return it.second;
		}
	}

	assert1(false);
	return UINT_MAX;
}

void ShaderOGL::Bind()
{
	for (ParameterMap::value_type value : m_ShaderParameters)
	{
		value.second->Bind();
	}
}

void ShaderOGL::UnBind()
{
	for (ParameterMap::value_type value : m_ShaderParameters)
	{
		value.second->UnBind();
	}
}

void ShaderOGL::Dispatch(const glm::uvec3& numGroups)
{
	glDispatchCompute(numGroups.x, numGroups.y, numGroups.z);
}

uint32 ShaderOGL::GetGLObject()
{
	return m_GLObj;
}

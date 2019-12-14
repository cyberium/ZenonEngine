#include <stdafx.h>

// General
#include "ShaderOGL.h"

// FORWARD BEGIN
GLenum GLTranslateShaderType(IShader::ShaderType _type);
// FORWARD END

ShaderOGL::ShaderOGL(std::weak_ptr<IRenderDevice> RenderDevice)
	: m_RenderDevice(RenderDevice)
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
}



bool ShaderOGL::LoadShaderFromString(ShaderType shaderType, const std::string& fileName, const std::string& source, const ShaderMacros & shaderMacros, const std::string& entryPoint, const std::string& profile, std::shared_ptr<IShaderInputLayout> _customLayout)
{
    m_ShaderType = shaderType;
    m_ShaderFileName = fileName;

    return false;
}

bool ShaderOGL::LoadShaderFromFile(ShaderType shaderType, const std::string& fileName, const ShaderMacros& shaderMacros, const std::string& entryPoint, const std::string& profile, std::shared_ptr<IShaderInputLayout> _customLayout)
{
    m_ShaderType = shaderType;
    m_ShaderFileName = fileName;

    std::shared_ptr<IFile> file = GetManager<IFilesManager>(m_RenderDevice.lock()->GetBaseManager())->Open(fileName);
    std::string fileSource = RecursionInclude(m_RenderDevice.lock()->GetBaseManager(), file);
    const GLchar *source = (const GLchar *)fileSource.c_str();

    m_GLObj = glCreateShaderProgramv(GLTranslateShaderType(shaderType), 1, &source);
    _ASSERT(m_GLObj != 0);
    OGLCheckError();

    std::string errMsg;
    if (false == GetShaderProgramLog(m_GLObj, &errMsg))
    {
        _ASSERT_EXPR(false, errMsg);
        return false;
    }

    /*GLint uniformsCount;
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

        IShaderParameter::Type parameterType = IShaderParameter::Type::Invalid;
        switch (type)
        {
            case GL_IMAGE_2D:
                parameterType = IShaderParameter::Type::Texture;
                break;
            case GL_SAMPLER_2D:
                parameterType = IShaderParameter::Type::Sampler;
                break;
            case GL_SAMPLER_BUFFER:
            case GL_IMAGE_BUFFER:
                parameterType = IShaderParameter::Type::Buffer;
                break;
            default:
                Log::Warn("Unknown shader parameter type [%s] [%s]", name, fileName.c_str());
                break;
        }

        if (parameterType == IShaderParameter::Type::Invalid)
            continue;

        GLint loc = glGetUniformLocation(m_GLObj, name);

        std::shared_ptr<IShaderParameter> shaderParameter = std::make_shared<ShaderParameterBase>(name, loc, shared_from_this(), parameterType);
        m_ShaderParameters.insert(ParameterMap::value_type(name, shaderParameter));
    }*/



    GLint uniformsBlocksCount;
    glGetProgramiv(m_GLObj, GL_ACTIVE_UNIFORM_BLOCKS, &uniformsBlocksCount);

    GLint uniformsBlocksNameMaxLenght;
    glGetProgramiv(m_GLObj, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &uniformsBlocksNameMaxLenght);

    for (GLint j = 0; j < uniformsBlocksCount; ++j)
    {
        char name[32];
        GLsizei length;

        GLint binding;
        // (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
        glGetActiveUniformBlockiv(m_GLObj, j, GL_UNIFORM_BLOCK_BINDING, &binding);
        OGLCheckError();

        GLint dataSize;
        glGetActiveUniformBlockiv(m_GLObj, j, GL_UNIFORM_BLOCK_DATA_SIZE, &dataSize);
        OGLCheckError();

        // (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
        glGetActiveUniformBlockName(m_GLObj, j, uniformsBlocksNameMaxLenght, &length, name);
        OGLCheckError();

        GLuint index = glGetUniformBlockIndex(m_GLObj, name);
        OGLCheckError();
        _ASSERT(index != -1);

        std::shared_ptr<IShaderParameter> shaderParameter = std::make_shared<ShaderParameterBase>(name, binding, shared_from_this(), IShaderParameter::Type::Buffer);
        m_ShaderParameters.insert(ParameterMap::value_type(name, shaderParameter));
    }

    return true;
}

bool ShaderOGL::LoadInputLayoutFromReflector()
{
    if (m_InputLayout)
        return true;

    std::shared_ptr<ShaderInputLayoutOGL> inputLayout = std::make_shared<ShaderInputLayoutOGL>();
    inputLayout->LoadFromReflector(m_GLObj);
    m_InputLayout = inputLayout;

    return true;
}

bool ShaderOGL::LoadInputLayoutFromCustomElements(const std::vector<SCustomVertexElement>& declIn)
{
    if (m_InputLayout)
        return true;

    std::shared_ptr<ShaderInputLayoutOGL> inputLayout = std::make_shared<ShaderInputLayoutOGL>();
	inputLayout->LoadFromCustomElements(m_GLObj, declIn);
    m_InputLayout = inputLayout;

    return true;
}


void ShaderOGL::Bind() const
{
    for (ParameterMap::value_type value : m_ShaderParameters)
    {
        value.second->Bind();
    }
}

void ShaderOGL::UnBind() const
{
    for (ParameterMap::value_type value : m_ShaderParameters)
    {
        value.second->Unbind();
    }
}

void ShaderOGL::Dispatch(const glm::uvec3& numGroups)
{
    glDispatchCompute(numGroups.x, numGroups.y, numGroups.z);
}

uint32 ShaderOGL::GetGLObject() const
{
    return m_GLObj;
}



//
// HELPERS
//
GLenum GLTranslateShaderType(IShader::ShaderType _type)
{
    switch (_type)
    {
        case IShader::ShaderType::VertexShader:
            return GL_VERTEX_SHADER;
        case IShader::ShaderType::PixelShader:
            return GL_FRAGMENT_SHADER;
        case IShader::ShaderType::GeometryShader:
            return GL_GEOMETRY_SHADER;
        case IShader::ShaderType::TessellationControlShader:
            return GL_TESS_CONTROL_SHADER;
        case IShader::ShaderType::TessellationEvaluationShader:
            return GL_TESS_EVALUATION_SHADER;
        case IShader::ShaderType::ComputeShader:
            return GL_COMPUTE_SHADER;
        default:
            _ASSERT(false);
    }
}

GLbitfield GLTranslateShaderBitType(IShader::ShaderType _type)
{
	GLbitfield result = GL_VERTEX_SHADER_BIT;
	switch (_type)
	{
	case IShader::ShaderType::VertexShader:
		result = GL_VERTEX_SHADER_BIT;
		break;
	case IShader::ShaderType::PixelShader:
		result = GL_FRAGMENT_SHADER_BIT;
		break;
	case IShader::ShaderType::GeometryShader:
		result = GL_GEOMETRY_SHADER_BIT;
		break;
	case IShader::ShaderType::TessellationControlShader:
		result = GL_TESS_CONTROL_SHADER_BIT;
		break;
	case IShader::ShaderType::TessellationEvaluationShader:
		result = GL_TESS_EVALUATION_SHADER_BIT;
		break;
	case IShader::ShaderType::ComputeShader:
		result = GL_COMPUTE_SHADER_BIT;
		break;
	default:
		Log::Error("Unknown shader type factor.");
	}

	return result;
}
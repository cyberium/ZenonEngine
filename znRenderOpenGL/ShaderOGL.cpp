#include <stdafx.h>

// General
#include "ShaderOGL.h"

// FORWARD BEGIN
GLenum GLTranslateShaderType(Shader::ShaderType _type);
// FORWARD END

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

bool ShaderOGL::LoadInputLayoutFromReflector()
{
    if (m_InputLayout)
        return true;

    std::shared_ptr<ShaderInputLayoutOGL> inputLayout = std::make_shared<ShaderInputLayoutOGL>();
    inputLayout->LoadFromReflector(m_GLObj);
    m_InputLayout = inputLayout;

    return true;
}

bool ShaderOGL::LoadInputLayoutFromD3DElement(const std::vector<D3DVERTEXELEMENT9>& declIn)
{
    if (m_InputLayout)
        return true;

    std::shared_ptr<ShaderInputLayoutOGL> inputLayout = std::make_shared<ShaderInputLayoutOGL>();
    //m_InputLayout->LoadFromD3D9(m_pShaderBlob, declIn);
    m_InputLayout = inputLayout;

    assert1(false);

    return true;
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



//
// HELPERS
//
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
#include "stdafx.h"

// General
#include "ShaderInputLayoutOGL.h"


static InputSemantic gs_InvalidShaderSemantic;


void GLTranslateAttribType(GLenum _type, GLint _size, GLenum * _newType, GLint * _newSize);

ShaderInputLayoutOGL::ShaderInputLayoutOGL()
{
}

ShaderInputLayoutOGL::~ShaderInputLayoutOGL()
{
}



//
// IShaderInputLayout
//
bool ShaderInputLayoutOGL::HasSemantic(const BufferBinding& binding) const
{
	for (auto& it : m_InputSemantics)
	{
		if (it.first.Name == binding.Name && it.first.Index == binding.Index)
		{
			return true;
		}
	}

	return false;
}

const InputSemantic& ShaderInputLayoutOGL::GetSemantic(const BufferBinding& binding) const
{
	for (auto& it : m_InputSemantics)
	{
		if (it.first.Name == binding.Name && it.first.Index == binding.Index)
		{
			return it.first;
		}
	}

	assert1(false);
	return gs_InvalidShaderSemantic;
}

UINT ShaderInputLayoutOGL::GetSemanticSlot(const BufferBinding& binding) const
{
	for (auto& it : m_InputSemantics)
	{
		if (it.first.Name == binding.Name && it.first.Index == binding.Index)
		{
			return it.second;
		}
	}

	assert1(false);
	return UINT_MAX;
}



bool ShaderInputLayoutOGL::LoadFromReflector(GLuint GLObj)
{
    GLint attribCount;
    glGetProgramiv(GLObj, GL_ACTIVE_ATTRIBUTES, &attribCount);

    GLint attribNameMaxLenght;
    glGetProgramiv(GLObj, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attribNameMaxLenght);

    for (GLint j = 0; j < attribCount; ++j)
    {
        char name[32];
        GLsizei length, size;
        GLenum type;
        glGetActiveAttrib(GLObj, j, attribNameMaxLenght, &length, &size, &type, name);
        OGLCheckError();

        GLenum newType;
        GLint newSize;
        GLTranslateAttribType(type, size, &newType, &newSize);
        OGLCheckError();

        GLint slot = glGetAttribLocation(GLObj, name);
        OGLCheckError();

        m_InputSemantics.insert(SemanticMap::value_type(InputSemantic(name, slot, newType, newSize), slot));
    }

	return true;
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
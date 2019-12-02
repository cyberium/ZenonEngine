#pragma once

struct SOGLCustomAttribute
{
	std::string name;
	GLint slot;
	GLenum attrType;
	GLint size;
};

void ConvertVertexDeclaration(const std::vector<SCustomVertexElement>& CustomElements, std::vector<SOGLCustomAttribute>& declOut);
#pragma once

class ShaderInputLayoutOGL : public IShaderInputLayout
{
public:
	                                                ShaderInputLayoutOGL();
	virtual                                         ~ShaderInputLayoutOGL();

	// ShaderInputLayoutOGL
	bool                                            LoadFromReflector(GLuint GLObj);

private:
};
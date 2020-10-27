#pragma once

namespace
{
	__declspec(align(16)) struct SMaterialEditorToolProperties
	{
		SMaterialEditorToolProperties()
			: Color(1.0f, 1.0f, 1.0f, 1.0f)
		{}
		glm::vec4 Color;
		//-------------------------- ( 16 bytes )
	};
}

class MaterialEditorTool
	: public MaterialProxieT<SMaterialEditorToolProperties>
{
public:
	MaterialEditorTool(IRenderDevice& RenderDevice);
	virtual ~MaterialEditorTool();

	glm::vec4 GetColor() const;
	void SetColor(const glm::vec4& Color);
};
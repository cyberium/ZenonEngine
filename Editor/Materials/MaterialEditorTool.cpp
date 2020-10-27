#include "stdafx.h"

// General
#include "MaterialEditorTool.h"

MaterialEditorTool::MaterialEditorTool(IRenderDevice& RenderDevice)
	: MaterialProxieT(RenderDevice, "MaterialEditorTool")
{}

MaterialEditorTool::~MaterialEditorTool()
{}

glm::vec4 MaterialEditorTool::GetColor() const
{
	return MaterialDataReadOnly().Color;
}

void MaterialEditorTool::SetColor(const glm::vec4& Color)
{
	MaterialData().Color = Color;
	MarkMaterialDataDirty();
}

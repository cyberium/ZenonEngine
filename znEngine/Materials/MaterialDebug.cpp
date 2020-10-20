#include "stdafx.h"

// General
#include "MaterialDebug.h"

MaterialDebug::MaterialDebug(IRenderDevice& RenderDevice)
	: MaterialProxieT(RenderDevice, "MaterialDebug")
{}

MaterialDebug::~MaterialDebug()
{}

glm::vec4 MaterialDebug::GetDiffuseColor() const
{
	return MaterialDataReadOnly().DiffuseColor;
}

//-----

void MaterialDebug::SetDiffuseColor(const glm::vec4& diffuse)
{
	MaterialData().DiffuseColor = diffuse;
	MarkMaterialDataDirty();
}

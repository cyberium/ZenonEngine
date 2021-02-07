#include "stdafx.h"

// General
#include "FontMaterial.h"

CFontMaterial::CFontMaterial(const IRenderDevice & RenderDevice)
	: MaterialProxieT<SGPUPerCharacterDataPS>(RenderDevice.GetObjectsFactory().CreateMaterial("FontMaterial"))
{
}

CFontMaterial::~CFontMaterial()
{
}

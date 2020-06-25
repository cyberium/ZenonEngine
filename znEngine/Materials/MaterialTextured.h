#pragma once

#include "MaterialDebug.h"

class ZN_API MaterialTextured 
	: public MaterialDebug
{
public:
	MaterialTextured(IRenderDevice& RenderDevice);
	virtual ~MaterialTextured();
};
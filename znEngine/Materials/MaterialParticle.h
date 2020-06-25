#pragma once

#include "MaterialDebug.h"

class ZN_API MaterialParticle 
	: public MaterialDebug
{
public:
	MaterialParticle(IRenderDevice& RenderDevice);
	virtual ~MaterialParticle();

};
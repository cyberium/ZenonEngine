#include "stdafx.h"

// General
#include "ModelDX11.h"

// Additional
#include "GeometryDX11.h"

ModelDX11::ModelDX11(IRenderDeviceDX11& RenderDeviceDX11)
{}

ModelDX11::~ModelDX11()
{}



//
// IModel
//
bool ModelDX11::Render(const RenderEventArgs& renderArgs) const
{
	return true;
}

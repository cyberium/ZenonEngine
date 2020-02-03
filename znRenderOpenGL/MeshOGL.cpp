#include "stdafx.h"

// General
#include "MeshOGL.h"

// Additional
#include "GeometryOGL.h"

MeshOGL::MeshOGL()
{
	m_Geometry = std::make_shared<GeometryOGL>();
}

MeshOGL::~MeshOGL()
{

}

void MeshOGL::SetPrimitiveTopology(PrimitiveTopology _topology)
{
	m_Geometry->SetPrimitiveTopology(_topology);
}

bool MeshOGL::Render(const RenderEventArgs * renderArgs, const IConstantBuffer * perObject, SGeometryDrawArgs GeometryDrawArgs)
{
	return true;
}

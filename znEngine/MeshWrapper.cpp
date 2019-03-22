#include "stdafx.h"

// General
#include "MeshWrapper.h"

// Additional
#include "Application.h"

MeshWrapper::MeshWrapper(SceneNodeTypes type, std::shared_ptr<IMesh> _mesh) :
	m_Mesh(_mesh)
{
	if (m_Mesh == nullptr)
		m_Mesh = _RenderDevice->CreateMesh();

	m_Mesh->SetType(type);
}

MeshWrapper::~MeshWrapper()
{
}

SceneNodeTypes MeshWrapper::GetType() const
{
	return m_Mesh->GetType();
}

void MeshWrapper::SetType(SceneNodeTypes type)
{
	m_Mesh->SetType(type);
}

void MeshWrapper::AddVertexBuffer(const BufferBinding & binding, std::shared_ptr<IBuffer> buffer)
{
	m_Mesh->AddVertexBuffer(binding, buffer);
}

void MeshWrapper::SetVertexBuffer(std::shared_ptr<IBuffer> buffer)
{
	m_Mesh->SetVertexBuffer(buffer);
}

void MeshWrapper::SetIndexBuffer(std::shared_ptr<IBuffer> buffer)
{
	m_Mesh->SetIndexBuffer(buffer);
}

void MeshWrapper::SetPrimitiveTopology(PrimitiveTopology _topology)
{
	m_Mesh->SetPrimitiveTopology(_topology);
}

void MeshWrapper::SetMaterial(std::shared_ptr<const Material> material)
{
	m_Mesh->SetMaterial(material);
}

std::shared_ptr<const Material> MeshWrapper::GetMaterial() const
{
	return m_Mesh->GetMaterial();
}

bool MeshWrapper::Render(RenderEventArgs& renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
	return m_Mesh->Render(renderEventArgs, perObject, indexStartLocation, indexCnt, vertexStartLocation, vertexCnt);
}

bool MeshWrapper::Accept(IVisitor& visitor, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
	return visitor.Visit(*this, indexStartLocation, indexCnt, vertexStartLocation, vertexCnt);
}

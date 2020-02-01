#include "stdafx.h"

// General
#include "RenderPrimitivesFactory.h"

// Additional
#include "Primitives/Geometry.h"

CRenderPrimitivesFactory::CRenderPrimitivesFactory(IRenderDevice& RenderDevice)
	: m_RenderDevice(RenderDevice)
{
}

CRenderPrimitivesFactory::~CRenderPrimitivesFactory()
{
	printf("");

}



//
// IRenderPrimitivesFactory
//
std::shared_ptr<IMesh> CRenderPrimitivesFactory::CreateLine(cvec3 _dest)
{
	vec3 p[2];
	p[0] = vec3(0.0f, 0.0f, 0.0f);
	p[1] = _dest;

	std::shared_ptr<IMesh> mesh = m_RenderDevice.GetObjectsFactory().CreateMesh();
	mesh->SetPrimitiveTopology(PrimitiveTopology::LineList);

	std::shared_ptr<IBuffer> __vb = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(p, 2);
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	return mesh;
}

std::shared_ptr<IMesh> CRenderPrimitivesFactory::CreatePlane(cvec3 N)
{
	DirectX::VertexCollection vertices;
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3( 1.0f, 0.0f,  1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)));
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(-1.0f, 0.0f,  1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(-1.0f, 0.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)));
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(1.0f, 0.0f, -1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)));

	DirectX::IndexCollection indices;
	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(3);

	std::shared_ptr<IMesh> mesh = m_RenderDevice.GetObjectsFactory().CreateMesh();

	std::shared_ptr<IBuffer> __vbPos = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	std::shared_ptr<IBuffer> __vbTex = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 12, sizeof(DirectX::VertexPositionTextureNormal));
	std::shared_ptr<IBuffer> __vbNor = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 20, sizeof(DirectX::VertexPositionTextureNormal));
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPos);
	mesh->AddVertexBuffer(BufferBinding("NORMAL", 0), __vbNor);
	mesh->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __vbTex);

	std::shared_ptr<IBuffer> __ib = m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}

std::shared_ptr<IMesh> CRenderPrimitivesFactory::CreateScreenQuad(float left, float right, float bottom, float top, float z)
{
	DirectX::VertexCollection vertices;
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(right, top, z), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)));
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(left, top, z), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(left, bottom, z), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)));
	vertices.push_back(DirectX::VertexPositionTextureNormal(DirectX::XMFLOAT3(right, bottom, z), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)));

	DirectX::IndexCollection indices;
	indices.push_back(1);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(3);

	std::shared_ptr<IMesh> mesh = m_RenderDevice.GetObjectsFactory().CreateMesh();

	std::shared_ptr<IBuffer> __vb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	mesh->SetVertexBuffer(__vb);

	std::shared_ptr<IBuffer> __ib = m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}

std::shared_ptr<IMesh> CRenderPrimitivesFactory::CreateSphere()
{
	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputeSphere(vertices, indices, 1.0f, 32, false, false);

	std::shared_ptr<IMesh> mesh = m_RenderDevice.GetObjectsFactory().CreateMesh();

	std::shared_ptr<IBuffer> __vbPos = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	std::shared_ptr<IBuffer> __vbTex = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 12, sizeof(DirectX::VertexPositionTextureNormal));
	std::shared_ptr<IBuffer> __vbNor = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 20, sizeof(DirectX::VertexPositionTextureNormal));
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPos);
	mesh->AddVertexBuffer(BufferBinding("NORMAL", 0), __vbNor);
	mesh->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __vbTex);

	std::shared_ptr<IBuffer> __ib = m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}

std::shared_ptr<IMesh> CRenderPrimitivesFactory::CreateCube()
{
	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputeBox(vertices, indices, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), false, false);

	std::shared_ptr<IMesh> mesh = m_RenderDevice.GetObjectsFactory().CreateMesh();
	mesh->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

	std::shared_ptr<IBuffer> __vbPos = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	std::shared_ptr<IBuffer> __vbTex = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 12, sizeof(DirectX::VertexPositionTextureNormal));
	std::shared_ptr<IBuffer> __vbNor = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 20, sizeof(DirectX::VertexPositionTextureNormal));
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPos);
	mesh->AddVertexBuffer(BufferBinding("NORMAL", 0), __vbNor);
	mesh->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __vbTex);

	std::shared_ptr<IBuffer> __ib = m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}


std::shared_ptr<IMesh> CRenderPrimitivesFactory::CreateCone()
{
	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputeCone(vertices, indices, 0.77f, 1.0f, 32, false);

	std::shared_ptr<IMesh> mesh = m_RenderDevice.GetObjectsFactory().CreateMesh();
	mesh->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

	std::shared_ptr<IBuffer> __vbPos = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	std::shared_ptr<IBuffer> __vbTex = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 12, sizeof(DirectX::VertexPositionTextureNormal));
	std::shared_ptr<IBuffer> __vbNor = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 20, sizeof(DirectX::VertexPositionTextureNormal));
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPos);
	mesh->AddVertexBuffer(BufferBinding("NORMAL", 0), __vbNor);
	mesh->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __vbTex);

	std::shared_ptr<IBuffer> __ib = m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}

std::shared_ptr<IMesh> CRenderPrimitivesFactory::CreateQuad()
{
	vec2 vertices[4];
	vertices[0] = vec2( 1.0f, 1.0f);
	vertices[1] = vec2(-1.0f, 1.0f);
	vertices[2] = vec2(-1.0f, -1.0f);
	vertices[3] = vec2( 1.0f, -1.0f);

	vec2 texCoords[4];
	texCoords[0] = vec2(1.0f, 0.0f);
	texCoords[1] = vec2(0.0f, 0.0f);
	texCoords[2] = vec2(0.0f, 1.0f);
	texCoords[3] = vec2(1.0f, 1.0f);

	uint16 indices[6];
	indices[0] = 1;
	indices[1] = 0;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 0;
	indices[5] = 3;

	std::shared_ptr<IMesh> mesh = m_RenderDevice.GetObjectsFactory().CreateMesh();

	std::shared_ptr<IBuffer> __vbPosition = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(vertices, 4);
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPosition);

	std::shared_ptr<IBuffer> __vbTexCoord = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(texCoords, 4);
	mesh->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __vbTexCoord);

	std::shared_ptr<IBuffer> __ib = m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices, 6);
	mesh->SetIndexBuffer(__ib);

	return mesh;
}



//
// 3D meshes
//
std::shared_ptr<IMesh> CRenderPrimitivesFactory::Create3DQuad(float width, float height)
{
    vec3 vertices[4];
    vertices[0] = vec3(width, 0.0f, height);
    vertices[1] = vec3(0.0f, 0.0f, height);
    vertices[2] = vec3(0.0f, 0.0f, 0.0f);
    vertices[3] = vec3(width, 0.0f, 0.0f);

    vec2 texCoords[4];
    texCoords[0] = vec2(1.0f, 0.0f);
    texCoords[1] = vec2(0.0f, 0.0f);
    texCoords[2] = vec2(0.0f, 1.0f);
    texCoords[3] = vec2(1.0f, 1.0f);

    uint16 indices[6];
    indices[0] = 1;
    indices[1] = 0;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 0;
    indices[5] = 3;

    std::shared_ptr<IMesh> mesh = m_RenderDevice.GetObjectsFactory().CreateMesh();

    std::shared_ptr<IBuffer> __vbPosition = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(vertices, 4);
    mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPosition);

    std::shared_ptr<IBuffer> __vbTexCoord = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(texCoords, 4);
    mesh->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __vbTexCoord);

    std::shared_ptr<IBuffer> __ib = m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices, 6);
    mesh->SetIndexBuffer(__ib);

    return mesh;
}

std::shared_ptr<IMesh> CRenderPrimitivesFactory::Create3DBeizerLine(vec3 start, vec3 end)
{
    vec3 p[4];
    p[0] = start;
    p[1] = vec3((end - start).x / 2.0f, start.y, 0.0f);
    p[2] = vec3((end - start).x / 2.0f, end.y, 0.0f);
    p[3] = end;

    std::shared_ptr<IMesh> mesh = m_RenderDevice.GetObjectsFactory().CreateMesh();
    mesh->SetPrimitiveTopology(PrimitiveTopology::LineList);

    std::shared_ptr<IBuffer> __vbPosition = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(p, 4);
    mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPosition);

    return mesh;
}



//
// UI meshes
//
std::shared_ptr<IMesh> CRenderPrimitivesFactory::CreateUIQuad(float width, float height)
{
    vec2 vertices[4];
    vertices[0] = vec2(width, height);
    vertices[1] = vec2(0.0f, height);
    vertices[2] = vec2(0.0f, 0.0f);
    vertices[3] = vec2(width, 0.0f);

    vec2 texCoords[4];
    texCoords[0] = vec2(1.0f, 0.0f);
    texCoords[1] = vec2(0.0f, 0.0f);
    texCoords[2] = vec2(0.0f, 1.0f);
    texCoords[3] = vec2(1.0f, 1.0f);

    uint16 indices[6];
    indices[0] = 1;
    indices[1] = 0;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 0;
    indices[5] = 3;

    std::shared_ptr<IMesh> mesh = m_RenderDevice.GetObjectsFactory().CreateMesh();

    std::shared_ptr<IBuffer> __vbPosition = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(vertices, 4);
    mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPosition);

    std::shared_ptr<IBuffer> __vbTexCoord = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(texCoords, 4);
    mesh->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __vbTexCoord);

    std::shared_ptr<IBuffer> __ib = m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices, 6);
    mesh->SetIndexBuffer(__ib);

    return mesh;
}

std::shared_ptr<IMesh> CRenderPrimitivesFactory::CreateUIBeizerLine(vec2 start, vec2 end)
{
    vec2 vertices[4];
    vertices[0] = start;
    vertices[1] = vec2((end - start).x / 2.0f, start.y);
    vertices[2] = vec2((end - start).x / 2.0f, end.y);
    vertices[3] = end;

    std::shared_ptr<IMesh> mesh = m_RenderDevice.GetObjectsFactory().CreateMesh();
    mesh->SetPrimitiveTopology(PrimitiveTopology::LineList);

    std::shared_ptr<IBuffer> __vbPosition = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(vertices, 4);
    mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPosition);

    return mesh;
}

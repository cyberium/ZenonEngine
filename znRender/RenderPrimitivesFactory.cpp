#include "stdafx.h"

// General
#include "RenderPrimitivesFactory.h"

// Additional
#include "Primitives/Geometry.h"

namespace
{
	BoundingBox CalculateBounds(const DirectX::VertexCollection& vertices)
	{
		glm::vec3 min(Math::MaxFloat), max(Math::MinFloat);
		for (const auto& v : vertices)
		{
			if (v.position.x < min.x) 
				min.x = v.position.x;
			if (v.position.x > max.x) 
				max.x = v.position.x;

			if (v.position.y < min.y) 
				min.y = v.position.y;
			if (v.position.y > max.y) 
				max.y = v.position.y;

			if (v.position.z < min.z) 
				min.z = v.position.z;
			if (v.position.z > max.z) 
				max.z = v.position.z;
		}

		return BoundingBox(min, max);
	}
}

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
std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateLine(const glm::vec3& _dest)
{
	glm::vec3 p[2];
	p[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	p[1] = _dest;

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
	geometry->SetPrimitiveTopology(PrimitiveTopology::LineList);

	std::shared_ptr<IBuffer> __vb = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(p, 2);
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	return geometry;
}

std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateLines(size_t count)
{
	std::vector<glm::vec3> points;

	int halfOfCount = count / 2;
	float halfOfCountFloat = static_cast<float>(halfOfCount);

	for (int i = -halfOfCount; i <= halfOfCount; i++)
	{
		points.push_back(glm::vec3(-halfOfCount, 0.f, i));
		points.push_back(glm::vec3(halfOfCount, 0.f, i));
	}

	for (int i = -halfOfCount; i <= halfOfCount; i++)
	{
		points.push_back(glm::vec3(i, 0.f, -halfOfCount));
		points.push_back(glm::vec3(i, 0.f, halfOfCount));
	}

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
	geometry->SetPrimitiveTopology(PrimitiveTopology::LineList);

	std::shared_ptr<IBuffer> __vb = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(points.data(), points.size());
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	return geometry;
}

std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreatePlane(const glm::vec3& N)
{
	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputePlane(vertices, indices, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), false, false);

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();

	std::shared_ptr<IBuffer> __vb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);
	std::shared_ptr<IBuffer> __tb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), sizeof(glm::vec3), sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __tb);
	std::shared_ptr<IBuffer> __nb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), sizeof(glm::vec3) + sizeof(glm::vec2), sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("NORMAL", 0), __nb);

	//geometry->SetVertexBuffer(m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal)));

	geometry->SetIndexBuffer(m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices));
	geometry->SetBounds(CalculateBounds(vertices));
	return geometry;
}

std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateScreenQuad(float left, float right, float bottom, float top, float z)
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

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
	geometry->SetVertexBuffer(m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal)));
	geometry->SetIndexBuffer(m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices));
	geometry->SetBounds(CalculateBounds(vertices));
	return geometry;
}

std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateSphere()
{
	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputeSphere(vertices, indices, 1.0f, 32, false, false);

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
	std::shared_ptr<IBuffer> __vb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);
	std::shared_ptr<IBuffer> __tb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), sizeof(glm::vec3), sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __tb);
	std::shared_ptr<IBuffer> __nb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), sizeof(glm::vec3) + sizeof(glm::vec2), sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("NORMAL", 0), __nb);
	//geometry->SetVertexBuffer(m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal)));
	geometry->SetIndexBuffer(m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices));
	geometry->SetBounds(CalculateBounds(vertices));
	return geometry;
}

std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateCube()
{
	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputeBox(vertices, indices, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), false, false);

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
	std::shared_ptr<IBuffer> __vb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);
	std::shared_ptr<IBuffer> __tb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), sizeof(glm::vec3), sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __tb);
	std::shared_ptr<IBuffer> __nb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), sizeof(glm::vec3) + sizeof(glm::vec2), sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("NORMAL", 0), __nb);
	//geometry->SetVertexBuffer(m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal)));
	geometry->SetIndexBuffer(m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices));
	geometry->SetBounds(CalculateBounds(vertices));
	return geometry;
}

std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateBBox()
{
	std::vector<glm::vec3> points;
	points.push_back(glm::vec3(0.0, 0.0, 0.0));
	points.push_back(glm::vec3(1.0, 0.0, 0.0));
	points.push_back(glm::vec3(0.0, 0.0, 0.0));
	points.push_back(glm::vec3(0.0, 1.0, 0.0));
	points.push_back(glm::vec3(0.0, 0.0, 0.0));
	points.push_back(glm::vec3(0.0, 0.0, 1.0));


	points.push_back(glm::vec3(1.0, 0.0, 0.0));
	points.push_back(glm::vec3(1.0, 1.0, 0.0));
	points.push_back(glm::vec3(1.0, 0.0, 0.0));
	points.push_back(glm::vec3(1.0, 0.0, 1.0));
	
	points.push_back(glm::vec3(0.0, 1.0, 0.0));
	points.push_back(glm::vec3(0.0, 1.0, 1.0));
	points.push_back(glm::vec3(0.0, 1.0, 0.0));
	points.push_back(glm::vec3(1.0, 1.0, 0.0));

	points.push_back(glm::vec3(0.0, 0.0, 1.0));
	points.push_back(glm::vec3(0.0, 1.0, 1.0));
	points.push_back(glm::vec3(0.0, 0.0, 1.0));
	points.push_back(glm::vec3(1.0, 0.0, 1.0));

	points.push_back(glm::vec3(1.0, 1.0, 1.0));
	points.push_back(glm::vec3(1.0, 1.0, 0.0));
	points.push_back(glm::vec3(1.0, 1.0, 1.0));
	points.push_back(glm::vec3(0.0, 1.0, 1.0));
	points.push_back(glm::vec3(1.0, 1.0, 1.0));
	points.push_back(glm::vec3(1.0, 0.0, 1.0));

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
	geometry->SetPrimitiveTopology(PrimitiveTopology::LineList);

	std::shared_ptr<IBuffer> __vb = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(points.data(), points.size());
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	/*DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputeBox(vertices, indices, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), false, false);

	for (auto& v : vertices)
		v.position += 0.5f;

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
	geometry->SetPrimitiveTopology(PrimitiveTopology::TriangleList);

	std::shared_ptr<IBuffer> __vbPos = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPos);

	std::shared_ptr<IBuffer> __ib = m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices);
	geometry->SetIndexBuffer(__ib);*/

	return geometry;
}


std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateCone()
{
	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputeCone(vertices, indices, 0.5f, 1.0f, 8, false);

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
	std::shared_ptr<IBuffer> __vb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);
	std::shared_ptr<IBuffer> __tb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), sizeof(glm::vec3), sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __tb);
	std::shared_ptr<IBuffer> __nb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), sizeof(glm::vec3) + sizeof(glm::vec2), sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("NORMAL", 0), __nb);
	//geometry->SetVertexBuffer(m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal)));
	geometry->SetIndexBuffer(m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices));
	geometry->SetBounds(CalculateBounds(vertices));
	return geometry;
}

std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateTorus(float Radius, float Thickness)
{
	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputeTorus(vertices, indices, Radius * 2.0f, Thickness / 2.0f, 32, false);

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
	std::shared_ptr<IBuffer> __vb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);
	std::shared_ptr<IBuffer> __tb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), sizeof(glm::vec3), sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __tb);
	std::shared_ptr<IBuffer> __nb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), sizeof(glm::vec3) + sizeof(glm::vec2), sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("NORMAL", 0), __nb);
	//geometry->SetVertexBuffer(m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal)));
	geometry->SetIndexBuffer(m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices));
	geometry->SetBounds(CalculateBounds(vertices));
	return geometry;
}

std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateQuad()
{
	glm::vec2 vertices[4];
	vertices[0] = glm::vec2( 1.0f, 1.0f);
	vertices[1] = glm::vec2(-1.0f, 1.0f);
	vertices[2] = glm::vec2(-1.0f, -1.0f);
	vertices[3] = glm::vec2( 1.0f, -1.0f);

	glm::vec2 texCoords[4];
	texCoords[0] = glm::vec2(1.0f, 0.0f);
	texCoords[1] = glm::vec2(0.0f, 0.0f);
	texCoords[2] = glm::vec2(0.0f, 1.0f);
	texCoords[3] = glm::vec2(1.0f, 1.0f);

	uint16 indices[6];
	indices[0] = 1;
	indices[1] = 0;
	indices[2] = 2;
	indices[3] = 2;
	indices[4] = 0;
	indices[5] = 3;

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();

	std::shared_ptr<IBuffer> __vbPosition = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(vertices, 4);
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPosition);

	std::shared_ptr<IBuffer> __vbTexCoord = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(texCoords, 4);
	geometry->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __vbTexCoord);

	std::shared_ptr<IBuffer> __ib = m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices, 6);
	geometry->SetIndexBuffer(__ib);

	return geometry;
}



//
// 3D geometryes
//
std::shared_ptr<IGeometry> CRenderPrimitivesFactory::Create3DQuad(float width, float height)
{
	glm::vec3 vertices[4];
    vertices[0] = glm::vec3(width, 0.0f, height);
    vertices[1] = glm::vec3(0.0f, 0.0f, height);
    vertices[2] = glm::vec3(0.0f, 0.0f, 0.0f);
    vertices[3] = glm::vec3(width, 0.0f, 0.0f);

	glm::vec2 texCoords[4];
    texCoords[0] = glm::vec2(1.0f, 0.0f);
    texCoords[1] = glm::vec2(0.0f, 0.0f);
    texCoords[2] = glm::vec2(0.0f, 1.0f);
    texCoords[3] = glm::vec2(1.0f, 1.0f);

    uint16 indices[6];
    indices[0] = 1;
    indices[1] = 0;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 0;
    indices[5] = 3;

    std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();

    std::shared_ptr<IBuffer> __vbPosition = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(vertices, 4);
    geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPosition);

    std::shared_ptr<IBuffer> __vbTexCoord = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(texCoords, 4);
    geometry->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __vbTexCoord);

    std::shared_ptr<IBuffer> __ib = m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices, 6);
    geometry->SetIndexBuffer(__ib);

    return geometry;
}

std::shared_ptr<IGeometry> CRenderPrimitivesFactory::Create3DBeizerLine(glm::vec3 start, glm::vec3 end)
{
	glm::vec3 p[4];
    p[0] = start;
    p[1] = glm::vec3((end - start).x / 2.0f, start.y, 0.0f);
    p[2] = glm::vec3((end - start).x / 2.0f, end.y, 0.0f);
    p[3] = end;

    std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
    geometry->SetPrimitiveTopology(PrimitiveTopology::LineList);

    std::shared_ptr<IBuffer> __vbPosition = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(p, 4);
    geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPosition);

    return geometry;
}



//
// UI geometryes
//
std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateUIQuad(float width, float height)
{
	glm::vec2 vertices[4];
    vertices[0] = glm::vec2(width, height);
    vertices[1] = glm::vec2(0.0f, height);
    vertices[2] = glm::vec2(0.0f, 0.0f);
    vertices[3] = glm::vec2(width, 0.0f);

	glm::vec2 texCoords[4];
    texCoords[0] = glm::vec2(1.0f, 0.0f);
    texCoords[1] = glm::vec2(0.0f, 0.0f);
    texCoords[2] = glm::vec2(0.0f, 1.0f);
    texCoords[3] = glm::vec2(1.0f, 1.0f);

    uint16 indices[6];
    indices[0] = 1;
    indices[1] = 0;
    indices[2] = 2;
    indices[3] = 2;
    indices[4] = 0;
    indices[5] = 3;

    std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();

    std::shared_ptr<IBuffer> __vbPosition = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(vertices, 4);
    geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPosition);

    std::shared_ptr<IBuffer> __vbTexCoord = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(texCoords, 4);
    geometry->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __vbTexCoord);

    std::shared_ptr<IBuffer> __ib = m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices, 6);
    geometry->SetIndexBuffer(__ib);

    return geometry;
}

std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateUIBeizerLine(glm::vec2 start, glm::vec2 end)
{
	glm::vec2 vertices[4];
    vertices[0] = start;
    vertices[1] = glm::vec2((end - start).x / 2.0f, start.y);
    vertices[2] = glm::vec2((end - start).x / 2.0f, end.y);
    vertices[3] = end;

    std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
    geometry->SetPrimitiveTopology(PrimitiveTopology::LineList);

    std::shared_ptr<IBuffer> __vbPosition = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(vertices, 4);
    geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vbPosition);

    return geometry;
}

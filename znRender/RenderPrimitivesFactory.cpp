#include "stdafx.h"

// General
#include "RenderPrimitivesFactory.h"

// Additional
#include "Primitives/Geometry.h"

namespace
{
	BoundingBox CalculateBounds(const std::vector<glm::vec3>& vertices)
	{
		glm::vec3 min(Math::MaxFloat), max(Math::MinFloat);
		for (const auto& v : vertices)
		{
			if (v.x < min.x) min.x = v.x;
			if (v.x > max.x) max.x = v.x;

			if (v.y < min.y) min.y = v.y;
			if (v.y > max.y) max.y = v.y;

			if (v.z < min.z) min.z = v.z;
			if (v.z > max.z) max.z = v.z;
		}
		return BoundingBox(min, max);
	}

	BoundingBox CalculateBounds(const DirectX::VertexCollection& vertices)
	{
		glm::vec3 min(Math::MaxFloat), max(Math::MinFloat);
		for (const auto& v : vertices)
		{
			if (v.position.x < min.x) min.x = v.position.x;
			if (v.position.x > max.x) max.x = v.position.x;

			if (v.position.y < min.y) min.y = v.position.y;
			if (v.position.y > max.y) max.y = v.position.y;

			if (v.position.z < min.z) min.z = v.position.z;
			if (v.position.z > max.z) max.z = v.position.z;
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

	geometry->SetBounds(CalculateBounds(points));

	return geometry;
}

std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateLines(const std::vector<glm::vec3>& Points)
{
	std::vector<glm::vec3> points2;

	if (Points.size() < 2)
		throw CException("Unable to build points geometry from '%d' points.", Points.size());

	for (size_t i = 0; i < Points.size() - 1; i++)
	{
		points2.push_back(Points[i]);
		points2.push_back(Points[i + 1]);
	}

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
	geometry->SetPrimitiveTopology(PrimitiveTopology::LineList);

	std::shared_ptr<IBuffer> __vb = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(points2.data(), points2.size());
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	geometry->SetBounds(CalculateBounds(points2));

	return geometry;
}

std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreatePlane(const glm::vec3& N)
{
	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputePlane(vertices, indices, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), false, false);

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();

	/*std::shared_ptr<IBuffer> __vb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);
	std::shared_ptr<IBuffer> __tb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), sizeof(glm::vec3), sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __tb);
	std::shared_ptr<IBuffer> __nb = m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), sizeof(glm::vec3) + sizeof(glm::vec2), sizeof(DirectX::VertexPositionTextureNormal));
	geometry->AddVertexBuffer(BufferBinding("NORMAL", 0), __nb);
	*/

	geometry->SetVertexBuffer(m_RenderDevice.GetObjectsFactory().CreateVoidVertexBuffer(vertices.data(), vertices.size(), 0, sizeof(DirectX::VertexPositionTextureNormal)));

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

std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateSphere(float Radius)
{
	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputeSphere(vertices, indices, Radius, 32, false, false);

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

	geometry->SetBounds(CalculateBounds(points));

	return geometry;
}

std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateFrustum(const Frustum & Frustum)
{
	std::vector<glm::vec3> points;
	points.push_back(Frustum.getCorner(0));
	points.push_back(Frustum.getCorner(3));
	points.push_back(Frustum.getCorner(3));
	points.push_back(Frustum.getCorner(2));
	points.push_back(Frustum.getCorner(2));
	points.push_back(Frustum.getCorner(1));
	points.push_back(Frustum.getCorner(1));
	points.push_back(Frustum.getCorner(0));


	points.push_back(Frustum.getCorner(4 + 0));
	points.push_back(Frustum.getCorner(4 + 3));
	points.push_back(Frustum.getCorner(4 + 3));
	points.push_back(Frustum.getCorner(4 + 2));
	points.push_back(Frustum.getCorner(4 + 2));
	points.push_back(Frustum.getCorner(4 + 1));
	points.push_back(Frustum.getCorner(4 + 1));
	points.push_back(Frustum.getCorner(4 + 0));

	for (size_t i = 0; i < 4; i++)
	{
		points.push_back(Frustum.getCorner(i));
		points.push_back(Frustum.getCorner(4 + i));
	}

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
	geometry->SetPrimitiveTopology(PrimitiveTopology::LineList);

	std::shared_ptr<IBuffer> __vb = m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(points.data(), points.size());
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	geometry->SetBounds(CalculateBounds(points));

	return geometry;
}


void Test()
{
	Frustum f;




}


std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateCone(float diameter, float height)
{
	DirectX::VertexCollection vertices;
	DirectX::IndexCollection indices;
	DirectX::ComputeCone(vertices, indices, diameter, height, 32, false);

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
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(vertices, 4));
	geometry->AddVertexBuffer(BufferBinding("TEXCOORD", 0), m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(texCoords, 4));
	geometry->SetIndexBuffer(m_RenderDevice.GetObjectsFactory().CreateIndexBuffer(indices, 6));

	return geometry;
}



//
// UI geometryes
//
std::shared_ptr<IGeometry> CRenderPrimitivesFactory::CreateUIQuad(glm::vec2 Size, glm::vec2 TexCoordStart, glm::vec2 TexCoordEnd)
{
	glm::vec2 vertices[6];
	vertices[0] = glm::vec2(0.0f, 0.0f);
	vertices[1] = glm::vec2(0.0f, Size.y);
	vertices[2] = glm::vec2(Size.x, 0.0f);

	vertices[3] = glm::vec2(Size.x, 0.0f);
	vertices[4] = glm::vec2(0.0f, Size.y);
	vertices[5] = glm::vec2(Size.x, Size.y);

	glm::vec2 texCoords[6];
	texCoords[0] = glm::vec2(TexCoordStart.x, TexCoordStart.y);
	texCoords[1] = glm::vec2(TexCoordStart.x, TexCoordEnd.y);
	texCoords[2] = glm::vec2(TexCoordEnd.x,   TexCoordStart.y);

	texCoords[3] = glm::vec2(TexCoordEnd.x,   TexCoordStart.y);
	texCoords[4] = glm::vec2(TexCoordStart.x, TexCoordEnd.y);
	texCoords[5] = glm::vec2(TexCoordEnd.x,   TexCoordEnd.y);

	std::shared_ptr<IGeometry> geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
	geometry->AddVertexBuffer(BufferBinding("POSITION", 0), m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(vertices, 6));
	geometry->AddVertexBuffer(BufferBinding("TEXCOORD", 0), m_RenderDevice.GetObjectsFactory().CreateVertexBuffer(texCoords, 6));
	return geometry;
}

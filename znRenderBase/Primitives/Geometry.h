//--------------------------------------------------------------------------------------
// File: Geometry.h
//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
// http://go.microsoft.com/fwlink/?LinkID=615561
//--------------------------------------------------------------------------------------

#include <DirectXMath.h>

namespace DirectX
{
	// Vertex struct holding position, normal vector, and texture mapping information.
	struct VertexPositionTextureNormal
	{
		VertexPositionTextureNormal() = default;

		VertexPositionTextureNormal(const VertexPositionTextureNormal&) = default;
		VertexPositionTextureNormal& operator=(const VertexPositionTextureNormal&) = default;

		VertexPositionTextureNormal(VertexPositionTextureNormal&&) = default;
		VertexPositionTextureNormal& operator=(VertexPositionTextureNormal&&) = default;

		VertexPositionTextureNormal(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT2 const& textureCoordinate)
		{
			this->position = vec3(position.x, position.y, position.z);
			this->textureCoordinate = vec2(textureCoordinate.x, textureCoordinate.y);
			this->normal = vec3(normal.x, normal.y, normal.z);
		}

		VertexPositionTextureNormal(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR textureCoordinate)
		{
			XMFLOAT3 position2;
			XMFLOAT2 textureCoordinate2;
			XMFLOAT3 normal2;

			XMStoreFloat3(&position2, position);
			XMStoreFloat2(&textureCoordinate2, textureCoordinate);
			XMStoreFloat3(&normal2, normal);

			this->position = vec3(position2.x, position2.y, position2.z);
			this->textureCoordinate = vec2(textureCoordinate2.x, textureCoordinate2.y);
			this->normal = vec3(normal2.x, normal2.y, normal2.z);
		}

		vec3 position;
		vec2 textureCoordinate;
		vec3 normal;
	};

    typedef std::vector<VertexPositionTextureNormal> VertexCollection;
    typedef std::vector<uint16_t> IndexCollection;

    void ComputeBox(VertexCollection& vertices, IndexCollection& indices, const XMFLOAT3& size, bool rhcoords, bool invertn);
    void ComputeSphere(VertexCollection& vertices, IndexCollection& indices, float diameter, size_t tessellation, bool rhcoords, bool invertn);
    void ComputeGeoSphere(VertexCollection& vertices, IndexCollection& indices, float diameter, size_t tessellation, bool rhcoords);
    void ComputeCylinder(VertexCollection& vertices, IndexCollection& indices, float height, float diameter, size_t tessellation, bool rhcoords);
    void ComputeCone(VertexCollection& vertices, IndexCollection& indices, float diameter, float height, size_t tessellation, bool rhcoords);
    void ComputeTorus(VertexCollection& vertices, IndexCollection& indices, float diameter, float thickness, size_t tessellation, bool rhcoords);
    void ComputeTetrahedron(VertexCollection& vertices, IndexCollection& indices, float size, bool rhcoords);
    void ComputeOctahedron(VertexCollection& vertices, IndexCollection& indices, float size, bool rhcoords);
    void ComputeDodecahedron(VertexCollection& vertices, IndexCollection& indices, float size, bool rhcoords);
    void ComputeIcosahedron(VertexCollection& vertices, IndexCollection& indices, float size, bool rhcoords);
}

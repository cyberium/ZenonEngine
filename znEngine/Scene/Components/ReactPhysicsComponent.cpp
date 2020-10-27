#include "stdafx.h"

// General
#include "ReactPhysicsComponent.h"

using namespace rp3d;

CReactPhysicsComponent::CReactPhysicsComponent(const ISceneNode3D& OwnerNode, rp3d::RigidBody * RPRigidBody)
	: CComponentBase(OwnerNode)
	, m_RPRigidBody(RPRigidBody)
{
}

CReactPhysicsComponent::~CReactPhysicsComponent()
{
}

void Test()
{
	float vertices[24];
	vertices[0] = -3; vertices[1] = -3; vertices[2] = 3;
	vertices[3] = 3; vertices[4] = -3; vertices[5] = 3;
	vertices[6] = 3; vertices[7] = -3; vertices[8] = -3;
	vertices[9] = -3; vertices[10] = -3; vertices[11] = -3;
	vertices[12] = -3; vertices[13] = 3; vertices[14] = 3;
	vertices[15] = 3; vertices[16] = 3; vertices[17] = 3;
	vertices[18] = 3; vertices[19] = 3; vertices[20] = -3;
	vertices[21] = -3; vertices[22] = 3; vertices[23] = -3;

	// Array with the vertices indices for each face of the mesh
	int indices[24];
	indices[0] = 0; indices[1] = 3; indices[2] = 2; indices[3] = 1;
	indices[4] = 4; indices[5] = 5; indices[6] = 6; indices[7] = 7;
	indices[8] = 0; indices[9] = 1; indices[10] = 5; indices[11] = 4;
	indices[12] = 1; indices[13] = 2; indices[14] = 6; indices[15] = 5;
	indices[16] = 2; indices[17] = 3; indices[18] = 7; indices[19] = 6;
	indices[20] = 0; indices[21] = 4; indices[22] = 7; indices[23] = 3;

	// Description of the six faces of the convex mesh
	PolygonVertexArray::PolygonFace* polygonFaces = ZN_NEW PolygonVertexArray::PolygonFace[6];
	PolygonVertexArray::PolygonFace * face = polygonFaces;
	for (int f = 0; f < 6; f++) 
	{
		// First vertex of the face in the indices array
		face->indexBase = f * 4;
		// Number of vertices in the face
		face->nbVertices = 4;
		face++;
	}

	// Create the polygon vertex array
	PolygonVertexArray * polygonVertexArray = ZN_NEW PolygonVertexArray(8, vertices, 3 * sizeof(float), indices, sizeof(int), 6, polygonFaces, PolygonVertexArray::VertexDataType::VERTEX_FLOAT_TYPE, PolygonVertexArray::IndexDataType::INDEX_INTEGER_TYPE);
	// Create the polyhedron mesh
	//PolyhedronMesh * polyhedronMesh = physicsCommon.createPolyhedronMesh(polygonVertexArray);
	// Create the convex mesh collision shape
	//ConvexMeshShape * convexMeshShape = physicsCommon.createConvexMeshShape(polyhedronMesh);
}

void CReactPhysicsComponent::Update(const UpdateEventArgs & e)
{
	ISceneNode3D& sceneNode3D = const_cast<ISceneNode3D&>(GetOwnerNode());
	sceneNode3D.SetTranslate(GetPhysicsPosition());

	const auto& rot = m_RPRigidBody->getTransform().getOrientation();
	//sceneNode3D.SetRotation(glm::vec3(rot.x, rot.y, rot.z));
	sceneNode3D.SetRotationQuaternion(glm::quat(rot.w, rot.x, rot.y, rot.z));
}

glm::vec3 CReactPhysicsComponent::GetPhysicsPosition() const
{
	_ASSERT(m_RPRigidBody != NULL);
	const auto& pos = m_RPRigidBody->getTransform().getPosition();
	return glm::vec3(pos.x, pos.y, pos.z);
}

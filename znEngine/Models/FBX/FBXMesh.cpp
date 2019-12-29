#include "stdafx.h"

// Include
#include "FBXScene.h"

// General
#include "FBXMesh.h"

// Additional
#include "FBXDisplayCommon.h"

CFBXMesh::CFBXMesh(const IBaseManager * BaseManager, std::shared_ptr<CFBXScene> OwnerScene, fbxsdk::FbxMesh * NativeMesh)
	: MeshProxie(BaseManager->GetManager<IRenderDevice>()->CreateMesh())
	, m_BaseManager(BaseManager)
	, m_OwnerScene(OwnerScene)
	, m_NativeMesh(NativeMesh)
{
}

CFBXMesh::~CFBXMesh()
{
}

void CFBXMesh::Load()
{
	std::shared_ptr<IRenderDevice> renderDevice = m_BaseManager->GetManager<IRenderDevice>();


	DisplayMetaDataConnections(m_NativeMesh);


	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;

	FbxVector4* lControlPoints = m_NativeMesh->GetControlPoints();
	for (int i = 0; i < m_NativeMesh->GetPolygonCount(); i++)
	{
		int lPolygonSize = m_NativeMesh->GetPolygonSize(i);

		for (int j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = m_NativeMesh->GetPolygonVertex(i, j);
			vertices.push_back(glm::vec3(lControlPoints[lControlPointIndex][0], lControlPoints[lControlPointIndex][1], lControlPoints[lControlPointIndex][2]));


			//
			// Textures
			//
			for (int l = 0; l < m_NativeMesh->GetElementUVCount(); ++l)
			{
				fbxsdk::FbxGeometryElementUV* leUV = m_NativeMesh->GetElementUV(l);

				switch (leUV->GetMappingMode())
				{

				case FbxGeometryElement::eByControlPoint:
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						glm::vec2 uv = glm::vec2(leUV->GetDirectArray().GetAt(lControlPointIndex)[0], leUV->GetDirectArray().GetAt(lControlPointIndex)[1]);
						uvs.push_back(uv);
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
						glm::vec2 uv = glm::vec2(leUV->GetDirectArray().GetAt(id)[0], leUV->GetDirectArray().GetAt(id)[1]);
						uvs.push_back(uv);
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					int lTextureUVIndex = m_NativeMesh->GetTextureUVIndex(i, j);
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
					{
						uvs.push_back(glm::vec2(leUV->GetDirectArray().GetAt(lTextureUVIndex)[0], leUV->GetDirectArray().GetAt(lTextureUVIndex)[1]));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;

				default:
					break;
				}


				break;
			}
		}
	}

	AddVertexBuffer(BufferBinding("POSITION", 0), renderDevice->CreateVertexBuffer(vertices));
	AddVertexBuffer(BufferBinding("TEXCOORD", 0), renderDevice->CreateVertexBuffer(uvs));

#if 0

	//
	// Vertices (aka Control points)
	//
	{
		FbxVector4* elem = m_NativeMesh->GetControlPoints();
		int arrSize = m_NativeMesh->GetControlPointsCount();

		std::vector<glm::vec3> vertices;
		vertices.resize(arrSize);
		for (int i = 0; i < arrSize; i++)
			vertices[i] = vec3(elem[i][0], elem[i][1], elem[i][2]);

		mesh->AddVertexBuffer(BufferBinding("POSITION", 0), renderDevice->CreateVertexBuffer(vertices));
	}



	//
	// Normals
	//
	for (int l = 0; l < m_NativeMesh->GetElementNormalCount(); l++)
	{
		fbxsdk::FbxGeometryElementNormal* elem = m_NativeMesh->GetElementNormal(l);
		int arrSize = elem->GetDirectArray().GetCount();

		std::vector<glm::vec3> normals;
		normals.resize(arrSize);
		for (int uvIter = 0; uvIter < arrSize; uvIter++)
			normals[uvIter] = glm::vec3(elem->GetDirectArray().GetAt(uvIter)[0], elem->GetDirectArray().GetAt(uvIter)[1], elem->GetDirectArray().GetAt(uvIter)[2]);

		mesh->AddVertexBuffer(BufferBinding("NORMAL", l), renderDevice->CreateVertexBuffer(normals));
	}


	//
	// Binormal
	//
	for (int l = 0; l < m_NativeMesh->GetElementBinormalCount(); l++)
	{
		fbxsdk::FbxGeometryElementBinormal* elem = m_NativeMesh->GetElementBinormal(l);
		int arrSize = elem->GetDirectArray().GetCount();

		std::vector<glm::vec3> binormals;
		binormals.resize(arrSize);
		for (int uvIter = 0; uvIter < arrSize; uvIter++)
			binormals[uvIter] = glm::vec3(elem->GetDirectArray().GetAt(uvIter)[0], elem->GetDirectArray().GetAt(uvIter)[1], elem->GetDirectArray().GetAt(uvIter)[2]);

		mesh->AddVertexBuffer(BufferBinding("BINORMAL", l), renderDevice->CreateVertexBuffer(binormals));
	}



	//
	// Tangent
	//
	for (int l = 0; l < m_NativeMesh->GetElementTangentCount(); l++)
	{
		fbxsdk::FbxGeometryElementTangent* elem = m_NativeMesh->GetElementTangent(l);
		int arrSize = elem->GetDirectArray().GetCount();

		std::vector<glm::vec3> tangents;
		tangents.resize(arrSize);
		for (int uvIter = 0; uvIter < arrSize; uvIter++)
			tangents[uvIter] = glm::vec3(elem->GetDirectArray().GetAt(uvIter)[0], elem->GetDirectArray().GetAt(uvIter)[1], elem->GetDirectArray().GetAt(uvIter)[2]);

		mesh->AddVertexBuffer(BufferBinding("TANGENT", l), renderDevice->CreateVertexBuffer(tangents));
	}



	//
	// VertexColor
	//
	for (int l = 0; l < m_NativeMesh->GetElementVertexColorCount(); l++)
	{
		fbxsdk::FbxGeometryElementVertexColor* elem = m_NativeMesh->GetElementVertexColor(l);
		int arrSize = elem->GetDirectArray().GetCount();

		std::vector<glm::vec4> colors;
		colors.resize(arrSize);
		for (int uvIter = 0; uvIter < arrSize; uvIter++)
			colors[uvIter] = glm::vec4(elem->GetDirectArray().GetAt(uvIter)[0], elem->GetDirectArray().GetAt(uvIter)[1], elem->GetDirectArray().GetAt(uvIter)[2], elem->GetDirectArray().GetAt(uvIter)[3]);

		mesh->AddVertexBuffer(BufferBinding("COLOR", l), renderDevice->CreateVertexBuffer(colors));
	}



	//
	// Textures UV
	//
	for (int l = 0; l < m_NativeMesh->GetElementUVCount(); ++l)
	{
		fbxsdk::FbxGeometryElementUV* elem = m_NativeMesh->GetElementUV(l);
		_ASSERT(elem->GetMappingMode() == FbxGeometryElement::eByPolygonVertex);

		if (elem->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			int arrSize = elem->GetDirectArray().GetCount();

			std::vector<glm::vec2> uvs;
			uvs.resize(arrSize);
			for (int uvIter = 0; uvIter < arrSize; uvIter++)
				uvs[uvIter] = glm::vec2(elem->GetDirectArray().GetAt(uvIter)[0], elem->GetDirectArray().GetAt(uvIter)[1]);
			mesh->AddVertexBuffer(BufferBinding("TEXCOORD", l), renderDevice->CreateVertexBuffer(uvs));
		}
		else if (elem->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
		{
			int arrSize = elem->GetIndexArray().GetCount();

			std::vector<glm::vec2> uvs;
			uvs.resize(arrSize);
			for (int uvIter = 0; uvIter < arrSize; uvIter++)
				uvs[uvIter] = glm::vec2(elem->GetDirectArray().GetAt(elem->GetIndexArray().GetAt(uvIter))[0], elem->GetDirectArray().GetAt(elem->GetIndexArray().GetAt(uvIter))[1]);
			mesh->AddVertexBuffer(BufferBinding("TEXCOORD", l), renderDevice->CreateVertexBuffer(uvs));
		}
		else
		{
			_ASSERT(false);
		}

		/*leUV->GetDirectArray().ReadLock();
		mesh->AddVertexBuffer
		(
			BufferBinding("TEXCOORD", l),
			renderDevice->CreateVoidVertexBuffer(leUV->GetDirectArray().GetLocked((fbxsdk::FbxDouble2*)0x00, fbxsdk::FbxLayerElementArray::eReadLock), leUV->GetDirectArray().GetCount(), 0, sizeof(fbxsdk::FbxDouble2))
		);
		leUV->GetDirectArray().ReadUnlock();*/
	}


#endif

	//
	// Incides
	//
	{
		//mesh->SetIndexBuffer(renderDevice->CreateIndexBuffer(m_NativeMesh->GetPolygonVertices(), m_NativeMesh->GetPolygonVertexCount()));
	}



	//
	// Materials
	//
	_ASSERT_EXPR(m_NativeMesh->GetElementMaterialCount() == 1, L"Only one material must supported for mesh.");
	for (int l = 0; l < m_NativeMesh->GetElementMaterialCount(); l++)
	{
		fbxsdk::FbxGeometryElementMaterial* materialElement = m_NativeMesh->GetElementMaterial(l);
		_ASSERT_EXPR(materialElement->GetMappingMode() == FbxGeometryElement::eAllSame, L"Material mapping must be all same.");

		int materialID = materialElement->GetIndexArray().GetAt(0);
		Log::Print("Mesh refer to material '%d'. Index size '%d'.", materialID, materialElement->GetIndexArray().GetCount());
		SetMaterial(m_OwnerScene->GetMaterial(materialID));
	}

	/*
	DisplayMaterialMapping(m_NativeMesh);
	Log::Print("--------------------------------------------------------------------------------------\n");
	//mesh->SetMaterial(DisplayMaterial(renderDevice, m_NativeMesh));
	Log::Print("--------------------------------------------------------------------------------------\n");
	DisplayMaterialConnections(m_NativeMesh);
	Log::Print("--------------------------------------------------------------------------------------\n");
	DisplayLink(m_NativeMesh);
	Log::Print("--------------------------------------------------------------------------------------\n");
	DisplayShape(m_NativeMesh);
	Log::Print("--------------------------------------------------------------------------------------\n");
	DisplayCache(m_NativeMesh);
	Log::Print("--------------------------------------------------------------------------------------\n");
	*/
}

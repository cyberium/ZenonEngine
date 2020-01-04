#include "stdafx.h"

// Include
#include "FBXSceneNode.h"

// General
#include "FBXMesh.h"

// Additional
#include "FBXDisplayCommon.h"
#include "FBXDisplayMesh.h"

CFBXMesh::CFBXMesh(const IBaseManager * BaseManager, std::weak_ptr<CFBXSceneNode> OwnerFBXNode, fbxsdk::FbxMesh * NativeMesh)
	: MeshProxie(BaseManager->GetManager<IRenderDevice>()->CreateMesh())
	, m_BaseManager(BaseManager)
	, m_OwnerFBXNode(OwnerFBXNode)
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
#if 0
	_ASSERT_EXPR(m_NativeMesh->GetElementMaterialCount() == 1, L"Only one material must supported for mesh.");
	for (int l = 0; l < m_NativeMesh->GetElementMaterialCount(); l++)
	{
		fbxsdk::FbxGeometryElementMaterial* materialElement = m_NativeMesh->GetElementMaterial(l);
		//_ASSERT_EXPR(materialElement->GetMappingMode() == FbxGeometryElement::eAllSame, L"Material mapping must be all same.");

		int materialID = materialElement->GetIndexArray().GetAt(0);
		Log::Print("Mesh refer to material '%d'. Index size '%d'.", materialID, materialElement->GetIndexArray().GetCount());
		AddMaterial(m_OwnerFBXNode.lock()->GetMaterial(materialID));
	}
#endif

	Log::Info("CFBXMesh: Mesh '%s' loaded. Polygons count = '%d'.", m_NativeMesh->GetName(), m_NativeMesh->GetPolygonCount());


	DisplayMaterialConnections();
}

void CFBXMesh::DisplayMaterialConnections()
{
	struct SPolygonConnectionInfo
	{
		int PolygonBegin;
		int PolygonEnd;
	};
	std::unordered_map<int, SPolygonConnectionInfo> polygonConnectionInfos;


	bool lIsAllSame = true;
	for (int l = 0; l < m_NativeMesh->GetElementMaterialCount(); l++)
	{
		FbxGeometryElementMaterial* lMaterialElement = m_NativeMesh->GetElementMaterial(l);
		if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
		{
			lIsAllSame = false;
			break;
		}
	}

	if (lIsAllSame)
	{
		_ASSERT(m_NativeMesh->GetElementMaterialCount() == 1);
		for (int l = 0; l < m_NativeMesh->GetElementMaterialCount(); l++)
		{
			FbxGeometryElementMaterial* lMaterialElement = m_NativeMesh->GetElementMaterial(l);
			if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
			{
				FbxSurfaceMaterial* lMaterial = m_NativeMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(0));
				int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
				_ASSERT(lMatId >= 0);

				SetMaterial(m_OwnerFBXNode.lock()->GetMaterial(lMatId));
			}
			else
			{
				_ASSERT(false);
			}
		}

		//no material
		if (m_NativeMesh->GetElementMaterialCount() == 0)
			DisplayString("        no material applied");
	}
	else
	{
		for (int polygonIndex = 0; polygonIndex < m_NativeMesh->GetPolygonCount(); polygonIndex++)
		{
			_ASSERT(m_NativeMesh->GetElementMaterialCount() == 1);
			for (int l = 0; l < m_NativeMesh->GetElementMaterialCount(); l++)
			{
				FbxGeometryElementMaterial* lMaterialElement = m_NativeMesh->GetElementMaterial(l);
				int lMatId = lMaterialElement->GetIndexArray().GetAt(polygonIndex);
				_ASSERT(lMatId >= 0);

				std::unordered_map<int, SPolygonConnectionInfo>::iterator it = polygonConnectionInfos.find(lMatId);
				if (it == polygonConnectionInfos.end())
				{
					polygonConnectionInfos.insert(std::make_pair(lMatId, SPolygonConnectionInfo{ polygonIndex, polygonIndex }));
				}
				else
				{
					(*it).second.PolygonEnd = polygonIndex;
				}
			}
		}

		for (const auto& it : polygonConnectionInfos)
		{
			SGeometryPartParams geometryPartParams;
			geometryPartParams.VertexStartLocation = it.second.PolygonBegin * 3;
			geometryPartParams.VertexCnt = it.second.PolygonEnd * 3 - geometryPartParams.VertexStartLocation + 3;

			AddMaterial(m_OwnerFBXNode.lock()->GetMaterial(it.first), geometryPartParams);

			//Log::Info("Material with id '%d' added for (%d to %d)", it.first, geometryPartParams.VertexStartLocation, geometryPartParams.VertexCnt);
		}
	}
}

void CFBXMesh::DisplayMaterialMapping(fbxsdk::FbxGeometryElementMaterial* materialElement)
{
	const char* lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };
	const char* lReferenceMode[] = { "Direct", "Index", "Index to Direct" };


	DisplayString("           Mapping: ", lMappingTypes[materialElement->GetMappingMode()]);
	DisplayString("           ReferenceMode: ", lReferenceMode[materialElement->GetReferenceMode()]);

	if (materialElement->GetReferenceMode() == FbxGeometryElement::eDirect)
	{

	}
	else if (materialElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
	{
		FbxString lString;

		lString = "           Indices: ";

		for (int i = 0; i < materialElement->GetIndexArray().GetCount(); i++)
		{
			lString += materialElement->GetIndexArray().GetAt(i);
			lString += ", ";
		}

		Log::Print(lString);
	}
}
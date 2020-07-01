#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// Include
#include "FBXSceneNode.h"

// General
#include "FBXMesh.h"

// Additional
#include "FBXDisplayCommon.h"


inline glm::vec3 ToGLMVec3(const FbxPropertyT<FbxDouble3>& FBXVec3)
{
	return glm::vec3(FBXVec3.Get()[0], FBXVec3.Get()[1], FBXVec3.Get()[2]);
}


CFBXMesh::CFBXMesh(const IBaseManager& BaseManager)
	: ModelProxie(BaseManager.GetApplication().GetRenderDevice().GetObjectsFactory().CreateModel())
	, m_BaseManager(BaseManager)
{
}

CFBXMesh::~CFBXMesh()
{
}

void CFBXMesh::Load(const CFBXSceneNode& FBXNode, fbxsdk::FbxMesh* NativeMesh)
{
	IRenderDevice& renderDevice = m_BaseManager.GetApplication().GetRenderDevice();

	if (NativeMesh == nullptr)
		return;

	FbxVector4* lControlPoints = NativeMesh->GetControlPoints();
	if (lControlPoints == nullptr)
		return;

	SetName(NativeMesh->GetName());

	NativeMesh->ComputeBBox();
	SetBounds(BoundingBox(ToGLMVec3(NativeMesh->BBoxMin), ToGLMVec3(NativeMesh->BBoxMax)));

	NativeMesh->GenerateNormals(true, true);

	DisplayMetaDataConnections(NativeMesh);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> binormal;
	std::vector<glm::vec3> tangent;

	for (int i = 0; i < NativeMesh->GetPolygonCount(); i++)
	{
		int lPolygonSize = NativeMesh->GetPolygonSize(i);
		if (lPolygonSize > 3)
			lPolygonSize = 3;

		for (int j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = NativeMesh->GetPolygonVertex(i, j);
			vertices.push_back(glm::vec3(lControlPoints[lControlPointIndex][0], lControlPoints[lControlPointIndex][1], lControlPoints[lControlPointIndex][2]));

			//
			// Textures
			//
			int cnt = NativeMesh->GetElementUVCount();
			if (cnt > 1)
				cnt = 1;

			for (int l = 0; l < cnt; ++l)
			{
				const fbxsdk::FbxGeometryElementUV* leUV = NativeMesh->GetElementUV(l);

				switch (leUV->GetMappingMode())
				{

				case FbxGeometryElement::eByControlPoint:
				{
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
						_ASSERT(false);
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					int lTextureUVIndex = NativeMesh->GetTextureUVIndex(i, j);
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
					{
						uvs.push_back(glm::vec2(leUV->GetDirectArray().GetAt(lTextureUVIndex)[0], leUV->GetDirectArray().GetAt(lTextureUVIndex)[1]));
					}
					break;
					default:
						_ASSERT(false);
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					_ASSERT(false);
					break;

				default:
					_ASSERT(false);
					break;
				}
			}



			//
			// Normals
			//
			cnt = NativeMesh->GetElementNormalCount();
			if (cnt > 1)
				cnt = 1;
			for (int l = 0; l < cnt; ++l)
			{
				fbxsdk::FbxGeometryElementNormal* elem = NativeMesh->GetElementNormal(l);
				switch (elem->GetMappingMode())
				{

				case FbxGeometryElement::eByControlPoint:
				{
					switch (elem->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						glm::vec3 uv = glm::vec3(elem->GetDirectArray().GetAt(lControlPointIndex)[0], elem->GetDirectArray().GetAt(lControlPointIndex)[1], elem->GetDirectArray().GetAt(lControlPointIndex)[2]);
						normals.push_back(uv);
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = elem->GetIndexArray().GetAt(lControlPointIndex);
						glm::vec3 uv = glm::vec3(elem->GetDirectArray().GetAt(id)[0], elem->GetDirectArray().GetAt(id)[1], elem->GetDirectArray().GetAt(id)[2]);
						normals.push_back(uv);
					}
					break;
					default:
						_ASSERT(false);
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					int lTextureUVIndex = NativeMesh->GetTextureUVIndex(i, j);
					switch (elem->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						normals.push_back(glm::vec3(elem->GetDirectArray().GetAt(lTextureUVIndex)[0], elem->GetDirectArray().GetAt(lTextureUVIndex)[1], elem->GetDirectArray().GetAt(lTextureUVIndex)[2]));
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = elem->GetIndexArray().GetAt(lTextureUVIndex);
						normals.push_back(glm::vec3(elem->GetDirectArray().GetAt(id)[0], elem->GetDirectArray().GetAt(id)[1], elem->GetDirectArray().GetAt(id)[2]));
					}
					break;
					default:
						_ASSERT(false);
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					_ASSERT(false);
					break;

				default:
					_ASSERT(false);
					break;
				}
			}



			//
			// Binormal
			//
			cnt = NativeMesh->GetElementBinormalCount();
			if (cnt > 1)
				cnt = 1;
			for (int l = 0; l < cnt; ++l)
			{
				fbxsdk::FbxGeometryElementBinormal* elem = NativeMesh->GetElementBinormal(l);
				switch (elem->GetMappingMode())
				{

				case FbxGeometryElement::eByControlPoint:
				{
					switch (elem->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						glm::vec3 uv = glm::vec3(elem->GetDirectArray().GetAt(lControlPointIndex)[0], elem->GetDirectArray().GetAt(lControlPointIndex)[1], elem->GetDirectArray().GetAt(lControlPointIndex)[2]);
						binormal.push_back(uv);
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = elem->GetIndexArray().GetAt(lControlPointIndex);
						glm::vec3 uv = glm::vec3(elem->GetDirectArray().GetAt(id)[0], elem->GetDirectArray().GetAt(id)[1], elem->GetDirectArray().GetAt(id)[2]);
						binormal.push_back(uv);
					}
					break;
					default:
						_ASSERT(false);
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					int lTextureUVIndex = NativeMesh->GetTextureUVIndex(i, j);
					switch (elem->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
					{
						binormal.push_back(glm::vec3(elem->GetDirectArray().GetAt(lTextureUVIndex)[0], elem->GetDirectArray().GetAt(lTextureUVIndex)[1], elem->GetDirectArray().GetAt(lTextureUVIndex)[2]));
					}
					break;
					default:
						_ASSERT(false);
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					_ASSERT(false);
					break;

				default:
					_ASSERT(false);
					break;
				}
			}



			//
			// Tangent
			//
			cnt = NativeMesh->GetElementTangentCount();
			if (cnt > 1)
				cnt = 1;
			for (int l = 0; l < cnt; ++l)
			{
				fbxsdk::FbxGeometryElementTangent* elem = NativeMesh->GetElementTangent(l);
				switch (elem->GetMappingMode())
				{

				case FbxGeometryElement::eByControlPoint:
				{
					switch (elem->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					{
						glm::vec3 uv = glm::vec3(elem->GetDirectArray().GetAt(lControlPointIndex)[0], elem->GetDirectArray().GetAt(lControlPointIndex)[1], elem->GetDirectArray().GetAt(lControlPointIndex)[2]);
						tangent.push_back(uv);
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = elem->GetIndexArray().GetAt(lControlPointIndex);
						glm::vec3 uv = glm::vec3(elem->GetDirectArray().GetAt(id)[0], elem->GetDirectArray().GetAt(id)[1], elem->GetDirectArray().GetAt(id)[2]);
						tangent.push_back(uv);
					}
					break;
					default:
						_ASSERT(false);
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					int lTextureUVIndex = NativeMesh->GetTextureUVIndex(i, j);
					switch (elem->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
					{
						tangent.push_back(glm::vec3(elem->GetDirectArray().GetAt(lTextureUVIndex)[0], elem->GetDirectArray().GetAt(lTextureUVIndex)[1], elem->GetDirectArray().GetAt(lTextureUVIndex)[2]));
					}
					break;
					default:
						_ASSERT(false);
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					_ASSERT(false);
					break;

				default:
					_ASSERT(false);
					break;
				}
			}
		}
	}

	m_Geometry = m_BaseManager.GetApplication().GetRenderDevice().GetObjectsFactory().CreateGeometry();

	m_Geometry->AddVertexBuffer(BufferBinding("POSITION", 0), renderDevice.GetObjectsFactory().CreateVertexBuffer(vertices));
	m_Geometry->AddVertexBuffer(BufferBinding("TEXCOORD", 0), renderDevice.GetObjectsFactory().CreateVertexBuffer(uvs));
	m_Geometry->AddVertexBuffer(BufferBinding("NORMAL", 0), renderDevice.GetObjectsFactory().CreateVertexBuffer(normals));

	//if (!binormal.empty())
	//	AddVertexBuffer(BufferBinding("BINORMAL", 0), renderDevice->CreateVertexBuffer(binormal));

	//if (!tangent.empty())
	//	AddVertexBuffer(BufferBinding("TANGENT", 0), renderDevice->CreateVertexBuffer(tangent));

#if 0
	// Vertices (aka Control points)
	{
		FbxVector4* elem = NativeMesh->GetControlPoints();
		int arrSize = NativeMesh->GetControlPointsCount();

		std::vector<glm::vec3> vertices;
		vertices.resize(arrSize);
		for (int i = 0; i < arrSize; i++)
			vertices[i] = vec3(elem[i][0], elem[i][1], elem[i][2]);

		mesh->AddVertexBuffer(BufferBinding("POSITION", 0), renderDevice->CreateVertexBuffer(vertices));
	}



	//
	// Normals
	//
	for (int l = 0; l < NativeMesh->GetElementNormalCount(); l++)
	{
		fbxsdk::FbxGeometryElementNormal* elem = NativeMesh->GetElementNormal(l);
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
	for (int l = 0; l < NativeMesh->GetElementBinormalCount(); l++)
	{
		fbxsdk::FbxGeometryElementBinormal* elem = NativeMesh->GetElementBinormal(l);
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
	for (int l = 0; l < NativeMesh->GetElementTangentCount(); l++)
	{
		fbxsdk::FbxGeometryElementTangent* elem = NativeMesh->GetElementTangent(l);
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
	for (int l = 0; l < NativeMesh->GetElementVertexColorCount(); l++)
	{
		fbxsdk::FbxGeometryElementVertexColor* elem = NativeMesh->GetElementVertexColor(l);
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
	for (int l = 0; l < NativeMesh->GetElementUVCount(); ++l)
	{
		fbxsdk::FbxGeometryElementUV* elem = NativeMesh->GetElementUV(l);
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

	Log::Info("CFBXMesh: Mesh '%s' loaded. Polygons count = '%d'. (Has normals: '%s')", NativeMesh->GetName(), NativeMesh->GetPolygonCount(), (!normals.empty()) ? "true" : "false");

	DisplayMaterialConnections(FBXNode, NativeMesh);
}

void CFBXMesh::DisplayMaterialConnections(const CFBXSceneNode& FBXNode, fbxsdk::FbxMesh* NativeMesh)
{
	struct SPolygonConnectionInfo
	{
		int PolygonBegin;
		int PolygonEnd;
	};
	std::unordered_map<int, SPolygonConnectionInfo> polygonConnectionInfos;


	bool lIsAllSame = true;
	for (int l = 0; l < NativeMesh->GetElementMaterialCount(); l++)
	{
		FbxGeometryElementMaterial* lMaterialElement = NativeMesh->GetElementMaterial(l);
		if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
		{
			lIsAllSame = false;
			break;
		}
	}

	if (lIsAllSame)
	{
		_ASSERT(NativeMesh->GetElementMaterialCount() == 1);
		for (int l = 0; l < NativeMesh->GetElementMaterialCount(); l++)
		{
			FbxGeometryElementMaterial* lMaterialElement = NativeMesh->GetElementMaterial(l);
			if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
			{
				FbxSurfaceMaterial* lMaterial = NativeMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(0));
				int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
				_ASSERT(lMatId >= 0);

				AddConnection(FBXNode.GetMaterial(lMatId), m_Geometry);
			}
			else
			{
				_ASSERT(false);
			}
		}

		//no material
		if (NativeMesh->GetElementMaterialCount() == 0)
			DisplayString("        no material applied");
	}
	else
	{
		for (int polygonIndex = 0; polygonIndex < NativeMesh->GetPolygonCount(); polygonIndex++)
		{
			_ASSERT(NativeMesh->GetElementMaterialCount() == 1);
			for (int l = 0; l < NativeMesh->GetElementMaterialCount(); l++)
			{
				FbxGeometryElementMaterial* lMaterialElement = NativeMesh->GetElementMaterial(l);
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
			SGeometryDrawArgs GeometryDrawArgs;
			GeometryDrawArgs.VertexStartLocation = it.second.PolygonBegin * 3;
			GeometryDrawArgs.VertexCnt = it.second.PolygonEnd * 3 - GeometryDrawArgs.VertexStartLocation + 3;

			AddConnection(FBXNode.GetMaterial(it.first), m_Geometry, GeometryDrawArgs);

			//Log::Info("Material with id '%d' added for (%d to %d)", it.first, GeometryDrawArgs.VertexStartLocation, GeometryDrawArgs.VertexCnt);
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

#endif

#include "stdafx.h"

// General
#include "FBXDisplayMesh.h"

// Additionals
#include <fbxsdk.h>
#include <fbxsdk/scene/geometry/fbxlayer.h>
#include <fbxsdk/utils/fbxgeometryconverter.h>

#include "FBXDisplayLink.h"
#include "FBXDisplayShape.h"
#include "FBXDisplayCache.h"


using namespace fbxsdk;

#define MAT_HEADER_LENGTH 200



std::shared_ptr<IMesh> DisplayMesh(FbxNode* pNode)
{
	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();
	Log::Info("FBX: Mesh name is '%s'", pNode->GetName());

	const IBaseManager* null;
	std::shared_ptr<IRenderDevice> renderDevice = null->GetManager<IRenderDevice>();

	std::shared_ptr<IMesh> mesh = renderDevice->CreateMesh();

	DisplayMetaDataConnections(lMesh);


	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;

	FbxVector4* lControlPoints = lMesh->GetControlPoints();
	for (int i = 0; i < lMesh->GetPolygonCount(); i++)
	{
		int lPolygonSize = lMesh->GetPolygonSize(i);

		for (int j = 0; j < lPolygonSize; j++)
		{
			int lControlPointIndex = lMesh->GetPolygonVertex(i, j);
			vertices.push_back(glm::vec3(lControlPoints[lControlPointIndex][0], lControlPoints[lControlPointIndex][1], lControlPoints[lControlPointIndex][2]));


			//
			// Textures
			//
			for (int l = 0; l < lMesh->GetElementUVCount(); ++l)
			{
				fbxsdk::FbxGeometryElementUV* leUV = lMesh->GetElementUV(l);

				switch (leUV->GetMappingMode())
				{
				default:
					break;
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
					int lTextureUVIndex = lMesh->GetTextureUVIndex(i, j);
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
				}

				break;
			}
		}
	}

	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), renderDevice->CreateVertexBuffer(vertices));
	mesh->AddVertexBuffer(BufferBinding("TEXCOORD", 0), renderDevice->CreateVertexBuffer(uvs));

#if 0

	//
	// Vertices (aka Control points)
	//
	{
		FbxVector4* elem = lMesh->GetControlPoints();
		int arrSize = lMesh->GetControlPointsCount();

		std::vector<glm::vec3> vertices;
		vertices.resize(arrSize);
		for (int i = 0; i < arrSize; i++)
			vertices[i] = vec3(elem[i][0], elem[i][1], elem[i][2]);

		mesh->AddVertexBuffer(BufferBinding("POSITION", 0), renderDevice->CreateVertexBuffer(vertices));
	}



	//
	// Normals
	//
	for (int l = 0; l < lMesh->GetElementNormalCount(); l++)
	{
		fbxsdk::FbxGeometryElementNormal* elem = lMesh->GetElementNormal(l);
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
	for (int l = 0; l < lMesh->GetElementBinormalCount(); l++)
	{
		fbxsdk::FbxGeometryElementBinormal* elem = lMesh->GetElementBinormal(l);
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
	for (int l = 0; l < lMesh->GetElementTangentCount(); l++)
	{
		fbxsdk::FbxGeometryElementTangent* elem = lMesh->GetElementTangent(l);
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
	for (int l = 0; l < lMesh->GetElementVertexColorCount(); l++)
	{
		fbxsdk::FbxGeometryElementVertexColor* elem = lMesh->GetElementVertexColor(l);
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
	for (int l = 0; l < lMesh->GetElementUVCount(); ++l)
	{
		fbxsdk::FbxGeometryElementUV* elem = lMesh->GetElementUV(l);
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
		//mesh->SetIndexBuffer(renderDevice->CreateIndexBuffer(lMesh->GetPolygonVertices(), lMesh->GetPolygonVertexCount()));
	}
	


	//
	// Materials
	//
	_ASSERT_EXPR(lMesh->GetElementMaterialCount() == 1, L"Only one material must supported for mesh.");
	for (int l = 0; l < lMesh->GetElementMaterialCount(); l++)
	{
		fbxsdk::FbxGeometryElementMaterial* elem = lMesh->GetElementMaterial(l);
		_ASSERT_EXPR(elem->GetMappingMode() == FbxGeometryElement::eAllSame, L"Material mapping must be all same.");

		Log::Print("Material name is '%s'.", elem->GetName());

	}


	DisplayMaterialMapping(lMesh);
	Log::Print("--------------------------------------------------------------------------------------\n");
	//mesh->SetMaterial(DisplayMaterial(renderDevice, lMesh));
	Log::Print("--------------------------------------------------------------------------------------\n");
	DisplayMaterialConnections(lMesh);
	Log::Print("--------------------------------------------------------------------------------------\n");
	DisplayLink(lMesh);
	Log::Print("--------------------------------------------------------------------------------------\n");
	DisplayShape(lMesh);
	Log::Print("--------------------------------------------------------------------------------------\n");
	DisplayCache(lMesh);
	Log::Print("--------------------------------------------------------------------------------------\n");

	return mesh;
}


/*void DisplayTextureNames(fbxsdk::FbxProperty &pProperty, FbxString& pConnectionString)
{
	int lLayeredTextureCount = pProperty.GetSrcObjectCount<FbxLayeredTexture>();
	if (lLayeredTextureCount > 0)
	{
		for (int j = 0; j < lLayeredTextureCount; ++j)
		{
			FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
			int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
			pConnectionString += " Texture ";

			for (int k = 0; k < lNbTextures; ++k)
			{
				//lConnectionString += k;
				pConnectionString += "\"";
				pConnectionString += (char*)lLayeredTexture->GetName();
				pConnectionString += "\"";
				pConnectionString += " ";
			}
			pConnectionString += "of ";
			pConnectionString += pProperty.GetName();
			pConnectionString += " on layer ";
			pConnectionString += j;
		}
		pConnectionString += " |";
	}
	else
	{
		//no layered texture simply get on the property
		int lNbTextures = pProperty.GetSrcObjectCount<FbxTexture>();

		if (lNbTextures > 0)
		{
			pConnectionString += " Texture ";
			pConnectionString += " ";

			for (int j = 0; j < lNbTextures; ++j)
			{
				FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
				if (lTexture)
				{
					pConnectionString += "\"";
					pConnectionString += (char*)lTexture->GetName();
					pConnectionString += "\"";
					pConnectionString += " ";
				}
			}
			pConnectionString += "of ";
			pConnectionString += pProperty.GetName();
			pConnectionString += " |";
		}
	}
}*/

void DisplayMaterialConnections(FbxMesh* pMesh)
{
	int i, l, lPolygonCount = pMesh->GetPolygonCount();

	char header[MAT_HEADER_LENGTH];

	DisplayString("    Polygons Material Connections");

	//check whether the material maps with only one mesh
	bool lIsAllSame = true;
	for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
	{

		FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
		if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
		{
			lIsAllSame = false;
			break;
		}
	}

	//For eAllSame mapping type, just out the material and texture mapping info once
	if (lIsAllSame)
	{
		for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
		{

			FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);

			if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
			{
				int lMatId = lMaterialElement->GetIndexArray().GetAt(0);

				FbxSurfaceMaterial* lMaterial = pMesh->GetNode()->GetMaterial(lMatId);
				
				if (lMatId >= 0)
				{
					DisplayInt("        All polygons share the same material in mesh ", l);
					//DisplayMaterialTextureConnections(lMaterial, header, lMatId, l);
				}
			}
		}

		//no material
		if (l == 0)
			DisplayString("        no material applied");
	}

	//For eByPolygon mapping type, just out the material and texture mapping info once
	else
	{
		/*for (i = 0; i < lPolygonCount; i++)
		{
			DisplayInt("        Polygon ", i);

			for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
			{

				FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
				FbxSurfaceMaterial* lMaterial = NULL;
				int lMatId = -1;
				lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(i));
				lMatId = lMaterialElement->GetIndexArray().GetAt(i);

				if (lMatId >= 0)
				{
					DisplayMaterialTextureConnections(lMaterial, header, lMatId, l);
				}
			}
		}*/
	}
}

std::shared_ptr<IMaterial> DisplayMaterialMapping(FbxMesh* pMesh)
{
	const char* lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };
	const char* lReferenceMode[] = { "Direct", "Index", "Index to Direct" };

	int lMtrlCount = 0;
	FbxNode* lNode = NULL;
	if (pMesh) 
	{
		lNode = pMesh->GetNode();
		if (lNode)
			lMtrlCount = lNode->GetMaterialCount();
	}

	for (int l = 0; l < pMesh->GetElementMaterialCount(); l++)
	{
		FbxGeometryElementMaterial* leMat = pMesh->GetElementMaterial(l);
		if (leMat)
		{
			char header[100];
			FBXSDK_sprintf(header, 100, "    Material Element %d: ", l);
			DisplayString(header);


			DisplayString("           Mapping: ", lMappingTypes[leMat->GetMappingMode()]);
			DisplayString("           ReferenceMode: ", lReferenceMode[leMat->GetReferenceMode()]);


			int lIndexArrayCount = leMat->GetIndexArray().GetCount();

			int lMaterialCount = 0;
			FbxString lString;

			if (leMat->GetReferenceMode() == FbxGeometryElement::eDirect || leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				lMaterialCount = lMtrlCount;
			}

			if (leMat->GetReferenceMode() == FbxGeometryElement::eIndex || leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				lString = "           Indices: ";

				int lIndexArrayCount = leMat->GetIndexArray().GetCount();
				for (int i = 0; i < lIndexArrayCount; i++)
				{
					lString += leMat->GetIndexArray().GetAt(i);

					if (i < lIndexArrayCount - 1)
					{
						lString += ", ";
					}
				}

				lString += "\n";

				Log::Print(lString);
			}
		}
	}

	DisplayString("");

	return nullptr;
}

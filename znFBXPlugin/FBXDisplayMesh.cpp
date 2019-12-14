#include "stdafx.h"

// General
#include "FBXDisplayMesh.h"

// Additionals
#include <fbxsdk.h>
#include <fbxsdk/scene/geometry/fbxlayer.h>
#include <fbxsdk/utils/fbxgeometryconverter.h>

#include "FBXDisplayMaterial.h"
#include "FBXDisplayTexture.h"
#include "FBXDisplayLink.h"
#include "FBXDisplayShape.h"
#include "FBXDisplayCache.h"

#include "FBX.h"

using namespace fbxsdk;

#define MAT_HEADER_LENGTH 200

void DisplayMaterialMapping(FbxMesh* pMesh);
void DisplayTextureNames(FbxProperty &pProperty, FbxString& pConnectionString);
void DisplayMaterialConnections(FbxMesh* pMesh);
void DisplayMaterialTextureConnections(FbxSurfaceMaterial* pMaterial, char * header, int pMatId, int l);

std::shared_ptr<IMesh> DisplayMesh(CFBX * FBX, FbxManager* Manager, FbxNode* pNode)
{
	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();
	Log::Info("FBX: Mesh name is '%s'", pNode->GetName());

	std::shared_ptr<IRenderDevice> renderDevice = GetManager<IRenderDevice>(FBX->GetBaseManager());

	std::shared_ptr<IMesh> mesh = renderDevice->CreateMesh();

	DisplayMetaDataConnections(lMesh);



	//
	// Vertices (aka Control points)
	//
	{
		FbxVector4* arr = lMesh->GetControlPoints();
		int arrSize = lMesh->GetControlPointsCount();

		std::vector<glm::vec3> vertices;
		vertices.resize(arrSize);
		for (int i = 0; i < arrSize; i++)
			vertices[i] = vec3(arr[i][0], arr[i][1], arr[i][2]);

		mesh->AddVertexBuffer(BufferBinding("POSITION", 0), renderDevice->CreateVertexBuffer(vertices));
	}



	//
	// Normals
	//
	for (int l = 0; l < lMesh->GetElementNormalCount(); l++)
	{
		fbxsdk::FbxGeometryElementNormal* arr = lMesh->GetElementNormal(l);
		int arrSize = arr->GetDirectArray().GetCount();

		std::vector<glm::vec3> normals;
		normals.resize(arrSize);
		for (int uvIter = 0; uvIter < arrSize; uvIter++)
			normals[uvIter] = glm::vec3(arr->GetDirectArray().GetAt(uvIter)[0], arr->GetDirectArray().GetAt(uvIter)[1], arr->GetDirectArray().GetAt(uvIter)[2]);

		mesh->AddVertexBuffer(BufferBinding("NORMAL", l), renderDevice->CreateVertexBuffer(normals));
	}


	//
	// Binormal
	//
	for (int l = 0; l < lMesh->GetElementBinormalCount(); l++)
	{
		fbxsdk::FbxGeometryElementBinormal* arr = lMesh->GetElementBinormal(l);
		int arrSize = arr->GetDirectArray().GetCount();

		std::vector<glm::vec3> binormals;
		binormals.resize(arrSize);
		for (int uvIter = 0; uvIter < arrSize; uvIter++)
			binormals[uvIter] = glm::vec3(arr->GetDirectArray().GetAt(uvIter)[0], arr->GetDirectArray().GetAt(uvIter)[1], arr->GetDirectArray().GetAt(uvIter)[2]);

		mesh->AddVertexBuffer(BufferBinding("BINORMAL", l), renderDevice->CreateVertexBuffer(binormals));
	}



	//
	// Tangent
	//
	for (int l = 0; l < lMesh->GetElementTangentCount(); l++)
	{
		fbxsdk::FbxGeometryElementTangent* arr = lMesh->GetElementTangent(l);
		int arrSize = arr->GetDirectArray().GetCount();

		std::vector<glm::vec3> tangents;
		tangents.resize(arrSize);
		for (int uvIter = 0; uvIter < arrSize; uvIter++)
			tangents[uvIter] = glm::vec3(arr->GetDirectArray().GetAt(uvIter)[0], arr->GetDirectArray().GetAt(uvIter)[1], arr->GetDirectArray().GetAt(uvIter)[2]);

		mesh->AddVertexBuffer(BufferBinding("TANGENT", l), renderDevice->CreateVertexBuffer(tangents));
	}



	//
	// VertexColor
	//
	for (int l = 0; l < lMesh->GetElementVertexColorCount(); l++)
	{
		fbxsdk::FbxGeometryElementVertexColor* arr = lMesh->GetElementVertexColor(l);
		int arrSize = arr->GetDirectArray().GetCount();

		std::vector<glm::vec4> colors;
		colors.resize(arrSize);
		for (int uvIter = 0; uvIter < arrSize; uvIter++)
			colors[uvIter] = glm::vec4(arr->GetDirectArray().GetAt(uvIter)[0], arr->GetDirectArray().GetAt(uvIter)[1], arr->GetDirectArray().GetAt(uvIter)[2], arr->GetDirectArray().GetAt(uvIter)[3]);

		mesh->AddVertexBuffer(BufferBinding("COLOR", l), renderDevice->CreateVertexBuffer(colors));
	}



	//
	// Textures UV
	//
	for (int l = 0; l < lMesh->GetElementUVCount(); ++l)
	{
		fbxsdk::FbxGeometryElementUV* arr = lMesh->GetElementUV(l);
		int arrSize = arr->GetDirectArray().GetCount();

		std::vector<glm::vec2> uvs;
		uvs.resize(arrSize);
		for (int uvIter = 0; uvIter < arrSize; uvIter++)
			uvs[uvIter] = glm::vec2(arr->GetDirectArray().GetAt(uvIter)[0], arr->GetDirectArray().GetAt(uvIter)[1]);
		mesh->AddVertexBuffer(BufferBinding("TEXCOORD", l), renderDevice->CreateVertexBuffer(uvs));
	
		/*leUV->GetDirectArray().ReadLock();
		mesh->AddVertexBuffer
		(
			BufferBinding("TEXCOORD", l), 
			renderDevice->CreateVoidVertexBuffer(leUV->GetDirectArray().GetLocked((fbxsdk::FbxDouble2*)0x00, fbxsdk::FbxLayerElementArray::eReadLock), leUV->GetDirectArray().GetCount(), 0, sizeof(fbxsdk::FbxDouble2))
		);
		leUV->GetDirectArray().ReadUnlock();*/
	}




	//
	// Incides
	//
	{
		mesh->SetIndexBuffer(renderDevice->CreateIndexBuffer(lMesh->GetPolygonVertices(), lMesh->GetPolygonVertexCount()));
	}
	




	DisplayMaterialMapping(lMesh);
	DisplayMaterial(lMesh);
	DisplayTexture(lMesh);
	DisplayMaterialConnections(lMesh);
	DisplayLink(lMesh);
	DisplayShape(lMesh);

	DisplayCache(lMesh);

	return mesh;
}


void DisplayTextureNames(fbxsdk::FbxProperty &pProperty, FbxString& pConnectionString)
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
}

void DisplayMaterialTextureConnections(FbxSurfaceMaterial* pMaterial, char * header, int pMatId, int l)
{
	if (!pMaterial)
		return;

	FbxString lConnectionString = "            Material %d -- ";
	//Show all the textures

	FbxProperty lProperty;
	//Diffuse Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	DisplayTextureNames(lProperty, lConnectionString);

	//DiffuseFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Emissive Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
	DisplayTextureNames(lProperty, lConnectionString);

	//EmissiveFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissiveFactor);
	DisplayTextureNames(lProperty, lConnectionString);


	//Ambient Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbient);
	DisplayTextureNames(lProperty, lConnectionString);

	//AmbientFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbientFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Specular Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
	DisplayTextureNames(lProperty, lConnectionString);

	//SpecularFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Shininess Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	DisplayTextureNames(lProperty, lConnectionString);

	//Bump Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sBump);
	DisplayTextureNames(lProperty, lConnectionString);

	//Normal Map Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
	DisplayTextureNames(lProperty, lConnectionString);

	//Transparent Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparentColor);
	DisplayTextureNames(lProperty, lConnectionString);

	//TransparencyFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Reflection Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sReflection);
	DisplayTextureNames(lProperty, lConnectionString);

	//ReflectionFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sReflectionFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Update header with material info
	bool lStringOverflow = (lConnectionString.GetLen() + 10 >= MAT_HEADER_LENGTH); // allow for string length and some padding for "%d"
	if (lStringOverflow)
	{
		// Truncate string!
		lConnectionString = lConnectionString.Left(MAT_HEADER_LENGTH - 10);
		lConnectionString = lConnectionString + "...";
	}
	FBXSDK_sprintf(header, MAT_HEADER_LENGTH, lConnectionString.Buffer(), pMatId, l);
	DisplayString(header);
}

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
				FbxSurfaceMaterial* lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(0));
				int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
				if (lMatId >= 0)
				{
					DisplayInt("        All polygons share the same material in mesh ", l);
					DisplayMaterialTextureConnections(lMaterial, header, lMatId, l);
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
		for (i = 0; i < lPolygonCount; i++)
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
		}
	}
}

void DisplayMaterialMapping(FbxMesh* pMesh)
{
	const char* lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };
	const char* lReferenceMode[] = { "Direct", "Index", "Index to Direct" };

	int lMtrlCount = 0;
	FbxNode* lNode = NULL;
	if (pMesh) {
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

				FBXSDK_printf(lString);
			}
		}
	}

	DisplayString("");
}

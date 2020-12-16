#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// General
#include "FBXModel.h"

// Additional
#include "FBXDisplayCommon.h"
#include "FBXDisplayLink.h"
#include "FBXDisplayShape.h"
#include "FBXUtils.h"


namespace
{
	template <typename T>
	void ExtractLocalVertexBufferFromGlobal(const void* Data, size_t DataSize, size_t Offset, size_t Stride, std::vector<T> * Result)
	{
		_ASSERT(Result != nullptr);
		for (size_t e = 0; e < DataSize; e += Stride)
			Result->push_back(reinterpret_cast<T&>(*((uint8*)Data + e + Offset)));
	}

	BoundingBox CalculateBounds(const SGeometryDrawArgs& DrawArgs, const std::vector<FBXVertex>& Vertices)
	{
		glm::vec3 min(Math::MaxFloat);
		glm::vec3 max(Math::MinFloat);

		size_t from = 0;
		size_t to = Vertices.size();
		if (DrawArgs.VertexCnt != UINT_MAX)
		{
			from = DrawArgs.VertexStartLocation;
			_ASSERT(from < Vertices.size());

			to = DrawArgs.VertexStartLocation + DrawArgs.VertexCnt;
			_ASSERT(to < Vertices.size());
		}

		for (size_t i = from; i < to; i++)
		{
			const auto& v = Vertices.at(i).pos;

			if (v.x < min.x) min.x = v.x;
			if (v.x > max.x) max.x = v.x;

			if (v.y < min.y) min.y = v.y;
			if (v.y > max.y) max.y = v.y;
			
			if (v.z < min.z) min.z = v.z;
			if (v.z > max.z) max.z = v.z;
		}
		
		return BoundingBox(min, max);
	}
}

CFBXModel::CFBXModel(const IBaseManager& BaseManager, const IFBXNode& FBXNode)
	: ModelProxie(BaseManager.GetApplication().GetRenderDevice().GetObjectsFactory().CreateModel())
	, m_BaseManager(BaseManager)
	, m_FBXNode(FBXNode)
	, m_HasBoneWeights(false)
{
}

CFBXModel::~CFBXModel()
{
}

bool CFBXModel::Load(fbxsdk::FbxMesh* NativeMesh)
{
	_ASSERT(NativeMesh != nullptr);

	SetName(NativeMesh->GetName());

	int fbxControlPointsCount = NativeMesh->GetControlPointsCount();
	FbxVector4* fbxControlPoints = NativeMesh->GetControlPoints();
	if (fbxControlPoints == nullptr)
		throw CException("FBXModel: There is no control points.");

	NativeMesh->ComputeBBox();
	if (false == NativeMesh->GenerateNormals(true, true))
		throw CException("Error while generate normals.");


	/*for (int c = 0; c < fbxControlPointsCount; c++)
	{
		FBXVertex v = {};
		v.pos = glm::vec3(fbxControlPoints[c][0], fbxControlPoints[c][1], fbxControlPoints[c][2]);
	}*/


	/*DisplayMetaDataConnections(NativeMesh);
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> binormal;
	std::vector<glm::vec3> tangent;*/

	//DisplayLink(NativeMesh);
	//DisplayShape(NativeMesh);

	m_Vertices.clear();
	for (int p = 0; p < NativeMesh->GetPolygonCount(); p++)
	{
		int polygonSize = NativeMesh->GetPolygonSize(p);
		if (polygonSize > 3)
		{
			Log::Warn("FBXModel: Model '%s' has polygon with size '%d' insted of '3'.", GetName().c_str(), polygonSize);
			polygonSize = 3;
		}

		for (int j = 0; j < polygonSize; j++)
		{
			FBXVertex v = {};
		
			int controlPointIndex = NativeMesh->GetPolygonVertex(p, j);
			v.controlPointIndex = controlPointIndex;
			v.pos = glm::vec3(fbxControlPoints[controlPointIndex][0], fbxControlPoints[controlPointIndex][1], fbxControlPoints[controlPointIndex][2]);

			//
			// Textures
			//
			int cnt = NativeMesh->GetElementUVCount();
			if (cnt > 1) cnt = 1;
			for (int l = 0; l < cnt; ++l)
			{
				const fbxsdk::FbxGeometryElementUV* leUV = NativeMesh->GetElementUV(l);

				switch (leUV->GetMappingMode())
				{
				case FbxGeometryElement::EMappingMode::eByControlPoint:
				{
					switch (leUV->GetReferenceMode())
					{
						case FbxGeometryElement::eDirect:
						{
							v.uv = glm::vec2(leUV->GetDirectArray().GetAt(controlPointIndex)[0], leUV->GetDirectArray().GetAt(controlPointIndex)[1]);
						}
						break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = leUV->GetIndexArray().GetAt(controlPointIndex);
							v.uv = glm::vec2(leUV->GetDirectArray().GetAt(id)[0], leUV->GetDirectArray().GetAt(id)[1]);
						}
						break;

						default:
							throw CException("Unsupported reference mode '%d'", leUV->GetReferenceMode());
					}
				}
				break;

				case FbxGeometryElement::EMappingMode::eByPolygonVertex:
				{
					int lTextureUVIndex = NativeMesh->GetTextureUVIndex(p, j);
					switch (leUV->GetReferenceMode())
					{
						//case FbxGeometryElement::eDirect:
						//{
						//	v.uv = glm::vec2(leUV->GetDirectArray().GetAt(lTextureUVIndex)[0], leUV->GetDirectArray().GetAt(lTextureUVIndex)[1]);
						//}
						//break;
						case FbxGeometryElement::eIndexToDirect:
						{
							//int id = leUV->GetIndexArray().GetAt(lTextureUVIndex);
							v.uv = glm::vec2(leUV->GetDirectArray().GetAt(lTextureUVIndex)[0], leUV->GetDirectArray().GetAt(lTextureUVIndex)[1]);
						}
						break;

						default:
							throw CException("Unsupported reference mode '%d'", leUV->GetReferenceMode());
					}
				}
				break;

				default:
					throw CException("Unsupported mapping mode '%d'", leUV->GetMappingMode());
				}
			}


			//
			// Normals
			//
			cnt = NativeMesh->GetElementNormalCount();
			if (cnt > 1) cnt = 1;
			for (int l = 0; l < cnt; ++l)
			{
				fbxsdk::FbxGeometryElementNormal* elem = NativeMesh->GetElementNormal(l);
				switch (elem->GetMappingMode())
				{
				case FbxGeometryElement::EMappingMode::eByControlPoint:
				{
					switch (elem->GetReferenceMode())
					{
						case FbxGeometryElement::eDirect:
						{
							v.normal = glm::vec3(elem->GetDirectArray().GetAt(controlPointIndex)[0], elem->GetDirectArray().GetAt(controlPointIndex)[1], elem->GetDirectArray().GetAt(controlPointIndex)[2]);
						}
						break;
						//case FbxGeometryElement::eIndexToDirect:
						//{
						//	int id = elem->GetIndexArray().GetAt(controlPointIndex);
						//	v.normal = glm::vec3(elem->GetDirectArray().GetAt(id)[0], elem->GetDirectArray().GetAt(id)[1], elem->GetDirectArray().GetAt(id)[2]);
						//}
						//break;

						default:
							throw CException("Unsupported reference mode '%d'", elem->GetReferenceMode());
					}
				}
				break;

				/*case FbxGeometryElement::EMappingMode::eByPolygonVertex:
				{
					int lTextureUVIndex = NativeMesh->GetPolygonVertexNormal(p, j);
					switch (elem->GetReferenceMode())
					{
						case FbxGeometryElement::eDirect:
						{
							v.normal = glm::vec3(elem->GetDirectArray().GetAt(lTextureUVIndex)[0], elem->GetDirectArray().GetAt(lTextureUVIndex)[1], elem->GetDirectArray().GetAt(lTextureUVIndex)[2]);
						}
						break;
						case FbxGeometryElement::eIndexToDirect:
						{
							int id = elem->GetIndexArray().GetAt(lTextureUVIndex);
							v.normal = glm::vec3(elem->GetDirectArray().GetAt(id)[0], elem->GetDirectArray().GetAt(id)[1], elem->GetDirectArray().GetAt(id)[2]);
						}
						break;
						default:
							_ASSERT(false);
							break; // other reference modes not shown here!
					}
				}
				break;*/

				default:
					throw CException("Unsupported mapping mode '%d'", elem->GetMappingMode());
				}
			}

#if 0

			//
			// Binormal
			//
			cnt = NativeMesh->GetElementBinormalCount();
			if (cnt > 1) cnt = 1;
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
						v.binormal = glm::vec3(elem->GetDirectArray().GetAt(controlPointIndex)[0], elem->GetDirectArray().GetAt(lControlPointIndex)[1], elem->GetDirectArray().GetAt(lControlPointIndex)[2]);
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = elem->GetIndexArray().GetAt(controlPointIndex);
						v.binormal = glm::vec3(elem->GetDirectArray().GetAt(id)[0], elem->GetDirectArray().GetAt(id)[1], elem->GetDirectArray().GetAt(id)[2]);
					}
					break;
					default:
						_ASSERT(false);
						break; // other reference modes not shown here!
					}
				}
				break;

				/*case FbxGeometryElement::eByPolygonVertex:
				{
					int lTextureUVIndex = NativeMesh->GetTextureUVIndex(p, j);
					switch (elem->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
					{
						v.binormal = glm::vec3(elem->GetDirectArray().GetAt(lTextureUVIndex)[0], elem->GetDirectArray().GetAt(lTextureUVIndex)[1], elem->GetDirectArray().GetAt(lTextureUVIndex)[2]);
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
					break;*/

				default:
					_ASSERT(false);
					break;
				}
			}


			//
			// Tangent
			//
			cnt = NativeMesh->GetElementTangentCount();
			if (cnt > 1) cnt = 1;
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
						v.tangent = glm::vec3(elem->GetDirectArray().GetAt(controlPointIndex)[0], elem->GetDirectArray().GetAt(lControlPointIndex)[1], elem->GetDirectArray().GetAt(lControlPointIndex)[2]);
					}
					break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = elem->GetIndexArray().GetAt(controlPointIndex);
						v.tangent = glm::vec3(elem->GetDirectArray().GetAt(id)[0], elem->GetDirectArray().GetAt(id)[1], elem->GetDirectArray().GetAt(id)[2]);
					}
					break;
					default:
						_ASSERT(false);
						break; // other reference modes not shown here!
					}
				}
				break;

				/*case FbxGeometryElement::eByPolygonVertex:
				{
					int lTextureUVIndex = NativeMesh->GetTextureUVIndex(p, j);
					switch (elem->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
					{
						v.tangent = glm::vec3(elem->GetDirectArray().GetAt(lTextureUVIndex)[0], elem->GetDirectArray().GetAt(lTextureUVIndex)[1], elem->GetDirectArray().GetAt(lTextureUVIndex)[2]);
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
					break;*/

				default:
					_ASSERT(false);
					break;
				}
			}

#endif

			m_Vertices.push_back(v);
		}
	}

	SkeletonLoad(NativeMesh);

	glm::vec3 bboxMin = glm::vec3(Math::MaxFloat), bboxMax = glm::vec3(Math::MinFloat);
	for (auto& verts : m_Vertices)
	{
		verts.pos = m_FBXNode.GetTransform() * glm::vec4(verts.pos, 1.0f);

		if (verts.pos.x < bboxMin.x)
			bboxMin.x = verts.pos.x;
		if (verts.pos.y < bboxMin.y)
			bboxMin.y = verts.pos.y;
		if (verts.pos.z < bboxMin.z)
			bboxMin.z = verts.pos.z;

		if (verts.pos.x > bboxMax.x)
			bboxMax.x = verts.pos.x;
		if (verts.pos.y > bboxMax.y)
			bboxMax.y = verts.pos.y;
		if (verts.pos.z > bboxMax.z)
			bboxMax.z = verts.pos.z;
	}

	BoundingBox bbox(bboxMin, bboxMax);
	GetModelInternal()->SetBounds(bbox);

	auto loaderParams = m_FBXNode.GetFBXScene().GetLoaderParams();
	if (auto loaderFBXParams = dynamic_cast<const CznFBXLoaderParams*>(loaderParams))
	{
		if (loaderFBXParams->MakeCenterIsX0Z)
		{
			glm::vec3 offset = bbox.getCenter();
			offset.y = 0.0f;
			
			for (auto& verts : m_Vertices)
				verts.pos -= offset;

			GetModelInternal()->SetBounds(BoundingBox(bbox.getMin() - offset, bbox.getMax() - offset));
		}
	}


	IRenderDevice& renderDevice = m_BaseManager.GetApplication().GetRenderDevice();
	m_Geometry = renderDevice.GetObjectsFactory().CreateGeometry();

	{
		size_t offset = 0;

		std::vector<glm::vec3> positions;
		ExtractLocalVertexBufferFromGlobal<glm::vec3>(m_Vertices.data(), m_Vertices.size() * sizeof(FBXVertex), offset, sizeof(FBXVertex), &positions);
		m_Geometry->AddVertexBuffer(BufferBinding("POSITION", 0), renderDevice.GetObjectsFactory().CreateVertexBuffer(positions));
	}

	{
		size_t offset = sizeof(glm::vec3);

		std::vector<glm::vec2> texcoords;
		ExtractLocalVertexBufferFromGlobal<glm::vec2>(m_Vertices.data(), m_Vertices.size() * sizeof(FBXVertex), offset, sizeof(FBXVertex), &texcoords);
		m_Geometry->AddVertexBuffer(BufferBinding("TEXCOORD", 0), renderDevice.GetObjectsFactory().CreateVertexBuffer(texcoords));
	}

	{
		size_t offset = sizeof(glm::vec3) + sizeof(glm::vec2);

		std::vector<glm::vec3> normals;
		ExtractLocalVertexBufferFromGlobal<glm::vec3>(m_Vertices.data(), m_Vertices.size() * sizeof(FBXVertex), offset, sizeof(FBXVertex), &normals);
		m_Geometry->AddVertexBuffer(BufferBinding("NORMAL", 0), renderDevice.GetObjectsFactory().CreateVertexBuffer(normals));
	}

	if (m_HasBoneWeights)
	{
		{
			size_t offset = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3);

			std::vector<glm::vec4> blendWeights;
			ExtractLocalVertexBufferFromGlobal<glm::vec4>(m_Vertices.data(), m_Vertices.size() * sizeof(FBXVertex), offset, sizeof(FBXVertex), &blendWeights);
			m_Geometry->AddVertexBuffer(BufferBinding("BLENDWEIGHT", 0), renderDevice.GetObjectsFactory().CreateVertexBuffer(blendWeights));
		}

		{
			size_t offset = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(glm::vec4);

			std::vector<glm::uvec4> blendIndices;
			ExtractLocalVertexBufferFromGlobal<glm::uvec4>(m_Vertices.data(), m_Vertices.size() * sizeof(FBXVertex), offset, sizeof(FBXVertex), &blendIndices);
			m_Geometry->AddVertexBuffer(BufferBinding("BLENDINDICES", 0), renderDevice.GetObjectsFactory().CreateVertexBuffer(blendIndices));
		}



		{
			size_t offset = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::vec4);

			std::vector<glm::vec4> blendWeights2;
			ExtractLocalVertexBufferFromGlobal<glm::vec4>(m_Vertices.data(), m_Vertices.size() * sizeof(FBXVertex), offset, sizeof(FBXVertex), &blendWeights2);
			m_Geometry->AddVertexBuffer(BufferBinding("BLENDWEIGHT", 1), renderDevice.GetObjectsFactory().CreateVertexBuffer(blendWeights2));
		}

		{
			size_t offset = sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(glm::vec4) + sizeof(glm::vec4);

			std::vector<glm::uvec4> blendIndices;
			ExtractLocalVertexBufferFromGlobal<glm::uvec4>(m_Vertices.data(), m_Vertices.size() * sizeof(FBXVertex), offset, sizeof(FBXVertex), &blendIndices);
			m_Geometry->AddVertexBuffer(BufferBinding("BLENDINDICES", 1), renderDevice.GetObjectsFactory().CreateVertexBuffer(blendIndices));
		}

	}

	//m_Geometry->SetVertexBuffer(renderDevice.GetObjectsFactory().CreateVoidVertexBuffer(m_Vertices.data(), m_Vertices.size(), 0, sizeof(FBXVertex)));
	m_Geometry->SetBounds(CalculateBounds(SGeometryDrawArgs(), m_Vertices)); // TODO: Fixme

	MaterialLoad(NativeMesh);

	m_Vertices.clear();

#if 0
	if (!binormal.empty())
		m_Geometry->AddVertexBuffer(BufferBinding("BINORMAL", 0), renderDevice.GetObjectsFactory().CreateVoidVertexBuffer(binormal));

	if (!tangent.empty())
		m_Geometry->AddVertexBuffer(BufferBinding("TANGENT", 0), renderDevice.GetObjectsFactory().CreateVoidVertexBuffer(tangent));
#endif

	Log::Info("FBXModel: Mesh '%s' loaded. Polygons count = '%d'.", NativeMesh->GetName(), NativeMesh->GetPolygonCount());

	return true;
}



//
// IFBXModel
//
const IFBXNode & CFBXModel::GetOwner() const
{
	return m_FBXNode;
}

std::shared_ptr<IModel> CFBXModel::GetModel()
{
	return shared_from_this();
}



//
// Private
//
void CFBXModel::MaterialLoad(fbxsdk::FbxMesh* NativeMesh)
{
	bool lIsAllSame = true;
	for (int l = 0; l < NativeMesh->GetElementMaterialCount(); l++)
	{
		fbxsdk::FbxGeometryElementMaterial* lMaterialElement = NativeMesh->GetElementMaterial(l);
		if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
		{
			lIsAllSame = false;
			break;
		}
	}

	if (lIsAllSame)
	{
		if (NativeMesh->GetElementMaterialCount() == 0)
		{
			AddConnection(nullptr, m_Geometry);
		}
		else
		{
			//_ASSERT(NativeMesh->GetElementMaterialCount() == 1);
			for (int l = 0; l < NativeMesh->GetElementMaterialCount(); l++)
			{
				fbxsdk::FbxGeometryElementMaterial* lMaterialElement = NativeMesh->GetElementMaterial(l);
				if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
				{
					const char* matName = lMaterialElement->GetName();

					int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
					_ASSERT(lMatId >= 0);

					//FbxSurfaceMaterial* lMaterial = NativeMesh->GetNode()->GetMaterial(lMatId);

					AddConnection(m_FBXNode.GetFBXScene().GetFBXMaterials()->GetMaterial(m_FBXNode.GetFBXMaterialNameByIndex(lMatId)), m_Geometry);
				}
				else
				{
					_ASSERT(false);
				}
			}
		}
	}
	else
	{
		struct SPolygonConnectionInfo
		{
			int PolygonBegin;
			int PolygonEnd;
		};
		std::unordered_map<int, SPolygonConnectionInfo> polygonConnectionInfos;

		for (int polygonIndex = 0; polygonIndex < NativeMesh->GetPolygonCount(); polygonIndex++)
		{
			_ASSERT(NativeMesh->GetElementMaterialCount() == 1);
			for (int l = 0; l < NativeMesh->GetElementMaterialCount(); l++)
			{
				fbxsdk::FbxGeometryElementMaterial* lMaterialElement = NativeMesh->GetElementMaterial(l);
				int lMatId = lMaterialElement->GetIndexArray().GetAt(polygonIndex);
				_ASSERT(lMatId >= 0);

				const auto& it = polygonConnectionInfos.find(lMatId);
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

			AddConnection(m_FBXNode.GetFBXScene().GetFBXMaterials()->GetMaterial(m_FBXNode.GetFBXMaterialNameByIndex(it.first)), m_Geometry, GeometryDrawArgs);

			//Log::Info("Material with id '%d' added for (%d to %d)", it.first, GeometryDrawArgs.VertexStartLocation, GeometryDrawArgs.VertexCnt);
		}
	}
}

void CFBXModel::SkeletonLoad(fbxsdk::FbxMesh* NativeMesh)
{
	FbxVector4* controlPoints = NativeMesh->GetControlPoints();
	if (controlPoints == nullptr)
	{
		Log::Warn("FBXMesh: Control points not found for model %s.", NativeMesh->GetName());
		return;
	}

	const FbxVector4 lT = NativeMesh->GetNode()->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = NativeMesh->GetNode()->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = NativeMesh->GetNode()->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxAMatrix geometryTransform = FbxAMatrix(lT, lR, lS);

	int skinDeformersCount = NativeMesh->GetDeformerCount(/*fbxsdk::FbxDeformer::eSkin*/);
	if (skinDeformersCount == 0)
		return;

	for (int deformerIndex = 0; deformerIndex < skinDeformersCount; deformerIndex++)
	{
		fbxsdk::FbxSkin* skinDeformer = static_cast<fbxsdk::FbxSkin*>(NativeMesh->GetDeformer(deformerIndex, fbxsdk::FbxDeformer::eSkin));
		if (skinDeformer == nullptr)
		{
			throw CException("FBXMesh: Skin not found for model '%s'.", NativeMesh->GetName());
		}

		fbxsdk::FbxSkin::EType skinningType = skinDeformer->GetSkinningType();
		if (skinningType != fbxsdk::FbxSkin::EType::eRigid)
			throw CException("Only FbxSkin::EType::eRigid skinning supported.");

		for (int clusterIndex = 0; clusterIndex < skinDeformer->GetClusterCount(); ++clusterIndex)
		{
			fbxsdk::FbxCluster* cluster = skinDeformer->GetCluster(clusterIndex);

			fbxsdk::FbxCluster::ELinkMode clusterLinkMode = cluster->GetLinkMode();
			if (clusterLinkMode != fbxsdk::FbxCluster::ELinkMode::eNormalize)
				throw CException("Only FbxCluster::ELinkMode::eNormalize skinning supported.");

			FbxAMatrix transformMatrix;
			cluster->GetTransformMatrix(transformMatrix);

			FbxAMatrix transformLinkMatrix;
			cluster->GetTransformLinkMatrix(transformLinkMatrix);

			FbxAMatrix globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;
			glm::mat4 globalBindposeInverseMatrixGLM = ToGLMMat4(globalBindposeInverseMatrix);

			glm::mat4 fuck(0.0f);

			std::string jointname = cluster->GetLink()->GetName();
			size_t jointIndex = m_FBXNode.GetFBXScene().GetFBXSkeleton()->GetBoneIndexByName(jointname);
			auto joint = m_FBXNode.GetFBXScene().GetFBXSkeleton()->GetBoneByName(jointname);
			
			// Pivot
			fbxsdk::FbxAMatrix pivotMatrix;
			NativeMesh->GetPivot(pivotMatrix);
			joint->SetPivotMatrix(ToGLMMat4(pivotMatrix));



			// controlPoint - weight, boneIndex
			/*std::map<size_t, std::pair<float, size_t>> weights;
			for (int i = 0; i < cluster->GetControlPointIndicesCount(); ++i)
			{
				const float weight = static_cast<float>(cluster->GetControlPointWeights()[i]);
				weights.insert(std::make_pair(i, std::make_pair(weight, jointIndex)));
			}

			for (auto& v : m_Vertices)
			{
				if (v.controlPointIndex == UINT32_MAX)
					throw CException("Fail.");

				const auto& weightIt = weights.find(v.controlPointIndex);
				if (weightIt == weights.end())
					continue;

				v.weights[0] += weightIt->second.first;
				v.indexes[0] = weightIt->second.second;
				m_HasBoneWeights = true;
			}*/

			struct SBoneWDef
			{
				size_t ControlPoint;

				float Weight;
				size_t BoneIndex;
			};

			std::vector<SBoneWDef> bonesDefs;

			for (int i = 0; i < cluster->GetControlPointIndicesCount(); ++i)
			{
				SBoneWDef def;
				def.ControlPoint = cluster->GetControlPointIndices()[i];
				def.Weight = static_cast<float>(cluster->GetControlPointWeights()[i]);
				def.BoneIndex = jointIndex;
				bonesDefs.push_back(def);
			}

			for (auto& v : m_Vertices)
			{
				const auto& boneDef = std::find_if(bonesDefs.begin(), bonesDefs.end(), [v](const SBoneWDef& Def) {
					return v.controlPointIndex == Def.ControlPoint;
				});
				if (boneDef == bonesDefs.end())
					continue;

				glm::mat4 newM = globalBindposeInverseMatrixGLM * boneDef->Weight;
				fuck += newM;

				if (v.bonesCnt >= 8)
				{
					Log::Error("TODO: bones in vertices OOB. %d.", v.bonesCnt);
					continue;
				}

				v.weights[v.bonesCnt] = boneDef->Weight;
				v.indexes[v.bonesCnt] = jointIndex;
				v.bonesCnt = v.bonesCnt + 1;
				

				m_HasBoneWeights = true;
			}

			/*std::map<int, std::vector<std::pair<float, size_t>>> weightIndexes;
			for (int i = 0; i < cluster->GetControlPointIndicesCount(); ++i)
			{
				auto it = weightIndexes.find(i);
				if (it == weightIndexes.end())
				{
					auto weight = cluster->GetControlPointWeights()[i];
					if (weight == 0.0f)
						continue;

					weightIndexes.insert(std::make_pair(i, std::vector<std::pair<float, size_t>>({ std::make_pair(static_cast<float>(cluster->GetControlPointWeights()[i]), jointIndex) })));
					continue;
				}

				it->second.push_back(std::make_pair(static_cast<float>(cluster->GetControlPointWeights()[i]), jointIndex));
			}

			for (const auto& w : weightIndexes)
			{
				for (auto& v : m_Vertices)
				{
					if (v.controlPointIndex == cluster->GetControlPointIndices()[w.first])
					{
						for (size_t i = 0; i < w.second.size(); i++)
						{


							v.indexes[v.bonesCnt] = w.second[i].second;
							v.weights[v.bonesCnt] = w.second[i].first;
							v.bonesCnt += 1;

							if (v.bonesCnt > 1)
								printf("Test");



							m_HasBoneWeights = true;
						}
					}
				}
			}*/

			// Skin
			joint->SetSkinMatrix(globalBindposeInverseMatrixGLM);
		}
	}
}

#endif

#include "stdafx.h"

// General
#include "TerrainPass.h"

// Additional
#include "Materials/MaterialDebug.h"
#include "Materials/MaterialModel.h"

CTerrainPass::CTerrainPass(IRenderDevice& RenderDevice, IScene& Scene)
	: Base3DPass(Scene)
{
	auto materialModel = MakeShared(MaterialModel, RenderDevice.GetBaseManager());
	materialModel->SetSpecularFactor(64.0f);

	materialModel->SetSpecularColor(ColorRGB(0.4f, 0.4f, 0.4f));
	materialModel->SetDiffuseColor(ColorRGB(0.4f, 0.4f, 0.4f));

	//materialModel->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("pirate-gold-unity//pirate-gold_albedo.png"));
	//materialModel->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("pirate-gold-unity//pirate-gold_normal-ogl.png"));
	//materialModel->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("pirate-gold-unity//pirate-gold_ao.png"));
	//materialModel->SetTexture(MaterialModel::ETextureType::TextureDisplacement, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("pirate-gold-unity//pirate-gold_height.png"));

	//materialModel->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("aerial_grass_rock_1k_png/aerial_grass_rock_diff_1k.png"));
	//materialModel->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("aerial_grass_rock_1k_png/aerial_grass_rock_nor_1k.png"));
	//materialModel->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("aerial_grass_rock_1k_png/aerial_grass_rock_ao_1k.png"));
	//materialModel->SetTexture(MaterialModel::ETextureType::TextureDisplacement, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("aerial_grass_rock_1k_png/aerial_grass_rock_disp_1k.png"));

	materialModel->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("aerial_rocks_02_1k_png/aerial_rocks_02_diff_1k.png"));
	materialModel->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("aerial_rocks_02_1k_png/aerial_rocks_02_nor_1k.png"));
	materialModel->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("aerial_rocks_02_1k_png/aerial_rocks_02_ao_1k.png"));
	//materialModel->SetTexture(MaterialModel::ETextureType::TextureDisplacement, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("aerial_rocks_02_1k_png/aerial_rocks_02_disp_1k.png"));

	m_Material = materialModel;

	m_Geometry = GetRenderDevice().GetObjectsFactory().CreateGeometry();
	m_Geometry->SetPrimitiveTopology(PrimitiveTopology::PointList);
}

CTerrainPass::~CTerrainPass()
{}




void CTerrainPass::SetEnviorementTexture(std::shared_ptr<ITexture> Texture)
{
	GetPipeline().SetTexture(15, Texture);
}

IShaderParameter * CTerrainPass::GetLightsShaderParameter() const
{
	return m_ShaderLightsBufferParameter;
}


//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CTerrainPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	{
		std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Terrain.hlsl", "VS_main");
		vertexShader->LoadInputLayoutFromReflector();
		GetPipeline().SetShader(vertexShader);
	}

	{
		std::shared_ptr<IShader> geometryShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::GeometryShader, "3D/Terrain.hlsl", "GS_Terrain");

		m_HeightMapBufferParameter = geometryShader->GetShaderParameterByName("Heights");
		_ASSERT(m_HeightMapBufferParameter);

		m_TerrainParamsBufferParameter = geometryShader->GetShaderParameterByName("TerrainParams");
		_ASSERT(m_TerrainParamsBufferParameter);

		GetPipeline().SetShader(geometryShader);
	}

	{
		std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Terrain.hlsl", "PS_main");

		m_ShaderLightsBufferParameter = pixelShader->GetShaderParameterByName("GPULightsVS");
		_ASSERT(m_ShaderLightsBufferParameter);

		GetPipeline().SetShader(pixelShader);
	}

	// PIPELINES
	//GetPipeline().GetDepthStencilState()->SetDepthMode(enableTestDisableWrites);
	//GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	//GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Wireframe, IRasterizerState::FillMode::Wireframe);
	
	GetPipeline().SetTexture(11, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("terrain32Normal.png"));

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CTerrainPass::Visit(const std::shared_ptr<ISceneNode>& SceneNode)
{
	if (false == SceneNode->IsComponentExistsT<ITerrainComponent>())
		return EVisitResult::AllowVisitChilds;

	BindPerObjectData(PerObject());
	
	return EVisitResult::AllowAll;
}

EVisitResult CTerrainPass::Visit(const std::shared_ptr<IModel>& Model)
{
	return Base3DPass::Visit(Model);
}

EVisitResult CTerrainPass::Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
{
	auto objMaterial = std::dynamic_pointer_cast<MaterialModel>(Material);
	if (objMaterial == nullptr)
		return EVisitResult::Block;

	return Base3DPass::Visit(Geometry, Material, GeometryDrawArgs);
}

EVisitResult CTerrainPass::Visit(const std::shared_ptr<ITerrain>& Terrain)
{
	size_t heightsCount = 0;

	{
		heightsCount = Terrain->GetTerrainWidth() * Terrain->GetTerrainHeight();

		if (m_HeightMapBuffer == nullptr || heightsCount != m_HeightMapBuffer->GetElementCount())
			m_HeightMapBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(Terrain->GetHeightsArray(), heightsCount, sizeof(float), EAccess::CPUWrite);
		else
			m_HeightMapBuffer->Set(Terrain->GetHeightsArray(), sizeof(float), heightsCount);

		m_HeightMapBufferParameter->SetStructuredBuffer(m_HeightMapBuffer);
		m_HeightMapBufferParameter->Bind();
	}

	{
		STerrainParams terrainParams;
		terrainParams.Scaling = glm::vec3(1.0f, 1.0f, 1.0f);
		terrainParams.ImageWidth = Terrain->GetTerrainWidth();
		terrainParams.ImageHeight = Terrain->GetTerrainHeight();

		if (m_TerrainParamsBuffer == nullptr)
			m_TerrainParamsBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(terrainParams);
		else
			m_TerrainParamsBuffer->Set(terrainParams);

		m_TerrainParamsBufferParameter->SetConstantBuffer(m_TerrainParamsBuffer);
		m_TerrainParamsBufferParameter->Bind();
	}


	{
		if (m_ShaderLightsBufferParameter)
			m_ShaderLightsBufferParameter->Bind();
	}

	m_Material->Bind(GetPipeline().GetPixelShaderPtr());
	{
		// Draw geom
		SGeometryDrawArgs args;
		args.VertexCnt = heightsCount;
		m_Geometry->Render(GetPipeline().GetVertexShaderPtr(), args);
	}
	m_Material->Unbind(GetPipeline().GetPixelShaderPtr());

	return EVisitResult::AllowAll;
}
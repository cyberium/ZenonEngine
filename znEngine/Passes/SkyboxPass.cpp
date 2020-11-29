#include "stdafx.h"

// General
#include "SkyboxPass.h"

// Additional
#include "Materials/MaterialDebug.h"

CSkyboxPass::CSkyboxPass(IRenderDevice& RenderDevice)
	: RenderPassPipelined(RenderDevice)
{
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject());
}

CSkyboxPass::~CSkyboxPass()
{}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CSkyboxPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	m_SkyGeometry = GetRenderDevice().GetPrimitivesFactory().CreateCube();

	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Skybox.hlsl", "VS_Skybox");
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Skybox.hlsl", "PS_Skybox");
	}
	vertexShader->LoadInputLayoutFromReflector();

	//std::vector<SCustomInputElement> customElements;
	//customElements.push_back({ 0,  0, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION,     0 });
	//customElements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD,     0 });
	//vertexShader->LoadInputLayoutFromCustomElements(customElements);

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(disableBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableTestDisableWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	// Per object
	m_PerObjectShaderParameter = &vertexShader->GetShaderParameterByName("PerObject");
	_ASSERT(m_PerObjectShaderParameter->IsValid());
	m_PerObjectShaderParameter->SetConstantBuffer(m_PerObjectConstantBuffer);


	{
		std::vector<std::string> names;
		names.push_back("skybox_Right.png");
		names.push_back("skybox_Left.png");
		names.push_back("skybox_Top.png");
		names.push_back("skybox_Bottom.png");
		names.push_back("skybox_Front.png");
		names.push_back("skybox_Back.png");

		// 0 - +X
		// 1 - -X
		// 2 - +Y
		// 3 - -Y
		// 4 - +Z
		// 5 - -Z

		/*std::vector<std::string> names;
		names.push_back("Skybox2_PlusX.png");
		names.push_back("Skybox2_MinusX.png");
		names.push_back("Skybox2_PlusY.png");
		names.push_back("Skybox2_MinusZ.png");
		names.push_back("Skybox2_PlusZ.png");
		names.push_back("Skybox2_MinusZ.png");*/

		auto textureCube = GetBaseManager().GetManager<IznTexturesFactory>()->LoadTextureCube(names);
		std::dynamic_pointer_cast<ITextureInternal>(textureCube)->SetFileName("s");
		GetPipeline().SetTexture(0, textureCube);
	}

	return shared_from_this();
}



//
// IRenderPass
//
void CSkyboxPass::Render(RenderEventArgs & e)
{
	glm::mat4 matrix(1.0f);
	matrix = glm::translate(matrix, e.Camera->GetTranslation());
	matrix = glm::scale(matrix, glm::vec3(2500.0f));

	m_PerObjectConstantBuffer->Set(PerObject(matrix));
	m_PerObjectShaderParameter->Bind();

	m_SkyGeometry->Render(GetPipeline().GetShaders().at(EShaderType::VertexShader).get());
}

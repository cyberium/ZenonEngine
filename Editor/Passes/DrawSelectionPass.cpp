#include "stdafx.h"

// General
#include "DrawSelectionPass.h"

const float cBBoxSizeIncrement = 0.02f;

CDrawSelectionPass::CDrawSelectionPass(IRenderDevice& RenderDevice, IEditorToolSelector& Selector)
	: RenderPassPipelined(RenderDevice)
	, m_Selector(Selector)
	, m_IsDirty(false)
{
	m_InstancesBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(nullptr, 100, sizeof(SSelectorPerObject), EAccess::CPUWrite);
}

CDrawSelectionPass::~CDrawSelectionPass()
{}

void CDrawSelectionPass::SetNeedRefresh()
{
	m_IsDirty = true;
}



//
// IRenderPass
//
void CDrawSelectionPass::Render(RenderEventArgs& e)
{
	if (m_Selector.GetSelectedNodes().size() != m_InstancesCnt || m_IsDirty)
	{
		RefreshInstanceBuffer();
		m_IsDirty = false;
	}

	if (m_InstancesCnt > 0)
	{
		m_ShaderInstancesBufferParameter->SetStructuredBuffer(m_InstancesBuffer);
		m_ShaderInstancesBufferParameter->Bind();
		{
			const IShader* vertexShader = GetPipeline().GetShader(EShaderType::VertexShader).get();

			SGeometryDrawArgs args;
			args.InstanceCnt = m_InstancesCnt;
			m_QuadGeometry->Render(vertexShader, args);

		}
		m_ShaderInstancesBufferParameter->Unbind();
	}
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CDrawSelectionPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	m_QuadGeometry = GetRenderDevice().GetPrimitivesFactory().CreateBBox();

	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Editor/EditorShaders.hlsl", "VS_main", { {"INSTANCED", "1"} });
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Editor/EditorShaders.hlsl", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	m_ShaderInstancesBufferParameter = vertexShader->GetShaderParameterByName("Instances");
	_ASSERT(m_ShaderInstancesBufferParameter->IsValid());

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(disableBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);
	return shared_from_this();
}



//
// Private
//
void CDrawSelectionPass::RefreshInstanceBuffer()
{
	m_InstancesCnt = 0;

	auto selectedNodes = m_Selector.GetSelectedNodes();
	if (false == selectedNodes.empty())
	{
		std::vector<SSelectorPerObject> instances;
		instances.reserve(selectedNodes.size());
		std::for_each(selectedNodes.begin(), selectedNodes.end(), [&instances](const std::weak_ptr<ISceneNode>& selectedNode) {
			if (auto locked = selectedNode.lock())
			{
				const auto& colliderComponent = locked->GetComponentT<IColliderComponent3D>();
				if (colliderComponent)
				{
					const BoundingBox& bbox = colliderComponent->GetWorldBounds();
					const auto size = (bbox.getMax() - bbox.getMin());
					const auto sizeIncrement = size * cBBoxSizeIncrement;

					glm::mat4 bboxMatrix = glm::mat4(1.0f);
					bboxMatrix = glm::translate(bboxMatrix, bbox.getMin() - sizeIncrement);
					bboxMatrix = glm::scale(bboxMatrix, size + sizeIncrement * 2.0f);

					instances.push_back(SSelectorPerObject(bboxMatrix, glm::vec4(0.8f, 0.8f, 0.8f, 0.7f)));
				}
			}
		});

		if (instances.size() > m_InstancesBuffer->GetElementCount())
			m_InstancesBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(instances, EAccess::CPUWrite);
		else
			m_InstancesBuffer->Set(instances);

		m_InstancesCnt = instances.size();
	}
}

#include "stdafx.h"

// General
#include "DrawSelectionPass.h"

const float cBBoxSizeIncrement = 0.05f;

CDrawSelectionPass::CDrawSelectionPass(IRenderDevice& RenderDevice, IEditor_NodesSelector& Selector)
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
			m_QuadGeometry->Render(GetRenderEventArgs(), vertexShader, args);

		}
		m_ShaderInstancesBufferParameter->Unbind();
	}
}

//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CDrawSelectionPass::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_QuadGeometry = GetRenderDevice().GetPrimitivesFactory().CreateBBox();

	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_EDITOR", "VS_main_Inst");
		pixelShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_EDITOR", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	m_ShaderInstancesBufferParameter = &vertexShader->GetShaderParameterByName("Instances");
	_ASSERT(m_ShaderInstancesBufferParameter->IsValid());

	// PIPELINES
	auto Pipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
	Pipeline->GetBlendState()->SetBlendMode(disableBlending);
	Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	Pipeline->GetRasterizerState()->SetMultisampleEnabled(true);
	Pipeline->GetRasterizerState()->SetAntialiasedLineEnable(true);
	Pipeline->SetRenderTarget(RenderTarget);
	Pipeline->SetShader(EShaderType::VertexShader, vertexShader);
	Pipeline->SetShader(EShaderType::PixelShader, pixelShader);

	return SetPipeline(Pipeline);
}

EVisitResult CDrawSelectionPass::Visit(const ISceneNode3D * node)
{
	_ASSERT(false);
	return EVisitResult::Block;
}

EVisitResult CDrawSelectionPass::Visit(const IModel * Model)
{
	_ASSERT(false);
	return EVisitResult::Block;
}



//
// Private
//
void CDrawSelectionPass::RefreshInstanceBuffer()
{
	m_InstancesCnt = 0;

	auto selectedNodes = m_Selector.GetSelectedNodes();
	if (!selectedNodes.empty())
	{
		std::vector<SSelectorPerObject> instances;
		instances.reserve(selectedNodes.size());
		std::for_each(selectedNodes.begin(), selectedNodes.end(), [&instances](const std::shared_ptr<ISceneNode3D>& selectedNode) {
			const auto& colliderComponent = selectedNode->GetComponent<IColliderComponent3D>();
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
		});

		if (instances.size() > m_InstancesBuffer->GetElementCount())
			m_InstancesBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(instances, EAccess::CPUWrite);
		else
			m_InstancesBuffer->Set(instances);

		m_InstancesCnt = instances.size();
	}
}

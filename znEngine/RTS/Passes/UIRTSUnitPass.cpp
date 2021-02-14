#include "stdafx.h"

// General
#include "UIRTSUnitPass.h"

// Additional
#include "UIControls/Common/UIControlCommonMaterial.h"

namespace
{
	const glm::vec2 outlineHPBarSize = glm::vec2(50.0f, 7.0f);
	const glm::vec2 inlineHPBarSize = glm::vec2(45.0f, 3.0f);

	const glm::vec3 hpBarOutlineColor = glm::vec3(0.1f, 0.2f, 0.1f);

	const glm::vec3 hpBarColorLow = glm::vec3(0.745f, 0.0f, 0.0f);
	const glm::vec3 hpBarColorMiddle = glm::vec3(1.0f, 0.55f, 0.1f);
	const glm::vec3 hpBarColorHigh = glm::vec3(0.4f, 0.8f, 0.0f);

	template<class T>
	inline T interpolateLinear(const float r, const T& v1, const T& v2)
	{
		return v1 * (1.0f - r) + v2 * r;
	}

	template<class T>
	inline T InterpolateHealthColor(float currentTime, float mid, const T& a, const T& b, const T& c)
	{
		if (currentTime <= mid)
			return interpolateLinear<T>(currentTime / mid, a, b);
		else
			return interpolateLinear<T>((currentTime - mid) / (1.0f - mid), b, c);
	}
}

CUIRTSUnitPass::CUIRTSUnitPass(IRenderDevice& RenderDevice, IScene& Scene)
	: BaseUIPass(Scene)
{
	m_HPBarOutlineMaterial = MakeShared(CUIControlCommonMaterial, GetRenderDevice());
	m_HPBarOutlineMaterial->SetColor(glm::vec4(hpBarOutlineColor, 1.0f));

	m_HPBarMaterial = MakeShared(CUIControlCommonMaterial, GetRenderDevice());

	m_HPBarOutlineGeometry = GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(outlineHPBarSize);
	m_HPBarGeometry = GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(inlineHPBarSize);
}

CUIRTSUnitPass::~CUIRTSUnitPass()
{}



//
// IRenderPass
//
void CUIRTSUnitPass::PreRender(RenderEventArgs & e)
{
	__super::PreRender(e);

	m_Units.clear();

	const auto& cameraController = GetScene().GetCameraController();
	const auto& viewport = GetScene().GetRenderWindow().GetViewport();

	Frustum f;
	f.buildBoxFrustum(
		cameraController->ScreenToRay(viewport, glm::vec2(viewport.GetX(), viewport.GetY())),
		cameraController->ScreenToRay(viewport, glm::vec2(viewport.GetX(), viewport.GetHeight())),
		cameraController->ScreenToRay(viewport, glm::vec2(viewport.GetWidth(), viewport.GetY())),
		cameraController->ScreenToRay(viewport, glm::vec2(viewport.GetWidth(), viewport.GetHeight())),
		2500.0f
	);

	auto nodes = GetScene().GetFinder().FindIntersections(f, [](const std::shared_ptr<ISceneNode>& Node) -> bool {
		if (Node->GetClass() == cSceneNodeRTSUnit)
			if (auto rtsUnit = std::dynamic_pointer_cast<ISceneNodeRTSUnit>(Node))
				return false == rtsUnit->IsDead();
		return false;
	});

	for (const auto& n : nodes)
		m_Units.push_back(std::dynamic_pointer_cast<ISceneNodeRTSUnit>(n));
}

void CUIRTSUnitPass::Render(RenderEventArgs & e)
{
	const auto& cameraController = GetScene().GetCameraController();
	const auto& viewport = GetScene().GetRenderWindow().GetViewport();

	for (const auto& unit : m_Units)
	{
		glm::vec3 unitCoords = unit->GetPosition();
		float unitHealth = unit->GetHealth() / unit->GetMaxHealth();

		if (auto colliderComonent = unit->GetComponentT<IColliderComponent>())
		{
			unitCoords.y += colliderComonent->GetWorldBounds().getRadius();
		}

		glm::vec2 screenCoords = cameraController->WorldToScreen(viewport, unitCoords);

		{
			PerObject perObject;
			perObject.Model = glm::translate(perObject.Model, glm::vec3(screenCoords - outlineHPBarSize / 2.0f, 0.0f));

			BindPerObjectData(perObject);

			m_HPBarOutlineMaterial->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
			m_HPBarOutlineGeometry->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr(), SGeometryDrawArgs());
			m_HPBarOutlineMaterial->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
		}

		{
			PerObject perObject;
			perObject.Model = glm::translate(perObject.Model, glm::vec3(screenCoords - inlineHPBarSize / 2.0f, 0.0f));
			perObject.Model = glm::scale(perObject.Model, glm::vec3(unitHealth, 1.0f, 1.0f));

			BindPerObjectData(perObject);

			m_HPBarMaterial->SetColor(glm::vec4(InterpolateHealthColor(unitHealth, 0.5f, hpBarColorLow, hpBarColorMiddle, hpBarColorHigh), 1.0f));

			m_HPBarMaterial->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
			m_HPBarGeometry->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr(), SGeometryDrawArgs());
			m_HPBarMaterial->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
		}
	}
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CUIRTSUnitPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	BaseUIPass::ConfigurePipeline(RenderTarget);
	
	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UI_Texture.hlsl", "VS_main");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UI_Texture.hlsl", "PS_main");

	// Material
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().SetShader(vertexShader);
	GetPipeline().SetShader(pixelShader);

	return shared_from_this();
}

//
// IVisitor
//
EVisitResult CUIRTSUnitPass::Visit(const std::shared_ptr<IUIControl>& node)
{
	/*if (auto textNode = std::dynamic_pointer_cast<const CUIControlCommon>(node))
	{
		for (const auto& subGeom : textNode->GetSubgeometries())
		{
			PerObject perObject(node->GetWorldTransfom());
			perObject.Model = glm::translate(perObject.Model, glm::vec3(subGeom.Translate.x, subGeom.Translate.y, 0.0f));
			perObject.Model = glm::scale(perObject.Model, glm::vec3(subGeom.Size.x, subGeom.Size.y, 1.0f));

			BindPerObjectData(perObject);

			subGeom.Material->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
			subGeom.Geom->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr(), SGeometryDrawArgs());
			subGeom.Material->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
		}

		return EVisitResult::AllowVisitChilds;
	}*/

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CUIRTSUnitPass::Visit(const std::shared_ptr<IModel>& Model)
{
	//return __super::Visit(Model);
	return EVisitResult::Block;
}
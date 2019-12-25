#include "stdafx.h"

// Additional
#include "FBXSceneNode.h"
#include "FBXMaterialPass.h"

extern CLog* gLogInstance;

class CznFBXPlugin 
	: public IznPlugin
	, public ISceneNodeCreator
	, public IRenderPassCreator
{
public:
	CznFBXPlugin(IBaseManager* BaseManager)
		: m_BaseManager(BaseManager)
	{

	}
	virtual ~CznFBXPlugin()
	{
	}



	//
	// IznPlugin
	//
	bool Initialize()
	{
		gLogInstance = std::dynamic_pointer_cast<CLog>(m_BaseManager->GetManager<ILog>()).get();

		m_BaseManager->GetManager<IFilesManager>()->RegisterFilesStorage(std::make_shared<CLibraryResourceFileStotage>(GetModuleHandle(L"znFBXPlugin.dll")));

		return true;
	}
	void Finalize()
	{

	}
	std::string GetName() const override final
	{
		return "FBX plguin";
	}
	std::string GetDescription() const override final
	{
		return "";
	}



	//
	// ISceneNodeCreator
	//
	size_t GetSceneNodesCount() const override
	{
		return 1;
	}
	std::string GetSceneNodeTypeName(size_t Index) const override
	{
		if (Index == 0)
		{
			return "FBXSceneNode";
		}

		return nullptr;
	}
	std::shared_ptr<ISceneNode> CreateSceneNode(std::weak_ptr<ISceneNode> Parent, size_t Index) const override
	{
		if (Index == 0)
		{
			return Parent.lock()->CreateSceneNode<CFBXSceneNode>("D:\\L_FG_Assets_Pack1.0\\Meshes\\Skeleton01_c.FBX", nullptr);
		}

		return nullptr;
	}



	//
	// IRenderPassCreator
	//
	size_t GetRenderPassCount() const override
	{
		return 1;
	}
	std::string GetRenderPassName(size_t Index) const override
	{
		if (Index == 0)
		{
			return "FBXPass";
		}

		return "<error>";
	}
	std::shared_ptr<IRenderPass> CreateRenderPass(size_t Index, std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport, std::shared_ptr<IScene> Scene) const override
	{
		if (Index == 0)
		{
			IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
			IBlendState::BlendMode disableBlending;
			IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
			IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);

			std::shared_ptr<IShader> g_pVertexShader;
			std::shared_ptr<IShader> g_pPixelShader;

			if (RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
			{
				g_pVertexShader = RenderDevice->CreateShader(
					IShader::ShaderType::VertexShader, "IDB_SHADER_3D_FBX", IShader::ShaderMacros(), "VS_main", "latest"
				);
				g_pPixelShader = RenderDevice->CreateShader(
					IShader::ShaderType::PixelShader, "IDB_SHADER_3D_FBX", IShader::ShaderMacros(), "PS_main", "latest"
				);
			}
			else
			{
				g_pVertexShader = RenderDevice->CreateShader(
					IShader::ShaderType::VertexShader, "IDB_SHADER_OGL_3D_FBX_VS", IShader::ShaderMacros(), "", ""
				);
				g_pPixelShader = RenderDevice->CreateShader(
					IShader::ShaderType::PixelShader, "IDB_SHADER_OGL_3D_FBX_PS", IShader::ShaderMacros(), "", ""
				);
			}
			g_pVertexShader->LoadInputLayoutFromReflector();

			// PIPELINES
			std::shared_ptr<IPipelineState> Pipeline = RenderDevice->CreatePipelineState();
			Pipeline->GetBlendState()->SetBlendMode(disableBlending);
			Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
			Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
			Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
			Pipeline->SetRenderTarget(RenderTarget);
			Pipeline->GetRasterizerState()->SetViewport(Viewport);
			Pipeline->SetShader(IShader::ShaderType::VertexShader, g_pVertexShader);
			Pipeline->SetShader(IShader::ShaderType::PixelShader, g_pPixelShader);

			return std::make_shared<CFBXMaterialPass>(RenderDevice, Scene, Pipeline);
		}

		return nullptr;
	}

private:
	IBaseManager* m_BaseManager;
};

IznPlugin* plugin = nullptr;
extern "C" __declspec(dllexport) IznPlugin* GetPlugin(IBaseManager* BaseManager)
{
	if (plugin == nullptr)
	{
		plugin = new CznFBXPlugin(BaseManager);
	}

	return plugin;
}

#pragma once

class CznPluginM2Models
	: public IznPlugin
	, public IRendererExtender
{
public:
	CznPluginM2Models(IBaseManager& BaseManager);
	virtual ~CznPluginM2Models();

	// IznPlugin
	bool Initialize() override;
	void Finalize() override;
	std::string GetName() const override;
	std::string GetDescription() const override;

	// IRendererExtender
	void Extend3DPasses(IRenderer& Renderer, IRenderDevice& RenderDevice, IScene& Scene, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport) const override;
	void Extend3DPasses(IRenderer& Renderer, IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneNodeListPass, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport) const override;
private:
	std::shared_ptr<IznModelsLoader> m_M2ModelsLoader;
	std::shared_ptr<IImageLoader> m_BLPImageLoader;

private:
	IBaseManager& m_BaseManager;
};

extern "C" __declspec(dllexport) IznPlugin* WINAPI GetPlugin(IBaseManager* BaseManager);
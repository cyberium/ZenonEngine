#pragma once

class ZN_API RendererBase
	: public IRenderer
{
public:
	RendererBase(IBaseManager& BaseManagerl, IScene& Scene);
	virtual ~RendererBase();

	// IRenderer
	void Add3DPass(std::shared_ptr<IRenderPass> pass) override;
	void AddUIPass(std::shared_ptr<IRenderPass> pass) override;

	void Render3D(RenderEventArgs& renderEventArgs) override;
	void RenderUI(RenderEventArgs& renderEventArgs) override;

	void Resize(glm::ivec2 Size) override;

private:
	std::vector<std::shared_ptr<IRenderPass>> m_3DPasses;
	std::vector<std::shared_ptr<IRenderPass>> m_UIPasses;

protected:
	IBaseManager& m_BaseManager;
	IRenderDevice& m_RenderDevice;
	IScene& m_Scene;
};
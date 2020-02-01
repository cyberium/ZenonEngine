#pragma once

class ZN_API RasterizerStateBase 
	: public IRasterizerState
	, public Object
{
public:
	RasterizerStateBase();
    virtual ~RasterizerStateBase();

	// IRasterizerState
	virtual void SetFillMode(FillMode frontFace = FillMode::Solid, FillMode backFace = FillMode::Solid) override;
	virtual void GetFillMode(FillMode& frontFace, FillMode& backFace) const override;

	virtual void SetCullMode(CullMode cullMode = CullMode::Back) override;
	virtual CullMode GetCullMode() const override;

	virtual void SetFrontFacing(FrontFace frontFace = FrontFace::CounterClockwise) override;
	virtual FrontFace GetFrontFacing() const override;

	virtual void SetDepthBias(float depthBias = 0.0f, float slopeBias = 0.0f, float biasClamp = 0.0f) override;
	virtual void GetDepthBias(float& depthBias, float& slopeBias, float& biasClamp) const override;

	virtual void SetDepthClipEnabled(bool depthClipEnabled = true) override;
	virtual bool GetDepthClipEnabled() const override;

	virtual void SetViewport(const Viewport* viewport) override;
	virtual void SetViewports(const std::vector<const Viewport*>& viewports) override;
	virtual const std::vector<const Viewport *>& GetViewports() const override;

	virtual void SetScissorEnabled(bool scissorEnable = false) override;
	virtual bool GetScissorEnabled() const override;

	virtual void SetScissorRect(const Rect& rect) override;
	virtual void SetScissorRects(const std::vector<Rect>& rects) override;
	virtual const std::vector<Rect>& GetScissorRects() const override;

	virtual void SetMultisampleEnabled(bool multisampleEnabled = false) override;
	virtual bool GetMultisampleEnabled() const override;

	virtual void SetAntialiasedLineEnable(bool antialiasedLineEnable) override;
	virtual bool GetAntialiasedLineEnable() const override;

	virtual void SetForcedSampleCount(uint8_t forcedSampleCount = 0) override;
	virtual uint8_t GetForcedSampleCount() const override;

	virtual void SetConservativeRasterizationEnabled(bool conservativeRasterizationEnabled = false);
	virtual bool GetConservativeRasterizationEnabled() const;

protected:
    FillMode                                        m_FrontFaceFillMode;
    FillMode                                        m_BackFaceFillMode;

    CullMode                                        m_CullMode;

    FrontFace                                       m_FrontFace;

    float                                           m_DepthBias;
    float                                           m_SlopeBias;
    float                                           m_BiasClamp;

    bool                                            m_DepthClipEnabled;
    bool                                            m_ScissorEnabled;

    bool                                            m_MultisampleEnabled;
    bool                                            m_AntialiasedLineEnabled;

    bool                                            m_ConservativeRasterization;

    uint8_t                                         m_ForcedSampleCount;

    typedef std::vector<Rect>						RectList;
    RectList                                        m_ScissorRects;

    typedef std::vector<const Viewport*>			ViewportList;
    ViewportList                                    m_Viewports;

    bool                                            m_StateDirty;
    bool                                            m_ViewportsDirty;
    bool                                            m_ScissorRectsDirty;
};

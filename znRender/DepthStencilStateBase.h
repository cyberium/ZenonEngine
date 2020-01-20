#pragma once

class ZN_API DepthStencilStateBase 
	: public IDepthStencilState
	, public Object
{
public:
	DepthStencilStateBase();
    virtual ~DepthStencilStateBase();

	virtual void SetDepthMode(const DepthMode& depthMode);
	virtual const DepthMode& GetDepthMode() const;

	virtual void SetStencilMode(const StencilMode& stencilMode);
	virtual const StencilMode& GetStencilMode() const;

protected:
    DepthMode                                       m_DepthMode;
    StencilMode                                     m_StencilMode;

    bool                                            m_bDirty;
};

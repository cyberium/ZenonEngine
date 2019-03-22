#pragma once

class DepthStencilStateOGL : public DepthStencilState
{
public:
	DepthStencilStateOGL();
	DepthStencilStateOGL(const DepthStencilStateOGL& copy);

	virtual ~DepthStencilStateOGL();

	const DepthStencilStateOGL& operator=(const DepthStencilStateOGL& other);

	virtual void SetDepthMode(const DepthMode& depthMode) override;
	virtual const DepthMode& GetDepthMode() const override;

	virtual void SetStencilMode(const StencilMode& stencilMode) override;
	virtual const StencilMode& GetStencilMode() const override;

	// Can only be called by the pipeline state.
	void Bind();

private:
	DepthMode m_DepthMode;
	StencilMode m_StencilMode;

	bool m_bDirty;
};
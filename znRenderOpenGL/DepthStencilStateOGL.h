#pragma once

class DepthStencilStateOGL : public DepthStencilStateBase
{
public:
	                                                DepthStencilStateOGL();
	                                                DepthStencilStateOGL(const DepthStencilStateOGL& copy);
	virtual                                         ~DepthStencilStateOGL();

	const DepthStencilStateOGL&                     operator=(const DepthStencilStateOGL& other);

    // DepthStencilState
	void                                            Bind() override final;
};
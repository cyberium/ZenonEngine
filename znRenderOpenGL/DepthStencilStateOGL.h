#pragma once

class DepthStencilStateOGL : public DepthStencilState
{
public:
	                                                DepthStencilStateOGL();
	                                                DepthStencilStateOGL(const DepthStencilStateOGL& copy);
	virtual                                         ~DepthStencilStateOGL();

	const DepthStencilStateOGL&                     operator=(const DepthStencilStateOGL& other);

    // DepthStencilState
	void                                            Bind() override final;
};
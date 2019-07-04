#pragma once

class BlendStateOGL : public BlendState
{
public:
	                                                BlendStateOGL();
	                                                BlendStateOGL(const BlendStateOGL& copy);
	virtual                                         ~BlendStateOGL();

	const BlendStateOGL&                            operator=(const BlendStateOGL& other);

    // BlendState
	void                                            Bind() override final;
};
#pragma once

#include "../RenderPass.h"

class ZN_API CopyBufferPass 
	: public RenderPass
{
public:
	CopyBufferPass(IRenderDevice& RenderDevice, IBuffer* destinationBuffer, IBuffer* sourceBuffer);
	virtual ~CopyBufferPass();

	virtual void Render(RenderEventArgs& e);

private:
	IBuffer* m_SourceBuffer;
	IBuffer* m_DestinationBuffer;
};

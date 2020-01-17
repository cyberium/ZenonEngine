#pragma once

#include "../RenderPass.h"

class ZN_API CopyBufferPass 
	: public RenderPass
{
public:
	CopyBufferPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IBuffer> destinationBuffer, std::shared_ptr<IBuffer> sourceBuffer);
	virtual ~CopyBufferPass();

	virtual void Render(RenderEventArgs& e);

private:
	std::shared_ptr<IBuffer> m_SourceBuffer;
	std::shared_ptr<IBuffer> m_DestinationBuffer;
};

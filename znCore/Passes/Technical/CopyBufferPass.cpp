#include "stdafx.h"

// General
#include "CopyBufferPass.h"

CopyBufferPass::CopyBufferPass(IRenderDevice* RenderDevice, IBuffer* destinationBuffer, IBuffer* sourceBuffer)
	: RenderPass(RenderDevice)
	, m_DestinationBuffer(destinationBuffer)
	, m_SourceBuffer(sourceBuffer)
{}

CopyBufferPass::~CopyBufferPass()
{}

void CopyBufferPass::Render(RenderEventArgs& e)
{
	if (m_DestinationBuffer)
	{
		m_DestinationBuffer->Copy(m_SourceBuffer);
	}
}

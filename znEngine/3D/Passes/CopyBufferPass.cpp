#include <stdafx.h>

// General
#include "CopyBufferPass.h"

CopyBufferPass::CopyBufferPass(std::shared_ptr<IBuffer> destinationBuffer, std::shared_ptr<IBuffer> sourceBuffer)
	: m_DestinationBuffer(destinationBuffer)
	, m_SourceBuffer(sourceBuffer)
{}

CopyBufferPass::~CopyBufferPass()
{}

void CopyBufferPass::Render(Render3DEventArgs& e)
{
	if (m_DestinationBuffer)
	{
		m_DestinationBuffer->Copy(m_SourceBuffer);
	}
}

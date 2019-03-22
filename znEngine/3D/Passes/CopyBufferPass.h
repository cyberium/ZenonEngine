#pragma once

#include "AbstractPass.h"

class IBuffer;

/**
 * A render pass that copies a GPU buffer to another.
 * Both buffers must be the same size (in bytes) and their internal
 * formats should be the same.
 */
class CopyBufferPass : public AbstractPass
{
public:
	CopyBufferPass(std::shared_ptr<IBuffer> destinationBuffer, std::shared_ptr<IBuffer> sourceBuffer);
	virtual ~CopyBufferPass();

	virtual void Render(Render3DEventArgs& e);

private:
	std::shared_ptr<IBuffer> m_SourceBuffer;
	std::shared_ptr<IBuffer> m_DestinationBuffer;
};

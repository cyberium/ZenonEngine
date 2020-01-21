#pragma once

#include "../RenderPass.h"

class ZN_API DispatchPass 
	: public RenderPass
{
public:
	DispatchPass(IRenderDevice* RenderDevice, IShader* computeShader, const glm::uvec3& numGroups);
	virtual ~DispatchPass();

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void PreRender(RenderEventArgs& e);
	virtual void Render(RenderEventArgs& e);
	virtual void PostRender(RenderEventArgs& e);

	void SetNumGroups(const glm::ivec3& numGroups);
	glm::ivec3 GetNumGroups() const;

private:

	IShader* m_pComputeShader;

	// The number of groups to dispatch for the compute shader kernel.
	glm::uvec3 m_NumGroups;

};
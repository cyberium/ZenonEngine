#pragma once

#include "DBC/DBC__Storage.h"

// Factory

class ZN_API CWorldObjectCreator
	: public IManager
{
public:
	ZN_OBJECTCLASS(cWoWManager);

	CWorldObjectCreator(IBaseManager& BaseManager);

	void InitEGxBlend(IRenderDevice& RenderDevice);
	std::shared_ptr<IBlendState> GetEGxBlend(uint32 Index) const;
	
	IBlendState::BlendMode GetEGxBlendMode(uint32 Index);

private:
	IBaseManager& m_BaseManager;

	std::map<uint32, std::shared_ptr<IBlendState>> m_EGxBlendStates;
};
#pragma once

#include "../Materials/MaterialUIControl.h"

class ZN_API CUIControlCommon
	: public CUIControl
{
public:
	struct SSubgeometry
	{
		glm::vec2                           Translate;
		glm::vec2                           Size;
		std::shared_ptr<CMaterialUIControl> Material;
		std::shared_ptr<IGeometry>          Geom;
	};

public:
	CUIControlCommon(IScene& Scene);
	virtual ~CUIControlCommon();

	// CUIControl
	void Initialize() override;
	const std::vector<CUIControlCommon::SSubgeometry>& GetSubgeometries() const;

protected:
	void AddSubgeometry(const SSubgeometry& Subgeometry);

private:
	std::vector<SSubgeometry> m_Subgeometries;
};
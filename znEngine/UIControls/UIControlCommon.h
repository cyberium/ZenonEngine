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

	void Initialize() override;

	// CUIControl
    virtual glm::vec2 GetSize() const override;
	const std::vector<CUIControlCommon::SSubgeometry>& GetSubgeometries() const;

private:
    glm::vec2                 m_Size;

	std::vector<SSubgeometry> m_Subgeometries;

	std::shared_ptr<IGeometry> m_DefaultQuadGeometry;
};
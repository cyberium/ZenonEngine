#pragma once

class ZN_API CUIControlCommonModel
	: virtual public ModelProxie
	, public IUIControlModel
{
public:
	CUIControlCommonModel(const IRenderDevice& RenderDevice);
	virtual ~CUIControlCommonModel();

	// IUIControlModel
	void SetTranslate(const glm::vec2& Translate) override;
	glm::vec2 GetTranslate() const override;
	void SetSize(const glm::vec2& Size) override;
	glm::vec2 GetSize() const override;
	void SetMaterial(std::shared_ptr<IMaterialUIControl> Material) override;
	std::shared_ptr<IMaterialUIControl> GetMaterial() const override;
	void SetGeom(std::shared_ptr<IGeometry> Geom) override;
	std::shared_ptr<IGeometry> GetGeom() const override;

private:
	glm::vec2 m_Translate;
	glm::vec2 m_Size;
	std::shared_ptr<IMaterialUIControl> m_Material;
	std::shared_ptr<IGeometry> m_Geom;
};
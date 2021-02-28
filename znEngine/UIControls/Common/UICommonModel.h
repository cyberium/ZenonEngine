#pragma once

namespace
{
	struct __declspec(align(16)) SUIControlCommonModelData
	{
		SUIControlCommonModelData()
			: Translate(0.0f)
			, Size(1.0f)
		{}

		SUIControlCommonModelData(const glm::vec2& Translate_, const glm::vec2& Size_)
			: Translate(Translate_)
			, Size(Size_)
		{}

		glm::vec2 Translate;
		glm::vec2 Size;
		// 16 bytes
	};
}

class ZN_API CUICommonModel
	: virtual public ModelProxie
	, public IUIControlCommonModel
{
public:
	CUICommonModel(const IRenderDevice& RenderDevice);
	virtual ~CUICommonModel();

	// IUIControlModel
	void SetTranslate(const glm::vec2& Translate) override;
	glm::vec2 GetTranslate() const override;
	void SetSize(const glm::vec2& Size) override;
	glm::vec2 GetSize() const override;
	void SetMaterial(std::shared_ptr<IUIControlCommonMaterial> Material) override;
	std::shared_ptr<IUIControlCommonMaterial> GetMaterial() const override;
	void SetGeom(std::shared_ptr<IGeometry> Geom) override;
	std::shared_ptr<IGeometry> GetGeom() const override;

	// IModel
	bool Render(const ShaderMap& Shaders) const override;

private:
	glm::vec2 m_Translate;
	glm::vec2 m_Size;
	std::shared_ptr<IUIControlCommonMaterial> m_Material;
	std::shared_ptr<IGeometry> m_Geom;
};
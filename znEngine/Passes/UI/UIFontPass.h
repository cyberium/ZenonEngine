#pragma once

namespace
{
	struct __declspec(align(16)) SFontPerCharacterData
	{
		SFontPerCharacterData()
			: Color(1.0f, 1.0f, 1.0f, 1.0f)
			, Offset(0.0f, 0.0f)
			, IsSelected(0)
		{}
		glm::vec4 Color;
		// 16 bytes

		glm::vec2 Offset;
		uint32 IsSelected;
		float Padding;
		// 16 bytes
	};
}

class ZN_API CUIFontPass
	: public BaseUIPass
{
public:
	CUIFontPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CUIFontPass();

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

	// IVisitor
	virtual EVisitResult Visit(const IUIControl* node) override;

private:
	void BindPerCharacterData(const SFontPerCharacterData& PerCharacterData);

private:
	std::shared_ptr<IConstantBuffer>   m_FontBuffer;
};
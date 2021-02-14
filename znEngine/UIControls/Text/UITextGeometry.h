#pragma once

struct __declspec(align(16)) SGPUPerCharacterDataVS
{
	SGPUPerCharacterDataVS()
		: Offset(0.0f, 0.0f)
	{}

	SGPUPerCharacterDataVS(const glm::vec2& Offset_)
		: Offset(Offset_)
	{}

	glm::vec2 Offset;
	glm::vec2 Padding;
	// 16 bytes
};

class CUITextGeometry
	: public GeometryProxie
{
public:
	CUITextGeometry(IRenderDevice& RenderDevice, std::shared_ptr<IGeometry> OriginalGeometry);
	virtual ~CUITextGeometry();

	void Render(const IShader* VertexShader, const std::shared_ptr<IznFont>& Font, const std::string& Text, ETextAlignHorizontal TextAlignHorizontal, ETextAlignVertical TextAlignVertical);

private:
	void BindPerCharacterVSData(const IShader * VertexShader, const SGPUPerCharacterDataVS& GPUPerCharacterDataVS);

private:
	std::shared_ptr<IConstantBuffer> m_FontBuffer;
};
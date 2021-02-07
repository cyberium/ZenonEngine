#pragma once

struct __declspec(align(16)) SGPUPerCharacterDataVS
{
	SGPUPerCharacterDataVS()
		: Offset(0.0f, 0.0f)
	{}
	glm::vec2 Offset;
	glm::vec2 Padding;
	// 16 bytes
};

class CFontGeometry
	: public GeometryProxie
{
public:
	CFontGeometry(IRenderDevice& RenderDevice, std::shared_ptr<IGeometry> OriginalGeometry);
	virtual ~CFontGeometry();

	void Render(const IShader* VertexShader, const std::shared_ptr<IznFont>& Font, const std::string& Text, ETextAlignHorizontal TextAlignHorizontal, ETextAlignVertical TextAlignVertical);

private:
	void BindPerCharacterVSData(const IShader * VertexShader, const SGPUPerCharacterDataVS& GPUPerCharacterDataVS);

private:
	std::shared_ptr<IConstantBuffer> m_FontBuffer;
};
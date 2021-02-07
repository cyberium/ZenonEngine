#pragma once

struct __declspec(align(16)) SGPUPerCharacterDataPS
{
	SGPUPerCharacterDataPS()
		: Color(1.0f, 1.0f, 1.0f, 1.0f)
		, IsSelected(0)
	{}
	glm::vec4 Color;
	// 16 bytes

	uint32 IsSelected;
	glm::vec3 Padding;
	// 16 bytes
};

class CFontMaterial
	: public MaterialProxieT<SGPUPerCharacterDataPS>
{
public:
	CFontMaterial(const IRenderDevice& RenderDevice);
	virtual ~CFontMaterial();


private:

};
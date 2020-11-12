#pragma once

#include "M2_Part_Material.h"
#include "M2_SkinSection.h"

// FORWARD BEGIN
class CM2;
class CM2_Base_Instance;
// FORWARD END


namespace
{
	struct __declspec(align(16)) ShaderM2BatchProperties
	{
		ShaderM2BatchProperties()
			: BlendMode(0)
			, Shader(0)
			, TextureAnimEnable(false)
			, TextureWeight(1.0f)

			, Color(glm::vec4(1.0))

			, TextureAnimMatrix(glm::mat4(1.0f))

			, InstanceColor(glm::vec4(1.0))
		{}

		uint32     BlendMode;
		uint32     Shader;
		uint32     TextureAnimEnable;
		float      TextureWeight;
		//--------------------------------------------------------------( 16 bytes )

		glm::vec4  Color;
		//--------------------------------------------------------------( 16 bytes )

		glm::mat4  TextureAnimMatrix;
		//--------------------------------------------------------------( 64 bytes )

		glm::vec4  InstanceColor;
		//--------------------------------------------------------------( 16 bytes )
	};
}

/**
  * M2 Batch что-то вроде прохода над геометрией. Имеет свои текстуры, цвета и тд.
  * Несколько батчей могут рисовать одну и ту же геометрию
*/
class CM2_Skin_Batch 
	: public MaterialProxieT<ShaderM2BatchProperties>
{
public:
	CM2_Skin_Batch(IBaseManager& BaseManager, IRenderDevice& RenderDevice, const CM2& M2Model, const SM2_SkinBatch& SkinBatchProto);
	virtual ~CM2_Skin_Batch();

	const std::shared_ptr<const CM2_Part_Material>& GetM2Material() const {	return m_M2ModelMaterial; }
	void UpdateMaterialProps(const RenderEventArgs& RenderEventArgs, const CM2_Base_Instance* M2Instance);

public:
	int32												m_PriorityPlan;
	
	// Material props
	std::shared_ptr<const CM2_Part_Color>				m_Color;
	std::shared_ptr<const CM2_Part_Material>			m_M2ModelMaterial;
	std::vector<std::weak_ptr<const CM2_Part_Texture>>	m_Textures;
	int16												m_TextureUnit;
	std::shared_ptr<const CM2_Part_TextureWeight>		m_TextureWeight;
	std::shared_ptr<const CM2_Part_TextureTransform>	m_TextureTransform;

	int32												newShader;

private:
	IBaseManager& m_BaseManager;
	IRenderDevice& m_RenderDevice;
	const CM2& m_M2Model;
	const SM2_SkinBatch m_SkinBatchProto;
};

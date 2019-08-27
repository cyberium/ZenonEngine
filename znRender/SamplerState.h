#pragma once

#include "common.h"

#include "Shader.h"
#include "ShaderParameter.h"

/**
 * Texture Sampler State. Defines how a texture is sampled in a shader.
 */
class OW_ENGINE_API SamplerState : public Object
{	
    typedef Object base;
public:
                                                    SamplerState();
    virtual                                         ~SamplerState();

	enum OW_ENGINE_API MinFilter
	{
		MinNearest,                // The nearest texel to the sampled texel.
		MinLinear,                 // Linear average of the 4 closest texels.
	};

	enum OW_ENGINE_API MagFilter
	{
		MagNearest,                // The nearest texel to the sampled texel.
		MagLinear,                 // Weighted average of the closest texels.
	};

	enum OW_ENGINE_API MipFilter
	{
		MipNearest,                // Choose the nearest mip level.
		MipLinear,                 // Linear interpolate between the 2 nearest mip map levels.
	};

	enum OW_ENGINE_API WrapMode
	{
		Repeat,                 // Texture is repeated when texture coordinates are out of range.
		Mirror,                 // Texture is mirrored when texture coordinates are out of range.
		Clamp,                  // Texture coordinate is clamped to [0, 1] 
		Border,                 // Texture border color is used when texture coordinates are out of range.
	};

	enum OW_ENGINE_API CompareMode
	{
		None,                   // Don't perform any comparison
		CompareRefToTexture,    // Compare the reference value (usually the currently bound depth buffer) to the value in the texture.
	};

	enum OW_ENGINE_API CompareFunc
	{
		Never,                  // Never pass the comparison function.
		Less,                   // Pass if the source data is less than the destination data.
		Equal,                  // Pass if the source data is equal to the destination data.
		LessEqual,              // Pass if the source data is less than or equal to the destination data.
		Greater,                // Pass if the source data is greater than the destination data.
		NotEqual,               // Pass if the source data is not equal to the destination data.
		GreaterEqual,           // Pass if the source data is greater than or equal to the destination data.
		Always,                 // Always pass the comparison function.
	};

    virtual void SetFilter(MinFilter minFilter, MagFilter magFilter, MipFilter mipFilter);
	virtual void GetFilter(MinFilter& minFilter, MagFilter& magFilter, MipFilter& mipFilter) const;

	virtual void SetWrapMode(WrapMode u = WrapMode::Repeat, WrapMode v = WrapMode::Repeat, WrapMode w = WrapMode::Repeat);
	virtual void GetWrapMode(WrapMode& u, WrapMode& v, WrapMode& w) const;

	virtual void SetCompareFunction(CompareFunc compareFunc);
	virtual CompareFunc GetCompareFunc() const;

	/**
	 * Set the offset from the calculated mipmap level.  For example, if mipmap texture 1 should be sampled and
	 * LOD bias is set to 2, then the texture will be sampled at mipmap level 3.
	 */
	virtual void SetLODBias(float lodBias);
	virtual float GetLODBias() const;

	/**
	 * Set the minimum LOD level that will be sampled.  The highest resolution mip map is level 0.
	 */
	virtual void SetMinLOD(float minLOD);
    virtual float GetMinLOD() const;

	/**
	 * Set the maximum LOD level that will be sampled. The LOD level increases as the resolution of the mip-map decreases.
	 */
	virtual void SetMaxLOD(float maxLOD);
	virtual float GetMaxLOD() const;

	/**
	 * Sets the border color to use if the wrap mode is set to Border.
	 */
	virtual void SetBorderColor(cvec4 borderColor);
	virtual vec4 GetBorderColor() const;

	/**
	 * Enable Anisotropic filtering (where supported).
	 */
	virtual void EnableAnisotropicFiltering(bool enabled);
	virtual bool IsAnisotropicFilteringEnabled() const;

	/**
	 * When Anisotropic filtering is enabled, use this value to determine the maximum level
	 * of anisotropic filtering to apply.  Valid values are in the range [1, 16].
	 * May not be supported on all platforms.
	 */
	virtual void SetMaxAnisotropy(uint8_t maxAnisotropy);
	virtual uint8_t GetMaxAnisotropy() const;

	virtual void Bind(uint32_t ID, const Shader* shader, ShaderParameter::Type parameterType) const = 0;
	virtual void UnBind(uint32_t ID, const Shader* shader, ShaderParameter::Type parameterType) const = 0;

protected:
    MinFilter                                       m_MinFilter;
    MagFilter                                       m_MagFilter;
    MipFilter                                       m_MipFilter;
    WrapMode                                        m_WrapModeU, m_WrapModeV, m_WrapModeW;
    CompareMode                                     m_CompareMode;
    CompareFunc                                     m_CompareFunc;

    float                                           m_fLODBias;
    float                                           m_fMinLOD;
    float                                           m_fMaxLOD;

    vec4		                                    m_BorderColor;

    bool                                            m_bIsAnisotropicFilteringEnabled;
    uint8_t                                         m_AnisotropicFiltering;

    mutable bool                                            m_bIsDirty;
};
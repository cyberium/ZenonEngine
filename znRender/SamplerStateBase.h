#pragma once

class OW_ENGINE_API SamplerStateBase : public ISamplerState, public Object
{	
public:
	SamplerStateBase();
    virtual ~SamplerStateBase();

    virtual void SetFilter(MinFilter minFilter, MagFilter magFilter, MipFilter mipFilter);
	virtual void GetFilter(MinFilter& minFilter, MagFilter& magFilter, MipFilter& mipFilter) const;

	virtual void SetWrapMode(WrapMode u = WrapMode::Repeat, WrapMode v = WrapMode::Repeat, WrapMode w = WrapMode::Repeat);
	virtual void GetWrapMode(WrapMode& u, WrapMode& v, WrapMode& w) const;

	virtual void SetCompareFunction(CompareFunc compareFunc);
	virtual CompareFunc GetCompareFunc() const;

	virtual void SetLODBias(float lodBias);
	virtual float GetLODBias() const;

	virtual void SetMinLOD(float minLOD);
    virtual float GetMinLOD() const;

	virtual void SetMaxLOD(float maxLOD);
	virtual float GetMaxLOD() const;

	virtual void SetBorderColor(cvec4 borderColor);
	virtual vec4 GetBorderColor() const;

	virtual void EnableAnisotropicFiltering(bool enabled);
	virtual bool IsAnisotropicFilteringEnabled() const;

	virtual void SetMaxAnisotropy(uint8_t maxAnisotropy);
	virtual uint8_t GetMaxAnisotropy() const;

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

    mutable bool                                    m_bIsDirty;
};
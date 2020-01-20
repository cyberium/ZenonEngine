#pragma once

class ZN_API BlendStateBase 
	: public IBlendState
	, public Object
{
public:
	BlendStateBase();
    virtual ~BlendStateBase();

	virtual void SetBlendMode(const BlendMode& blendMode);
	virtual void SetBlendModes(const std::vector<BlendMode>& blendModes);
	virtual const std::vector<BlendMode>& GetBlendModes() const;

	virtual void SetConstBlendFactor(cvec4 constantBlendFactor);
	virtual cvec4 GetConstBlendFactor() const;

    virtual void SetSampleMask(uint32_t sampleMask);
	virtual uint32_t GetSampleMask() const;

	virtual void SetAlphaCoverage(bool enabled);
	virtual bool GetAlphaCoverage() const;

	virtual void SetIndependentBlend(bool enabled);
	virtual bool GetIndependentBlend() const;

protected:
    typedef std::vector<BlendMode> BlendModeList;
    BlendModeList                                   m_BlendModes;

    bool                                            m_bAlphaToCoverageEnabled;
    bool                                            m_bIndependentBlendEnabled;
    uint32_t                                        m_SampleMask;

    vec4                                            m_ConstBlendFactor;

    bool                                            m_bDirty;
};

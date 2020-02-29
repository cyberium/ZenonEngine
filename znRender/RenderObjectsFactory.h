#pragma once

class ZN_API CRenderObjectsFactory
	: public IRenderObjectsFactory
{
public:
	CRenderObjectsFactory();
	virtual ~CRenderObjectsFactory();

	// IRenderObjectsFactory
	bool                              HoldRenderObject(RenderObjectID ID) const;
	bool                              UnholdRenderObject(RenderObjectID ID) const;
	std::shared_ptr<IRenderObject>    GetRenderObject(RenderObjectID ID) const;
	void                              ClearCache() override;

protected:
	RenderObjectID                    GenerateRenderObjectID();


protected:
	std::unordered_map<RenderObjectID, std::weak_ptr<IRenderWindow>>      m_RenderWindows;

	std::unordered_map<RenderObjectID, std::weak_ptr<IShader>>            m_Shaders;
	std::unordered_map<std::string,    std::weak_ptr<IShader>>            m_ShadersByName;

	std::unordered_map<RenderObjectID, std::weak_ptr<IGeometry>>          m_Geometries;
	std::unordered_map<RenderObjectID, std::weak_ptr<IModel>>             m_Models;

	std::unordered_map<RenderObjectID, std::weak_ptr<ITexture>>           m_Textures;
	std::unordered_map<std::string,    std::weak_ptr<ITexture>>           m_TexturesByName;
	
	std::unordered_map<RenderObjectID, std::weak_ptr<IMaterial>>          m_Materials;
	std::unordered_map<RenderObjectID, std::weak_ptr<IRenderTarget>>      m_RenderTargets;
	std::unordered_map<RenderObjectID, std::weak_ptr<IQuery>>             m_Queries;
	std::unordered_map<RenderObjectID, std::weak_ptr<ISamplerState>>      m_Samplers;
	std::unordered_map<RenderObjectID, std::weak_ptr<IBlendState>>        m_BlendStates;
	std::unordered_map<RenderObjectID, std::weak_ptr<IDepthStencilState>> m_DepthStencilStates;
	std::unordered_map<RenderObjectID, std::weak_ptr<IRasterizerState>>   m_RasterizerStates;
	std::unordered_map<RenderObjectID, std::weak_ptr<IPipelineState>>     m_Pipelines;
	std::unordered_map<RenderObjectID, std::weak_ptr<IBuffer>>            m_Buffers;

private:
	RenderObjectID m_RenderObjectIDCntr;
};
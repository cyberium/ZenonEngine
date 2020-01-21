#pragma once

class ZN_API CRenderDevicePrimitiveCollection 
	: public IRenderDevicePrimitiveCollection
{
public:
	CRenderDevicePrimitiveCollection(IRenderDevice* RenderDevice);
	virtual ~CRenderDevicePrimitiveCollection();

	IMesh* CreateLine(cvec3 _dest); // Create a line
	IMesh* CreatePlane(cvec3 N = vec3(0, 1, 0)); // Create a plane in 3D.
	IMesh* CreateScreenQuad(float left = 0.0f, float right = 1.0f, float bottom = 0.0f, float top = 1.0f, float z = 0.0f); // Create a screen-space quad that can be used to render full-screen post-process effects to the screen.
	IMesh* CreateSphere(); // Create a sphere in 3D
	IMesh* CreateCube(); // Create a cube in 3D.
	IMesh* CreateCone(); // Create a cone.
	IMesh* CreateQuad() override;

	// 3D meshes
	IMesh* Create3DQuad(float width = 10.0f, float height = 10.0f);
	IMesh* Create3DBeizerLine(vec3 start = vec3(0.0f, 0.0f, 0.0f), vec3 end = vec3(500.0f, 0.0f, 500.0f));

	// UI meshes
	IMesh* CreateUIQuad(float width = 10.0f, float height = 10.0f);
	IMesh* CreateUIBeizerLine(vec2 start = vec2(0.0f, 0.0f), vec2 end = vec2(500.0f, 500.0f));

private:
	IRenderDevice* m_RenderDevice;
};


class ZN_API RenderDeviceBase 
	: public IRenderDevice
	, public Object
	, public std::enable_shared_from_this<IRenderDevice>
{
public:
	RenderDeviceBase();
	virtual ~RenderDeviceBase();

	virtual IRenderDevicePrimitiveCollection* GetPrimitiveCollection() const override;

protected:
	std::vector<std::shared_ptr<IRenderWindow>>     m_RenderWindows;

	std::vector<std::shared_ptr<IShader>>           m_Shaders;
	std::unordered_map<std::string, std::shared_ptr<IShader>> m_ShadersByName;

	std::vector<std::shared_ptr<IMesh>>             m_Meshes;

	std::vector<std::shared_ptr<ITexture>>          m_Textures;
	std::unordered_map<std::string, std::shared_ptr<ITexture>> m_TexturesByName;
	ITexture*                                       m_pDefaultTexture;

	std::vector<std::shared_ptr<IMaterial>>         m_Materials;

	std::vector<std::shared_ptr<IRenderTarget>>     m_RenderTargets;

	std::vector<std::shared_ptr<IQuery>>            m_Queries;

	std::vector<std::shared_ptr<ISamplerState>>     m_Samplers;

	std::vector<std::shared_ptr<IPipelineState>>    m_Pipelines;

	std::vector<std::shared_ptr<IBuffer>>           m_Buffers;
	
private:
	std::shared_ptr<IRenderDevicePrimitiveCollection> m_RenderDevicePrimitiveCollection;
};

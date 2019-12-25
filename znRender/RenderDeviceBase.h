#pragma once

class ZN_API CRenderDevicePrimitiveCollection : public IRenderDevicePrimitiveCollection
{
public:
	CRenderDevicePrimitiveCollection(IRenderDevice* RenderDevice);
	virtual ~CRenderDevicePrimitiveCollection();

	std::shared_ptr<IMesh> CreateLine(cvec3 _dest); // Create a line
	std::shared_ptr<IMesh> CreatePlane(cvec3 N = vec3(0, 1, 0)); // Create a plane in 3D.
	std::shared_ptr<IMesh> CreateScreenQuad(float left = 0.0f, float right = 1.0f, float bottom = 0.0f, float top = 1.0f, float z = 0.0f); // Create a screen-space quad that can be used to render full-screen post-process effects to the screen.
	std::shared_ptr<IMesh> CreateSphere(); // Create a sphere in 3D
	std::shared_ptr<IMesh> CreateCube(); // Create a cube in 3D.
	std::shared_ptr<IMesh> CreateCone(); // Create a cone.

	// 3D meshes
	std::shared_ptr<IMesh> Create3DQuad(float width = 10.0f, float height = 10.0f);
	std::shared_ptr<IMesh> Create3DBeizerLine(vec3 start = vec3(0.0f, 0.0f, 0.0f), vec3 end = vec3(500.0f, 0.0f, 500.0f));

	// UI meshes
	std::shared_ptr<IMesh> CreateUIQuad(float width = 10.0f, float height = 10.0f);
	std::shared_ptr<IMesh> CreateUIBeizerLine(vec2 start = vec2(0.0f, 0.0f), vec2 end = vec2(500.0f, 500.0f));

private:
	IRenderDevice* m_RenderDevice;
};


class ZN_API RenderDeviceBase : public IRenderDevice, public Object
{
public:
	RenderDeviceBase();
	virtual ~RenderDeviceBase();

	virtual std::shared_ptr<IRenderDevicePrimitiveCollection> GetPrimitiveCollection() const override;

protected:
    virtual void									CreateDevice() = 0;

protected:
	BufferList                                      m_Buffers;
	MeshList                                        m_Meshes;

	ShaderList                                      m_Shaders;
	ShaderNameMap                                   m_ShadersByName;

	TextureList                                     m_Textures;
	TextureNameMap                                  m_TexturesByName;
	std::shared_ptr<ITexture>                       m_pDefaultTexture;

	MaterialList                                    m_Materials;
	RenderTargetList                                m_RenderTargets;
	SamplerList                                     m_Samplers;
	PipelineList                                    m_Pipelines;
    QueryList                                       m_Queries;

private:
	std::shared_ptr<IRenderDevicePrimitiveCollection> m_RenderDevicePrimitiveCollection;
};

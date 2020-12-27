#pragma once

namespace
{
	struct __declspec(align(16)) ZN_API STerrainParams
	{
		glm::vec3 Scaling;
		float __padding0;
		//--------------------------------------------------------------( 16 bytes )

		uint32   ImageWidth;
		uint32   ImageHeight;
		float __padding1[2];
		//--------------------------------------------------------------( 16 bytes )
	};
}

class ZN_API CTerrainPass
	: public Base3DPass
{
public:
	CTerrainPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CTerrainPass();

	void SetEnviorementTexture(std::shared_ptr<ITexture> Texture);
	IShaderParameter* GetLightsShaderParameter() const;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override final;

	// IVisitor
	EVisitResult Visit(const std::shared_ptr<ISceneNode>& SceneNode) override final;
	EVisitResult Visit(const std::shared_ptr<IModel>& Model) override final;
	EVisitResult Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override final;
	EVisitResult Visit(const std::shared_ptr<ITerrain>& Terrain) override final;

private:
	std::shared_ptr<IStructuredBuffer> m_HeightMapBuffer;
	IShaderParameter*                  m_HeightMapBufferParameter;

	std::shared_ptr<IConstantBuffer>   m_TerrainParamsBuffer;
	IShaderParameter*                  m_TerrainParamsBufferParameter;

	IShaderParameter*                  m_ShaderLightsBufferParameter;

	std::shared_ptr<IMaterial>         m_Material;
	std::shared_ptr<IGeometry>         m_Geometry;
};
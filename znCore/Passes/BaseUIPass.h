#pragma once

#include "BaseScenePass.h"

// FORWARD BEGIN
class CUIBaseNode;
// FORWARD END

class ZN_API BaseUIPass : public CBaseScenePass
{
public:
	BaseUIPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> pipeline);
	virtual ~BaseUIPass();

	// IVisitor
	virtual bool VisitUI(ISceneNode* node) override;
	virtual bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(IGeometry* Geometry, const IShader* VertexShader, const SRenderGeometryArgs& RenderGeometryArgs) override;

protected: // PerObject functional
	__declspec(align(16)) struct PerObjectUI
	{
		glm::mat4 Model;
		glm::mat4 Projection;
	};
	PerObjectUI*                                    m_PerObjectData;
	std::shared_ptr<IConstantBuffer>                m_PerObjectConstantBuffer;

	void                                            SetPerObjectConstantBufferData();
	std::shared_ptr<IConstantBuffer>                GetPerObjectConstantBuffer() const;
	void                                            BindPerObjectConstantBuffer(std::shared_ptr<IShader> shader);
};
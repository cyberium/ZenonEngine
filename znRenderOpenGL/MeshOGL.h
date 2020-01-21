#pragma once

class ZN_API MeshOGL : public MeshBase
{
public:
    MeshOGL();
	virtual ~MeshOGL();

	void SetPrimitiveTopology(PrimitiveTopology _topology) override final;

	virtual bool Render(const RenderEventArgs* renderArgs, const IConstantBuffer* perObject, SGeometryPartParams GeometryPartParams);

};
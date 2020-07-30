#pragma once

#include "ObjectsFactories/Object.h"

class ZN_API GeometryProxie
	: public IGeometry
	, public IGeometryInternal
	, public Object
{
public:
	GeometryProxie(const std::shared_ptr<IGeometry>& Geometry);
	virtual ~GeometryProxie();

	// IGeometry
	virtual void SetBounds(const BoundingBox& Bounds) override;
	virtual BoundingBox GetBounds() const override;

	void AddVertexBuffer(const BufferBinding& Binding, const std::shared_ptr<IBuffer>& VertexBuffer) override;

	void SetVertexBuffer(const std::shared_ptr<IBuffer>& GlobalVertexBuffer) override final;
	void SetIndexBuffer(const std::shared_ptr<IBuffer>& IndexBuffer) override final;

	void SetPrimitiveTopology(PrimitiveTopology Topology) override final;
	PrimitiveTopology GetPrimitiveTopology() const override final;

	virtual void Render(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) const override;

	virtual void Accept(IVisitor* visitor, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

	// IGeometryInternal
	void Render_BindAllBuffers(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader) const override final;
	void Render_Draw(const SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) const override final;
	void Render_UnbindAllBuffers(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader) const override final;

	// IObject
	Guid GetGUID() const override final { return Object::GetGUID(); };
	std::string GetName() const override final { return Object::GetName(); };
	void SetName(const std::string& Name) override final { Object::SetName(Name); };

	// IObjectLoadSave
	void Load(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

private:
	std::shared_ptr<IGeometry> m_Geometry;
	std::shared_ptr<IGeometryInternal> m_GeometryInternal;
};
#pragma once

#include "Interfaces/Render/Types/RenderGeometryParams.h"

// FORWARD BEGIN
ZN_INTERFACE IBuffer;
ZN_INTERFACE IShader;
ZN_INTERFACE IConstantBuffer;
class RenderEventArgs;
// FORWARD END

ZN_INTERFACE ZN_API IGeometry : public std::enable_shared_from_this<IGeometry>
{
	typedef std::unordered_map<BufferBinding, IBuffer*> BufferMap;

	virtual ~IGeometry() {}

	virtual size_t GetHash() const = 0;

	virtual void SetBounds(const BoundingBox& Bounds) = 0;
	virtual const BoundingBox& GetBounds() const = 0;

	virtual void AddVertexBuffer(const BufferBinding& Binding, std::shared_ptr<IBuffer> Buffer) = 0;
	virtual void SetVertexBuffer(std::shared_ptr<IBuffer> Buffer) = 0;
	virtual void SetIndexBuffer(std::shared_ptr<IBuffer> Buffer) = 0;

	virtual void SetPrimitiveTopology(PrimitiveTopology Topology) = 0;

	virtual bool Render(         const RenderEventArgs& RenderEventArgs, const ShaderMap& ShadersMap, const IMaterial* Material, const SGeometryDrawArgs& GeometryDrawArgs) const = 0;
	virtual bool RenderInstanced(const RenderEventArgs& RenderEventArgs, const ShaderMap& ShadersMap, const IMaterial* Material, const SGeometryDrawInstancedArgs& GeometryDrawInstancedArgs) const = 0;
	
	virtual bool Accept(IVisitor* visitor, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;
};

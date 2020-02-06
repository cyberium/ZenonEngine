#pragma once

#include "Interfaces/Render/Types/RenderGeometryParams.h"

// FORWARD BEGIN
ZN_INTERFACE IBuffer;
ZN_INTERFACE IShader;
class RenderEventArgs;
// FORWARD END


/**
  * Геометрия содержит в себе информацию:
  *   - Вершинные буфферы объекта (например буффер позиций, буффер нормалей и буффер текстурных координат),
  *   - Способ объединения данных в буффере вершин - PrimitiveTopology
  *   - BoundingBox для текущего набора вершин
  * 
  * ! Важно помнить, что для правильной отрисовки моделей требуется предварительно выставить материал и
  *   параметры, специфичные для каждого объекта (World матрица). Данному объекту необходим доступ только в вершинный шейдер, 
  *   для передачи в него данных о каждой вершине.
*/
ZN_INTERFACE ZN_API IGeometry : public std::enable_shared_from_this<IGeometry>
{
	typedef std::unordered_map<BufferBinding, std::shared_ptr<IBuffer>> BuffersMap;

	virtual ~IGeometry() {}

	virtual void SetBounds(const BoundingBox& Bounds) = 0;
	virtual BoundingBox GetBounds() const = 0;

	/** Добавить буффер с вершинными данными определённого типа (POSITION, TEXCOORD, NORMAL и тд)
	  * Обязательным является только буффер позиций (POSITION). Остальные буфферы являются опциональными.
	  *
	  * ! Важно помнить что бы BufferBinding (имя и слот) имелись в InputSemantic вершинного шейдера.
	  */
	virtual void AddVertexBuffer(const BufferBinding& Binding, const std::shared_ptr<IBuffer>& VertexBuffer) = 0;

	/** Установить буффер с полным набором вершинных данных. 
	  * Способ и порядок представления данных в нём должен полностью совпадать с InputSemantics для конкретного вершинного шейдера.
	  */
	virtual void SetVertexBuffer(const std::shared_ptr<IBuffer>& GlobalVertexBuffer) = 0;
	virtual void SetIndexBuffer(const std::shared_ptr<IBuffer>& IndexBuffer) = 0;

	virtual void SetPrimitiveTopology(PrimitiveTopology Topology) = 0;

	virtual bool Render(         const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) const = 0;
	virtual bool RenderInstanced(const RenderEventArgs& RenderEventArgs, const IShader* VertexShader, const SGeometryDrawInstancedArgs GeometryDrawInstancedArgs = SGeometryDrawInstancedArgs()) const = 0;
	
	// For IVisitor

	virtual void Accept(IVisitor* visitor, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;
};

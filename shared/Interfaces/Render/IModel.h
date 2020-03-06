#pragma once

#include "Interfaces/Render/Types/RenderGeometryParams.h"

// FORWARD BEGIN
ZN_INTERFACE IMaterial;
ZN_INTERFACE IGeometry;
class RenderEventArgs;
// FORWARD END

/**
  * Модель представляет собой совокупность логически объединенной геометрии (например: геометрия ножен стола и геометрия столешницы),
  * материалы, используемые для отрисовки каждой геометрии (например: металл для ножен и дерево для столешницы)
  * и связь материала и геометрии между собой
  *
*/
ZN_INTERFACE ZN_API IModel 
	: public std::enable_shared_from_this<IModel>
{
	struct ZN_API SConnection
	{
		std::shared_ptr<IMaterial> Material;
		std::shared_ptr<IGeometry> Geometry;
		SGeometryDrawArgs          GeometryDrawArgs;
	};

	virtual ~IModel() {}

	virtual void                                    SetName(const std::string& Name) = 0;
	virtual std::string                             GetName() const = 0;

	virtual void                                    SetBounds(const BoundingBox& Bounds) = 0;
	virtual BoundingBox                             GetBounds() const = 0;

	/**
	 * Добавляет пару геометрия[+GeometryPart] - Материал
	*/
	virtual void									AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;
	virtual const std::vector<SConnection>&         GetConnections() const = 0;

	virtual bool                                    Render(const RenderEventArgs& renderEventArgs) const = 0;

	// For IVisitor

	virtual void                                    Accept(IVisitor* visitor) = 0;
};
typedef std::shared_ptr<IModel> IModelPtr;

typedef std::vector<std::shared_ptr<IModel>> ModelsList;

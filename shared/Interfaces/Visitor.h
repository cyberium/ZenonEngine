#pragma once

#include "Interfaces/Render/Types/RenderGeometryParams.h"
#include "Interfaces/Render/Types/ShaderType.h"

// FORWARD BEGIN
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
ZN_INTERFACE IModel;
ZN_INTERFACE IGeometry;
ZN_INTERFACE IMaterial;
ZN_INTERFACE ILightComponent3D;
// FORWARD END

/**
  * Паттерн посетитель используется в ZenonEngine в частности для отрисовки сцены.
  *
  * Пример для 3D:
  *   1) Посетитель вызывает Accept(IVisitor*) у ISceneNode3D
  *   2) ISceneNode3D вызывает или не вызывает (в случае отсутствия необходимости) Visit(ISceneNode3D*) у посетителя и передает в него себя
  *   3) Посетитель сообщает ISceneNode3D что "визит" прошел либо успешно либо нет
  *   4 Успех) В случае если визит ноды был одобрен посетителем, то нода    
*/
ZN_INTERFACE ZN_API IVisitor : public std::enable_shared_from_this<IVisitor>
{
	virtual ~IVisitor() {}

	virtual bool Visit(const ISceneNode3D* node) = 0;
	virtual bool Visit(const ISceneNodeUI* node) = 0;
	virtual bool Visit(const IModel* Model) = 0;
	virtual bool Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;
	virtual bool Visit(const ILightComponent3D* Light) = 0;
};
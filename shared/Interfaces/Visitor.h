#pragma once

#include "Interfaces/Render/Types/RenderTypes.h"

// FORWARD BEGIN
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
ZN_INTERFACE IModel;
ZN_INTERFACE IGeometry;
ZN_INTERFACE IMaterial;
ZN_INTERFACE ISceneNodeComponent;
ZN_INTERFACE ILight3D;
ZN_INTERFACE IParticleSystem;
// FORWARD END

enum ZN_API EVisitResult : size_t
{
	Block = 1 << 0,
	AllowVisitChilds = 1 << 1,
	AllowVisitContent = 1 << 2,
	AllowAll = AllowVisitChilds | AllowVisitContent
};

/**
  * Паттерн посетитель используется в ZenonEngine в частности для отрисовки сцены.
  *
  * Пример для 3D:
  *   1) Посетитель вызывает Accept(IVisitor*) у ISceneNode3D
  *   2) ISceneNode3D вызывает или не вызывает (в случае отсутствия необходимости) Visit(ISceneNode3D*) у посетителя и передает в него себя
  *   3) Посетитель сообщает ISceneNode3D что "визит" прошел либо успешно либо нет
  *   4 Успех) В случае если визит ноды был одобрен посетителем, то нода    
*/
ZN_INTERFACE ZN_API IVisitor 
{
	virtual ~IVisitor() {}

	virtual EVisitResult Visit(const ISceneNode3D* SceneNode) = 0;
	virtual EVisitResult Visit(const ISceneNodeUI* SceneNode) = 0;
	virtual EVisitResult Visit(const IModel* Model) = 0;
	virtual EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;
	virtual EVisitResult Visit(const ISceneNodeComponent* Component) = 0;
	virtual EVisitResult Visit(const ILight3D* Light) = 0;
	virtual EVisitResult Visit(const IParticleSystem* ParticleSystem) = 0;
};
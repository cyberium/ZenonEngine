#pragma once

#include "Interfaces/Render/Types/RenderTypes.h"

// FORWARD BEGIN
ZN_INTERFACE ISceneNode;
ZN_INTERFACE IUIControl;
ZN_INTERFACE IModel;
ZN_INTERFACE IGeometry;
ZN_INTERFACE IMaterial;
ZN_INTERFACE ISceneNodeComponent;
ZN_INTERFACE ILight;
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
  *   1) Посетитель вызывает Accept(IVisitor*) у ISceneNode
  *   2) ISceneNode вызывает или не вызывает (в случае отсутствия необходимости) Visit(ISceneNode*) у посетителя и передает в него себя
  *   3) Посетитель сообщает ISceneNode что "визит" прошел либо успешно либо нет
  *   4 Успех) В случае если визит ноды был одобрен посетителем, то нода    
*/
ZN_INTERFACE ZN_API IVisitor 
{
	virtual ~IVisitor() {}

	virtual EVisitResult Visit(const std::shared_ptr<ISceneNode>& SceneNode) = 0;
	virtual EVisitResult Visit(const std::shared_ptr<IUIControl>& SceneNode) = 0;
	virtual EVisitResult Visit(const std::shared_ptr<IModel>& Model) = 0;
	virtual EVisitResult Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;
	virtual EVisitResult Visit(const std::shared_ptr<ISceneNodeComponent>& Component) = 0;
	virtual EVisitResult Visit(const std::shared_ptr<ILight>& Light) = 0;
	virtual EVisitResult Visit(const std::shared_ptr<IParticleSystem>& ParticleSystem) = 0;
};
#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNodeComponent;
// FORWARD END


ZN_INTERFACE IPortalRoom;


ZN_INTERFACE ZN_API IPortal
{
	enum class ZN_API Side
	{
		None,
		Inner,
		Outer
	};

	virtual ~IPortal() {}

	virtual Frustum CreatePolyFrustum(glm::vec3 Eye) const = 0;

	virtual bool IsVisible(const Frustum& Frustum) const = 0;
	virtual bool IsVisible(const std::vector<Plane>& Planes) const = 0;
	virtual bool IsPositive(const glm::vec3& InvTranslateCamera) const = 0;

	virtual std::shared_ptr<IPortalRoom> GetRoomObject(glm::vec3 Eye) const = 0;
};


/**
  * В каждой комнате содержаться объекты.
  * Объект видим если соблюдены следующие условия:
  *   - комната видима
  *     а) комната видима через портал
  *     б) комната видима из-за того что игрок находится в ней
  *   - объект видим через портал
*/
ZN_INTERFACE ZN_API IPortalRoomObject
{
	virtual ~IPortalRoomObject() {}

	virtual BoundingBox GetBoundingBox() const = 0;
	virtual void SetVisibilityState(bool Value) = 0;
};


/**
  * Комната в которой содержаться объекты и которые отделены
  * друг от друга и от внешнего мира порталами.
  * Внутри комнаты могут быть и другие комнаты.
*/
ZN_INTERFACE ZN_API IPortalRoom
{
	virtual ~IPortalRoom() {}

	virtual void AddPortal(const std::shared_ptr<IPortal>& Portal) = 0;
	virtual const std::vector<std::shared_ptr<IPortal>>& GetPortals() const = 0;
	virtual void AddRoomObject(const std::weak_ptr<IPortalRoomObject>& RoomObject) = 0;
	virtual const std::vector<std::weak_ptr<IPortalRoomObject>>& GetRoomObjects() const = 0;

	virtual void Reset() = 0;
	virtual BoundingBox GetBoundingBox() const = 0;
	virtual void SetVisibilityState(bool Value) = 0;
	virtual void SetCalculatedState(bool Value) = 0;
	virtual bool IsCalculated() const = 0;
};

ZN_INTERFACE ZN_API IPortalsComponent3D
{
	ZN_OBJECTCLASS(cSceneNodePortalsComponent);

	virtual ~IPortalsComponent3D() {}
};
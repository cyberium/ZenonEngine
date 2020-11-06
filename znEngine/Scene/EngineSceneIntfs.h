#pragma once

const int32 cCellsCount = 32;
const int32 cCellsHalfCount = cCellsCount / 2;
const float cCellSize = 10.0f;
const float cCellHalfSize = cCellSize / 2.0f;

enum class ERTSCellType : uint8
{
	ctGround = 0,
	ctWater = 1
};


struct SRTSCellCoords
{
	SRTSCellCoords(int32 X_, int32 Z_)
		: X(X_)
		, Z(Z_)
	{
		if ((X < 0) || (X >= cCellsCount) || (Z < 0) || (Z >= cCellsCount))
		{
			X = -1;
			Z = -1;
		}
	}

	int32 GetX() const 
	{ 
		return X; 
	}

	int32 GetZ() const 
	{ 
		return Z; 
	}

	bool IsCorrect() const 
	{ 
		return (X != -1) && (Z != -1); 
	}

	glm::vec3 ToPosition() const
	{
		return glm::vec3(cCellSize * GetX(), 7.5f, cCellSize * GetZ());
	}

	std::string ToString() const
	{
		if (IsCorrect())
			return "Cell_X" + std::to_string(GetX()) + "_Z" + std::to_string(GetZ());
		return "Cell_Incorrect";
	}

private:
	int32 X, Z;
};


struct SRTSCell
{
	SRTSCell()
		: Type(ERTSCellType::ctGround)
		, Coords(-1, -1)
	{}

	SRTSCellCoords Coords;
	ERTSCellType Type;
	std::shared_ptr<IModel> Model;
};

ZN_INTERFACE ZN_API ISceneNodeRTSGround
	: public virtual ISceneNode
{
	ZN_OBJECTCLASS(cSceneNodeRTSGround);

	virtual ~ISceneNodeRTSGround() {}

	virtual bool AddCell(ERTSCellType CellType, SRTSCellCoords Coords) = 0;
	virtual SRTSCell& GetCell(SRTSCellCoords Coords) = 0;
	virtual const SRTSCell& GetCellConst(SRTSCellCoords Coords) const = 0;
	virtual SRTSCellCoords PositionToCoords(const glm::vec3& Position) = 0;
	virtual glm::vec3 PositionToPosition(const glm::vec3& Position, float Height = 0.0f) = 0;
};



ZN_INTERFACE ISceneNodeRTSUnit;
ZN_INTERFACE ISceneNodeRTSTower;
ZN_INTERFACE ISceneNodeRTSPath;
ZN_INTERFACE ISceneNodeRTSPoint;


ZN_INTERFACE ZN_API ISceneNodeRTSUnit
	: public virtual ISceneNode
{
	ZN_OBJECTCLASS(cSceneNodeRTSUnit);
	virtual ~ISceneNodeRTSUnit() {}

	virtual void SetHealth(float Health) = 0;
	virtual void SetMaxHealth(float MaxHealth) = 0;
	virtual float GetHealth() const = 0;
	virtual void SetSpeed(float Speed) = 0;
	virtual float GetSpeed() const = 0;

	virtual void DealDamage(float Damage) = 0;

	virtual void SetPath(std::shared_ptr<ISceneNodeRTSPath> Path) = 0;
};


ZN_INTERFACE ZN_API ISceneNodeRTSBullet
	: public virtual ISceneNode
{
	ZN_OBJECTCLASS(cSceneNodeRTSBullet);
	virtual ~ISceneNodeRTSBullet() {}

	virtual void SetTarget(std::shared_ptr<ISceneNodeRTSUnit> Target) = 0;
	virtual std::shared_ptr<ISceneNode> GetTarget() const = 0;
	virtual void SetDamage(float Damage) = 0;
	virtual float GetDamage() const = 0;
	virtual void SetSpeed(float Speed) = 0;
	virtual float GetSpeed() const = 0;
};


ZN_INTERFACE ZN_API ISceneNodeRTSTower
	: public virtual ISceneNode
{
	virtual ~ISceneNodeRTSTower() {}

	virtual void SetAttackDamage(float Value) = 0;
	virtual float GetAttackDamage() const = 0;
	virtual void SetAttackInterval(float Value) = 0;
	virtual float GetAttackInterval() const = 0;
	virtual void SetAttackRange(float Value) = 0;
	virtual float GetAttackRange() const = 0;
};


ZN_INTERFACE ZN_API ISceneNodeRTSPoint
	: public virtual ISceneNode
{
	ZN_OBJECTCLASS(cSceneNodeRTSPoint);
	virtual ~ISceneNodeRTSPoint() {}

};


ZN_INTERFACE ZN_API ISceneNodeRTSPath
	: public virtual ISceneNode
{
	ZN_OBJECTCLASS(cSceneNodeRTSPath);
	virtual ~ISceneNodeRTSPath() {}

	virtual std::shared_ptr<ISceneNodeRTSPoint> AddPoint(glm::vec3 XYZ) = 0;
	virtual std::shared_ptr<ISceneNodeRTSPoint> InsertPoint(glm::vec3 XYZ, size_t AfterIndex) = 0;
	virtual std::shared_ptr<ISceneNodeRTSPoint> GetLastPoint() = 0;
	virtual std::vector<std::shared_ptr<ISceneNodeRTSPoint>> GetPoints() const = 0;
	virtual std::shared_ptr<ISceneNodeRTSPoint> GetPoint(size_t Index) const = 0;
};

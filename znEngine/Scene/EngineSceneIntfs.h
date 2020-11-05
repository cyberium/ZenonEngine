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

ZN_INTERFACE ZN_API IRTSGround
	: public virtual ISceneNode
{
	ZN_OBJECTCLASS(cCRTSSceneNodeGround);

	virtual ~IRTSGround() {}

	virtual bool AddCell(ERTSCellType CellType, SRTSCellCoords Coords) = 0;
	virtual SRTSCell& GetCell(SRTSCellCoords Coords) = 0;
	virtual const SRTSCell& GetCellConst(SRTSCellCoords Coords) const = 0;
	virtual SRTSCellCoords PositionToCoords(const glm::vec3& Position) = 0;
	virtual glm::vec3 PositionToPosition(const glm::vec3& Position, float Height = 0.0f) = 0;
};

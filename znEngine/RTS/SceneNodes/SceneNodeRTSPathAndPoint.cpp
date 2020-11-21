#include "stdafx.h"

// General
#include "SceneNodeRTSPathAndPoint.h"

// Additional
#include "Materials/MaterialDebug.h"

CSceneNodeRTSPoint::CSceneNodeRTSPoint(IScene & Scene)
	: CSceneNode(Scene)
{}

CSceneNodeRTSPoint::~CSceneNodeRTSPoint()
{}






CSceneNodeRTSPath::CSceneNodeRTSPath(IScene & Scene)
	: CSceneNode(Scene)
{}

CSceneNodeRTSPath::~CSceneNodeRTSPath()
{}

//
// ISceneNodeRTSPath
//
std::shared_ptr<ISceneNodeRTSPoint> CSceneNodeRTSPath::AddPoint(glm::vec3 XYZ)
{
	auto point = GetScene().CreateSceneNodeCast<ISceneNodeRTSPoint>(cSceneNodeRTSPoint);
	point->SetName("Point");
	point->SetTranslate(XYZ);
	point->GetComponentT<IColliderComponent3D>()->SetBounds(BoundingBox(glm::vec3(-2.5f), glm::vec3(2.5f)));
	return point;
}

std::shared_ptr<ISceneNodeRTSPoint> CSceneNodeRTSPath::InsertPoint(glm::vec3 XYZ, size_t AfterIndex)
{
	throw CException("Not implemented");
}

std::shared_ptr<ISceneNodeRTSPoint> CSceneNodeRTSPath::GetLastPoint()
{
	const auto& points = GetPoints();
	if (points.empty())
		return nullptr;
	auto lastPoint = points.at(points.size() - 1);
	return points.at(points.size() - 1);
}

std::vector<std::shared_ptr<ISceneNodeRTSPoint>> CSceneNodeRTSPath::GetPoints() const
{
	const auto& childs = GetChilds();
	std::vector<std::shared_ptr<ISceneNodeRTSPoint>> points;
	std::for_each(childs.begin(), childs.end(), [&points](const std::shared_ptr<ISceneNode>& Node) {
		points.push_back(std::dynamic_pointer_cast<ISceneNodeRTSPoint>(Node));
	});
	return points;
}

std::shared_ptr<ISceneNodeRTSPoint> CSceneNodeRTSPath::GetPoint(size_t Index) const
{
	const auto& points = GetPoints();
	if (points.empty())
		return nullptr;
	if (Index >= points.size())
		return nullptr;
	return points.at(Index);
}



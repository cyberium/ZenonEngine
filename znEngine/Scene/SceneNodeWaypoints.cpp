#include "stdafx.h"

// General
#include "SceneNodeWaypoints.h"

// Additional
#include "Materials/MaterialDebug.h"

CSceneNodePoint::CSceneNodePoint(IScene & Scene)
	: CSceneNode(Scene)
{}

CSceneNodePoint::~CSceneNodePoint()
{}






CSceneNodePath::CSceneNodePath(IScene & Scene)
	: CSceneNode(Scene)
{}

CSceneNodePath::~CSceneNodePath()
{}

//
// ISceneNodePath
//
std::shared_ptr<ISceneNodePoint> CSceneNodePath::AddPoint(glm::vec3 XYZ)
{
	auto point = GetScene().CreateSceneNodeT<ISceneNodePoint>(shared_from_this());
	point->SetName("Point");
	point->SetTranslate(XYZ);
	point->GetComponent<IColliderComponent3D>()->SetBounds(BoundingBox(glm::vec3(-2.5f), glm::vec3(2.5f)));
	return point;
}

std::shared_ptr<ISceneNodePoint> CSceneNodePath::InsertPoint(glm::vec3 XYZ, size_t AfterIndex)
{
	throw CException("Not implemented");
}

std::shared_ptr<ISceneNodePoint> CSceneNodePath::GetLastPoint()
{
	const auto& points = GetPoints();
	if (points.empty())
		return nullptr;
	auto lastPoint = points.at(points.size() - 1);
	return points.at(points.size() - 1);
}

std::vector<std::shared_ptr<ISceneNodePoint>> CSceneNodePath::GetPoints() const
{
	const auto& childs = GetChilds();
	std::vector<std::shared_ptr<ISceneNodePoint>> points;
	std::for_each(childs.begin(), childs.end(), [&points](const std::shared_ptr<ISceneNode>& Node) {
		points.push_back(std::dynamic_pointer_cast<ISceneNodePoint>(Node));
	});
	return points;
}



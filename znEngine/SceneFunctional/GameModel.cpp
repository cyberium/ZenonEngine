#include "stdafx.h"

// General
#include "GameModel.h"



CGameModel::CGameModel(const std::shared_ptr<IModel>& Model)
	: ModelProxie(Model)
{
}

CGameModel::~CGameModel()
{
}



//
// IModel
//

bool CGameModel::Render(const RenderEventArgs & renderEventArgs) const
{
	return ModelProxie::Render(renderEventArgs);
}

void CGameModel::Accept(IVisitor * visitor)
{
	EVisitResult visitResult = visitor->Visit(this);

	if (visitResult & EVisitResult::AllowVisitContent)
	{

	}

	if (visitResult & EVisitResult::AllowVisitChilds)
	{
		for (const auto& connection : GetConnections())
		{
			connection.Geometry->Accept(visitor, connection.Material.get(), connection.GeometryDrawArgs);
		}
	}
}


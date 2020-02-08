#pragma once

#include "GeometryBase.h"

class ZN_API ModelBase 
	: public IModel
	, public Object
{
public:
	ModelBase();
	virtual ~ModelBase();

	virtual void                                    SetName(const std::string& Name) override;
	virtual std::string                             GetName() const override;

	virtual void                                    SetBounds(const BoundingBox& Bounds) override;
	virtual BoundingBox                             GetBounds() const override;

	virtual void									AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	const std::vector<SConnection>&                 GetConnections() const override;

	virtual void                                    Accept(IVisitor* visitor) override;

protected:
	std::string                                     m_Name;

	std::vector<SConnection>				        m_Connections;
};

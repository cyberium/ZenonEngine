#pragma once

#include "GeometryBase.h"

class ZN_API ModelBase 
	: public IModel
	, public Object
{
public:
	ModelBase(IRenderDevice& RenderDevice);
	virtual ~ModelBase();

	void                                            SetFileName(const std::string& FileName) override;
	std::string                                     GetFileName() const override;
	void                                            SetBounds(const BoundingBox& Bounds) override;
	BoundingBox                                     GetBounds() const override;
	void									        AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	const std::vector<SConnection>&                 GetConnections() const override;

	virtual void                                    Accept(IVisitor* visitor) override;

protected:
	void                                            UpdateBounds(const std::shared_ptr<IGeometry>& Geometry);

protected:
	BoundingBox m_BoundingBox;
	std::vector<SConnection> m_Connections;
	std::string m_FileName;

private: // Link to parent d3d11 device
	IRenderDevice& m_RenderDevice;
};

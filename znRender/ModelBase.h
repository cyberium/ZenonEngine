#pragma once

#include "GeometryBase.h"

class ZN_API ModelBase 
	: public IModel
	, public IObjectLoadSave
{
public:
	ModelBase(IRenderDevice& RenderDevice);
	virtual ~ModelBase();

	virtual void                                    SetBounds(const BoundingBox& Bounds) override;
	virtual BoundingBox                             GetBounds() const override;

	virtual void									AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	const std::vector<SConnection>&                 GetConnections() const override;

	virtual void                                    Accept(IVisitor* visitor) override;

	// IObjectLoadSave
	void											Load(const std::shared_ptr<IByteBuffer>& ByteBuffer);
	void											Save(const std::shared_ptr<IByteBuffer>& ByteBuffer);

protected:
	std::vector<SConnection>				        m_Connections;

private: // Link to parent d3d11 device
	IRenderDevice& m_RenderDevice;
};

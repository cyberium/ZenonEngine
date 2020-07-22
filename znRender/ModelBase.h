#pragma once

#include "GeometryBase.h"

class ZN_API ModelBase 
	: public IModel
	, public Object
{
public:
	ModelBase(IRenderDevice& RenderDevice);
	virtual ~ModelBase();

	void                                            SetBounds(const BoundingBox& Bounds) override;
	BoundingBox                                     GetBounds() const override;
	void									        AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	const std::vector<SConnection>&                 GetConnections() const override;
	void											SetFileName(const std::string& FileName) override;
	std::string										GetFileName() const override;

	virtual void                                    Accept(IVisitor* visitor) override;

	// IObject
	Guid                                            GetGUID() const override final { return Object::GetGUID(); };
	std::string                                     GetName() const override final { return Object::GetName(); };
	void                                            SetName(const std::string& Name) override final { Object::SetName(Name); };
	std::string                                     GetTypeName() const override final { return Object::GetTypeName(); };
	std::string                                     GetClassNameW() const override final { return Object::GetClassNameW(); };

	// IObjectLoadSave
	void											Load(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;
	void											Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const override;
	void											Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void											Save(const std::shared_ptr<IXMLWriter>& Writer) const override;
protected:
	BoundingBox                                     m_BoundingBox;
	std::vector<SConnection>				        m_Connections;
	std::string                                     m_FileName;

private: // Link to parent d3d11 device
	IRenderDevice& m_RenderDevice;
};

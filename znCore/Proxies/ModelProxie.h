#pragma once

#include "ObjectsFactories/Object.h"

class ZN_API ModelProxie 
	: public IModel
	, public Object
{
public:
	ModelProxie(const std::shared_ptr<IModel>& Model);
	virtual ~ModelProxie();

	// IModel
	virtual void SetBounds(const BoundingBox& Bounds) override;
	virtual BoundingBox GetBounds() const override;
	void SetFileName(const std::string& FileName) override;
	std::string GetFileName() const override;
	void AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override final;
	const std::vector<SConnection>& GetConnections() const override final;

	virtual bool Render(const RenderEventArgs& renderEventArgs) const override;
	virtual void Accept(IVisitor* visitor) override;

	// IObject
	Guid GetGUID() const override final { return Object::GetGUID(); };
	std::string GetName() const override final { return Object::GetName(); };
	void SetName(const std::string& Name) override final { Object::SetName(Name); };

	// IObjectLoadSave
	void Load(const std::shared_ptr<IByteBuffer>& Buffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& Buffer) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

protected:
	const std::shared_ptr<IModel> GetModel() const;

private:
	std::shared_ptr<IModel> m_Model;
};
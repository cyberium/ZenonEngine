#pragma once

class ZN_API ModelProxie 
	: public IModel
{
public:
	ModelProxie(std::shared_ptr<IModel> Model);
	virtual ~ModelProxie();

	// IModel
	virtual void SetName(const std::string& Name) override;
	virtual std::string GetName() const override;

	virtual void SetBounds(const BoundingBox& Bounds) override;
	virtual BoundingBox GetBounds() const override;

	virtual void AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

	virtual bool Render(const RenderEventArgs& renderEventArgs) const override;

	virtual void Accept(IVisitor* visitor) override;

private:
	std::shared_ptr<IModel> m_Model;
};
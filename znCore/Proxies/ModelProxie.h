#pragma once

class ZN_API ModelProxie 
	: public IModel
	, public ILoadableFromFile
{
public:
	ModelProxie(const std::shared_ptr<IModel>& Model);
	virtual ~ModelProxie();

	// IModel
	void SetName(const std::string& Name) override final;
	std::string GetName() const override final;

	virtual void SetBounds(const BoundingBox& Bounds) override;
	virtual BoundingBox GetBounds() const override;

	void AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override final;
	const std::vector<SConnection>& GetConnections() const override final;

	virtual bool Render(const RenderEventArgs& renderEventArgs) const override;

	virtual void Accept(IVisitor* visitor) override;

	// ILoadableFromFile
	void Load(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) override;

private:
	std::shared_ptr<IModel> m_Model;
};
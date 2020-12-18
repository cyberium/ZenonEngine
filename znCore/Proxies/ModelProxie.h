#pragma once

#include "../ObjectsFactories/ObjectProxy.h"

class ZN_API ModelProxie 
	: virtual public IModel
	, public CObjectProxy
{
public:
	ModelProxie(const std::shared_ptr<IModel>& Model);
	virtual ~ModelProxie();

	// IModel
	std::string GetFileName() const override;
	BoundingBox GetBounds() const override;
	void AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override final;
	const std::vector<SConnection>& GetConnections() const override final;

	// Skeleton
	const std::shared_ptr<ISkeleton>& GetSkeleton() const override;

	// Animations
	void AddAnimation(const std::string& AnimationName, const std::shared_ptr<IAnimation>& Animation) override;
	const Animations_t& GetAnimations() const override;

	virtual bool Render() const override;
	virtual void Accept(IVisitor* visitor) override;

	// IObjectLoadSave
	void Load(const std::shared_ptr<IByteBuffer>& Buffer) override;
	void Save(const std::shared_ptr<IByteBuffer>& Buffer) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

protected:
	const std::shared_ptr<IModel> GetModel() const;
	const std::shared_ptr<IModelInternal> GetModelInternal() const;

private:
	std::shared_ptr<IModel> m_Model;
};
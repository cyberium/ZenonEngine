#pragma once

class ZN_API CSceneXMLFucntional
	: public IVisitor
{
public:

	// IVisitor
	virtual bool Visit(const ISceneNode3D* node) override;
	virtual bool Visit(const ISceneNodeUI* node) override;
	virtual bool Visit(const IModel* Model) override;
	virtual bool Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	virtual bool Visit(const ISceneNodeComponent* Component) override;
	virtual bool Visit(const ILightComponent3D* light) override;

private:

};
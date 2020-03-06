#pragma once

class ZN_API CSceneXMLFucntional
	: public IVisitor
{
public:

	// IVisitor
	virtual EVisitResult Visit(const ISceneNode3D* node) override;
	virtual EVisitResult Visit(const ISceneNodeUI* node) override;
	virtual EVisitResult Visit(const IModel* Model) override;
	virtual EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	virtual EVisitResult Visit(const ISceneNodeComponent* Component) override;
	virtual EVisitResult Visit(const ILightComponent3D* light) override;

private:

};
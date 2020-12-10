#pragma once

class ZN_API CLightComponent
	: public CComponentBase
	, public ILightComponent3D
{
public:
    CLightComponent(const ISceneNode& OwnerNode);
    virtual ~CLightComponent();

	// ILightComponent3D
	void SetLight(std::shared_ptr<ILight> Light) override;
	std::shared_ptr<ILight> GetLight() const override;

    // ISceneNodeComponent
	void OnMessage(const ISceneNodeComponent* Component, ComponentMessageType Message) override;
    void Accept(IVisitor* visitor) override;

	// IObjectSaveLoad
	void CopyTo(std::shared_ptr<IObject> Destination) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

private:
	std::shared_ptr<ILight> m_Light;
};
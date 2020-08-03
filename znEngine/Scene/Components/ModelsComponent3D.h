#pragma once

class __declspec(UUID_ModelsComponent) ZN_API CModelsComponent3D
	: public IModelsComponent3D
	, public CComponentBase
{
public:
	CModelsComponent3D(const ISceneNode3D& OwnerNode);
    virtual ~CModelsComponent3D();

	void Copy(std::shared_ptr<ISceneNodeComponent> Destination) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

	// IModelsComponent3D
    void SetModel(const std::shared_ptr<IModel>& Model) override;
	void ResetModel() override;
    std::shared_ptr<IModel> GetModel() const override;

    // CComponentBase
    virtual void Accept(IVisitor* visitor) override;

private:
	std::shared_ptr<IModel> m_Model;
};
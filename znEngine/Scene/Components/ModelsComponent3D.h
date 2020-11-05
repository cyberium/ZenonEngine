#pragma once

class /*__declspec(UUID_ModelsComponent)*/ ZN_API CModelsComponent3D
	: public IModelsComponent3D
	, public CComponentBase
{
public:
	CModelsComponent3D(const ISceneNode& OwnerNode);
    virtual ~CModelsComponent3D();

	// IModelsComponent3D
    void SetModel(const std::shared_ptr<IModel>& Model) override;
	void ResetModel() override;
    std::shared_ptr<IModel> GetModel() const override;
	void SetCastShadows(bool Value) override;
	bool IsCastShadows() const  override;

    // ISceneNodeComponent
    virtual void Accept(IVisitor* visitor) override;

	// IObjectLoadSave
	void CopyTo(std::shared_ptr<IObject> Destination) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

private:
	std::shared_ptr<IModel> m_Model;
	bool m_IsCastShadows;
};
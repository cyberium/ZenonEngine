#pragma once

#include "../ComponentBase.h"

class __declspec(UUID_ModelsComponent) ZN_API CModelsComponent3D
	: public IModelsComponent3D
	, public CComponentBase
{
public:
	CModelsComponent3D(const ISceneNode3D& OwnerNode);
    virtual ~CModelsComponent3D();

	void Copy(std::shared_ptr<ISceneNodeComponent> Destination) const override;

	// IModelsComponent3D
    void AddModel(const std::shared_ptr<IModel>& Model) override;
    void RemoveModel(const std::shared_ptr<IModel>& Model) override;
    const ModelsList& GetModels() override;

    // CComponentBase
    virtual void Accept(IVisitor* visitor) override;

private:
    ModelsList m_Models;
};
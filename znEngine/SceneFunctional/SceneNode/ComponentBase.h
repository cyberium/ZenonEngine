#pragma once

// FORWARD BEGIN
class SceneNode3D;
// FORWARD END

class CComponentBase : public ISceneNodeComponent
{
public:
    CComponentBase(std::shared_ptr<SceneNode3D> OwnerNode);
    virtual ~CComponentBase();

    std::shared_ptr<SceneNode3D> GetOwnerNode() const;

    // ISceneNodeComponent
    virtual void OnParentChanged() override;

    virtual bool Accept(IVisitor& visitor) override;

private:
    std::weak_ptr<SceneNode3D> m_OwnerNode;
};
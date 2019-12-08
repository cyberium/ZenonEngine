#pragma once

#include "../TransformComponentBase.h"

class __declspec(UUID_TransformComponent) OW_ENGINE_API CTransformComponentUI
	: public ITransformComponentUI
	, public CTransformComponentBase
{
    typedef CTransformComponentBase base;
public:
	CTransformComponentUI(std::shared_ptr<ISceneNode> OwnerNode);
    virtual ~CTransformComponentUI();

	// ITransformComponentUI
    void            SetTranslate(cvec2 _translate);
    cvec2           GetTranslation() const;
    glm::vec2       GetTranslationAbs() const;
    void            SetRotation(cvec3 _rotate);
    cvec3           GetRotation() const;
    void            SetScale(cvec2 _scale);
    cvec2           GetScale() const;
    glm::vec2       GetScaleAbs() const;

protected:
    virtual void    UpdateLocalTransform() override;
    virtual void    UpdateWorldTransform() override;

private:
    vec2            m_Translate;
    vec3            m_Rotate;
    vec2            m_Scale;
};

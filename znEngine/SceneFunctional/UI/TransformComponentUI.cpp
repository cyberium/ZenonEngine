#include "stdafx.h"

// Include
#include "SceneFunctional/Base/SceneNode.h"

// General
#include "TransformComponentUI.h"

CTransformComponentUI::CTransformComponentUI(std::shared_ptr<SceneNode> OwnerNode)
    : base(OwnerNode)
    , m_Translate(vec2())
    , m_Rotate(vec3())
    , m_Scale(1.0f, 1.0f)
{
}

CTransformComponentUI::~CTransformComponentUI()
{
}


void CTransformComponentUI::SetTranslate(cvec2 _translate)
{
    m_Translate = _translate;

    UpdateLocalTransform();

    RaiseComponentMessage(UUID_TransformComponent_OnTranslateChanged);
}
cvec2 CTransformComponentUI::GetTranslation() const
{
    return m_Translate;
}
glm::vec2 CTransformComponentUI::GetTranslationAbs() const
{
    glm::vec2 parentTranslate = vec2(0.0f, 0.0f);
    if (std::shared_ptr<SceneNode> parent = GetOwnerNode()->GetParent())
        parentTranslate = parent->GetComponent<CTransformComponentUI>()->GetTranslationAbs();
    return parentTranslate + GetTranslation();
}



glm::vec2 CTransformComponentUI::GetTranslationAbs() const
{
    return glm::vec2();
}


void CTransformComponentUI::SetRotation(cvec3 _rotate)
{
    m_Rotate = _rotate;

    UpdateLocalTransform();
}
cvec3 CTransformComponentUI::GetRotation() const
{
    return m_Rotate;
}



void CTransformComponentUI::SetScale(cvec2 _scale)
{
    m_Scale = _scale;

    UpdateLocalTransform();
}
cvec2 CTransformComponentUI::GetScale() const
{
    return m_Scale;
}
glm::vec2 CTransformComponentUI::GetScaleAbs() const
{
    glm::vec2 parentScale = vec2(1.0f, 1.0f);
    if (std::shared_ptr<SceneNode> parent = GetOwnerNode()->GetParent())
        parentScale = parent->GetComponent<CTransformComponentUI>()->GetScaleAbs();
    return parentScale * GetScale();
}

glm::vec2 CTransformComponentUI::GetScaleAbs() const
{
    return glm::vec2();
}

//
// Protected
//

void CTransformComponentUI::UpdateLocalTransform()
{
    m_LocalTransform = glm::mat4(1.0f);

    m_LocalTransform = glm::translate(m_LocalTransform, glm::vec3(m_Translate, 0.0f));
    m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.x, glm::vec3(1, 0, 0));
    m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.y, glm::vec3(0, 1, 0));
    m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.z, glm::vec3(0, 0, 1));
    m_LocalTransform = glm::scale(m_LocalTransform, glm::vec3(m_Scale, 1.0f));
    m_InverseLocalTransform = glm::inverse(m_LocalTransform);

    RaiseComponentMessage(UUID_TransformComponent_OnLocalTransformChanged);

    // Don't forget to update world transform
    UpdateWorldTransform();
}

void CTransformComponentUI::UpdateWorldTransform()
{
    m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
    m_InverseWorldTransform = glm::inverse(m_WorldTransform);

    RaiseComponentMessage(UUID_TransformComponent_OnWorldTransformChanged);
}

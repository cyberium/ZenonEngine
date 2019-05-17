#pragma once

//
// Components engine template access specialization
//
template<>
inline std::shared_ptr<CTransformComponentUI> CUIBaseNode::GetComponent<CTransformComponentUI>()
{
    return m_Components_Transform;
}

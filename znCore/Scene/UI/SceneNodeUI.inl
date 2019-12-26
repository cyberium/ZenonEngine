#pragma once

//
// Components engine template access
//
template<typename T>
inline std::shared_ptr<T> CUIBaseNode::GetComponent()
{
    return ISceneNode::GetComponent<T>();
}



//
// Components engine template access specialization
//


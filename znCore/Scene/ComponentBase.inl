#pragma once

//
// Components engine template access
//
template<class T>
inline std::shared_ptr<T> CComponentBase::IsComponentExists() const
{
    return GetOwnerNode().IsComponentExists(T::GetClassT());
}

template<class T>
inline std::shared_ptr<T> CComponentBase::GetComponent() const
{
    std::shared_ptr<ISceneNodeComponent> component = GetOwnerNode().GetComponent(T::GetClassT());
    if (component == nullptr)
        return std::shared_ptr<T>();

    return std::dynamic_pointer_cast<T, ISceneNodeComponent>(component);
}
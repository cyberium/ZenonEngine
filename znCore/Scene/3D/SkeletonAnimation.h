#pragma once

#include "../ComponentBase.h"

class __declspec(UUID_SkeletonAnimationComponent) ZN_API CSkeletonAnimationComponent
	: public ISkeletonAnimationComponent
	, public CComponentBase
{
public:
	CSkeletonAnimationComponent(const ISceneNode3D& SceneNode);
	virtual ~CSkeletonAnimationComponent();



};
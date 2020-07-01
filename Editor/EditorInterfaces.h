#pragma once

ZN_INTERFACE IEditor3DFrame
{
	virtual ~IEditor3DFrame() {}
};

ZN_INTERFACE IEditorUIFrame
{
	virtual ~IEditorUIFrame() {}

	virtual void OnSceneNode3DSelected(const std::shared_ptr<ISceneNode3D>& SceneNode3D) const = 0;
};

#pragma once

/*

// FORWARD BEGIN
ZN_INTERFACE IScene;
// FORWARD END

ZN_INTERFACE OW_ENGINE_API ISceneCreator
{
	virtual ~ISceneCreator() {}

	virtual bool									IsSupportedSceneFormat(const std::string& FileName) = 0;
	virtual std::shared_ptr<IScene>					CreateScene(const std::string& FileName) = 0;
};


ZN_INTERFACE OW_ENGINE_API IScenesFactory
{
	virtual ~IScenesFactory() {}

	virtual void									RegisterSceneCreator(std::shared_ptr<ISceneCreator> SceneCreator) = 0;
	virtual void									UnregisterSceneCreator(std::shared_ptr<ISceneCreator> SceneCreator) = 0;
	virtual std::shared_ptr<IScene>					CreateScene(const std::string& FileName) = 0;
};

*/

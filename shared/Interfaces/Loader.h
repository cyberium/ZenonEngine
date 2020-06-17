#pragma once

// Forward BEGIN
ZN_INTERFACE IManager;
ZN_INTERFACE ICameraComponent3D;
// Forward END

ZN_INTERFACE ZN_API ILoadable
{
	enum class ZN_API ELoadableState : uint32
	{
		Created = 0,
		Loading,
		Loaded,
		Unloading,
		Unloaded,
		Deleted
	};

	virtual											~ILoadable() {}

	virtual bool									Load() = 0;
	virtual bool									Delete() = 0;

	virtual void                                    SetState(ELoadableState State) = 0;
	virtual ELoadableState                          GetState() const = 0;

	virtual void                                    AddDependense(const std::weak_ptr<ILoadable>& Loadable) = 0;
	virtual std::weak_ptr<ILoadable>                GetDependense() const = 0;
};

ZN_INTERFACE ZN_API
	__declspec(uuid("18A045EF-D291-45E5-8F61-E223C4AFEF83"))
	ILoader : public IManager
{
	virtual ~ILoader() {}

	virtual void Start() = 0;
	virtual void Stop() = 0;

	virtual void SetCamera(std::shared_ptr<ICameraComponent3D> _camera) = 0;

	virtual void AddToLoadQueue(const std::weak_ptr<ILoadable>& LoadableItem) = 0;
	virtual void AddToDeleteQueue(const std::shared_ptr<ILoadable>& LoadableItem) = 0;
};

ZN_INTERFACE ZN_API
	__declspec(uuid("ECC273E7-3F46-4A34-A69B-4CE3D814420C"))
	ILoadableFromFile
{
	virtual ~ILoadableFromFile() {}

	virtual void Load(const std::shared_ptr<IFile>& File) = 0;
	virtual void Save(const std::shared_ptr<IFile>& File) = 0;
};

const uint32 cBufferBindingSignature = 0x00100000;
const uint32 cInputSemanticSignature = 0x00100001;


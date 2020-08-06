#pragma once

class CSceneEngineCreator
	: public CObjectClassCreator
{
public:
	CSceneEngineCreator(IBaseManager& BaseManager);
	virtual ~CSceneEngineCreator();

	// IObjectClassCreator
	virtual std::shared_ptr<IObject> CreateObject(size_t Index, const IObjectCreationArgs* ObjectCreationArgs) override;

};

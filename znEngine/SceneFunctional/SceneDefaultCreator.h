#pragma once

#ifdef 0

class CSceneDefaultCreator
	: public ISceneCreator
{
public:
	CSceneDefaultCreator(IBaseManager* BaseManager);
	virtual ~CSceneDefaultCreator();

	// ISceneCreator
	size_t GetScenesCount() const override;
	std::string GetSceneTypeName(size_t Index) const override;
	std::shared_ptr<IScene> CreateScene(size_t Index) const override;

private:
	IBaseManager* m_BaseManager;
};

#endif

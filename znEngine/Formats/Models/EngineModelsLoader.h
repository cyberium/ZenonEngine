#pragma once

class CznEngineModelsLoader
	: public IznModelsLoader
{
public:
	CznEngineModelsLoader(IBaseManager& BaseManager);
	virtual ~CznEngineModelsLoader();

	// IznModelsLoader
	std::string GetName() const override;
	bool IsSupportedFormat(const std::shared_ptr<IFile>& ModelFile) const override;
	std::shared_ptr<IModel> LoadModel(const std::shared_ptr<IFile>& ModelFile, const std::string& TexturesPath = "") const override;

private:
	IBaseManager& m_BaseManager;
};
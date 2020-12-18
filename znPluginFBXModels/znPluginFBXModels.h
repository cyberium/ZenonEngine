#pragma once

class CznPluginFBXModels
	: public IznPlugin
{
public:
	CznPluginFBXModels(IBaseManager& BaseManager);
	virtual ~CznPluginFBXModels();

	// IznPlugin
	bool Initialize() override;
	void Finalize() override;
	std::string GetName() const override;
	std::string GetDescription() const override;

private:
	std::shared_ptr<IznModelsLoader> m_FBXModelsLoader;

private:
	IBaseManager& m_BaseManager;
};

extern "C" __declspec(dllexport) IznPlugin* __stdcall GetPlugin(IBaseManager* BaseManager);
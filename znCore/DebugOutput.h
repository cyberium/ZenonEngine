#pragma once

class OW_ENGINE_API CDebugOutput : public IDebugOutput
{
public:
	virtual ~CDebugOutput()	{}

	// IDebugOutput
	void PushMessage(const std::string& _message, IDebugOutput::DebugMessageType _type) override;
	void PushMessage(IDebugOutput::DebugMessageType _type, const char* _message, va_list& _vaList) override;

protected:
	virtual void Print(std::string _messageFmt, CDebugOutput::DebugMessageType _type) = 0;
};

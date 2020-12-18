#pragma once

namespace StaticAssert
{
	template<bool>
	struct StaticAssertFailed;

	template<>
	struct StaticAssertFailed<true> {};
}
#define assert_static(exp) (StaticAssert::StaticAssertFailed<(exp) != 0>())


class ZN_API CException 
{
public:
	CException(const char* Message, ...);
	CException(std::string Message);

	const std::string Message() const { return m_Message; }
	const char* MessageCStr() const { return m_Message.c_str(); }

protected:
	CException() 
		: m_Message("")
	{}

protected:
	std::string m_Message;
};

class ZN_API CznRenderException
	: public CException
{
public:
	CznRenderException(const char* Message, ...);
	CznRenderException(std::string Message);
};


class ZN_API CPluginException
	: public CException
{
public:
	CPluginException(const std::string& Message)
		: CException(Message)
	{}
	virtual ~CPluginException()
	{}
};


class ZN_API CPluginException_NotAPlguin
	: public CPluginException
{
public:
	CPluginException_NotAPlguin(const std::string& Message)
		: CPluginException(Message)
	{}
	virtual ~CPluginException_NotAPlguin()
	{}
};

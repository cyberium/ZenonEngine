#pragma once

#include <map>
#include <mutex>

template <typename T, typename P>
class MapAssync
{
public:
	void add(T element, P object)
	{
		std::lock_guard<std::mutex> lock(m_Lock);

		m_Map.insert(make_pair(element, object));
	}

	bool exists(T _index)
	{
		std::lock_guard<std::mutex> lock(m_Lock);

		return m_Map.find(_index) != m_Map.end();
	}

	uint32 size()
	{
		std::lock_guard<std::mutex> lock(m_Lock);

		return static_cast<uint32>(m_Map.size());
	}

	bool pop_front(T* _index, P* _element)
	{
		std::lock_guard<std::mutex> lock(m_Lock);

		auto it = m_Map.begin();
		bool result = (it != m_Map.end());

		if (result)
		{
			*_index = it->first;
			*_element = it->second;

			m_Map.erase(it);
		}

		return result;
	}

private:
	std::mutex m_Lock;
	std::map<T, P> m_Map;
};
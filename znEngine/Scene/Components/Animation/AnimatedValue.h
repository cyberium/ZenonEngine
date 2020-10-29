#pragma once

enum class Interpolations : uint16_t
{
	INTERPOLATION_NONE = 0,
	INTERPOLATION_LINEAR,
	INTERPOLATION_HERMITE
};

template<typename T>
struct AnimatedArray
{
	std::vector<T> array;
};

template<typename T>
struct AnimatedTrack
{
	Interpolations interpolation_type;
	AnimatedArray<AnimatedArray<uint32_t>> timestamps;
	AnimatedArray<AnimatedArray<T>> values;
};



//
// interpolation functions
//
template<class T>
inline T interpolateNone(const float r, const T& v1, const T& v2)
{
	return r > 0.5f ? v2 : v1;
}

template<class T>
inline T interpolateLinear(const float r, const T& v1, const T& v2)
{
	return v1 * (1.0f - r) + v2 * r;
}

// "linear" interpolation for quaternions should be slerp by default
template<>
inline glm::quat interpolateLinear<glm::quat>(const float r, const glm::quat& v1, const glm::quat& v2)
{
	return glm::slerp(v1, v2, r);
}



template<class T>
inline T interpolateHermite(const float r, const T& v1, const T& v2, const T& in, const T& out)
{
	const float rPow3 = glm::pow(r, 3);
	const float rPow2 = glm::pow(r, 2);

	// basis functions
	const float h1 = 2.0f * rPow3 - 3.0f * rPow2 + 1.0f;
	const float h2 = -2.0f * rPow3 + 3.0f * rPow2;
	const float h3 = rPow3 - 2.0f * rPow2 + r;
	const float h4 = rPow3 - rPow2;

	// interpolation
	return v1 * h1 + v2 * h2 + in * h3 + out * h4;
}

template<class T>
inline T interpolateBezier(const float r, const T& v1, const T& v2, const T& in, const T& out)
{
	const float InverseFactor = (1.0f - r);
	const float FactorTimesTwo = r * r;
	const float InverseFactorTimesTwo = InverseFactor * InverseFactor;

	// basis functions
	const float h1 = InverseFactorTimesTwo * InverseFactor;
	const float h2 = 3.0f * r * InverseFactorTimesTwo;
	const float h3 = 3.0f * FactorTimesTwo * InverseFactor;
	const float h4 = FactorTimesTwo * r;

	// interpolation
	return static_cast<T>((v1 * h1) + (v2 * h2) + (in * h3) + (out * h4));
}



/*
	Generic animated value class:

	T is the values type to animate
	D is the values type stored in the file (by default this is the same as T)
*/

template <class T, class D = T>
class CznAnimatedValue
{
public:
	CznAnimatedValue()
		: m_Type(Interpolations::INTERPOLATION_LINEAR)
	{}

	inline void Initialize(const AnimatedTrack<D>& b)
	{
		m_Type = b.interpolation_type;

		_ASSERT(b.timestamps.size == b.values.size);

		// Prepare data
		m_Times.resize(b.timestamps.size);
		m_Values.resize(b.values.size);
		if (m_Type == Interpolations::INTERPOLATION_HERMITE)
		{
			m_ValuesHermiteIn.resize(b.values.size);
			m_ValuesHermiteOut.resize(b.values.size);
		}

		// times
		const AnimatedArray<uint32>* pHeadTimes = (AnimatedArray<uint32>*)(b.timestamps.array.data());
		const AnimatedArray<D>* pHeadValues = (AnimatedArray<D>*)(b.values.array.data());
		for (uint32 j = 0; j < GetCount(); j++)
		{
			uint32* times = (uint32*)(File->getData() + pHeadTimes[j].array.data());
			D* values = (D*)(File->getData() + pHeadValues[j].array.data());

			_ASSERT(times != nullptr);
			m_Times[j].assign(times, times + pHeadTimes[j].size);

			_ASSERT(values != nullptr);



			for (uint32 i = 0; i < pHeadValues[j].size; i++)
			{
				switch (m_Type)
				{
				case Interpolations::INTERPOLATION_NONE:
				case Interpolations::INTERPOLATION_LINEAR:
					m_Values[j].push_back(values[i]);
					break;

				case Interpolations::INTERPOLATION_HERMITE:
					m_Values[j].push_back(values[i * 3 + 0]);
					m_ValuesHermiteIn[j].push_back(values[i * 3 + 1]);
					m_ValuesHermiteOut[j].push_back(values[i * 3 + 2]);
					break;

				default:
					_ASSERT_EXPR(false, "M2_Animated: Unknown interpolation type.");
				}
			}
		}
	}

	inline bool IsUsesBySequence(uint16 SequenceIndex) const
	{
		if (IsStaticValue())
			return true;

		if (SequenceIndex < GetCount())
		{
			if (m_Values[SequenceIndex].empty())
				return false;

			return true;
		}

		return false;
	}

	inline T GetValue(uint16 SequenceIndex, uint32 time) const
	{
		_ASSERT(IsUsesBySequence(SequenceIndex));

		if (IsStaticValue())
		{
			return m_Values.at(0).at(0);
		}

		_ASSERT(SequenceIndex < GetCount());

		const std::vector<uint32>& pTimes = m_Times[SequenceIndex];
		const std::vector<T>& pData = m_Values[SequenceIndex];

		if (SequenceIndex < GetCount() && pData.size() > 1 && pTimes.size() > 1)
		{
			int max_time = pTimes.at(pTimes.size() - 1);
			if (max_time > 0)
				time %= max_time; // I think this might not be necessary?

			uint32 pos = 0;
			for (uint32 i = 0; i < pTimes.size() - 1; i++)
			{
				if (time >= pTimes.at(i) && time < pTimes.at(i + 1))
				{
					pos = i;
					break;
				}
			}

			uint32 t1 = pTimes.at(pos);
			uint32 t2 = pTimes.at(pos + 1);
			float r = static_cast<float>(time - t1) / static_cast<float>(t2 - t1);
			switch (m_Type)
			{
			case Interpolations::INTERPOLATION_NONE:
				return interpolateNone(r, pData.at(pos), pData.at(pos + 1));
			case Interpolations::INTERPOLATION_LINEAR:
				return interpolateNone<T>(r, pData.at(pos), pData.at(pos + 1));
			case Interpolations::INTERPOLATION_HERMITE:
				return interpolateHermite<T>(r, pData.at(pos), pData.at(pos + 1), m_ValuesHermiteIn[SequenceIndex].at(pos), m_ValuesHermiteOut[SequenceIndex].at(pos));
			default:
				_ASSERT_EXPR(false, "M2_Animated: Unknown interpolation type.");
			}
		}
		else if (!pData.empty())
		{
			return pData.at(0);
		}

		//_ASSERT(false);
		return T();
	}

	void MergeWithOther(const CznAnimatedValue<T>& other)
	{
		m_Type = other.m_Type;

		for (const auto& t : other.m_Times)
			m_Times.push_back(t);

		for (const auto& v : other.m_Values)
			m_Values.push_back(v);

		for (const auto& v : other.m_ValuesHermiteIn)
			m_ValuesHermiteIn.push_back(v);

		for (const auto& v : other.m_ValuesHermiteOut)
			m_ValuesHermiteOut.push_back(v);
	}

private:
	inline size_t GetCount() const
	{
		return m_Times.size();
	}

	inline bool IsStaticValue() const
	{
		if (m_Type == Interpolations::INTERPOLATION_NONE && GetCount() == 1 && m_Times.at(0).size() == 1 && m_Values.at(0).size() == 1)
		{
			return true;
		}

		return false;
	}

public:
	Interpolations                   m_Type;
	std::vector<std::vector<uint32>> m_Times;
	std::vector<std::vector<T>>      m_Values;
	std::vector<std::vector<T>>      m_ValuesHermiteIn;
	std::vector<std::vector<T>>      m_ValuesHermiteOut;
};


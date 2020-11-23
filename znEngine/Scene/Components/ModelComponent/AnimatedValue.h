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
		: m_Type(Interpolations::INTERPOLATION_NONE)
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
					throw CException("Unknown interpolation type.");
				}
			}
		}
	}

	virtual bool IsUsesBySequence(uint16 SequenceIndex) const
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

	virtual T GetValue(uint16 SequenceIndex, uint32 time) const
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
				return interpolateLinear<T>(r, pData.at(pos), pData.at(pos + 1));
			case Interpolations::INTERPOLATION_HERMITE:
				return interpolateHermite<T>(r, pData.at(pos), pData.at(pos + 1), m_ValuesHermiteIn[SequenceIndex].at(pos), m_ValuesHermiteOut[SequenceIndex].at(pos));
			default:
				_ASSERT_EXPR(false, "M2_Animated: Unknown interpolation type.");
			}
		}
		else if (false == pData.empty())
		{
			return pData.at(0);
		}

		//_ASSERT(false);
		return T();
	}

	void MergeWithOther(const CznAnimatedValue<T>& other)
	{
		m_Type = other.m_Type;

		if (other.m_Times.size() == 1)
			m_Times.push_back(other.m_Times[0]);

		if (other.m_Values.size() == 1)
			m_Values.push_back(other.m_Values[0]);
		//m_ValuesHermiteIn = other.m_ValuesHermiteIn;
		//m_ValuesHermiteOut = other.m_ValuesHermiteOut;
	}

	void Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
	{
		ByteBuffer->read(&m_Type);

		// Times
		{
			size_t timesCnt;
			ByteBuffer->read(&timesCnt);
			m_Times.resize(timesCnt);
			for (size_t i = 0; i < timesCnt; i++)
			{
				size_t timesSequenseCnt;
				ByteBuffer->read(&timesSequenseCnt);
				m_Times[i].resize(timesSequenseCnt);
				for (size_t s = 0; s < timesSequenseCnt; s++)
				{
					ByteBuffer->read(&m_Times[i][s]);
				}
			}
		}

		// Values
		{
			size_t valuesCnt;
			ByteBuffer->read(&valuesCnt);
			m_Values.resize(valuesCnt);
			for (size_t i = 0; i < valuesCnt; i++)
			{
				size_t valuesSequenseCnt;
				ByteBuffer->read(&valuesSequenseCnt);
				m_Values[i].resize(valuesSequenseCnt);
				for (size_t s = 0; s < valuesSequenseCnt; s++)
				{
					ByteBuffer->read(&m_Values[i][s]);
				}
			}
		}
	}

	void Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const
	{
		ByteBuffer->write(&m_Type);

		// Times
		{
			size_t timesCnt = m_Times.size();
			ByteBuffer->write(&timesCnt);
			for (size_t i = 0; i < timesCnt; i++)
			{
				size_t timesSequenseCnt = m_Times[i].size();
				ByteBuffer->write(&timesSequenseCnt);
				for (size_t s = 0; s < timesSequenseCnt; s++)
				{
					ByteBuffer->write(&m_Times[i][s]);
				}
			}
		}

		// Values
		{
			size_t valuesCnt = m_Values.size();
			ByteBuffer->write(&valuesCnt);
			for (size_t i = 0; i < valuesCnt; i++)
			{
				size_t valuesSequenseCnt = m_Values[i].size();
				ByteBuffer->write(&valuesSequenseCnt);
				for (size_t s = 0; s < valuesSequenseCnt; s++)
				{
					ByteBuffer->write(&m_Values[i][s]);
				}
			}
		}
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


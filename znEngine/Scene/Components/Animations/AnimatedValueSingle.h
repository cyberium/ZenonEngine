#pragma once

enum class Interpolations : uint16_t
{
	None = 0,
	Linear,
	Hermite
};

template<typename T>
struct AnimatedTrack
{
	Interpolations interpolation_type;
	std::vector<uint32> timestamps;
	std::vector<T> values;
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
		: m_InterpolationType(Interpolations::None)
	{}

	/*inline void Initialize(const AnimatedTrack<D>& Track)
	{
		m_InterpolationType = Track.interpolation_type;

		if (Track.timestamps.size() != Track.values.size())
			throw CException("Animation track values mismatch.");

		// Prepare data
		m_Times.resize(Track.timestamps.size());
		m_Values.resize(Track.values.size());
		if (m_InterpolationType == Interpolations::Hermite)
		{
			m_ValuesHermiteIn.resize(Track.values.size());
			m_ValuesHermiteOut.resize(Track.values.size());
		}

		// times
		const uint32* pHeadTimes = (std::vector<uint32>*)(Track.timestamps.data());
		const D* pHeadValues = (std::vector<D>*)(Track.values.data());
		for (uint32 j = 0; j < GetCount(); j++)
		{
			uint32* times = (uint32*)(File->getData() + pHeadTimes[j].array.data());
			D* values = (D*)(File->getData() + pHeadValues[j].array.data());

			_ASSERT(times != nullptr);
			m_Times.assign(times, times + pHeadTimes[j].size);

			_ASSERT(values != nullptr);

			for (uint32 i = 0; i < pHeadValues[j].size; i++)
			{
				switch (m_InterpolationType)
				{
				case Interpolations::None:
				case Interpolations::Linear:
					m_Values.push_back(values[i]);
					break;

				case Interpolations::Hermite:
					m_Values.push_back(values[i * 3 + 0]);
					m_ValuesHermiteIn.push_back(values[i * 3 + 1]);
					m_ValuesHermiteOut.push_back(values[i * 3 + 2]);
					break;

				default:
					throw CException("Unknown interpolation type.");
				}
			}
		}
	}*/

	inline bool IsAnimated(uint32 FrameIndex) const
	{
		if (m_Times.empty())
			return false;
		return FrameIndex < m_Times[m_Times.size() - 1];
	}

	inline T GetValue(uint32 FrameIndex) const
	{
		//return m_Values.at(0);


		if (IsStaticValue())
			return m_Values.at(0);

		if (m_Values.size() > 1 && m_Times.size() > 1)
		{
			int maxTime = m_Times.at(m_Times.size() - 1);
			if (maxTime > 0)
				FrameIndex %= maxTime; // I think this might not be necessary?

			uint32 posInTimes = 0;
			for (uint32 i = 0; i < m_Times.size() - 1; i++)
			{
				if (FrameIndex >= m_Times.at(i) && FrameIndex < m_Times.at(i + 1))
				{
					posInTimes = i;
					break;
				}
			}

			uint32 t1 = m_Times.at(posInTimes);
			uint32 t2 = m_Times.at(posInTimes + 1);
			float r = static_cast<float>(FrameIndex - t1) / static_cast<float>(t2 - t1);

			switch (m_InterpolationType)
			{
				case Interpolations::None:
					return interpolateNone(r, m_Values.at(posInTimes), m_Values.at(posInTimes + 1));

				case Interpolations::Linear:
					return interpolateLinear<T>(r, m_Values.at(posInTimes), m_Values.at(posInTimes + 1));

				case Interpolations::Hermite:
					return interpolateHermite<T>(r, m_Values.at(posInTimes), m_Values.at(posInTimes + 1), m_ValuesHermiteIn.at(posInTimes), m_ValuesHermiteOut.at(posInTimes));

				default:
					throw CException("Unknown interpolation type.");
			}
		}
		else if (m_Values.size() == 1)
		{
			return m_Values.at(0);
		}

		throw CException("Unexpected behaviour.");
	}

	void Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
	{
		ByteBuffer->read(&m_InterpolationType);

		// Times
		{
			size_t timesSequenseCnt;
			ByteBuffer->read(&timesSequenseCnt);
			m_Times.resize(timesSequenseCnt);
			for (size_t s = 0; s < timesSequenseCnt; s++)
				ByteBuffer->read(&m_Times[s]);
		}

		// Values
		{
			size_t valuesSequenseCnt;
			ByteBuffer->read(&valuesSequenseCnt);
			m_Values.resize(valuesSequenseCnt);
			for (size_t s = 0; s < valuesSequenseCnt; s++)
				ByteBuffer->read(&m_Values[s]);
		}
	}

	void Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const
	{
		ByteBuffer->write(&m_InterpolationType);

		// Times
		{
			size_t timesSequenseCnt = m_Times.size();
			ByteBuffer->write(&timesSequenseCnt);
			for (size_t s = 0; s < timesSequenseCnt; s++)
				ByteBuffer->write(&m_Times[s]);
		}

		// Values
		{
			size_t valuesSequenseCnt = m_Values.size();
			ByteBuffer->write(&valuesSequenseCnt);
			for (size_t s = 0; s < valuesSequenseCnt; s++)
				ByteBuffer->write(&m_Values[s]);
		}
	}

private:
	inline bool IsStaticValue() const
	{
		if (m_InterpolationType == Interpolations::None && m_Times.size() == 1 && m_Values.size() == 1)
			return true;
		return false;
	}

public:
	Interpolations      m_InterpolationType;
	std::vector<uint32> m_Times;
	std::vector<T>      m_Values;
	std::vector<T>      m_ValuesHermiteIn;
	std::vector<T>      m_ValuesHermiteOut;
};


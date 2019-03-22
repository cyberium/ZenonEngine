#pragma once

class 
	__declspec(uuid("A1554C89-7BBC-4088-994F-7BC5162333A4"))
	CGroupVideo : public ISettingGroup
{
public:
	template<typename T>
	inline void SetWindowSize(T _x, T _y)
	{
		windowSizeX = static_cast<float>(_x);
		windowSizeY = static_cast<float>(_y);
		CalculateAspectFactor();
	}

	inline void CalculateAspectFactor()
	{
		aspectFactor = 1.0f;
		aspectRatio = windowSizeX / windowSizeY;
		if (aspectRatio >= 1.0f)
		{
			aspectFactor = aspectRatio;
		}
	}

	inline vec2 GetWindowSize() const { return vec2(windowSizeX, windowSizeY); }

	// Render settings
	float windowSizeX;
	float windowSizeY;
	float aspectRatio;
	float aspectFactor;

	// ISettingGroup
	void InitDefault() override;
};
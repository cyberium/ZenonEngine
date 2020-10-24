
float ShadowBlur(Texture2D Texture, sampler Sampler, float2 Coords, int Steps, int textureSize)
{
	const float2 shadowBlurStep = float2(1.0f, 1.0f) / textureSize;
	const int cBlurFrom = Steps;
	const float cBlurFromPow2 = ((cBlurFrom * 2) + 1) * ((cBlurFrom * 2) + 1);

	float sum = 0.0f;
	for (int i = -cBlurFrom; i <= cBlurFrom; i++)
		for (int j = -cBlurFrom; j <= cBlurFrom; j++)
			sum += Texture.Sample(Sampler, Coords + shadowBlurStep * float2(i, j)).r;
	return sum / cBlurFromPow2;
}


float IsShadowed(float4x4 LightProjectionMatrix, float4x4 LightViewMatrix, Texture2D TextureShadow, float4 PModel)
{
	const float4x4 mvpl = mul(LightProjectionMatrix, LightViewMatrix);
	const float4 lightViewPosition = mul(mvpl, PModel);

	const float2 projectTexCoord = float2(
		(lightViewPosition.x / lightViewPosition.w) * 0.5f + 0.5f,
		-(lightViewPosition.y / lightViewPosition.w) * 0.5f + 0.5f
		);
	const float lightDepthValue = (lightViewPosition.z / lightViewPosition.w);

	const int textureSize = 4096.0f;
	const float2 shadowBlurStep = float2(1.0f, 1.0f) / textureSize;
	const int cBlurFrom = 1;
	const float cBlurFromPow2 = ((cBlurFrom * 2) + 1) * ((cBlurFrom * 2) + 1);

	float sum = 0.0f;
	for (int i = -cBlurFrom; i <= cBlurFrom; i++)
	{
		for (int j = -cBlurFrom; j <= cBlurFrom; j++)
		{
			const float2 offset = shadowBlurStep * float2(i, j);
			const float2 textureCoord = projectTexCoord + offset;

			if (saturate(textureCoord.x) == textureCoord.x && saturate(textureCoord.y) == textureCoord.y)
			{
				float storedDepthValue = ShadowBlur(TextureShadow, LinearClampSampler, textureCoord, 0, textureSize);
				//float storedDepthValue = TextureShadow.Sample(LinearClampSampler, textureCoord).r;
				if (lightDepthValue > storedDepthValue)
					sum += (1.0f / float(cBlurFromPow2));
			}
		}
	}

	return sum;
}


float IsShadowedGauss(float4x4 LightProjectionMatrix, float4x4 LightViewMatrix, Texture2D TextureShadow, float4 PModel)
{
	const float4x4 mvpl = mul(LightProjectionMatrix, LightViewMatrix);
	const float4 lightViewPosition = mul(mvpl, PModel);

	const float2 projectTexCoord = float2(
		(lightViewPosition.x / lightViewPosition.w) * 0.5f + 0.5f,
		-(lightViewPosition.y / lightViewPosition.w) * 0.5f + 0.5f
		);
	const float lightDepthValue = (lightViewPosition.z / lightViewPosition.w);

	const int textureSize = 2048.0f;
	const float2 shadowBlurStep = float2(1.0f, 1.0f) / textureSize;
	const int cBlurFrom = 4;
	const float cBlurFromPow2 = ((cBlurFrom * 2) + 1) * ((cBlurFrom * 2) + 1);

	const float PI = 3.14159265f;
	float sigma = 3.3f;
	float3 incrementalGaussian;
	incrementalGaussian.x = 1.0f / (sqrt(2.0f * PI) * sigma);
	incrementalGaussian.y = exp(-0.5f / (sigma * sigma));
	incrementalGaussian.z = incrementalGaussian.y * incrementalGaussian.y;

	float avgValue = 0.0f;
	float coefficientSum = 0.0f;

	const float2 ts00C = projectTexCoord;
	if (saturate(ts00C.x) == ts00C.x && saturate(ts00C.y) == ts00C.y)
	{
		float ts00 = TextureShadow.Sample(LinearClampSampler, ts00C).r;
		if (lightDepthValue > ts00)
		{
			avgValue += incrementalGaussian.x;
		}
	}
	coefficientSum += incrementalGaussian.x;
	incrementalGaussian.xy *= incrementalGaussian.yz;


	// Go through the remaining 8 vertical samples (4 on each side of the center)
	for (int i = -cBlurFrom; i <= cBlurFrom; i++)
	{
		for (int j = -cBlurFrom; j <= cBlurFrom; j++)
		{
			const float2 ts0C = projectTexCoord + float2(i, j) * shadowBlurStep;
			if (saturate(ts0C.x) == ts0C.x && saturate(ts0C.y) == ts0C.y)
			{
				float ts0 = TextureShadow.Sample(LinearClampSampler, ts0C).r;
				if (lightDepthValue >= ts0)
				{
					avgValue += incrementalGaussian.x;

					coefficientSum += incrementalGaussian.x;
					incrementalGaussian.xy *= incrementalGaussian.yz;
				}
				else
				{
					coefficientSum += incrementalGaussian.x;
					//incrementalGaussian.xy *= incrementalGaussian.yz;
				}
			}
			else
			{
				coefficientSum += incrementalGaussian.x;
				//incrementalGaussian.xy *= incrementalGaussian.yz;
			}
		}
	}

	return avgValue / coefficientSum;
}


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



	float w;
	float h;
	TextureShadow.GetDimensions(w, h);

	const int textureSize = w; //4096.0f;
	const float2 shadowBlurStep = float2(1.0f, 1.0f) / w;
	const int cBlurFrom = 0;
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

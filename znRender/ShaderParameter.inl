template<>
void ShaderParameter::Set<ConstantBuffer>(const ConstantBuffer* value);

template<>
void ShaderParameter::Set<Texture>(const Texture* value);

template<>
void ShaderParameter::Set<SamplerState>(const SamplerState* value);

template<>
void ShaderParameter::Set<StructuredBuffer>(const StructuredBuffer* value);

template<typename T>
void ShaderParameter::Set(const T* value)
{
	static_assert(false, "This function must be specialized.");
}
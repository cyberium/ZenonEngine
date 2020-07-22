#pragma once

#include "MaterialProxie.h"

template <typename T>
class MaterialProxieT
	: public MaterialProxie
{
public:
	MaterialProxieT(IRenderDevice& RenderDevice)
		: MaterialProxie(RenderDevice.GetObjectsFactory().CreateMaterial())
	{
		InitializeMaterialData(sizeof(T));
		MaterialData() = T();
	}
	virtual ~MaterialProxieT()
	{}

protected:
	T& MaterialData()
	{
		void* materialData = GetMaterialDataEx();
		_ASSERT(materialData != nullptr);
		return *reinterpret_cast<T*>(materialData);
	}

	const T& MaterialDataReadOnly() const
	{
		const void* materialData = GetMaterialData();
		_ASSERT(materialData != nullptr);
		return *reinterpret_cast<const T*>(materialData);
	}
};
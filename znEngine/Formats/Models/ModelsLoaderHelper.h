#pragma once

namespace ModelsLoaderHelper
{
	struct SConnectionIndexed
	{
		size_t MaterialIndex;
		size_t GeometryIndex;
		SGeometryDrawArgs GeometryDrawArgs;
	};

	size_t FindOrAddMaterialIndex(std::vector<std::shared_ptr<IMaterial>>& Materials, std::shared_ptr<IMaterial> Material);
	size_t FindOrAddGeometryIndex(std::vector<std::shared_ptr<IGeometry>>& Geometries, std::shared_ptr<IGeometry> Geometry);
	void LoadGeometryDrawArgs(SGeometryDrawArgs& Args, const std::shared_ptr<IXMLReader>& Reader);
	void SaveGeometryDrawArgs(const SGeometryDrawArgs& Args, const std::shared_ptr<IXMLWriter>& Writer);
}
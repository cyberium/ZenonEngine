#include "stdafx.h"

// General
#include "ModelsLoaderHelper.h"

size_t ModelsLoaderHelper::FindOrAddMaterialIndex(std::vector<std::shared_ptr<IMaterial>>& Materials, std::shared_ptr<IMaterial> Material)
{
	const auto& matIter = std::find(Materials.begin(), Materials.end(), Material);
	if (matIter != Materials.end())
		return std::distance(Materials.begin(), matIter);
	Materials.push_back(Material);
	return Materials.size() - 1;
}

size_t ModelsLoaderHelper::FindOrAddGeometryIndex(std::vector<std::shared_ptr<IGeometry>>& Geometries, std::shared_ptr<IGeometry> Geometry)
{
	const auto& geomIter = std::find(Geometries.begin(), Geometries.end(), Geometry);
	if (geomIter != Geometries.end())
		return std::distance(Geometries.begin(), geomIter);
	Geometries.push_back(Geometry);
	return Geometries.size() - 1;
}

void ModelsLoaderHelper::LoadGeometryDrawArgs(SGeometryDrawArgs& Args, const std::shared_ptr<IXMLReader>& Reader)
{
	if (Reader->IsAttributeExists("IndexStartLocation"))
		Args.IndexStartLocation = Reader->GetUIntAttribute("IndexStartLocation");
	if (Reader->IsAttributeExists("IndexCnt"))
		Args.IndexCnt = Reader->GetUIntAttribute("IndexCnt");

	if (Reader->IsAttributeExists("VertexStartLocation"))
		Args.VertexStartLocation = Reader->GetUIntAttribute("VertexStartLocation");
	if (Reader->IsAttributeExists("VertexCnt"))
		Args.VertexCnt = Reader->GetUIntAttribute("VertexCnt");

	if (Reader->IsAttributeExists("InstanceStartIndex"))
		Args.InstanceStartIndex = Reader->GetUIntAttribute("InstanceStartIndex");
	if (Reader->IsAttributeExists("InstanceCnt"))
		Args.InstanceCnt = Reader->GetUIntAttribute("InstanceCnt");
}

void ModelsLoaderHelper::SaveGeometryDrawArgs(const SGeometryDrawArgs& Args, const std::shared_ptr<IXMLWriter>& Writer)
{
	if (Args.IndexStartLocation != 0)
		Writer->SetUIntAttribute(Args.IndexStartLocation, "IndexStartLocation");
	if (Args.IndexCnt != UINT_MAX)
		Writer->SetUIntAttribute(Args.IndexCnt, "IndexCnt");

	if (Args.VertexStartLocation != 0)
		Writer->SetUIntAttribute(Args.VertexStartLocation, "VertexStartLocation");
	if (Args.VertexCnt != UINT_MAX)
		Writer->SetUIntAttribute(Args.VertexCnt, "VertexCnt");

	if (Args.InstanceStartIndex != 0)
		Writer->SetUIntAttribute(Args.InstanceStartIndex, "InstanceStartIndex");
	if (Args.InstanceCnt != UINT_MAX)
		Writer->SetUIntAttribute(Args.InstanceCnt, "InstanceCnt");
}
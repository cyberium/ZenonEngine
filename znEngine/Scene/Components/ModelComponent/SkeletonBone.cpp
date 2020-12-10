#include "stdafx.h"

// General
#include "SkeletonBone.h"

CSkeletonBone::CSkeletonBone(const std::shared_ptr<IXMLReader>& Reader)
{
	Load(Reader);
}

CSkeletonBone::CSkeletonBone(const std::shared_ptr<IByteBuffer>& Buffer)
{
	Load(Buffer);
}

CSkeletonBone::CSkeletonBone(const std::string & Name, int32 ParentIndex)
	: m_Name(Name)
	, m_ParentIndex(ParentIndex)
	, m_PivotMatrix(glm::mat4(1.0f))
	, m_LocalTransform(glm::mat4(1.0f))
	, m_SkinMatrix(glm::mat4(1.0f))
{
}

CSkeletonBone::~CSkeletonBone()
{
}



//
// ISkeletonBone
//-
std::string CSkeletonBone::GetName() const
{
	return m_Name;
}

int32 CSkeletonBone::GetParentIndex() const
{
	return m_ParentIndex;
}

bool CSkeletonBone::IsRootBone() const
{
	return GetParentIndex() == -1;
}

void CSkeletonBone::SetLocalMatrix(const glm::mat4 & Matrix)
{
	m_LocalTransform = Matrix;
}

glm::mat4 CSkeletonBone::GetLocalMatrix() const
{
	return m_LocalTransform;
}

void CSkeletonBone::SetPivotMatrix(const glm::mat4 & Matrix)
{
	m_PivotMatrix = Matrix;
}

glm::mat4 CSkeletonBone::GetPivotMatrix() const
{
	return m_PivotMatrix;
}

void CSkeletonBone::SetSkinMatrix(const glm::mat4 & Matrix)
{
	m_SkinMatrix = Matrix;
}

glm::mat4 CSkeletonBone::GetSkinMatrix() const
{
	return m_SkinMatrix;
}



//
// IObjectLoadSave
//
std::shared_ptr<IObject> CSkeletonBone::Copy() const
{
	throw CException("Not implemented.");
}

void CSkeletonBone::CopyTo(std::shared_ptr<IObject> Destination) const
{
	throw CException("Not implemented.");
}

void CSkeletonBone::Load(const std::shared_ptr<IByteBuffer>& Buffer)
{
	Buffer->readString(&m_Name);
	Buffer->read(&m_ParentIndex);
	Buffer->read(&m_LocalTransform);
	Buffer->read(&m_PivotMatrix);
	Buffer->read(&m_SkinMatrix);
}

void CSkeletonBone::Save(const std::shared_ptr<IByteBuffer>& Buffer) const
{
	Buffer->writeString(m_Name);
	Buffer->write(&m_ParentIndex);
	Buffer->write(&m_LocalTransform);
	Buffer->write(&m_PivotMatrix);
	Buffer->write(&m_SkinMatrix);
}

void CSkeletonBone::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	m_Name = Reader->GetStrAttribute("Name");
	m_ParentIndex = Reader->GetIntAttribute("ParentIndex");
	m_LocalTransform = Utils::StringToMatrix(Reader->GetStrAttribute("LocalTransform"));
	m_PivotMatrix = Utils::StringToMatrix(Reader->GetStrAttribute("PivotMatrix"));
	m_SkinMatrix = Utils::StringToMatrix(Reader->GetStrAttribute("FuckingMatrix"));
}

void CSkeletonBone::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	Writer->SetStrAttribute(m_Name, "Name");
	Writer->SetIntAttribute(m_ParentIndex, "ParentIndex");
	Writer->SetStrAttribute(Utils::MatrixToString(m_LocalTransform), "LocalTransform");
	Writer->SetStrAttribute(Utils::MatrixToString(m_PivotMatrix), "PivotMatrix");
	Writer->SetStrAttribute(Utils::MatrixToString(m_SkinMatrix), "FuckingMatrix");
}

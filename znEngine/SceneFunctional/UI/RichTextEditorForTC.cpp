#include "stdafx.h"

// General
#include "RichTextEditorForTC.h"

namespace
{
	const char* cDefaultText = "<empty>";
	const vec2  cDefaultOffset = vec2(0.0f, 0.0f);

	const vec4  cColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	const vec4  cBackColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	const vec4  cSelectColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	const vec4  cSelectBackColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

CRichTextEditorForTC::CRichTextEditorForTC()
{
}

CRichTextEditorForTC::~CRichTextEditorForTC()
{
}

void CRichTextEditorForTC::Initialize()
{
	m_FontMesh = GetBaseManager()->GetManager<IFontsManager>()->GetMainFont();

	m_FontMaterial = std::make_shared<UI_Font_Material>(GetBaseManager()->GetManager<IRenderDevice>());
	m_FontMaterial->SetWrapper(m_FontMaterial.get());
	m_FontMaterial->SetColor(cColor);
	m_FontMaterial->SetTexture(0, m_FontMesh->GetTexture());

	std::shared_ptr<IFile> fs = GetBaseManager()->GetManager<IFilesManager>()->Open("IDB_SCRIPT");

	while (true)
	{
		std::string line;
		fs->readLine(&line);

		m_Lines.push_back(line);

		if (fs->isEof())
			break;
	}

	Log::Warn("CREATED!!!");
}

glm::vec2 CRichTextEditorForTC::GetTextSize() const
{
	return glm::vec2();
}

glm::vec2 CRichTextEditorForTC::GetSize()
{
	glm::ivec2 size = glm::ivec2(0);
	glm::ivec2 charSizeMono = glm::ivec2(m_FontMesh->GetCharWidth('A'), m_FontMesh->GetHeight());

	for (size_t line = 0; line < m_Lines.size(); line++)
	{
		size_t lineWidth = m_Lines.at(line).length();
		size.x = std::max((size_t)size.x, lineWidth * charSizeMono.x);
		size.y += charSizeMono.y;
	}

	return size;
}

bool CRichTextEditorForTC::AcceptMesh(IVisitor * visitor)
{
	m_FontMesh->SetMaterial(m_FontMaterial);


	vec2 _offset = glm::vec2(0.0f);

	for (size_t line = 0; line < m_Lines.size(); line++)
	{
		const std::string& lineData = m_Lines.at(line);

		_offset.x = 0.0f;
		for (size_t i = 0; i < lineData.length(); i++)
		{
			uint8 ch = lineData.c_str()[i];

			m_FontMaterial->SetSelected(SelectedChar.y == line && SelectedChar.x == i );

			m_FontMaterial->SetOffset(_offset);
			_offset.x += static_cast<float>(m_FontMesh->GetCharWidth(ch)) + 0.01f;

			SGeometryPartParams geometryPartParams;
			geometryPartParams.VertexStartLocation = (ch) * 6;
			geometryPartParams.VertexCnt = 6;
			m_FontMesh->Accept(visitor, geometryPartParams);
		}

		_offset.y += m_FontMesh->GetHeight();
	}

	return true;
}

bool CRichTextEditorForTC::OnKeyPressed(KeyEventArgs & e)
{
	if (e.Key == KeyCode::Left)
		SelectedChar.x -= 1;

	if (e.Key == KeyCode::Right)
		SelectedChar.x += 1;

	if (e.Key == KeyCode::Up)
		SelectedChar.y -= 1;

	if (e.Key == KeyCode::Down)
		SelectedChar.y += 1;

	FixSelectedChar();

	Log::Warn("Char = '%c'", e.Char);

	return true;
}

bool CRichTextEditorForTC::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	glm::ivec2 charSizeMono = glm::ivec2(m_FontMesh->GetCharWidth('A'), m_FontMesh->GetHeight());
	SelectedChar = glm::ivec2(e.X / charSizeMono.x, e.Y / charSizeMono.y);

	FixSelectedChar();

	return true;
}

void CRichTextEditorForTC::FixSelectedChar()
{

	if (SelectedChar.y < 0)
		SelectedChar.y = 0;
	if (SelectedChar.y >= m_Lines.size())
		SelectedChar.y = m_Lines.size() - 1;


	if (SelectedChar.x < 0)
		SelectedChar.x = 0;
	if (SelectedChar.x >= m_Lines[SelectedChar.y].length())
		SelectedChar.x = m_Lines[SelectedChar.y].length() - 1;
}

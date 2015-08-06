#include "Text.h"

Text::Text()
{
}


Text::~Text()
{
}

bool Text::Initialize(Graphics* pGraphics, int height, bool bold,
	bool italics, const char* fontName)
{
	m_pGraphics = pGraphics;
	UINT weight = bold ? FW_BOLD : FW_NORMAL;

	int hr = D3DXCreateFont(
		m_pGraphics->get3Ddevice(),
		height,
		0,
		weight,
		1,
		italics,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		fontName,
		&m_font);

	if (FAILED(hr))
	{
		return false;
	}

	D3DXMatrixTransformation2D(&m_matrix,
		NULL,
		0.0f,
		NULL,
		NULL,
		0.0f,
		NULL);

	return true;
}

int Text::Print(const char* text, int posX, int posY) 
{
	if (m_font == NULL)
	{
		throw GameException(GameExceptionNS::WARNING,
			"Attempted to print uninitialized text");

		return 0;
	}

	m_fontRect.top = posY;
	m_fontRect.left = posX;
	D3DXVECTOR2 rCenter = D3DXVECTOR2((float)posX, (float)posY);

	// Rotate text by orientation using rCenter above as rotation center
	D3DXMatrixTransformation2D(&m_matrix, NULL, 0.0f, NULL, &rCenter,
		m_orientation, NULL);

	// Introduce sprite to the matrix
	m_pGraphics->GetSprite()->SetTransform(&m_matrix);
	return m_font->DrawTextA(m_pGraphics->GetSprite(), text, -1,
		&m_fontRect, DT_LEFT, m_color);
}

int Text::Print(const char* text, RECT &rect, UINT format)
{
	if (m_font == NULL)
	{
		throw GameException(GameExceptionNS::WARNING,
			"Attempted to print uninitialized text");

		return 0;
	}

	// Don't rotate text
	D3DXMatrixTransformation2D(&m_matrix, NULL, 0.0f, NULL, NULL, NULL,
		NULL);

	m_pGraphics->GetSprite()->SetTransform(&m_matrix);
	return m_font->DrawTextA(m_pGraphics->GetSprite(), text, -1, &rect,
		format, m_color);

}
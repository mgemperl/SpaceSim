#pragma once

#include "Graphics.h"

class Text
{

public:

	Text();
	~Text();

	bool Initialize(Graphics* pGraphics, int height, bool bold,
		bool italics, const char* fontName);

	/** Print argued string at argued screen position 
	 *  Returns 0 on failure, height of the text on success **/
	int Print(const char* text, int posX, int posY);

	/** Print argued string in argued region with specified alignment 
	 *  Returns 0 on failure, height of the text on success **/
	int Print(const char* text, RECT &rect, UINT format);

	/** Return orientation of text in radians **/
	double GetOrientation() const;

	/** Set orientation of this text in radians **/
	void SetOrientation(double theta);

	/** Return the font color of this text **/
	COLOR_ARGB GetFontColor() const;

	/** Set the font color of this text **/
	void SetFontColor(COLOR_ARGB color);

private:

	Graphics* m_pGraphics;
	LPD3DXFONT m_font;
	D3DXMATRIX m_matrix;
	RECT m_fontRect;

	double m_orientation;
	COLOR_ARGB m_color;

};


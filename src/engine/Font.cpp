/* Font.cpp - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class models a _fixed_ font. It allows scaling, rotating the text,
 * rotating glyphs independently of the text, kerning, and keeps track of
 * an internal cursor.
 *
 * The font is built out of 96 ASCII characters in a texture atlas. The
 * order in which they appear is meaningful.
 */

#include "engine/Texture.h"
#include "engine/TextureRegion.h"
#include "engine/SpriteBatcher.h"

#include "util/Math.h"

#include "engine/Font.h"

#include <string>
#include <cmath>

namespace Engine {

Font::Font(Texture *texture, int offsetX, int offsetY, int glyphsperrow, int glyphwidth, int glyphheight, double scale) :
		mp_Texture {texture},
		m_Alpha{1.0},
		m_OrigGlyphWidth {glyphwidth},
		m_OrigGlyphHeight {glyphheight},
		m_GlyphWidth {glyphwidth},
		m_GlyphHeight {glyphheight},
		m_Scale {scale},
		m_Cursor{}
{
	int x = offsetX;
	int y = offsetY;
	for (auto &i : m_Glyphs) {
		i = TextureRegion(mp_Texture, x, y, m_OrigGlyphWidth, m_OrigGlyphHeight);
		x += m_OrigGlyphWidth;
		if (x == offsetX + glyphsperrow * m_OrigGlyphWidth) {
			x = offsetX;
			y += m_OrigGlyphHeight;
		}
	}
	// now proceed to scale and set default kerning
	m_GlyphWidth *= m_Scale;
	m_GlyphHeight *= m_Scale;
	m_Kerning = m_GlyphWidth;
}

void Font::Scale(double const scale, bool kerningreset)
{
	m_GlyphWidth = m_OrigGlyphWidth * scale;
	m_GlyphHeight = m_OrigGlyphHeight * scale;
	if (kerningreset)
		m_Kerning = m_GlyphWidth;
	m_Scale = scale;
}

Vector2<double> Font::GlyphBox(double angle)
{
	double rad = Util::to_radians(angle);
	double sin = std::abs(std::sin(rad));
	double cos = std::abs(std::cos(rad));
	return Vector2<double>{m_GlyphWidth * cos +
				m_GlyphHeight * sin,
				m_GlyphWidth * sin +
				m_GlyphHeight * cos};
}

void Font::DrawText(SpriteBatcher& batcher, std::string const& text, int x, int y,
				double angle, double glyphangle, GPoint const* center)
{
	Vector2<double> line;
	double dx = x, dy = y;

	// get the line direction an advance (kerning) as a line vector
	line.Angle(angle, static_cast<double>(m_Kerning));

	for (auto const& c : text) {
		unsigned char ch = c - ' ';
		if (ch < 0 || ch > NumGlyphs - 1)
			continue;

		batcher.DrawSprite(static_cast<int>(dx), static_cast<int>(dy),
				m_GlyphWidth, m_GlyphHeight, m_Glyphs[ch], glyphangle,
				m_Alpha, center);
		dx += line.X();
		dy += line.Y();
	}

	m_Cursor.X(static_cast<int>(dx));
	m_Cursor.Y(static_cast<int>(dy));
}

}	// Engine

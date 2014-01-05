/* Font.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * This class models a _fixed_ font. It allows scaling, rotating the text,
 * rotating glyphs independently of the text, kerning, and keeps track of
 * an internal cursor.
 *
 * The font is built out of 96 ASCII characters in a texture atlas. The
 * order in which they appear is meaningful.
 */

#ifndef ENGINE_FONT_H__
#define ENGINE_FONT_H__

#include "engine/Texture.h"
#include "engine/TextureRegion.h"
#include "engine/SpriteBatcher.h"

#include "util/Math.h"

#include <array>
#include <string>
#include <cmath>

namespace Engine {

class Font {
	public:
		Font(Texture *texture, int offsetX, int offsetY,
			int glyphsperrow, int glyphwidth, int glyphheight, double scale = 1.0);

		double Alpha() const { return m_Alpha; }
		void Alpha(double alpha) { m_Alpha = alpha; }

		int GlyphWidth() const { return m_GlyphWidth; }
		int GlyphHeight() const { return m_GlyphHeight; }

		int Kerning() const { return m_Kerning; }

		void Kerning(int kerning) { m_Kerning = kerning; }

		double Scale() const { return m_Scale; }

		void Scale(double const scale, bool kerningreset = true);

		Vector2<int> const& Cursor() const { return m_Cursor; }
		void Cursor(Vector2<int> const& cursor) { m_Cursor = cursor; }
		void Cursor(int x, int y) {
			m_Cursor.X(x);
			m_Cursor.Y(y);
		}

		/* A GlyphBox is a bounding box for a rotated glyph, useful
		 * if you want to implement a bit more complex rotation FX,
		 * eg. used to know how much space is letter M going to
		 * take when rotated <angle> degrees.
		 */
		Vector2<double> GlyphBox(double angle);

		void DrawText(SpriteBatcher& batcher, std::string const& text,
				double angle = 0, double glyphangle = 0, GPoint const* center = nullptr) {
			DrawText(batcher, text, m_Cursor.X(), m_Cursor.Y(), angle, glyphangle, center);
		}

		void DrawText(SpriteBatcher& batcher, char const c, double angle = 0,
				double glyphangle = 0, GPoint const* center = nullptr) {
			DrawText(batcher, std::string(1, c), m_Cursor.X(), m_Cursor.Y(), angle, glyphangle, center);
		}

		void DrawText(SpriteBatcher& batcher, char const c, int x, int y,
				double angle = 0, double glyphangle = 0, GPoint const* center = nullptr) {
			DrawText(batcher, std::string(1, c), x, y, angle, glyphangle, center);
		}

		void DrawText(SpriteBatcher& batcher, std::string const& text, int x, int y,
				double angle = 0, double glyphangle = 0, GPoint const* center = nullptr);

	private:
		static constexpr std::size_t NumGlyphs = 96;

		std::array<TextureRegion, NumGlyphs> m_Glyphs;
		Texture *mp_Texture;
		double m_Alpha;
		int m_OrigGlyphWidth;
		int m_OrigGlyphHeight;
		int m_GlyphWidth;
		int m_GlyphHeight;
		int m_Kerning;
		double m_Scale;
		Vector2<int> m_Cursor;
};

}	// Engine

#endif

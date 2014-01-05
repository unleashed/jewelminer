/* Assets.cpp - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * Singleton asset manager for the game.
 *
 * Its data is tied to the texture atlas used.
 */

#include "engine/Graphics.h"
#include "engine/Texture.h"
#include "engine/TextureRegion.h"
#include "engine/Animation.h"
#include "engine/Font.h"

#include "Assets.h"

using Engine::Graphics;
using Engine::Texture;
using Engine::TextureRegion;
using Engine::Animation;
using Engine::Font;

Assets* Assets::instance = nullptr;

Assets::Assets()
{
	background = new Texture("../res/BackGround.jpg");
	backgroundRegion = new TextureRegion(background, 0, 0, background->Width(), background->Height());
	atlas = new Texture("../res/textureatlas.png");
	jewelYellow = new TextureRegion(atlas, 0, 0, 40, 40);
	jewelRed = new TextureRegion(atlas, 40, 0, 40, 40);
	jewelPurple = new TextureRegion(atlas, 80, 0, 40, 40);
	jewelGreen = new TextureRegion(atlas, 120, 0, 40, 40);
	jewelBlue = new TextureRegion(atlas, 160, 0, 40, 40);
	star = new TextureRegion(atlas, 64, 64, 32, 30);
	font = new Font(atlas, 224, 0, 16, 16, 20);
	spark1 = new TextureRegion(atlas, 0, 128, 20, 20);
	spark2 = new TextureRegion(atlas, 22, 128, 20, 20);
	spark3 = new TextureRegion(atlas, 44, 128, 20, 20);
	spark4 = new TextureRegion(atlas, 66, 128, 20, 20);
	spark = new Animation(0.05, Animation::Mode::Looping, std::vector<TextureRegion*>{spark1, spark2, spark3, spark4});
}

Assets::Assets(Assets const& rhs)
{
	instance = rhs.instance;
}

Assets& Assets::operator=(Assets const& rhs)
{
	if (this != &rhs) {
		instance = rhs.instance;
	}

	return *this;
}

Assets::~Assets()
{
	delete background;
	delete backgroundRegion;
	delete star;
	delete font;
	delete jewelYellow;
	delete jewelRed;
	delete jewelPurple;
	delete jewelGreen;
	delete jewelBlue;
	delete spark1;
	delete spark2;
	delete spark3;
	delete spark4;
	delete atlas;
}

Assets& Assets::Instance() {
	static Assets theinstance;
	instance = &theinstance;

	return *instance;
}

// helper needed to set renderer on textures
void Assets::SetGraphics(Graphics const& g) {
	background->SetRenderer(g.GetRenderer());
	atlas->SetRenderer(g.GetRenderer());
}

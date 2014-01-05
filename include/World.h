/* World.h - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * Manages the Jewel Miner game. Implements a Miner::Listener, keeps track
 * of the game objects, score and game life cycle.
 */

#ifndef WORLD_H__
#define WORLD_H__

#include "engine/Graphics.h"
#include "engine/Texture.h"
#include "engine/TextureRegion.h"
#include "engine/Font.h"
#include "engine/SpriteBatcher.h"
#include "engine/Vector2.h"

#include "Assets.h"

#include "Jewel.h"
#include "Star.h"
#include "Spark.h"

#include "miner/Matrix.h"
#include "miner/Game.h"
#include "miner/Listener.h"

#include <cmath>

#include <memory>
#include <algorithm>
#include <list>
#include <string>

class World : public Miner::Listener<Miner::Jewel> {
	Engine::Graphics *mp_G;
	Engine::SpriteBatcher m_SB;
	Engine::Texture *m_Background;
	Engine::Texture *m_Atlas;
	Engine::Font *m_Font;
	Engine::GRect m_VisibleArea;

	// our matrix
	std::shared_ptr<Miner::Matrix<Miner::Jewel>> m_Matrix;

	// game objects
	std::shared_ptr<Jewel> *jewels;
	std::list<Star> m_StarList;
	Spark m_Spark;

	// rotation direction of the spinning jewels
	int m_Rotation;
	// coordinates of the selected jewel, if any
	int m_SelectedCol, m_SelectedRow;

	// width and height of cells in the visible matrix space
	int m_JewelWidth;
	int m_JewelHeight;
	// width and height of actual jewels, shrinked just a bit for margins
	int m_ShrinkedJewelWidth;
	int m_ShrinkedJewelHeight;
	// position coordinates for the top left jewel
	int m_MatrixWidthStart;
	int m_MatrixHeightStart;

	// this caches the matrix's number of columns and rows
	int m_NumCols;
	int m_NumRows;

	// game management variables
	int m_Score;
	double m_Time;
	double m_TimeRemaining;
	bool m_GameOver;
	Miner::Game<Miner::Jewel> m_Game;


	// Screen <=> Column, Row translation helpers
	Engine::Vector2<double> M2S(int col, int row) {
		return Engine::Vector2<double>(col * m_JewelWidth + m_MatrixWidthStart, row * m_JewelHeight + m_MatrixHeightStart);
	}

	void S2M(int x, int y, int& col, int& row) {
		col = (x - m_VisibleArea.x) / m_JewelWidth;
		row = (y - m_VisibleArea.y) / m_JewelHeight;
	}

	int IDX(int col, int row) {
		return row * m_NumCols + col;
	}

	// fills in out array with a representation of the matrix's state
	void AcquireMatrix();

	// draws those nice jewels
	void DrawMatrix();

	// renders the game's score and remaining time
	void DrawText();

	// helper to perform both an array and a graphical swap of jewels
	void SwapHelper(int col1, int row1, int col2, int row2, double vel, double accel);

	public:

	World(Engine::Graphics *graphics, Engine::GRect const& area, double time = 60, int numcols = 16, int numrows = 16);

	~World() { delete[] jewels; }

	// resets all of the game data to start a new game
	void ResetGame();

	// click handler
	void Click(int x, int y, bool up);

	void ClickJewel(int x, int y);

	// update the world!
	void Update(double deltatime);

	// draw a frame
	void Render(double deltatime);

	// Miner::Listener interface implementation
	virtual void Swapped(int col1, int row1, int col2, int row2);
	virtual void SwapOK(int col1, int row1, int col2, int row2);
	virtual void SwapFailed(int col1, int row1, int col2, int row2);
	virtual void Ready();
	virtual void Deletion(Miner::Event<Miner::Jewel>::Target target, int num, int start, Miner::Event<Miner::Jewel>::container_type& container);
	virtual void Insertion(Miner::Event<Miner::Jewel>::Target target, int num, int start, Miner::Event<Miner::Jewel>::container_type& container);
	virtual void CycleFinished();
	virtual void Fall(int column, int row, int gaps);
	// creates a new jewel ready to fall from the ceiling
	virtual void New(int column, int row, Miner::Jewel::Color color, int totalgaps);
	// spin a jewel to explosion
	virtual void Delete(Miner::Event<Miner::Jewel>::Target target, int colrow, int pos);
	virtual void Destroyed(int matches);
};

#endif

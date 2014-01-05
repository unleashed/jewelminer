/* World.cpp - Copyright (c) 2014 Alejandro Martinez Ruiz <alex@flawedcode.org>
 *
 * Manages the Jewel Miner game. Implements a Miner::Listener, keeps track
 * of the game objects, score and game life cycle.
 */

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

#include "World.h"

#include <cmath>

#include <memory>
#include <algorithm>
#include <list>
#include <string>

// fills in out array with a representation of the matrix's state
void World::AcquireMatrix()
{
	// m_Matrix's iterators use row order
	std::transform(m_Matrix->cbegin(), m_Matrix->cend(), &jewels[0],
		[this] (Miner::Jewel const& jewel) -> std::shared_ptr<Jewel> {
			std::shared_ptr<Jewel> j(std::make_shared<Jewel>());
			j->SetColor(jewel.GetColor());
			j->Size(m_ShrinkedJewelWidth, m_ShrinkedJewelHeight);
			j->SetPosition(M2S(jewel.GetColumn(), jewel.GetRow()));
			j->SetStarList(&m_StarList);
			return j;
		});
}

// draws those nice jewels
void World::DrawMatrix()
{
	std::for_each(&jewels[0], &jewels[m_NumCols * m_NumRows],
		[this](std::shared_ptr<Jewel>& j) {
			Engine::TextureRegion const* region = j->Region();
			if (region != nullptr)
				m_SB.DrawSprite(*j, *region);
		});
}

// renders the game's score and remaining time
void World::DrawText()
{
	m_Font->Scale(1.0);
	m_Font->Cursor(40, 150);
	m_Font->DrawText(m_SB, "Score: ");
	m_Font->DrawText(m_SB, std::to_string(m_Score));
	m_Font->Scale(2.0);
	m_Font->Cursor(93, 444);
	m_Font->DrawText(m_SB, std::to_string(static_cast<int>(m_TimeRemaining)));
}

// helper to perform both an array and a graphical swap of jewels
void World::SwapHelper(int col1, int row1, int col2, int row2, double vel, double accel)
{
	int index1 = IDX(col1, row1), index2 = IDX(col2, row2);
	std::shared_ptr<Jewel> tmp(jewels[index1]);

	jewels[index1] = jewels[index2];
	jewels[index2] = tmp;
	jewels[index1]->MoveTo(M2S(col1, row1), vel, accel);
	jewels[index2]->MoveTo(M2S(col2, row2), vel, accel);
}

World::World(Engine::Graphics *graphics, Engine::GRect const& area,
				double time, int numcols, int numrows) :
		mp_G{graphics}, m_SB(graphics),
		m_Background{Assets::Instance().background},
		m_Atlas{Assets::Instance().atlas},
		m_Font{Assets::Instance().font},
		m_Matrix{std::make_shared<Miner::Matrix<Miner::Jewel>>(numcols, numrows)},
		jewels{new std::shared_ptr<Jewel>[numcols * numrows]},
		m_Spark{time}, m_Rotation{1},
		m_NumCols{numcols}, m_NumRows{numrows},
		m_Score{0}, m_Time{time}, m_TimeRemaining{time},
		m_GameOver{true},
		m_Game{m_Matrix, this}
{
	m_VisibleArea = area;

	// select no jewel
	m_SelectedCol = -1;
	m_SelectedRow = -1;

	m_JewelWidth = m_VisibleArea.w / m_NumCols;
	m_JewelHeight = m_VisibleArea.h / m_NumRows;
	m_MatrixWidthStart = m_VisibleArea.x + m_JewelWidth / 2;
	m_MatrixHeightStart = m_VisibleArea.y + m_JewelHeight / 2;

	// shrink 5% to allow for a bit more space between them
	m_ShrinkedJewelWidth = m_JewelWidth - m_JewelWidth / 20;
	m_ShrinkedJewelHeight = m_JewelHeight - m_JewelHeight / 20;

	AcquireMatrix();
}

// resets all of the game data to start a new game
void World::ResetGame()
{
	m_Game.Populate();
	AcquireMatrix();
	m_Score = 0;
	m_TimeRemaining = m_Time;
	m_Spark.Reset();
	m_SelectedCol = -1;
	m_SelectedRow = -1;
}

// click handler
void World::Click(int x, int y, bool up)
{
	// only interested on button up movements
	if (not up || !m_Game.Ready())
		return;

	if (m_GameOver) {
		// new game
		ResetGame();
	} else {
		// ok, handle a click on a jewel
		ClickJewel(x, y);
	}
}

void World::ClickJewel(int x, int y)
{
	int col, row;

	S2M(x, y, col, row);
	// test for a click out of bounds, deselect any jewel selected
	if (col < 0 || col >= m_NumCols || row < 0 || row >= m_NumRows) {
		if (m_SelectedCol >= 0) {
			jewels[IDX(m_SelectedCol, m_SelectedRow)]->Deselect();
			m_SelectedCol = -1;
			m_SelectedRow = -1;
		}
		return;
	}

	int index = IDX(col, row);
	// if there was no selection, select this jewel
	if (m_SelectedCol < 0) {
		m_SelectedCol = col;
		m_SelectedRow = row;
		jewels[index]->Select();
	} else {
		// try to swap if possible... or deselect
		if (m_Game.CanSwap(m_SelectedCol, m_SelectedRow, col, row)) {
			jewels[index]->Select();
			m_Game.Swap(m_SelectedCol, m_SelectedRow, col, row);
		} else {
			jewels[IDX(m_SelectedCol, m_SelectedRow)]->Deselect();
		}
		m_SelectedCol = -1;
		m_SelectedRow = -1;
	}
}

// update the world!
void World::Update(double deltatime)
{
	bool animfinished = true;

	// don't keep updating the spark and timer when game over
	if (not m_GameOver) {
		m_Spark.Update(deltatime);

		m_TimeRemaining -= deltatime;
		if (m_TimeRemaining <= 0) {
			m_TimeRemaining = 0;
			m_GameOver = true;
		}
	}

	// update jewels game objects, check if any is still moving/animating
	std::for_each(&jewels[0], &jewels[m_NumCols * m_NumRows],
		[&deltatime, &animfinished](std::shared_ptr<Jewel>& j) {
			j->Update(deltatime);
			if (not j->Stopped())
				animfinished = false;
		});

	// Don't advance the game's state until all animations have been
	// performed. This is used as a simple synchronization mechanism
	if (animfinished && !m_Game.Ready()) {
		m_Game.Go();
	}

	// destroy stars that are of no more use
	m_StarList.remove_if([](Star const& s) { return s.Done(); });

	for (auto& star : m_StarList)
		star.Update(deltatime);
}

// draw a frame
void World::Render(double deltatime)
{
	// clear and draw the background
	mp_G->Clear();
	mp_G->RenderCopy(m_Background, nullptr, nullptr);

	/* the matrix render is performed within a clipping
	 * area that allows us to let jewels "drop in" from
	 * the ceiling, this is why we end this batch so
	 * prematurely (as compared to adding other objects
	 * living within the same atlas)
	 * Note: this is due to current engine backend API
	 * limitations (ie. SDL2 2D API)
	 */
	m_SB.BeginBatch(m_Atlas, &m_VisibleArea);
	DrawMatrix();
	m_SB.EndBatch();

	// draw the rest of the game elements
	m_SB.BeginBatch(m_Atlas);

	DrawText();
	if (m_GameOver) {
		if (m_Game.Ready()) {
			m_Font->Scale(3.0);
			m_Font->Cursor(175, 240);
			m_Font->DrawText(m_SB, "Game Over");
		}
	} else {
		m_SB.DrawSprite(m_Spark);
	}
	for (auto& star : m_StarList)
		m_SB.DrawSprite(star);

	m_SB.EndBatch();

	// throw it all to the screen
	mp_G->Present();
}

// Miner::Listener interface implementation
void World::Swapped(int col1, int row1, int col2, int row2) {
	SwapHelper(col1, row1, col2, row2, (col1 == col2) ? m_JewelHeight * 4 : m_JewelWidth * 4, 0);
}

void World::SwapOK(int col1, int row1, int col2, int row2) {
	// swap will match, deselect both jewels
	jewels[IDX(col1, row1)]->Deselect();
	jewels[IDX(col2, row2)]->Deselect();
}

void World::SwapFailed(int col1, int row1, int col2, int row2) {
	// swapfailed, deselect jewels and swap them back
	jewels[IDX(col1, row1)]->Deselect();
	jewels[IDX(col2, row2)]->Deselect();
	SwapHelper(col1, row1, col2, row2, (col1 == col2) ? m_JewelHeight * 4 : m_JewelWidth * 4, 0);
}

void World::Ready() {
	m_GameOver = false;
}

void World::Deletion(Miner::Event<Miner::Jewel>::Target target, int num,
		int start, Miner::Event<Miner::Jewel>::container_type& container) {}
void World::Insertion(Miner::Event<Miner::Jewel>::Target target, int num,
		int start, Miner::Event<Miner::Jewel>::container_type& container)
{
	m_Score += container.size() * 10;
}

void World::CycleFinished()
{
	// set new rotation direction for this new cycle
	m_Rotation = Util::rand_between(0, 1) ? 1 : -1;
}

void World::Fall(int column, int row, int gaps)
{
	// apply gravity to a jewel down to a new place
	SwapHelper(column, row, column, row + gaps, 0, 9.81 * 100);
}

// creates a new jewel ready to fall from the ceiling
void World::New(int column, int row, Miner::Jewel::Color color, int totalgaps)
{
	int index = IDX(column, row);

	jewels[index]->SetColor(color);
	jewels[index]->SetPosition(M2S(column, row - totalgaps));
	jewels[index]->MoveTo(M2S(column, row), 0, 9.81 * 100);
}

// spin a jewel to explosion
void World::Delete(Miner::Event<Miner::Jewel>::Target target, int colrow, int pos)
{
	int index;

	if (target == Miner::Event<Miner::Jewel>::Target::Column)
		index = IDX(colrow, pos);
	else
		index = IDX(pos, colrow);

	jewels[index]->Explode(0.6, 0, 4080 * m_Rotation);
}

void World::Destroyed(int matches) {}

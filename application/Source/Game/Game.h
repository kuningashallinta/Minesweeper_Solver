#pragma once

#include "imgui.h"

#include "Milkshake/Solver.hpp"

#include <iostream>
#include <chrono>

namespace game
{
	Milkshake::Board br;
	Milkshake::Solver sl(&br);

	Milkshake::Size size = { 10, 10 };
	uint32_t mines = 10;

	bool playing = false;
	bool started = false;

	bool hint = false;
	bool solving = false;
	int speed = 5;
	int flags = 0;

	bool should_step = false;

	std::chrono::steady_clock::time_point NextStep;

	void start();

	void update()
	{
		if (not should_step and (not solving or std::chrono::steady_clock::now() < NextStep))
			return;

		should_step = false;
		sl.step();
		const int delay = speed == 10 ? 10 : (speed >= 1 and speed <= 9 ? 100 * (10 - speed) : 1000);
		NextStep = std::chrono::steady_clock::now() + std::chrono::milliseconds(delay + 10);
	}

	void start()
	{
		started = true;
		playing = true;

		br.set_mine_count(mines);
		br.resize(size);
		br.generate();
	}


	void reveal(Milkshake::Cell* T, bool can_explode, bool can_chord)
	{
		if (can_explode and T->is_mine())
		{
			game::playing = false;

			for (uint32_t y = 0; y < br.get_size().y; y++)
			{
				for (uint32_t x = 0; x < br.get_size().x; x++)
				{
					if (br.get_cell({ x, y })->is_mine() and br.get_cell({ x, y })->get_state() != Milkshake::State::FLAGGED)
						br.get_cell({ x, y })->set_state(Milkshake::State::EXPLODED);
				}
			}

			return;
		}

		Milkshake::Cells sr = br.get_surrounding_cells(T->get_position());

		int mines = 0;
		int flags = 0;

		for (size_t i = 0; i < sr.size(); i++)
		{
			if (sr.at(i)->is_mine())
				mines++;

			if (sr.at(i)->get_state() == Milkshake::State::FLAGGED)
				flags++;
		}

		if (T->get_state() == Milkshake::State::REVEALED and can_chord and flags == (int)T->get_value())
		{
			Milkshake::Cells vg = br.get_surrounding_cells(T->get_position(), { Milkshake::State::HIDDEN });
			Milkshake::Cells fl = br.get_surrounding_cells(T->get_position(), { Milkshake::State::FLAGGED });

			if (fl.size() == (uint8_t)T->get_value())
				for (size_t i = 0; i < vg.size(); i++)
					reveal(vg.at(i), true, false);
		}

		if (T->get_state() == Milkshake::State::REVEALED)
			return;

		T->set_value((Milkshake::Value)(mines));
		T->set_state(Milkshake::State::REVEALED);

		if (mines == 0)
		{
			for (size_t i = 0; i < sr.size(); i++)
			{
				reveal(sr.at(i), false, false);
			}
		}
	}
}

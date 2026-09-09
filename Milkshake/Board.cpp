#include "Board.hpp"
#include <random>
#include <algorithm>
#include <iostream>
#include <cstring>

namespace Milkshake {
    Board::Board(Size size, uint32_t mines) : m_size(size), m_mines(mines)
    {
        resize(size);
    }

    void Board::resize(Size s) {
        m_size = s;
        m_data.resize(s.x * s.y);

        reset();
    }

    Size Board::get_size() const {
        return m_size;
    }

    Cell* Board::get_cell(Position pos) {
        if (!is_position_valid(pos))
            return nullptr;

        return &m_data[get_index(pos)];
    }

    Cell* Board::get_cell(uint32_t index) {
        if (!is_index_valid(index))
            return nullptr;
        
        return &m_data[index];
    }

    std::vector<Cell*> Board::get_surrounding_cells(Position cell, std::vector<State> states, std::vector<Value> values) {
        Cells surrounding;

        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                if (dx == 0 and dy == 0) continue;

                Position pos{
                    static_cast<uint32_t>(static_cast<int>(cell.x) + dx),
                    static_cast<uint32_t>(static_cast<int>(cell.y) + dy)
                };

                if (!is_position_valid(pos)) continue;

                Cell* neighbor = get_cell(pos);
                if (!neighbor) continue;

                if (!states.empty() &&
                    std::find(states.begin(), states.end(), neighbor->get_state()) == states.end()) {
                    continue;
                }

                if (!values.empty() &&
                    std::find(values.begin(), values.end(), neighbor->get_value()) == values.end()) {
                    continue;
                }

                surrounding.push_back(neighbor);
            }
        }

        return surrounding;
    }

    bool Board::is_cell_near(Position p1, Position p2) {
        uint32_t dx = std::abs(static_cast<int>(p1.x) - static_cast<int>(p2.x));
        uint32_t dy = std::abs(static_cast<int>(p1.y) - static_cast<int>(p2.y));
        return dx <= 1 && dy <= 1 && (dx != 0 || dy != 0);
    }

    void Board::set_mine_count(uint32_t mines) {
        m_mines = mines;
    }

    uint32_t Board::get_mine_count() const {
        return m_mines;
    }

    void Board::reset() {
        Size s = get_size();
        m_mines_remaining = m_mines;

        for (uint32_t y = 0; y < s.y; ++y) {
            for (uint32_t x = 0; x < s.x; ++x) {

                Cell* T = &m_data.at(get_index({ x, y }));

                memset(T, 0, sizeof Cell);
                T->set_position({ x, y });
                T->set_state(State::HIDDEN);
                memset(T->get_attributes(), 0, sizeof Attributes);
            }
        }
    }

    void Board::generate() {/*
        reset();
        
        place(Cell({ 0, 9 }, State::REVEALED, Value::MINES_1));
        place(Cell({ 0, 8 }, State::REVEALED, Value::MINES_2));
        place ( Cell ( { 0, 7 }, State::REVEALED, Value::MINES_2 ) );

        Cell Mine = { { 1, 9 }, State::HIDDEN };
        Mine.set_mine();
         Mine = { { 1, 7 }, State::HIDDEN };
        Mine.set_mine ( );
         Mine = { { 0, 6 }, State::HIDDEN };
        Mine.set_mine ( );

        return;*/
        
        uint32_t placed_mines = 0;

        while (placed_mines < m_mines) {
            
            Position pos = Position(rand() % m_size.x, rand() % m_size.y);

            Cell* cell = get_cell(pos);   

            if (not cell->is_mine()) {
                cell->set_mine(true);
                placed_mines++;
            } 
        }
    }

    bool Board::is_position_valid(Position pos) const {
        return pos.x < m_size.x && pos.y < m_size.y;
    }

    bool Board::is_index_valid(uint32_t index) const {
        return index >= 0 && index < static_cast<uint32_t>(m_size.x * m_size.y);
    }

    uint32_t Board::get_index(Position pos) const {
        return static_cast<uint32_t>(pos.y * m_size.x + pos.x);
    }

    Position Board::get_position(uint32_t index) const {
        return { index / m_size.x, index % m_size.x };
    }

    void Board::place(Cell T)
    {
        m_data.at(get_index(T.get_position())) = T;
    }

} // namespace Milkshake

#pragma once

#include "Cell.hpp"

#include <vector>

namespace Milkshake
{
    class Board
    {
    public:
        explicit Board(Size size = { 10, 10 }, uint32_t mines = 0);

        ~Board() noexcept = default;

        void resize(Size s);
        [[nodiscard]] Size get_size() const;

        [[nodiscard]] Cell* get_cell(Position pos);
        [[nodiscard]] Cell* get_cell(uint32_t index);
        [[nodiscard]] std::vector<Cell*> get_surrounding_cells(Position cell, std::vector<State> states = {}, std::vector<Value> values = {});

        [[nodiscard]] bool is_cell_near(Position p1, Position p2);

        void set_mine_count(uint32_t mines);
        [[nodiscard]] uint32_t get_mine_count() const;

        void reset();
        void generate();

        [[nodiscard]] bool is_position_valid(Position pos) const;
        [[nodiscard]] bool is_index_valid(uint32_t index) const;

        [[nodiscard]] uint32_t get_index(Position pos) const;
        [[nodiscard]] Position get_position(uint32_t index) const;

        void place(Cell T);
    private:
        Size m_size;
        std::vector<Cell> m_data;
        uint32_t m_mines;
        uint32_t m_mines_remaining;
    };

} // namespace Milkshake


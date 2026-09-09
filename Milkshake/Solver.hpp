#pragma once

#include "Board.hpp"
#include <memory>
#include <functional>
#include <chrono>
#include <thread>

namespace Milkshake
{
    class Solver {
    public:
        explicit Solver(Board* board);
        explicit Solver() noexcept = default;
        ~Solver() noexcept = default;

        bool step();
        bool cs_ = true;
        bool q_ = true;

        void attach(Board* board);
        [[nodiscard]] Board* get_board() const;
    private:
        bool solve_q(Cell* T);
        bool solve_cs ( Cell* T);
       
        Board* m_board;
        
        
    };

} // namespace Milkshake


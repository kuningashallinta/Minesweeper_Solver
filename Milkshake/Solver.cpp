    #include "Solver.hpp"

#include <functional>
#include <algorithm>
#include <iostream>
#include <unordered_set>

#include <print>

using namespace std;

namespace Milkshake
{
	Solver::Solver(Board* board) : m_board(board) {}



	bool Solver::step()
	{

        const Size bs = m_board->get_size ( );

        for (uint32_t y = 0; y < bs.y; ++y)
        {
            for (uint32_t x = 0; x < bs.x; ++x)
            {
                Cell* T = m_board->get_cell({ x, y });

                T->get_attributes()->mine = false;

            }
        }



        for ( uint32_t y = 0; y < bs.y; ++y )
        {
            for ( uint32_t x = 0; x < bs.x; ++x )
            {
                Cell* T = m_board->get_cell({ x, y });

                if (T->get_state() != State::REVEALED or (uint8_t)T->get_value() == 0)
                    continue; 

                //if ( T->get_position ( ).x != 0 or T->get_position ( ).y != 9 )
                //    continue;

                if (q_ and solve_q(T))
                    return true; 

                if (cs_ and solve_cs(T))
                    return true;

            }
        }

        return false;
	}

    void Solver::attach(Board* board)
    {
        m_board = board;
    }

    Board* Solver::get_board() const
    {
        return m_board;
    }

    bool Solver::solve_q(Cell* T)
    {
        bool result = false;

        uint8_t v = (uint8_t)(T->get_value());

        Cells nr = m_board->get_surrounding_cells(T->get_position(), {State::HIDDEN});

        if (nr.size() == 0)
            return false;

        Cells fl = m_board->get_surrounding_cells( T->get_position ( ), { State::FLAGGED });
        
        if (v == nr.size() + fl.size())
        {
            for ( auto& cell : nr)
            {
                cell->flag();
            }
                    
            result = true;
        }

        if (v - fl.size() == 0)
        {
            for ( auto& cell : nr)
            {
                cell->reveal();
            }
            
            result = true;
        }

        return result;
    } 

    bool Solver::solve_cs(Cell* T)
    {
        std::cout << T->get_position().x << " " << T->get_position().y << "\n";
        Cells inr = m_board->get_surrounding_cells(T->get_position(), { State::HIDDEN });
        Cells ifl = m_board->get_surrounding_cells(T->get_position(), { State::FLAGGED });

        uint8_t in = static_cast<uint8_t>(static_cast<uint8_t>(T->get_value()) - ifl.size());

        Cells fl;

        for (auto& cell : inr)
        {
            cell->get_attributes()->mine = true;

            printf("flagging: %d\t%d\n", cell->get_position().x, cell->get_position().y);

            fl.push_back(cell);
        }

        bool result = false;

        for (auto& cell : inr)
        {
            Cells nb = m_board->get_surrounding_cells(cell->get_position(), { State::REVEALED });

            for (auto& li : nb)
            {
                if (li == T)
                    continue;

                Cells inr2 = m_board->get_surrounding_cells(li->get_position(), { State::HIDDEN });
                Cells ifl2 = m_board->get_surrounding_cells(li->get_position(), { State::FLAGGED });

                uint8_t in2 = static_cast<uint8_t>(static_cast<uint8_t>(li->get_value()) - ifl2.size());

                bool found = true;

                for (const auto& f : fl)
                    if (std::find(inr2.begin(), inr2.end(), f) == inr2.end())
                        found = false;

                if (!found)
                {
                    result = false;
                    continue;
                }

                uint8_t safe = static_cast<uint8_t>(inr2.size() - fl.size());
                uint8_t score = static_cast<uint8_t>(static_cast<uint8_t>(li->get_value()) - ifl2.size() - in);
                 
                std::cout << "safe: " << (int)safe << " score: " << (int)score << "\n";
                std::cout << "inr2: " << inr2.size() << " inr: " << inr.size() << "\n";

                if (score == 0)
                {
                    std::cout << "rev\n";

                    for (auto& e : inr2) 
                        if (e->get_attributes()->mine == false)
                    {
                        std::print(std::cout, "rev {} {}\n", e->get_position().x, e->get_position().y);
                        e->reveal();

                        result = true;
                    }

                    if (result)
                        break;
                }

                else if (score == safe)
                {
                    std::cout << "flg\n";   

                    for (auto& e : inr2) if (not e->get_attributes()->mine and not e->is_safe())
                    {
                        std::print(std::cout, "flg {} {}\n", e->get_position().x, e->get_position().y);
                        e->flag();
                        

                        result = true;
                    }

                    if (result)
                        break;
                }

                else
                {
                    std::cout << "stuck" << endl;
                }
            }
        }

        for (auto& cell : inr)
        {
            cell->get_attributes()->mine = false;
        }

        return result;
    }
}

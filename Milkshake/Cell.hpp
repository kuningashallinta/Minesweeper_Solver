#pragma once

#include <cstdint> 
#include <vector>

namespace Milkshake
{
    enum class Value : uint8_t
    {
        MINES_0 = 0,
        MINES_1 = 1,
        MINES_2 = 2,
        MINES_3 = 3,
        MINES_4 = 4,
        MINES_5 = 5,
        MINES_6 = 6,
        MINES_7 = 7,
        MINES_8 = 8
    };

    enum class State : uint8_t
    {
        REVEALED = 0,
        FLAGGED = 1,
        HIDDEN = 2,
        EXPLODED = 3
    };

    struct Point
    {
        uint32_t x = 0;
        uint32_t y = 0;

        Point() noexcept = default;

        Point(uint32_t nx, uint32_t ny);

        bool operator==(const Point& other) const { return x == other.x && y == other.y; }
        bool operator!=(const Point& other) const { return not (*this == other); }
    };

    struct Attributes
    {
        uint8_t weight;
        uint8_t level;
        bool mine;
    };

    struct Visualization
    {
        bool render = false;
        unsigned int bcl;
        unsigned int fcl;
    };

    using Size = Point;
    using Position = Point;

    class Cell
    {
    public:
        Cell();

        Cell(Position position, State state = State::HIDDEN, Value value = Value::MINES_0);

        ~Cell() noexcept = default;

        [[nodiscard]] bool is_safe() const;
        void set_safe(bool s = true);

        [[nodiscard]] Position get_position() const;
        void set_position(Position p);

        [[nodiscard]] State get_state() const;
        void set_state(State s);

        [[nodiscard]] Value get_value() const;
        void set_value(Value v);

        [[nodiscard]] Attributes* get_attributes ( );
        void set_attributes ( Attributes a );

        void flag ();
        void reveal ( );

        [[nodiscard]] bool is_mine()
        {
            return m_mine;
        }

        void set_mine(bool m = true)
        {
            m_mine = m;
        }

        Visualization vs;
    private:
        bool m_safe = false;
        bool m_mine = false;

        Position m_position;
        State m_state;
        Value m_value;
        Attributes m_attributes;
    };

    using Cells = std::vector<Cell *>;

} // namespace Milkshake


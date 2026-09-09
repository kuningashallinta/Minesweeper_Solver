#include "Cell.hpp"

namespace Milkshake
{
    Point::Point(uint32_t nx, uint32_t ny) : x(nx), y(ny) {}

    Cell::Cell() : m_position({ 0, 0 }), m_safe(false), m_state(State::HIDDEN), m_value(Value::MINES_0) {}

    Cell::Cell(Position position, State state, Value value) 
        : m_position(position), m_state(state), m_value(value) {
    }

    bool Cell::is_safe() const {
        return m_safe;
    }

    void Cell::set_safe(bool s) {
        m_safe = s;
    }

    Position Cell::get_position() const {
        return m_position;
    }

    void Cell::set_position(Position p) {
        m_position = p;
    }

    State Cell::get_state() const {
        return m_state;
    }

    void Cell::set_state(State s) {
        m_state = s;
    }

    Value Cell::get_value() const {
        return m_value;
    }

    void Cell::set_value(Value v) {
        m_value = v;
    }

    void Cell::flag ( )
    {
        m_state = State::FLAGGED;
    }

    void Cell::reveal ( )
    {
        set_safe ( );
    }

    Attributes* Cell::get_attributes ( )
    {
        return &m_attributes;
    }

    void Cell::set_attributes ( Attributes a )
    {
        m_attributes = a;
    }

} // namespace Milkshake
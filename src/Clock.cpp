#include "Clock.h"
namespace pump
{
void
Clock::reset( )
{
    m_current_time = 0;
}

Time
Clock::current_time( ) const
{
    return m_current_time;
}

Time
Clock::next( Seconds inc )
{
    m_current_time += inc;
    return m_current_time;
}

}  // namespace pump

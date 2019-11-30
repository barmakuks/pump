#pragma once
#include "Types.h"

namespace pump
{
class Clock
{
public:
    void reset( );
    Time current_time( ) const;
    Time next( Seconds inc );

private:
    Time m_current_time = 0;
};

}  // namespace pump

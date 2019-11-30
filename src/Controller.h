#pragma once

#include "Types.h"

namespace pump
{
class ControllerImpl;
class Clock;

class Controller
{
public:
    Controller( );
    ~Controller( );

    void setup( );
    void update( Clock& clock );

public:
private:
    ControllerImpl* pimpl;
};
}  // namepsace pump

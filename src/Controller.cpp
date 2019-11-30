#include "Controller.h"
#include "Types.h"
#include "Clock.h"

#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/power.h>

// -------------------------------------------------------------------------------------------------

namespace pump
{
template < uint8_t PUMP_POWER_PIN >
class Pump
{
    void
    switchPumpOn( )
    {
        digitalWrite( PUMP_POWER_PIN, HIGH );
    }
    void
    switchPumpOff( )
    {
        digitalWrite( PUMP_POWER_PIN, LOW );
    }

public:
    void
    pump( Seconds duration )
    {
        switchPumpOn( );
        delay( 1000U * duration );
        switchPumpOff( );
    }
};

// -------------------------------------------------------------------------------------------------
#define MAX_SENSOR_VALUE 1024
#define DELAY 1000
#define MEASUREMENT_COUNT 4

template < uint8_t SENSOR_POWER_PIN, uint8_t SENSOR_DATA_PIN >
class HumiditySensor
{
public:
    Percentage
    getValue( ) const
    {
        // switch on power
        digitalWrite( SENSOR_POWER_PIN, HIGH );
        int value = 0;

        for ( int i = 0; i < MEASUREMENT_COUNT; ++i )
        {
            delay( DELAY );
            value += MAX_SENSOR_VALUE - analogRead( SENSOR_DATA_PIN );
        }

        // switch off power
        digitalWrite( SENSOR_POWER_PIN, LOW );

        return static_cast< Percentage >( ( value / MEASUREMENT_COUNT * 100 ) >> 10 );
    }
};

// -------------------------------------------------------------------------------------------------

template < typename T >
struct Range
{
    Range( ) = delete;
    Range( T min, T max )
        : m_min( min )
        , m_max( max )
    {
    }

    bool
    inRange( T value ) const
    {
        return value >= m_min && value <= m_max;
    }

private:
    T m_min;
    T m_max;
};

// -------------------------------------------------------------------------------------------------

struct Settings
{
    const Seconds pumpingDuration_s = 5;
    const Seconds pumpingInterval_s = 60;
    const Seconds measurementInterval_s = 3600;
    const Range< Percentage > HUMIDITY_DRY = {0, 15};
    const Range< Percentage > HUMIDITY_NORMAL = {16, 50};
    const Range< Percentage > HUMIDITY_WET = {51, 70};
    const Range< Percentage > HUMIDITY_WATERING = {71, 100};
};

// -------------------------------------------------------------------------------------------------

class ControllerImpl
{
public:
    void
    update( Clock& clock )
    {
        if ( isTimeToDoSomething( clock.current_time( ) ) )
        {
            clock.reset( );
            const auto humidity = m_humidity_sensor.getValue( );
            if ( m_settings.HUMIDITY_DRY.inRange( humidity ) )
            {
                m_pump.pump( m_settings.pumpingDuration_s );
            }
            else if ( m_settings.HUMIDITY_NORMAL.inRange( humidity ) )
            {
            }
            else if ( m_settings.HUMIDITY_WET.inRange( humidity ) )
            {
            }
            else if ( m_settings.HUMIDITY_WATERING.inRange( humidity ) )
            {
            }
        }
    }

private:
    bool
    isTimeToDoSomething( Time time )
    {
        return time > m_settings.measurementInterval_s;
    }

private:
    Settings m_settings;
    Pump< 12 > m_pump;
    HumiditySensor< 2, A0 > m_humidity_sensor;
    Clock lastPumpTime;
    Clock lastMeasurementTime;
};

// -------------------------------------------------------------------------------------------------

Controller::Controller( )
    : pimpl( new ControllerImpl )
{
}

// -------------------------------------------------------------------------------------------------

Controller::~Controller( )
{
    delete pimpl;
    pimpl = nullptr;
}

// -------------------------------------------------------------------------------------------------

void
Controller::setup( )
{
}

// -------------------------------------------------------------------------------------------------

void
Controller::update( Clock& clock )
{
    if ( pimpl )
    {
        pimpl->update( clock );
    }
}

// -------------------------------------------------------------------------------------------------
}  // namespace pump

#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/power.h>

#include "Controller.h"
#include "Clock.h"

static volatile bool flag = true;

static pump::Clock EEMEM main_clock;
static pump::Controller controller;

// Interruption from WatchDog
ISR( WDT_vect )
{
    if ( !flag )
    {
        flag = true;
        main_clock.next( 8 );
    }
    else
    {
        wdt_reset( );
    }
}

void
enterSleepModeWatchDog( )
{
    wdt_reset( );
    set_sleep_mode( SLEEP_MODE_PWR_DOWN ); /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest
                                              power consumption. */
    /* Now enter sleep mode. */
    sleep_mode( );

    /* The program will continue from here after the WDT timeout*/
    /* Re-enable the peripherals. */
    power_all_enable( );
}

void
setupWatchDog( )
{
    /*** Setup the WDT ***/

    /* Clear the reset flag. */
    MCUSR &= ~( 1 << WDRF );

    /* In order to change WDE or the prescaler, we need to
     * set WDCE (This will allow updates for 4 clock cycles).
     */
    WDTCSR |= ( 1 << WDCE ) | ( 1 << WDE );

    /* set new watchdog timeout prescaler value */
    WDTCSR = 1 << WDP0 | 1 << WDP3; /* 8.0 seconds */

    /* Enable the WD interrupt (note no reset). */
    WDTCSR |= _BV( WDIE );
}

void
setup( )
{
    controller.setup( );
    main_clock.reset( );
    setupWatchDog( );
}

void
loop( )
{
    if ( flag )
    {
        /* Don't forget to clear the flag. */
        flag = false;
        controller.update( main_clock );
        /* Re-enter sleep mode. */
        enterSleepModeWatchDog( );
    }
}

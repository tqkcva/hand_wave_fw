#ifndef _myDefine_h_
#define _myDefine_h_

/**
 * Pubic definitions
 */

/**
 * Project Details
 */
#define NAME "Hand Wave"
#define VERSION "0.1"

/**
 * Serial debug
 */
#define SERIAL_SPEED 115200


/**
 * LEDs
 */
#define LED_PIN 13
#define FAST_BLINK_TIME 100
#define NORMAL_BLINK_TIME 500
#define SLOW_BLINK_TIME 1000
enum LED_STATE
{
    LED_OFF = 0,
    LED_ON,
    LED_FAST_BLINK,
    LED_NORMAL_BLINK,
    LED_SLOW_BLINK
};

/**
 * Data structure for stored inside EEPROM
 */
struct hand_wave
{
    long int num_of_wave;
    unsigned long delay_time;
};

#endif

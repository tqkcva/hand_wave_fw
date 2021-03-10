#include <Servo.h>
#include <EEPROM.h>

#include "myDefine.h"

/**
 * global variables
 */
static Servo myservo;

static struct hand_wave _hand_wave;
static enum LED_STATE led = LED_NORMAL_BLINK;
static unsigned long hand_wave_time = 0;


void led_process(void)
{
    static int led_state = LOW;
    static unsigned long led_time = 0;

    switch (led)
    {
    case LED_OFF:
        digitalWrite(LED_PIN, LOW);
        led_state = LOW;
        break;
    case LED_ON:
        digitalWrite(LED_PIN, HIGH);
        led_state = HIGH;
        break;
    default:
        if (millis() > led_time)
        {
            led_state = led_state == LOW ? HIGH : LOW;
            digitalWrite(LED_PIN, led_state);
            switch (led)
            {
            case LED_FAST_BLINK:
                led_time = millis() + FAST_BLINK_TIME;
                break;
            case LED_NORMAL_BLINK:
                led_time = millis() + NORMAL_BLINK_TIME;
                break;
            case LED_SLOW_BLINK:
                led_time = millis() + SLOW_BLINK_TIME;
            }
        }
        break;
    }
}

void wave(void)
{
#define DOWN 0
#define UP 40
#if 1
    myservo.write(UP);
    delay(600);
    myservo.write(DOWN);
    delay(600);
#else
    for (int i = DOWN; i <= UP; i++)
    {
        myservo.write(i);
        delay(15);
    }
    for (int i = UP; i >= DOWN; i--)
    {
        myservo.write(i);
        delay(15);
    }
#endif
}
void hand_wave(void)
{
    for (int i = 0; i < _hand_wave.num_of_wave; i++)
    {
        Serial.println("Hand wave");
        wave();
    }
}

/**
 * Command line proceed
 **/
void process_cmd(char *cmd, int len)
{
    // key val
    char *tmp_chr;
    char tmp_cmd[32];
    char tmp_val[32];

    // validity check
    Serial.println(cmd);

    if (sscanf(cmd, "%s %s", tmp_cmd, tmp_val) != 2)
    {
        Serial.print("\r\nInvalid\r\n");
        return;
    }
    long int val = strtol(tmp_val, NULL, 10);
    if (val < 0)
    {
        Serial.print("\r\nWrongValue\r\n");
        return;
    }
    if (memcmp(tmp_cmd, "num_of_wave:", strlen("num_of_wave:")) == 0)
    {
        _hand_wave.num_of_wave = val;
        Serial.print("\r\nOK\r\n");
    }
    else if (memcmp(tmp_cmd, "delay_time:", strlen("delay_time:")) == 0)
    {
        _hand_wave.delay_time = val;
        Serial.print("\r\nOK\r\n");
    }
    else
    {
        Serial.print("\r\nUnknownCMD\r\n");
    }
    EEPROM.put(0, _hand_wave);
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);

    myservo.attach(6);
    myservo.write(0);

    // data loading
    EEPROM.get(0, _hand_wave);
    if (_hand_wave.num_of_wave < 0 || _hand_wave.delay_time < 0)
    {
        _hand_wave.num_of_wave = 3;
        _hand_wave.delay_time = 300000;
    }

    Serial.begin(SERIAL_SPEED);
    /** Arduino Leonado
    while (!Serial)
        ;
    **/
    Serial.println(NAME " Ver: " VERSION);
    Serial.print("num_of_wave: ");
    Serial.println(_hand_wave.num_of_wave);
    Serial.print("delay_time: ");
    Serial.println(_hand_wave.delay_time);
}

int len = 0;
char cmd[64];

void loop()
{
    led_process();

    if (millis() > hand_wave_time)
    {
        hand_wave();

        hand_wave_time = millis() + _hand_wave.delay_time;
    }

    if (Serial.available())
    {
        len = Serial.readBytesUntil('\n', cmd, 63);
        cmd[len] = 0;
        process_cmd(cmd, len);
    }
}

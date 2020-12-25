#include "Arduino.h"
#include "click.h"

#define CZAS_TRZYMANIA 1000
#define GRACE_PERIOD 75 //Time after letting the push button go, to prevent re-bounce effects.


void Guzik::update(bool debug) {

	bool read;
	if(m_analog_pin) {
		int i_read=analogRead(m_pin);
		read=i_read>511;
	} else {
		read=digitalRead(m_pin);
	}
}

void Guzik::setupUsingDigitalPin(int pin) {
	m_pin = pin;
	pinMode(pin,INPUT);
	m_analog_pin = false;
}

void Guzik::setupUsingAnalogPin(int pin) {
	m_pin = pin;
	pinMode(pin,INPUT);
	m_analog_pin = true;
}

void Guzik::setupClickHandler(FunctionObject<void(void)> event_click) {
	m_event_click = event_click;
	m_use_click = true;
}

void Guzik::setupHoldHandler(FunctionObject<void(void)> event_hold) {
	m_event_hold = event_hold;
	m_use_hold = true;
}

void Guzik::setupTurnOnHandler(FunctionObject<void(void)> event_on) {
	m_event_on = event_on;
	m_use_on = true;
}

void Guzik::setupTurnOffHandler(FunctionObject<void(void)> event_off) {
	m_event_off = event_off;
	m_use_off = true;
}

void nofun() {}

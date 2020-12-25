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
	if (m_state==click_state::pulled){
		if (read){//jeżeli naciśnięto
			m_state=click_state::grace_period_pulled;
			m_fast_t = millis();
		}
	} else if (m_state==click_state::grace_period_pulled){
		if (read && millis() - m_fast_t >= GRACE_PERIOD) {//jeżeli minie czas grace period
			m_state=click_state::pressed; //              to znaczy że naciśnięto
			m_czas_n=millis();
			if(m_use_on){
				m_event_on();
			}
		}else if(not read) {
			if (debug){
				Serial.println("detected bounce effect");
			}
		}
	} else if (m_state==click_state::pressed){
		if (not read){//jeżeli puszczono
			m_state=click_state::grace_period_pressed;
			m_fast_t = millis();
		}
	} else if (m_state==click_state::grace_period_pressed){
		if (not read && millis() - m_fast_t >= GRACE_PERIOD) {//jeżeli minie czas grace period
			m_state=click_state::pulled;//                 to znaczy że puszczono
			if (millis() - m_czas_n >= 1000){
				if (m_use_hold){
					m_event_hold();
				} else if (m_use_click) {
					m_event_click();
				}
			} else {
				if (m_use_click) {
					m_event_click();
				}
			}
			if(m_use_off){
				m_event_off();
			}
		}else if(read) {
			if (debug){
				Serial.println("detected bounce effect");
			}
		}
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

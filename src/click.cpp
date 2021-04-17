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
			m_state=click_state::pulled;//                       to znaczy że puszczono
			if (millis() - m_czas_n >= CZAS_TRZYMANIA){
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

void GuzikTrio::setupFirstUsingAnalogPin(int pin) {
	st.setupUsingAnalogPin(pin);
}

void GuzikTrio::setupSecondUsingAnalogPin(int pin) {
	nd.setupUsingAnalogPin(pin);
	if (m_type == 2){
		Serial.print("you must use type 1 to declare second pin!");
	}
}

void GuzikTrio::setupThirdUsingAnalogPin(int pin) {
	rd.setupUsingAnalogPin(pin);
}

void GuzikTrio::setupFirstUsingDigitalPin(int pin) {
	st.setupUsingDigitalPin(pin);
}

void GuzikTrio::setupSecondUsingDigitalPin(int pin) {
	nd.setupUsingDigitalPin(pin);
	if (m_type == 2){
		Serial.print("you must use type 1 to declare second pin!");
	}
}

void GuzikTrio::setupThirdUsingDigitalPin(int pin) {
	rd.setupUsingDigitalPin(pin);
}

FunctionObject<void(void)> m_event_nd;

void GuzikTrio::is_it_second(){
	if (digitalRead(st.m_pin) == 0 and digitalRead(rd.m_pin) == 0){
		m_event_nd();
	}
}

//TODO: Make sure GuzikTrio cannot be automatic

void GuzikTrio::setupTurnFirstHandler(FunctionObject<void(void)> event_st){
	st.setupTurnOnHandler(event_st);
	if (m_type == 2){
		st.setupTurnOffHandler([this](){this->is_it_second();});
	}
}

void GuzikTrio::setupTurnSecondHandler(FunctionObject<void(void)> event_nd){
	if (m_type == 1){
		nd.setupTurnOnHandler(event_nd);
	} else if (m_type == 2){
		m_event_nd = event_nd;
	}
}

void GuzikTrio::setupTurnThirdHandler(FunctionObject<void(void)> event_rd){
	rd.setupTurnOnHandler(event_rd);
	if (m_type == 2){
		rd.setupTurnOffHandler([this](){this->is_it_second();});
	}
}

void GuzikTrio::update(bool debug){
	st.update(debug); 
	nd.update(debug);
	rd.update(debug);
}

void GuzikCap::setupCapacitivePin(int pin) {
	m_pin = pin;
	if (m_pin == 2) {
		m_CapButton = CapacitiveSensor(4,2);
	}
	if (m_pin == 6) {
		m_CapButton = CapacitiveSensor(4,6);
	}
	if (m_pin == 8) {
		m_CapButton = CapacitiveSensor(4,8);
	}
}

void GuzikCap::setupCapacitiveClick(FunctionObject<void(void)> event_click){ 
	m_event_click = event_click;
}

void GuzikCap::setupCapacitiveHold(FunctionObject<void(void)> event_hold){
	m_event_hold = event_hold;
	m_use_hold = true;
}

void GuzikCap::update(){
	long read = m_CapButton.capacitiveSensor(30);
	if (read >= m_click_value){
		if (m_pressed == false){
			m_pressed = true;
			m_pressed_time = millis();
		}
	}
	if (read <= m_click_value){
		if (m_pressed == true){
			m_pressed = false;
			if (millis() - m_pressed_time >= CZAS_TRZYMANIA){
				if (m_use_hold){
					m_event_hold();
				} else {
					m_event_click();
				}
			} else {
				m_event_click();
			}
		}
	}
}
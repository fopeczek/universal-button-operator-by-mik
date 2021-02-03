#pragma once
#include "function_objects.h"
enum class click_state {pulled, grace_period_pulled, pressed, grace_period_pressed};

void nofun();

class Guzik {
public:
	Guzik() : m_use_click(false), m_use_hold(false), m_use_on(false), m_use_off(false), m_pin(-1), m_analog_pin(false), m_event_click(nofun), m_event_hold(nofun), m_event_on(nofun), m_event_off(nofun) {} // @suppress("Class members should be properly initialized")
	void update(bool debug=false);
	void setupUsingDigitalPin(int pin);
	void setupUsingAnalogPin(int pin);
	void setupClickHandler(FunctionObject<void(void)> event_click);
	void setupHoldHandler(FunctionObject<void(void)> event_hold);
	void setupTurnOnHandler(FunctionObject<void(void)> event_on);
	void setupTurnOffHandler(FunctionObject<void(void)> event_off);

private:
	bool m_use_click;
	bool m_use_hold;
	bool m_use_on;
	bool m_use_off;

	int m_pin;
	bool m_analog_pin=false;

	FunctionObject<void(void)> m_event_click;
	FunctionObject<void(void)> m_event_hold;
	FunctionObject<void(void)> m_event_on;
	FunctionObject<void(void)> m_event_off;
//	void (* m_event_doubleclick)(void)=0;

	unsigned long m_czas_n; //timer for push_event
	unsigned long m_fast_t; //timer_for_grace_period

	click_state m_state = click_state::pulled; // pulled - nic nie naciśnięte. pressed_in_grace_period - naciśnięty klawisz, ale w grace period, który blokuje zdarzenie puszczenia klawisza
};

class GuzikTrio {
public:	

	void setupFirstUsingDigitalPin(int pin);
	void setupFirstUsingAnalogPin(int pin);

	void setupSecondUsingDigitalPin(int pin);
	void setupSecondUsingAnalogPin(int pin);

	void setupThirdUsingDigitalPin(int pin);
	void setupThirdUsingAnalogPin(int pin);

	void setupTurnFirstHandler(FunctionObject<void(void)> event_st);

	void setupTurnSecondHandler(FunctionObject<void(void)> event_nd);
	
	void setupTurnThirdHandler(FunctionObject<void(void)> event_rd);

	void update(bool debug=false);
private:
	Guzik st;
	Guzik nd;
	Guzik rd;
};
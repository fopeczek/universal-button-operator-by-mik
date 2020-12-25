#include "mik_click.h"
#include "Arduino.h"

int pin = 2;

void onClick(int par) {
	Serial.print("Click par ");
	Serial.print(par);
	Serial.print("!\n");
}

void onDoubleClick() {
	//nothing, not implemented yet
}

void onHold(){
	Serial.print ("hold\n");
}

Guzik g;

void setup()
{
	int par = 10;
	g.setupUsingDigitalPin(pin);
	g.setupClickHandler( [pax=par](){ onClick(pax);} );
	g.setupHoldHandler(onHold);
	Serial.begin(9600);
}

void loop()
{
	g.update(false);
}


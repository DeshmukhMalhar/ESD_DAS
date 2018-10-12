#include <SoftwareSerial.h> 
#include <LiquidCrystal.h>

//core in ACW pinmapping order
#define PA0 (10) //Pressure sensor input
#define PA1 ( 9) //TX
#define PA2 ( 8) //RX
#define PA3 ( 7) //DIffin + for load cell
#define PA4 ( 6) //DIffin - for load cell
#define PA5 ( 5) //D7
#define PA6 ( 4) //D6
#define PA7 ( 3) //D5
#define PB0 ( 0) //RS
#define PB1 ( 1) //EN
#define PB2 ( 2) //D4

#define PB3	(11)  /* RESET */

#define sbi(x,y) ((x |= (1 << y)))

LiquidCrystal lcd(0, 1, 2, 3, 4, 5);
SoftwareSerial serial(8,9);
uint16_t differentialanalogRead(){
	
	//Setting ADC prescalar for acceptable conversion rate
	ADMUX |=((1<<REFS1)|(0<<REFS0)|(0b00010011));//ADC channels A3 and A4 in diff mode with gain x20,voltage reference set to 1.1V internal
	ADCSRA |= ((1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2));  // setting prescalar to 128 for 62.5K conversion clock
	ADCSRB = 0 ;
	//adc setup complete, will turn it on once needed
	sbi(ADCSRA, ADEN); //enable ADC
	sbi(ADCSRA, ADSC); //Start conversion
	
	while(ADCSRA & (1<<ADSC)); //Wait for conversion to complete.
	
	return ((ADCH<<8)|(ADCL));
}
void setup() {
	serial.begin(9600);
	lcd.begin(16, 2);
	lcd.print(F("Data Acquisition System"));
	delay(1000);
  
}
uint16_t diffread=0;
void loop() {
	diffread=differentialanalogRead();
	Serial.print("D:");
	serial.println(diffread);
	Serial.print("A:");
	serial.println(analogRead(A0));
	delay(100);
}


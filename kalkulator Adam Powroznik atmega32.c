/*
 * SysWbudowane-Kalkulator.c
 *
 * Created: 10.05.2018 09:46:51
 * Author : Adam POWROZNIK
 */ 

#define F_CPU 1000000UL


#include <avr/io.h>
#include "hd44780.h"
#include <avr/delay.h>
#include <avr/interrupt.h>



/*
LCD_CLEAR;
LCD_LOCATE(1, 0);
lcd_puts(&przycisk);
*/



char przycisk = 'r';
long int liczba1, liczba2;
long int arg1[20], arg2[20];
char strLiczba1[200], strLiczba2[200], strWynik[200];
int dzialanie; //1 - dodaj, 2 - odejmij, 3 - mnozenie, 4 - dzielenie
int oblicz;  //flaga oblicz
int cyfraNumer; //wskaznik pozycji cyfry w tablicy
int argPointer = 1; //wskaznik tablicy
int arg1Length, arg2Length; //wskaznik dlugosci tablicy
long int wynik;
int error;
volatile int fiveSecFlag = 0;

int main (void)
{

	DDRC = 0b00001111; //tu jest klawiatura JAKO WEJSCIE
	PORTC = 0b11110000; //pullup
	
	TCCR1B |= (0<<WGM12);
	TCCR1B |= (1<<CS12);
	OCR1A = 19531;
	TIMSK |= (1<<OCIE1A);
	sei();


		//fast pwm timer 1
 	DDRB = 0xFF;
    TCCR0 |=(1<<WGM00)|(1<<WGM01)|(1<<COM00)|(1<<COM01)|(1<<CS00);
    OCR0 = 0;




	DDRA = 0XFF;
	lcd_init();
	LCD_DISPLAY(LCDDISPLAY);
	LCD_CLEAR;
	LCD_LOCATE(0,0);
	lcd_puts("ADAM POWROZNIK");
	_delay_ms(2000);
	LCD_CLEAR;
	LCD_LOCATE(0, 0);




	while(1)
	{
		sprawdzKlawiature();
		if (wcisnietoPrzycisk()){
			TCNT1 = 0;
			stworzArgumenty();
			}
		if (oblicz)
    {
      setArg(1);
      setArg(2);
      switch (dzialanie)
      {
        case 1:
        wynik = liczba1 + liczba2;
        break;
        case 2:
        wynik = liczba1 - liczba2;
        break;
        case 3:
        wynik = liczba1 * liczba2;
        break;
        case 4:
		if(liczba2 == 0){
		error = 1;
		wynik = 0;
		}
		else{
        wynik = liczba1 / liczba2;
		}
        break;
        default:
        break;
      }
      if(!error){
      oblicz = 0;
      ltoa(liczba1,strLiczba1,10);
      ltoa(liczba2,strLiczba2,10);
      ltoa(wynik,strWynik,10);
	  LCD_CLEAR;
	  LCD_LOCATE(0,0);
      lcd_puts(strLiczba1);
      switch (dzialanie)
      {
        case 1:
        lcd_puts("+");
        break;
        case 2:
        lcd_puts("-");
        break;
        case 3:
        lcd_puts("*");
        break;
        case 4:
        lcd_puts("/");
        break;
        default:
        break;
      }
      lcd_puts(strLiczba2);
      lcd_puts("=");
      LCD_LOCATE(0,1);
      lcd_puts(strWynik);
      if(dzialanie==4)
      {
       lcd_puts(".");
        int mod = liczba1 % liczba2;
        mod = (mod * 100) / liczba2;
        char mod2[20];
        itoa(mod,mod2,10);
        lcd_puts(mod2);
      }
	  }
	  else if (error)
	  {
		LCD_CLEAR;
	  	LCD_LOCATE(0,0);
      	lcd_puts("DZIELISZ");
	  	LCD_LOCATE(0,1);
		lcd_puts("PRZEZ 0 ?!");
		error = 0;
	  }
      clearArray(arg1, 20);
      clearArray(arg2, 20);
      argPointer = 1;
      liczba1 = 0;
      liczba2 = 0;
      przycisk = 'r';
      arg1Length = 0;
      arg2Length = 0;
      dzialanie = 0;
      while(przycisk=='r' )
      {
        sprawdzKlawiature();
      }
      LCD_CLEAR;
    }
	
	 // Procedura obsługi przerwania resetującego, odkomentowac
    /*if(fiveSecFlag>0)
    {
		fiveSecFlag=0;
      clearArray(arg1, 20);
      clearArray(arg2, 20);
      oblicz = 0;
      dzialanie = 0;
      argPointer = 1;
      arg1Length = 0;
      arg2Length = 0;
      cyfraNumer = 0;
      przycisk = 'r';
      LCD_CLEAR;
    }*/
	
	}
}

void stworzArgumenty(){
	OCR1A = 19531;
	fiveSecFlag = 0;
	switch (przycisk) {
		case '=':
		if(argPointer == 2){
			arg2Length = cyfraNumer;
			cyfraNumer = 0;
		oblicz = 1;}	
		przycisk = 'r';
		break;
		case '1':
		if (argPointer == 1)
		{
			arg1[cyfraNumer] = 1;
			cyfraNumer++;
		}
		else if (argPointer == 2)
		{
			arg2[cyfraNumer] = 1;
			cyfraNumer++;
		}
		lcd_puts("1");
		przycisk = 'r';
		break;
		case '2':
		if (argPointer == 1)
		{
			arg1[cyfraNumer] = 2;
			cyfraNumer++;
		}
		else if (argPointer == 2)
		{
			arg2[cyfraNumer] = 2;
			cyfraNumer++;
		}
		lcd_puts("2");
		przycisk = 'r';
		break;
		case '3':
		if (argPointer == 1)
		{
			arg1[cyfraNumer] = 3;
			cyfraNumer++;
		}
		else if (argPointer == 2)
		{
			arg2[cyfraNumer] = 3;
			cyfraNumer++;
		}
		lcd_puts("3");
		przycisk = 'r';
		break;
		case '4':
		if (argPointer == 1)
		{
			arg1[cyfraNumer] = 4;
			cyfraNumer++;
		}
		else if (argPointer == 2)
		{
			arg2[cyfraNumer] = 4;
			cyfraNumer++;
		}
		lcd_puts("4");
		przycisk = 'r';
		break;
		case '5':
		if (argPointer == 1)
		{
			arg1[cyfraNumer] = 5;
			cyfraNumer++;
		}
		else if (argPointer == 2)
		{
			arg2[cyfraNumer] = 5;
			cyfraNumer++;
		}
		lcd_puts("5");
		przycisk = 'r';
		break;
		case '6':
		if (argPointer == 1)
		{
			arg1[cyfraNumer] = 6;
			cyfraNumer++;
		}
		else if (argPointer == 2)
		{
			arg2[cyfraNumer] = 6;
			cyfraNumer++;
		}
		lcd_puts("6");
		przycisk = 'r';
		break;
		case '7':
		if (argPointer == 1)
		{
			arg1[cyfraNumer] = 7;
			cyfraNumer++;
		}
		else if (argPointer == 2)
		{
			arg2[cyfraNumer] = 7;
			cyfraNumer++;
		}
		lcd_puts("7");
		przycisk = 'r';
		break;
		case '8':
		if (argPointer == 1)
		{
			arg1[cyfraNumer] = 8;
			cyfraNumer++;
		}
		else if (argPointer == 2)
		{
			arg2[cyfraNumer] = 8;
			cyfraNumer++;
		}
		lcd_puts("8");
		przycisk = 'r';
		break;
		case '9':
		if (argPointer == 1)
		{
			arg1[cyfraNumer] = 9;
			cyfraNumer++;
		}
		else if (argPointer == 2)
		{
			arg2[cyfraNumer] = 9;
			cyfraNumer++;
		}
		lcd_puts("9");
		przycisk = 'r';
		break;
		case '0':
		if (argPointer == 1)
		{
			arg1[cyfraNumer] = 0;
			cyfraNumer++;
		}
		else if (argPointer == 2)
		{
			arg2[cyfraNumer] = 0;
			cyfraNumer++;
		}
		lcd_puts("0");
		przycisk = 'r';
		break;
		case '+':
		dzialanie = 1;
		if (argPointer == 1)
		{
			argPointer = 2;
			arg1Length = cyfraNumer;
		}
		else
		{
			argPointer = 1;
			oblicz = 1;
			arg2Length = cyfraNumer;
		}
		cyfraNumer = 0;
		lcd_puts("+");
		przycisk = 'r';
		break;
		case '-':
		dzialanie = 2;
		if (argPointer == 1)
		{
			argPointer = 2;
			arg1Length = cyfraNumer;
		}
		else
		{
			argPointer = 1;
			oblicz = 1;
			arg2Length = cyfraNumer;
		}
		cyfraNumer = 0;
		lcd_puts("-");
		przycisk = 'r';
		break;
		case '*':
		dzialanie = 3;
		if (argPointer == 1)
		{
			argPointer = 2;
			arg1Length = cyfraNumer;
		}
		else
		{
			argPointer = 1;
			oblicz = 1;
			arg2Length = cyfraNumer;
		}
		cyfraNumer = 0;
		lcd_puts("*");
		przycisk = 'r';
		break;
		case '/':
		dzialanie = 4;
		if (argPointer == 1)
		{
			argPointer = 2;
			arg1Length = cyfraNumer;
		}
		else
		{
			argPointer = 1;
			oblicz = 1;
			arg2Length = cyfraNumer;
		}
		cyfraNumer = 0;
		lcd_puts("/");
		przycisk = 'r';
		break;
		case 'C':
		clearArray(arg1, 20);
		clearArray(arg2, 20);
		oblicz = 0;
		dzialanie = 0;
		argPointer = 1;
		arg1Length = 0;
		arg2Length = 0;
		cyfraNumer = 0;
		przycisk = 'r';
		LCD_CLEAR;
		OCR0 = 20;
		_delay_ms(200);
		OCR0 = 0;
		break;
		default:
		break;
	}
}

int wcisnietoPrzycisk()
{
	if(przycisk != 'r')
	return 1;
	else
	return 0;
}

void ktoryPrzycisk(int kolumna, int wiersz){
	if(kolumna == 1){
		switch(wiersz){
			case 1:
			przycisk = '1';
			break;
			case 2:
			przycisk = '4';
			break;
			case 3:
			przycisk = '7';
			break;
			case 4:
			przycisk = 'C';
			break;
		}
	}
	else if(kolumna == 2){
		switch(wiersz){
			case 1:
			przycisk = '2';
			break;
			case 2:
			przycisk = '5';
			break;
			case 3:
			przycisk = '8';
			break;
			case 4:
			przycisk = '0';
			break;
		}
	}
	else if(kolumna == 3){
		switch(wiersz){
			case 1:
			przycisk = '3';
			break;
			case 2:
			przycisk = '6';
			break;
			case 3:
			przycisk = '9';
			break;
			case 4:
			przycisk = '=';
			break;
		}
	}
	else if(kolumna == 4){
		switch(wiersz){
			case 1:
			przycisk = 'C';
			break;
			case 2:
			przycisk = '-';
			break;
			case 3:
			przycisk = '*';
			break;
			case 4:
			przycisk = '/';
			break;
		}
	}
}

void clearArray(long int array[], int numberOfElements) {
	for (int i = 0; i < numberOfElements; i++)
	{
		array[i] = 0;
	}
}

void setArg(int argNumber) {
	if (argNumber == 1) {
		int power1 = arg1Length-1;
		for (int i = 0; i < arg1Length; i++)
		{
			arg1[i] *= power(10, power1); 
			power1--;
			liczba1 += arg1[i];
			//if (power <= 0) // ???
			// break;
		}
		
		clearArray(arg1, 20);
	}
	else if (argNumber == 2) {
		int power2 = arg2Length-1;
		for (int i = 0; i < arg2Length; i++)
		{
			arg2[i] *= power(10, power2); 
			power2--;
			liczba2 += arg2[i];
			// if (power <= 0) // ???
			// break;
		}
		clearArray(arg2, 20);
	}
}

int power(int x, int y){
	int pow=1;
	for(int i =0; i<y; i++){
		pow = pow * x;
	}
	return pow;
}



void kolumna(int ktora){
	if(ktora==1)
	PORTC = 0b11111110;
	else if(ktora==2)
	PORTC = 0b11111101;
	else if(ktora==3)
	PORTC = 0b11111011;
	else if(ktora==4)
	PORTC = 0b11110111;

}

int wiersz(int ktory){
	if(ktory==1)
	return 4;
	else if(ktory==2)
	return 5;
	else if(ktory==3)
	return 6;
	else if(ktory==4)
	return 7;
}

void sprawdzKlawiature(){
	for(int i = 1; i<5; i++){
		kolumna(i);
		for(int j = 1; j<5; j++){
			if(bit_is_clear(PINC, wiersz(j)))
			ktoryPrzycisk(j, i);
			_delay_ms(30);
		}
	}
}

ISR(TIMER1_COMPA_vect)
{
	fiveSecFlag=0;
      clearArray(arg1, 20);
      clearArray(arg2, 20);
      oblicz = 0;
      dzialanie = 0;
      argPointer = 1;
      arg1Length = 0;
      arg2Length = 0;
      cyfraNumer = 0;
      przycisk = 'r';
      LCD_CLEAR;
}

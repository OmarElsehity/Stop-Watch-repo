/*
 *  Mini_Project 2   (STOP WATCH)
 *
 *  Created on: Feb 9, 2022
 *      Author: Omar Elsehity
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char second = 0;
unsigned char minute = 0;
unsigned char hour = 0;

void TIMER1_Init(void )
{
    TCCR1B = (1 << WGM12)|(1 << CS10)|(1 << CS11); // set timer 1 to CTC MODE with prescalar 64
    TCNT1 = 0;        // intilization of the counter
    OCR1A = 15624;  // compare the value in TCNT1 if = 15624 which is 1 sec

    SREG |= (1 << 7); //enable interrupt when compare

    TIMSK |= (1 << OCIE1A); // Timer1 Output Compare Match Interrupt Enable
}

ISR(TIMER1_COMPA_vect)
{
    second ++;
    if (second == 60){
        second = 0;
        minute ++;
    }
    if (minute == 60){
        second = 0;
        minute = 0;
        hour ++;
    }
    if (hour == 24){
        second = 0;
        minute = 0;
        hour = 0 ;
    }

}

void INT0_Reset(void)
{
    DDRD &= ~(1 << PD2); // PIN D2 input
    PORTD |= (1 << PD2); // internal pull up

    MCUCR |= (1 << ISC01);    // The falling edge of INT0 generates an interrupt request.
    GICR |= (1 << INT0);  // External Interrupt Request 0 Enable
    SREG |= (1 << 7);   // enable I bit
}

ISR(INT0_vect)
{
    second = 0;
    minute = 0;
    hour = 0 ;
}

void INT1_Pause(void)
{
    DDRD &= ~(1 << PD3); // PIN D3 input

    MCUCR |= (1 << ISC10)|(1 << ISC11);    // The rising edge of INT1 generates an interrupt request
    GICR |= (1 << INT1);   //External Interrupt Request 1 Enable
    SREG |= (1 << 7);  // enable I bit
}

ISR(INT1_vect)
{

    TCCR1B &= ~(1 << CS10) & ~(1 << CS11) & ~(1 << CS12);  //Stop timer counter

}

void INT2_Resume(void)
{
    DDRB &= ~(1 << PB2);   // PIN B2 input
    PORTB |= (1 << PB2);   // internal pull up

    MCUCR &= ~(1 << ISC2);    // The falling edge of INT2 generates an interrupt request.
    GICR |= (1 << INT2);  //External Interrupt Request 1 Enable
    SREG |= (1 << 7);   // enable I bit
}

ISR(INT2_vect)
{
    TCCR1B = (1 << WGM12)|(1 << CS10)|(1 << CS11);  // set timer 1 to CTC MODE with prescalar 64 again
}

int main()
{
    DDRA = 0xFF;    //configure the 7-segments data pins to output
    PORTA = 0xFF;  // 7-segments always ON
    DDRC = 0x0F;    //configure the 7seg data pins to output
    PORTC = 0x00;  // at beginning all 7-segments outputs 0


    INT0_Reset( );
    INT1_Pause( );
    INT2_Resume( );
    TIMER1_Init( );

    while(1)
    {


        PORTA = (1 << 0);
        PORTC = second % 10;
        _delay_ms(3);
        PORTA = (1 << 1);
        PORTC = second / 10;
        _delay_ms(3);
        PORTA = (1 << 2);
        PORTC = minute % 10;
        _delay_ms(3);
        PORTA = (1 << 3);
        PORTC = minute / 10;
        _delay_ms(3);
        PORTA = (1 << 4);
        PORTC = hour % 10;
        _delay_ms(3);
        PORTA = (1 << 5);
        PORTC = hour / 10;
        _delay_ms(3);
    }

}

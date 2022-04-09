#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

void USART_init(void);
void USART_send( unsigned char data);


int main(void)
{
    USART_init();
    DDRB = 0;
    
    unsigned char cTrue = '1';
    unsigned char cFalse = '0';
    unsigned char lastInput = '0';

    while(1)
    {
        if(PINB & 0b00000010)
        {
            if(lastInput != cTrue)
            {
                USART_send(cTrue);
                lastInput = cTrue;
            }
        }
        else
        {
            if(lastInput != cFalse)
            {
                USART_send(cFalse);
                lastInput = cFalse;
            }
        }
        _delay_ms(10);
    }

    return 0;
}

void USART_init(void)
{
    UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
    UBRR0L = (uint8_t)(BAUD_PRESCALLER);
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    UCSR0C = (3<<UCSZ00);
}

void USART_send( unsigned char data)
{
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}

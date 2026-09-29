#ifndef display.h
#define display.h

#include <avr/io.h>
#include <util/delay.h>

// definição dos pinos que serão usados pelo LCD
#define PORTA_LCD PORTD
#define DDR_LCS DDRD
#define RS_LCD PORTD3
#define EN_LCD PORTD2
// NAO USAR OS PINOS 0 E 1 - SERVEM PARA COMUNICAÇÃO SERIAL

// protótipos das funções básicas de todo LCD
void pulse_lcd(void);
void send_comand_data(char byte, char rs);
void send_text(char* texto);
void inicialize(void);
void position_cursor(char linha, char coluna);

#endif
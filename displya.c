#include <display.h>

void pulse_lcd() {

    PORTA_LCD |= (1 << EN_LCD);     // subida do pino EN
    _delay_us(2); // pulso de clock dura um nínimo de 450ns
    PORTA_LCD &= ~(1 << EN_LCD);    // descida do pino EN, zera apenas o pino EN
    _delay_us(2);
}

// mesma ideia do laboratório, deverá ser repatirdo os 8 bits em parte alta e parte baixa, enviá-los um por vez
void send_comand_data(char byte, char rs) {

    PORTA_LCD &= ~(1 << RS_LCD);     // limpa o valor anterior de rs, forçando-o para 0
    PORTA_LCD |= (rs << RS_LCD);     // vai deixar 0 ou 1 em rs dependendo do valor recebido

    PORTA_LCD &= 0x0F;              // zera a parte alta parra receber o novo byte
    PORTA_LCD |= (byte & 0xF0);     // introduz apenas a nova parte alta do byte
    pulse_lcd();

    PORTA_LCD &= 0x0F;             // novamente limpa 
    // agora a parte baixa será movida para a parte alta, e introduzida nos pinos
    PORTA_LCD |= ((byte << 4) & 0xF0);
    pulse_lcd();

    _delay_us(40)                   // no datasheet, o delay recomendado é de 37 a 40 microsegundos, 40 é um valor seguro
}

void inicialize(void) {
    _delay_ms(50); // para a energia da placa estabilizar, o tempo recomendado é esse
    send_comand_data(0x33, 0);   // rest no chip LCD
    send_comand_data(0x32, 0);   // será utilizado o modo 4 bits
    send_comand_data(0x28, 0);   // display possui 2 linhas
    send_comand_data(0x0C, 0);   // liga a tela, sem cursor 
    send_comand_data(0x01, 0);   // limpar a tela, exige um delay maior
    _delay_ms(2);                   // recomendado no datasheet
}

void send_text(char* texto) {

    while(*texto != '\0') {
        send_comand_data(*texto, 1);  // 1 em rs para dizer que é um dado
        texto++;

    }
}

void position_cursor(char line, char column) {

    char adress = 0;
    if (line == 1) {
        adress = 0x80 + column;     // primeira linha começa no endereço 0x80
    } else {
        adress = 0xC0 + column;     // segunda linha começa no endereço 0xC0
    }
    send_comand_data(adress, 0);    // instrução para o cursor ir para essa posição desejada
}
#include "display.h"

typedef enum {
    EST_SLEEP = 0,
    EST_TYPE_PLATE,
    EST_SELECTION_TIME,
    EST_PAYMENT_METHOD,
    EST_PAYMENT_CARD,
    EST_ADM
} Estados;

// FUNÇÃO PARA EST_SLEEP
// função vai receber valores de hora e minuto através do horário atual, que virá do timer2
void sleep_screen(int hour, int minute) {       // ver como esses dados vão chegar no futuro

    send_comand_data(0x01, 0);     _delay_ms(2);        // limpar o display e dar aquele delay necessário
    position_cursor(1, 3);
    send_text("AREA AZUL");     // escrever na primeira linha, a partir da 3ª coluna, o texto AREA AZUL
    position_cursor(2, 5);

    // O caractere visível 0 representa o numero 48 em ASCII, por isso é necessário essa soma de 48
    char text_hour[6];         // 5 digitos da hora + \0
    text_hour[0] = (hour / 10) + 48;        // casa alta da hora
    text_hour[1] = (hour % 10) + 48;        // casa baixa da hora
    text_hour[2] = ':';
    text_hour[3] = (minute / 10) + 48;      // casa alta do minuto
    text_hour[4] = (minute % 10) + 48;      // casa baixa do minuto
    text_hour[5] = '\0';

    send_text(text_hour)      // escreve o horário na segunda linha, coluna 5
}

char its_a_letter (char c) {
    // 65 - A e 90 - Z          97 - a e 122 - z    EM ASCII
    if (c >= 65 && c <= 90 || c >= 97 && c <= 122) return 1;   // retorna 1 se for uma letra
    else return 0;                                            // retorna 0 caso contrário
}

char its_a_number (char c) {
    // Em ASCII 48 - 0 e 57 - 9
    if (c >= 48 && c <=57 ) return 1;    // retorna 1 se for um numero
    else return 0;                       // retorna 0 caso contrário
}

char plate_validation(char[] plate) {
    // importante notar que as posições 0, 1 e 2 são sempre letras, e 3, 5 e 6 são sempre numeros
    if (its_a_letter(plate[0]) && its_a_letter(plate[1]) && its_a_letter(plate[2]) &&
        its_a_number(plate[3]) && its_a_number(plate[5]) && its_a_number(plate[6])) {

            if (its_a_letter(placa[4]) || its_a_number(placa[4])) {     // verificação necessária para não ser um caractere bizarro
                return 1;   // placa dentro dos padrões
            }
        }
    return 0;   // representa uma placa inválida
}

char plate_buffer[8]; // 7 digitos da placa + \0

// FUNÇÃO PARA EST_TYPE_PLATE
void typing_plate(char key) {
    static char pos_cursor = 0;        // posição atual do cursor
    char flag_begin = 0;               // flag para não ficar reescrevendo e apagando DIGITE A PLACA no cursor

    if (flag_begin == 0) {
    send_comand_data(0x01); _delay_ms(2);
    position_cursor(1, 2);
    send_text("DIGITE PLACA");
    flag_begin = 1;
    }

    if (key != '\0') {
        plate_buffer[pos_cursor] = key;

        position_cursor(2, pos_cursor + 4);                 // +4 apenas para ficar mais centralizado
        send_comand_data(key, 1);
        pos_cursor++;

        if (pos_cursor == 7) {
            plate_buffer[7] = '\0';

            pos_cursor = 0;     // reset de flag e posição do cursor para o próximo cliente usar
            flag_begin = 0;

            // ISSO TA NO LUGAR CERTO???
            if (plate_validation(plate_buffer)) {       // se a placa está dentro dos padrões

                // TROCAR O ESTADO
            } else {
                send_comand_data(0x01); _delay_ms(2);
                send_text("PLACA INVALIDA")
            }
            
        }
    }
    // verificação dos padrões de placas

}
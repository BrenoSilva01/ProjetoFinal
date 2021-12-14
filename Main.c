#include "config.h" 
#include "bits.h" 
#include "lcd.h" 
#include "keypad.h"
#include "stdio.h"
#include "rgb.h"
#include <stdlib.h>  
#include <time.h> 
#include "ds1307.h"
#include <pic18f4520.h>
#include "adc.h"
#include "so.h"
#include "ssd.h"
#include "io.h"
#include "atraso.h"


#define L0 0x80 // linha 0, coluna 0
#define L1 0xC0 // linha 1, coluna 0
#define CLR 0x01
#define ON 0x0F

void lcdPosition(unsigned char l, unsigned char c) {
    if (l == 0)
        lcdCommand(L0 + c);
    if (l == 1)
        lcdCommand(L1 + c);
}

void main(void) {
    unsigned int tecla = 16;
    short short int r = 6;
    short short int d = 2;
    short short int c = 5;
    int x, y, z;
    unsigned long int cont = 0;

    static const char valor[] = {
        0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
        0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
    };
    static char display = 0;
    static char v0;

    int v;

    pinMode(DISP_4_PIN, OUTPUT);
    soInit();
    ssdInit();
    adcInit();

    int tempo = 50;
    //int value = 0;
    //char i, x = 0;

    srand(time(NULL));

    lcdInit();
    kpInit();


    for (;;) {

        lcdCommand(L0);
        kpDebounce();
        if (kpRead() != tecla) {
            tecla = kpRead();
            //condiçoes randomica dos numeros das rodovias
            if (bitTst(tecla, 1)) { //d4 esquerda
                x = rand() % 10;
                y = rand() % 10;
                z = rand() % 10;
            }
            //condicoes aumento e decremento dos preços
            if (bitTst(tecla, 7)) { //d5 direita
                r++;
            }
            if (bitTst(tecla, 8)) { //d6 direita
                r--;
            }
            //----
            if (bitTst(tecla, 6)) { //d4 direita
                d++;
            }
            if (bitTst(tecla, 5)) { //d3 direita
                d--;
            }
        }

        lcdString("Rodovia - BR"); 
        lcdChar(x + 48);
        lcdChar(y + 48);
        lcdChar(z + 48);
        lcdCommand(0xC0);
        lcdString("R$");
        lcdChar(r + 48);
        lcdString(",");
        lcdChar(d + 48);
        lcdChar(c + 48);
        lcdString(" /");


        // simulaçao do relogio 

        lcdChar(((getHours() / 10) % 10) + 48);
        lcdChar((getHours() % 10) + 48);
        lcdChar(':');
        lcdChar(((getMinutes() / 10) % 10) + 48);
        lcdChar((getMinutes() % 10) + 48);
        lcdChar(':');
        lcdChar(((getSeconds() / 10) % 10) + 48);
        lcdChar((getSeconds() % 10) + 48);
        cont++;

        //display 7s
        v = adcRead(0);
        v0 = ((v / 9) % 10);


        digitalWrite(DISP_4_PIN, LOW);

        soWrite(0x00);

        switch (display) {
            case 0:
                // Valor do display
                soWrite(valor[v0]);
                // Liga??o do display
                digitalWrite(DISP_4_PIN, HIGH);
                //Pr?ximo estado a ser executado
                display = 1;
                // Atraso para a multiplexa??o
                v++;
                atraso_ms(100); 
                break;

            default:
                display = 0;
                break;
        }

    }
lcdCommand(0x01);
}




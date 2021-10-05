/**************************
 * File:   main.c                                                           *
 * Author: Esteban Lemos                                                    *
 * Created on 5 de febrero de 2014, 20:49                                   *
 * En caso de necesitar el proyecto completo ingresar en                    *
 * https://github.com/EEST1/Plantilla_6to                                   *
 **************************/


// PARPADEO DEL LED                        PLANTILLA TP //
#include <xc.h>
#include "confbits.h"       //no se debe de modificar este archivo para
//garantizar el funcionamiento del BOOTLOADER

#include "Tecnica1.h"       //segun el shield habilita distintas librerias

#include "ConfShield2_1.h" //cambiar para el teclado // esta viene x defecto
//el teclado utiliza los mismos pines que el entrenador pero la
//incializaci´on esta disponible en el Shield1.3 no en el 2.1.
#include "ConfShield1_3.h"
#include "lcd.h"
#include "teclado.h"
#include "EncoderIncremental.h"

#define LED1  LATDbits.LD1
#define LED2  LATDbits.LD0

float guarda_ADC, kilos, kgrs_pesado, cen_kg, dec_kg, uni_kg, gramos ; //variable para read adc
long int readADC(void); // si utilizo float o char mplab compila en error!!

long int tiempo_ventana_pesaje=5, activador_tiempo_ventana_pesaje=0; //flag y tiempo q la venta esta open
long int casos_valores_pesaje=0;

void descomposicion(void); 
void main(void) {

    PicIni21();

    TRISDbits.RD1 = 0;
    TRISDbits.RD0 = 0;
    
    TIMER0_INI(); //llamo a la funcion tmimer 0
    TMR0ON = 1; //lo prendo
    TIMER1_INI();
    TMR1ON = 1;
    ei(); //habilito las interrupciones
    tecladoIni13(); //inicializo teclado y conexiones etc

    LCD_init(); //inicializo el lcd;   



    while (1) {
        
        descomposicion();
    }
}

void __interrupt myISR(void) {
    
    //TIMMER 1 PARA SEGUNDOS
    if (TMR0IF == 1) { //termino termino de contar el timer?
        TMR0IF = 0; //bajamos el flag
        TMR0L = 0xD2; //el timer contará 47 fosc/4 * 256 = 12032 * 0,0833us
        TMR0H = 0x48; //en total aprox 1.0022ms  casi 1ms
          //    des =~ des;
 
        //aca escrir
        
 
        
    }

    if (TMR1IF == 1) { //TIMER 1 PARA ms
        TMR1IF = 0; //bajamos el flag
        TMR1L = 0x1B; //el timer contará 1200 fosc/4 * 1 = 12032 * 0,0833us
        TMR1H = 0xD1; //en total aprox 99.96us  casi 100us
        tic_teclado();
        tic_LCD();
        if (RBIF == 1) { //si RBIF es 1
            marca_tecla(); //llamo a la funcion marca_tecla
        }
        RBIF = 0;
         
        
    }

}


long int readADC() { // en esta funcion guardo el valor deladc en kilos.
    guarda_ADC = obtener_ADC10(); //acá llamo a la dfuncion y guardo el valor en una variable
    kilos = (guarda_ADC * 999.9) / 1023; // para esto hago la regla de 3simples.
    //5v son 999900gr, o sea, 999.9kilos.
    //entonces si 999.9kg son 1023bits (pq ahora el conversor está en 10 bits tengo 1023 posiciones))
    // (x*999.9kg)/1023= valor de bits 
    return kilos;
}

void descomposicion(){
      set_CURSOR(0x00);
            msg2LCD("PESO: ");
            cen_kg = (readADC() % 1000) / 100;
            dec_kg = ((readADC() % 1000) % 100) / 10;
            uni_kg = ((readADC() % 1000) % 100) % 10;
                    
//            set_CURSOR(0x40);
            char2LCD(cen_kg + 48);
            char2LCD(dec_kg + 48);
            char2LCD(uni_kg + 48);
            
            msg2LCD("Kg");
   
}




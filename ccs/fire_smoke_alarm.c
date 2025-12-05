#include <16F877A.h>
#fuses HS, NOWDT, NOPUT, NOLVP
#use delay(clock = 20000000)

#define LCD_ENABLE_PIN PIN_B2
#define LCD_RS_PIN     PIN_B4
#define LCD_RW_PIN     PIN_B3
#define LCD_DATA4      PIN_D7
#define LCD_DATA5      PIN_D6
#define LCD_DATA6      PIN_D5
#define LCD_DATA7      PIN_D4
#include <lcd.c>

#define MQ2_PIN   PIN_A1 
#define BUZZER    PIN_D2

int8 nd = 0;
int8 da = 0;


void dht11() {
    unsigned int8 raw[5] = {0, 0, 0, 0, 0};
    unsigned int16 timeout;
    int i, j;
    int1 success = 0;

    for (int attempt = 0; attempt < 3 && !success; attempt++) {
        output_drive(PIN_A0);
        output_low(PIN_A0);
        delay_ms(20);
        output_high(PIN_A0);
        delay_us(40);
        output_float(PIN_A0);

        timeout = 0;
        while (input(PIN_A0) && ++timeout < 10000);
        if (timeout >= 10000) continue;
        delay_us(80);

        timeout = 0;
        while (!input(PIN_A0) && ++timeout < 10000);
        if (timeout >= 10000) continue;
        delay_us(80);

        for (j = 0; j < 5; j++) {
            for (i = 0; i < 8; i++) {
                timeout = 0;
                while (!input(PIN_A0) && ++timeout < 20000);
                if (timeout >= 20000) break;

                delay_us(40);
                if (input(PIN_A0)) {
                    bit_set(raw[j], 7 - i);
                }

                timeout = 0;
                while (input(PIN_A0) && ++timeout < 20000);
                if (timeout >= 20000) break;
            }
        }

        if (((raw[0] + raw[1] + raw[2] + raw[3]) & 0xFF) != raw[4]) continue;

        da    = raw[0];
        nd    = raw[2];
        success = 1;
    }

    if (!success) {
        da    = 0xFF;
        nd    = 0xFF;
    }
}

void main() {
 
    set_tris_a(0b00000011); 
    set_tris_b(0b00000000); 
    set_tris_d(0b00000000); 

    lcd_init();
    output_low(BUZZER);

    lcd_gotoxy(1, 1);
    lcd_putc("    HA VU LAM   ");
    lcd_gotoxy(1, 2);
    lcd_putc("    42101323    ");
    delay_ms(3000);
    lcd_putc('\f');   

    lcd_gotoxy(1, 1);
    lcd_putc("    CANH BAO    ");
    lcd_gotoxy(1, 2);
    lcd_putc("  NHET DO - GAS ");
    delay_ms(2000);
    lcd_putc('\f');
     
    while (true) {
        dht11();

        
        lcd_gotoxy(1, 1);
        printf(lcd_putc, "Nhiet:%2uC Am:%2u%%", nd, da);

        int1 gas = !input(MQ2_PIN);    
        int1 quanhiet     = nd > 40;

        lcd_gotoxy(1, 2);
        if (quanhiet && gas) {
            printf(lcd_putc, "QUA NHIET-GAS!! ");
            output_high(BUZZER);
        }
        else if (quanhiet) {
            printf(lcd_putc, "  QUA NHIET!!   ");
            output_high(BUZZER);
        }
        else if (gas) {
            printf(lcd_putc, "   KHI GAS!!    ");
            output_high(BUZZER);
        }
        else {
            printf(lcd_putc, "   BINH THUONG  ");
            output_low(BUZZER);
        }

        delay_ms(1500);
    }
}

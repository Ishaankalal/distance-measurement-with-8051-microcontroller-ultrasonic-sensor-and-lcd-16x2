//code 

#include <REGX52.h> 
#include <stdio.h> 
 
#define dataport P2 
 
sbit trig = P3^5; 
sbit echo = P3^2; 
sbit rs = P3^1; 
sbit rw = P3^3; 
sbit e = P3^4; 
 
 
int cms; 
 
void delay(unsigned int msec) { 
    unsigned int i, j; 
    for(i = 0; i < msec; i++) 
        for(j = 0; j < 1275; j++); 
} 
 
void lcd_cmd(unsigned char item) { 
    dataport = item; 
    rs = 0; 
    rw = 0; 
    e = 1; 
    delay(1); 
    e = 0; 
} 
 
void lcd_data(unsigned char item) { 
    dataport = item; 
    rs = 1; 
    rw = 0; 
    e = 1; 
    delay(1); 
    e = 0; 
} 
 
void lcd_data_string(unsigned char *str) { 
    while(*str != '\0') { 
        lcd_data(*str); 
        str++; 
        delay(1); 
    } 
} 
 
void send_pulse(void) { 
    TH0 = 0x00; 
    TL0 = 0x00; 
    trig = 1; 

 
 
    delay(5); 
    trig = 0; 
} 
 
unsigned int get_range(void); 
 
void int_to_string(int num, char *buffer) { 
    sprintf(buffer, "%d", num); 
} 
 
unsigned int get_range(void) { 
    long int timer_val; 
 
    send_pulse(); 
    while(!echo); 
    while(echo); 
    timer_val = (TH0 << 8) + TL0; 
 
    return (timer_val < 38000) ? timer_val / 53 : -1; 
} 
 
void display_range(int range) {  
    char buffer[16]; 
    lcd_cmd(0x01); 
    lcd_cmd(0x80); 
    lcd_data_string("distance is  : "); 
 
    if (range != -1) { 
        char buffer[16]; 
        int_to_string(range, buffer); 
        lcd_data_string(buffer); 
        lcd_data_string("cm."); 
    } else { 
        lcd_cmd(0x01); 
        lcd_data_string("Object out of range"); 
    } 
} 
 
void initialize_lcd() { 
    lcd_cmd(0x38); 
    delay(5); 
    lcd_cmd(0x0C); 
    delay(5); 
    lcd_cmd(0x01); 
    delay(5); 
    lcd_cmd(0x80); 
    delay(5); 
} 
 
 
void main() { 
    initialize_lcd(); 
 
 
 
    lcd_data_string("Start"); 
    delay(100); 
 
    TMOD = 0x09; 
    TR0 = 1; 
    TH0 = 0x00; 
    TL0 = 0x00; 
 
    while(1) { 
        int range = get_range(); 
        display_range(range); 
        delay(10); 
    } 
} 

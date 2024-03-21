/*
 * Name:Dinesh A
 * Name of the Project: Car black box
 * Date:7-02-2024

*/

#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "adc.h"
#include "eeprom.h"
#include "isr.h"
#include "matrix_keypad.h"
#include "uart.h"
#include "timer.h"

unsigned char clock_reg[3];
unsigned char time[9];
unsigned char gear[8][3] = {"GN","GR","G1","G2","G3","G4"};
unsigned int index = -1;
unsigned int event = 0;
unsigned char address = 0x00;
unsigned int fl0 = 0;

void display_time(void)
{
    clcd_print(time,LINE2(0));
}

static void get_time(void)
{
    clock_reg[0] = read_ds1307(HOUR_ADDR);
    clock_reg[1] = read_ds1307(MIN_ADDR);
    clock_reg[2] = read_ds1307(SEC_ADDR);

    if (clock_reg[0] & 0x40)
    {
	time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
	time[1] = '0' + (clock_reg[0] & 0x0F);
    }
    else
    {
	time[0] = '0' + ((clock_reg[0] >> 4) & 0x0f);
	time[1] = '0' + (clock_reg[0] & 0x0F);
    }
    time[2] = ':';
    time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
    time[4] = '0' + (clock_reg[1] & 0x0F);
    time[5] = ':';
    time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
    time[7] = '0' + (clock_reg[2] & 0x0F);
    time[8] = '\0';
}


void init_config(void)
{
    init_clcd();
    init_i2c();
    init_ds1307();
    init_adc();
    init_matrix_keypad();
    init_uart();

}

void display_speed( unsigned long int adc_val )
{
    clcd_putch( '0'+(((adc_val*99)/1023)/10) ,LINE2(14));
    clcd_putch( '0'+(((adc_val*99)/1023)%10) ,LINE2(15));    
}

void display_gear( unsigned char key )
{
    if( key == MK_SW1 )
    {
	clcd_print("C ",LINE2(10));
	index = -1;
    }
    else if( key == MK_SW2 )
    {
	if( index == 6 )
	    index = 6;
	else if ( index == 7 )
	    index = 0;
	else
	    index++;
	clcd_print(gear[index],LINE2(10));
    }
    else if( key == MK_SW3 )
    {
	if( index == 0 )
	    index = 0;
	else if( index == -1 || index == 7 )
	    index = 7;
	else
	    index--;
	clcd_print(gear[index],LINE2(10));
    }
}

void store_eeprom( unsigned char key, unsigned long adc_val )
{
    if( key == MK_SW1 || key == MK_SW2 || key == MK_SW3 )
    {
	if( event % 10 == 0 )
	{
	    address = 0;
	}
	for( int i = 0; i < 8 ; i++ )
	{
	    write_eeprom(address++,time[i]);
	}
	write_eeprom(address++,' ');
	if( index == -1 )
	{
	    write_eeprom( address++,'C');
	    write_eeprom( address++,' ');
	}
	else
	{
	    for( int i = 0; i < 2 ; i++ )
	    {
		write_eeprom(address++,gear[index][i]);
	    }
	}
	write_eeprom(address++,' ');
	write_eeprom( address++, ('0'+(((adc_val * 99)/1023) / 10)));
	write_eeprom( address++, ('0'+(((adc_val * 99)/1023) % 10)));      
	event++;
    }

}

void main(void)
{
    init_config();

    unsigned char key  = 0;
    unsigned char once = 0 ;
    unsigned char flag = 1 ;
    unsigned long adc_val;
/*    for( unsigned int i = 0; i < 160 ; i++ )
    {
	write_eeprom(i, '0' );
    }
    for( unsigned char i = 0; i < 4 ; i++ )
    {
	write_eeprom(200+i, '0' );
    }
*/
    while(1)
    {
	get_time();
	key = read_switches(STATE_CHANGE);
	if( (key == MK_SW11) && once == 0 )
	{
	    enter_menu(key);
	    clcd_print("                ",LINE2(0) );
	    clcd_print(gear[index],LINE2(10));
	    once = 1;
	}
	else if( key == MK_SW11 )
	    once = 0;

	if( flag == 1 )
	{
	    clcd_print("ON  ",LINE2(10) );
	}
	clcd_print("  TIME  EV SP", LINE1(0));
	if( key != ALL_RELEASED )
	{
	    flag = 0;
	}
	display_gear(key);
	display_time();
	display_speed( adc_val = read_adc(CHANNEL4) );
	store_eeprom(key,adc_val);
	if( index == -1 )
	{
	    clcd_print("-C ",LINE2(10));
	}
    }
}

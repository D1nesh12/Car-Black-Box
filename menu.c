/*
 * Name:Dinesh A
 * Name of the Project: Car black box
 * Date:7-02-2024

*/

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

void view_log();
void clear_log();
void download_log();
void set_time();
void change_password();

extern unsigned int fl0;
extern unsigned char time[9];
extern unsigned int event;
unsigned char pass_key;
unsigned char pass[4];
unsigned char arr[4];

void enter_menu( unsigned char key )
{
    int add=0,k=0,delay=0,delay1=0,count=0,input=0,len=6,i=0,tr=0,blink=0,pos = 0,goback = 0;
    unsigned char menu[5][16] = {"1 View Log    ","2 Clear Log   ","3 Download Log","4 Set Time    ","5 Change Passwd "};
    unsigned char once1;
    unsigned int delay0 = 0;
    unsigned int ti0 = 0;
    unsigned char scr_key, long_key, count1 = 0, fl02 = 1, fl021 = 0, fl022 = 0, fl03 = 1;
    unsigned long ti1 = 0;

    for( unsigned char b = 0 ; b < 4 ; b++ )
    {
	pass[b] = read_eeprom(0);
    }

    clcd_print( "                ", LINE2(0) );
    clcd_print( "Enter Password:   ", LINE1(0) );

    while( 1 )
    {
	if( input < 3 )
	{
	    pass_key = read_switches(STATE_CHANGE);
	    if( len < 10 )
	    {
		if( goback == 3 )
		{
		    goback = 0;
		    return;
		}
		if( blink++ < 10 )
		{
		    clcd_putch(' ',LINE2(len));
		}
		else
		{
		    if( blink == 20 )
		    {
			goback++;
			blink=0;
		    }
		    clcd_putch(-1,LINE2(len));
		}

		if ( pass_key == MK_SW11 )
		{
		    clcd_putch('*',LINE2(len));
		    arr[pos++] = '0';
		    len++;
		}
		if( pass_key == MK_SW12 )
		{
		    clcd_putch('*',LINE2(len));
		    arr[pos++] = '1';
		    len++;
		}
		for( k=50000; i--;);
	    }
	    else
	    {
		while( i < 4 )
		{
		    if( pass[i] == arr[i] )
		    {
			count++;
		    }
		    i++;
		}
		if(count == 4)
		{

		    while( 1 )
		    {
			long_key = read_switches( LEVEL_CHANGE );

			if( long_key == MK_SW11 )
			{
			    if( ti0++ == 300 )
			    {
				if( count1 == 0 && fl02 == 1 )
				{
				    view_log();
				}
				else if( count1 == 0 && fl02 == 0 )
				{
				    clear_log();
				}
				else if( count1 == 1 && fl02 == 0 )
				{
				    download_log();
				}
				else if( count1 == 2 && fl02 == 0 )
				{
				    set_time();
				}
				else if( count1 == 3 && fl02 == 0 )
				{
				    change_password();
				}
				else if( count1 == 3 && fl02 == 1 )
				{
				    set_time();
				}
				else if( count1 == 2 && fl02 == 1 )
				{
				    download_log();
				}
				else if( count1 == 1 && fl02 == 1 )
				{
				    clear_log();
				}
				else if( count1 == 0 && fl02 == 1 )
				{
				    view_log();
				}
				ti0 = 0;
			    }
			    else if( ti0 > 300 )
			    {
				ti0 = 301;
			    }
			}
			else if( ti0 > 0 && ti0 < 300 )
			{
			    ti0 = 0;
			    if(count1 == 0 )
			    {
				count1 = 0;
			    }
			    else if( fl022 == 1 )
			    {
				count1--;
			    }
			    fl02 = 1;
			    fl022 = 1;
			    fl021 = 0;
			}
			if( long_key == MK_SW12 )
			{
			    if( delay0++ == 300 )
			    {
				// Long press
				return;
			    }
			    else if( delay0 > 300 )
			    {
				delay0 = 301;
			    }
			}
			else if( delay0 > 0 && delay0 < 300  )
			{

			    // Short press
			    delay0 = 0;
			    if( count1 == 3 )
			    {
				count1 = 3;
			    }
			    else if( fl021 == 1 )
			    {
				count1++;
			    }
			    fl02 = 0;
			    fl021 = 1;
			    fl022 = 0;
			}
			clcd_print( menu[count1],LINE1(2) );
			clcd_print( menu[count1+1],LINE2(2) );
			if( fl02 == 1 )
			{
			    clcd_print( "* ", LINE1(0));
			    clcd_print( "  ", LINE2(0) ); 
			}
			else
			{
			    clcd_print( "  ", LINE1(0) ); 
			    clcd_print( "* ", LINE2(0) ); 
			}
		    }
		}
		else
		{
		    clcd_print("Try again       ",LINE2(0));
		    if( delay1 ++ == 200 )
		    {
			delay1=0;
		    }
		    if(tr++==2000 )
		    {
			i=0;
			clcd_print("                ",LINE2(0));
			input++;
			add=0;
			len=6;
			count=0;
			blink=0;
			tr=0;
			pos = 0;
		    }
		}
	    }
	}
	else
	{
	    clcd_print("TRY AGAIN AFTER",LINE1(0));
	    clcd_print("     3 MIN     ",LINE2(0));
	    for(unsigned long u = 5000000 ; u-- ;);
	    return;
	}
    }
}

void view_log()
{
    clcd_print("                ", LINE2(0) );
    clcd_print("                ", LINE1(0) );
    unsigned char key1;
    unsigned char pos = '0';
    unsigned long ti2 = 0;
    unsigned long ti21 = 0;
    unsigned long addr1 = 0;
    clcd_print("Logs:           ", LINE1(0));
    clcd_putch( pos, LINE2(0) );
    if( event > 10)
    {
	event = 10;
    }
    for( int i = 0; i < 14 ; i++ )
    {
	clcd_putch( read_eeprom(addr1+i),LINE2(i+2) );
    }
    while(1)
    {
	key1 = read_switches(LEVEL_CHANGE);
	if( event == 0 )
	{
	    clcd_print("   Logs Empty   ",LINE2(0));
	    if( ti2++ == 1000 )
	    {
		ti2 = 0;
		return;
	    }
	}
	else
	{
	    if( key1 == MK_SW12 )
	    {
		if( ti2++ == 300 )
		{
		    ti2 = 0;
		    return;
		}
		else if( ti2 > 300 )
		{
		    ti2 = 301;
		}
	    }
	    else if( ti2 > 0 && ti2 < 300 )
	    {
		ti2 = 0;
		if( pos == (event+'0'-1) )
		{
		    pos = (event + '0'-1);
		    addr1 = (event * 14)-14;
		}
		else
		{
		    pos++;
		    addr1 = addr1 + 14;
		}
	    }
	    if( key1 == MK_SW11 )
	    {
		if( ti21++ == 500 )
		{
		}
		else if( ti21 > 500 )
		{
		    ti21 = 501;
		}	    
	    }
	    else if( ti21 > 0 && ti21 < 500 )
	    {
		ti21 = 0;
		if( pos == '0' && addr1 == 0 )
		{
		    pos = '0';
		    addr1 = 0;
		}
		else
		{
		    pos--;
		    addr1 = addr1 - 14;
		}
	    }
	    clcd_putch( pos, LINE2(0) );
	    for( int i = 0; i < 14 ; i++ )
	    {
		clcd_putch( read_eeprom(addr1+i),LINE2(i+2) );
	    }
	}
    }
}

void clear_log()
{
    event = 0;
    unsigned long ti3 = 0;
    while(1)
    {
	clcd_print("Clear Log:      ",LINE1(0));
	clcd_print("Logs Are Cleared",LINE2(0));
	if( ti3++ == 1000 )
	{
	    ti3 = 0;
	    return;
	}

    }
}


void download_log()
{
    unsigned long ti4 = 0;
    unsigned char k1 = 0;
    char data[16];
    unsigned int addr2 = 0;
    if( event == 0 )
    {
	puts("Downloaded Details from Block Box\n\r");
	puts("Logs Are Empty!\n\r");
    }
    else
    {
	puts("Downloaded Details from Block Box\n\r");
	puts("  Time     Gear Spd \n\r");

	for( int i = 0; i < 10 && i < event ; i++ )
	{
	    for( int j = 0; j < 14 ; j++ )
	    {
		data[j] = read_eeprom(addr2++);
	    }
	    data[14] = '\n';
	    data[15] = '\r';
	    puts("  ");
	    puts(data);
	}
    }
    while(1)
    {
	clcd_print("Download Log:      ",LINE1(0));
	clcd_print(" Uart Successful",LINE2(0));
	if( ti4++ == 1000 )
	{
	    ti4 = 0;
	    return;
	}
    }
}


void set_time()
{
    unsigned long ti5 = 0, ti51 = 0, wait = 0, wait1 = 0, wait2 = 0;
    unsigned char k2 = 0;

    CLEAR_DISP_SCREEN;

    unsigned char bcd_sec = read_ds1307(SEC_ADDR),sec;
    unsigned char bcd_min = read_ds1307(MIN_ADDR),min;
    unsigned char bcd_hr = read_ds1307(HOUR_ADDR),hr;


    sec = (( bcd_sec >> 4 ) & 0x0f ) * 10;
    sec = ( bcd_sec & 0x0f ) + sec;


    min = (( bcd_min >> 4 ) & 0x0f ) * 10;
    min = ( bcd_min & 0x0f ) + min;

    hr = (( bcd_hr >> 4 ) & 0xff ) * 10;
    hr = ( bcd_hr & 0x0f ) + hr;

    char field = 0;

    while(1)
    {
	if(wait++ < 150)
	{
	    clcd_putch(hr / 10 + 48, LINE2(5));
	    clcd_putch(hr % 10 + 48, LINE2(6));
	}
	else if(wait > 150 && wait < 300 && field == 2)
	{
	    clcd_putch(' ', LINE2(5));
	    clcd_putch(' ', LINE2(6));
	}
	else
	    wait = 0;

	clcd_putch(':', LINE2(7));

	if( wait1++ < 150 )
	{
	    clcd_putch(min / 10 + 48, LINE2(8));
	    clcd_putch(min % 10 + 48, LINE2(9));
	}
	else if( wait1 > 150 && wait1 < 300 && field == 1 )
	{
	    clcd_putch(' ', LINE2(8));
	    clcd_putch(' ', LINE2(9));
	}
	else
	    wait1 = 0;

	clcd_putch(':', LINE2(10));

	if( wait2++ < 150 )
	{
	    clcd_putch(sec / 10 + 48, LINE2(11));
	    clcd_putch(sec % 10 + 48, LINE2(12));
	}
	else if( wait2 > 150 && wait2 < 300 && field == 0 )
	{
	    clcd_putch(' ', LINE2(11));
	    clcd_putch(' ', LINE2(12));
	}
	else
	    wait2 = 0;

	k2 = read_switches(LEVEL_CHANGE);
	clcd_print("Set Time:      ",LINE1(0));

	if( k2 == MK_SW12 )
	{
	    if( ti5++ == 200 )
	    {
		ti5 = 0;

		break;
	    }
	    else if( ti5 > 200 )
	    {
		ti5 = 201;
	    }
	}
	else if( ti5 > 0 && ti5 < 200 )
	{
	    ti5 = 0;

	    field++;

	    if(field > 2)
		field = 0;
	}
	if( k2 == MK_SW4 )
	{
	    if( ti51++ == 300 )
	    {
		ti51 = 0;
	    }
	    else if( ti51 > 300 )
	    {
		ti51 = 301;
	    }
	}
	else if( ti51 > 0 && ti51 < 300 )
	{
	    ti51 = 0;
	    if(field == 0)
	    {
		sec++;
		if(sec > 59)
		    sec = 0;
	    }
	    else if(field == 1)
	    {
		min++;
		if(min > 59)
		    min = 0;
	    }
	    else if(field == 2)
	    {
		hr++;
		if(hr > 23)
		    hr = 0;
	    }
	}
    }

    bcd_sec = (sec % 10) & bcd_sec;
    bcd_sec = ((sec / 10) << 4 ) | bcd_sec;
    write_ds1307(SEC_ADDR, bcd_sec);

    bcd_min = (min % 10) & bcd_min;
    bcd_min = ((min / 10) << 4 ) | bcd_min;
    write_ds1307(MIN_ADDR, bcd_min);

    bcd_hr = ((hr % 10) & 0x0f) | (0 & bcd_hr);
    bcd_hr = ((hr / 10) & 0xf0) | bcd_hr;
    write_ds1307(HOUR_ADDR, bcd_hr);
}

void change_password()
{
    unsigned int pass_key3 = 0,k30 = 0, len3 = 6,blink3 = 0,pos3 = 0,count30 = 0, i3 = 0,delay6 = 0;
    unsigned char arr5[4];
    unsigned long ti6 = 0, s_delay = 0;
    unsigned int k10 = 0,cn_flag=0;;
    unsigned char arr1[4];
    unsigned int input1 = 0,pass_key1 = 0,len1 = 6,blink1 = 0,pos1 = 0,count10 = 0, i1 = 0,delay4 = 0;
    clcd_print("Enter Old Password:",LINE1(0));
    clcd_print("                ",LINE2(0));
    for( unsigned long l = 90000 ; l-- ; );
    while(1)
    {
	pass_key1 = read_switches(STATE_CHANGE);
	if( len1 < 10 )
	{
	    if( blink1++ < 10 )
	    {
		clcd_putch(' ',LINE2(len1));
	    }
	    else
	    {
		if( blink1 == 20 )
		{
		    blink1=0;
		}
		clcd_putch(-1,LINE2(len1));
	    }

	    if ( pass_key1 == MK_SW11 )
	    {
		clcd_putch('*',LINE2(len1));
		arr1[pos1++]='0';
		len1++;
	    }
	    if( pass_key1 == MK_SW12 )
	    {
		clcd_putch('*',LINE2(len1));
		arr1[pos1++] = '1';
		len1++;
	    }
	    for ( k10 = 50000 ; k10--;);
	}
	else
	{
	    while( i1 < 4 )
	    {
		if( pass[i1] == arr1[i1] )
		{
		    count10++;
		}
		i1++;
	    }
	    if(count10 == 4)
	    {
		unsigned int pass_key2 = 0,k20 = 0, len2 = 6,blink2 = 0,pos2 = 0,count20 = 0, i2 = 0,delay5 = 0;
		unsigned char arr4[4];
		clcd_print("Enter Password:  ",LINE1(0));
		clcd_print("                ",LINE2(0));
		while(1)
		{
		    pass_key2 = read_switches(STATE_CHANGE);
		    if( len2 < 10 )
		    {
			if( blink2++ < 10 )
			{
			    clcd_putch(' ',LINE2(len2));
			}
			else
			{
			    if( blink2 == 20 )
			    {
				blink2=0;
			    }
			    clcd_putch(-1,LINE2(len2));
			}

			if ( pass_key2 == MK_SW11 )
			{
			    clcd_putch('*',LINE2(len2));
			    arr4[pos2] = '0';
			    len2++;
			    pos2++;
			}
			if( pass_key2 == MK_SW12 )
			{
			    clcd_putch('*',LINE2(len2));
			    arr4[pos2] = '1';
			    len2++;
			    pos2++;
			}
			for ( k20 = 50000 ; i2--;);
		    }
		    else 
			cn_flag=1;
		    if(cn_flag)
		    {
			clcd_print("Renter new Password:",LINE1(0));
			clcd_print("                ",LINE2(0));
			while(1)
			{
			    pass_key3 = read_switches(STATE_CHANGE);
			    if( len3 < 10 )
			    {
				if( blink3++ < 10 )
				{
				    clcd_putch(' ',LINE2(len2));
				}
				else
				{
				    if( blink3 == 20 )
				    {
					blink3=0;
				    }
				    clcd_putch(-1,LINE2(len3));
				}

				if ( pass_key3 == MK_SW11 )
				{
				    clcd_putch('*',LINE2(len3));
				    arr5[pos3] = '0';
				    len3++;
				    pos3++;
				}
				if( pass_key3 == MK_SW12 )
				{
				    clcd_putch('*',LINE2(len3));
				    arr5[pos3] = '1';
				    len3++;
				    pos3++;
				}
				for ( k30 = 50000 ; i3--;);
			    }
			    else
			    {
				for( i3 = 0; i3 < 4 ; i3++ )
				{
				    if( arr4[i3] == arr5[i3] )
					count30++;
				}
				if( count30 == 4 )
				{
				    for( int h = 0 ; h < 4 ; h++ )
				    {
					write_eeprom( ( 200 + h ) , arr5[h] );
					pass[h] = arr5[h];
				    }
				    cn_flag=0;
				    clcd_print(" Passwd Changed ",LINE1(0));
				    clcd_print("**Successfull** ",LINE2(0));
				    if( delay6++ == 500 )
				    {
					delay6 = 0;	
					return;
				    }
				}
				else
				{
				    clcd_print("    Password    ",LINE1(0));
				    clcd_print("  Not Matched   ",LINE2(22));
				    if( delay6++ == 1000 ) 
				    {
					delay6 = 0;
					return;
				    }
				}
			    }
			}
		    }
		}
	    }
	    else
	    {
		clcd_print(" Wrong Password ",LINE1(0));
		clcd_print("                ",LINE2(0));
		if( delay4++ == 2000 )
		{
		    delay4 = 0;
		    return;
		}
	    }
	}

    }
}


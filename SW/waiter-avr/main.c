#define F_CPU 16000000UL
#include<avr/io.h>
#include<util/delay.h>
#include<stdio.h>

#define rs PB2
#define en PB3
#define Rows PORTA //Pc0,pc1,pc2,pc3

void lcd_init();
void dis_cmd(char);
void dis_data(char);
void lcdcmd(char);
void lcddata(char);
void dis_print(char*);
void lcd_clear();
void lcd_clear_animation();
void second_line();
void first_line();
int press_key();
void show_menu();

const char ITEMS[12][10] = {
	"Beef",
	"Pizza",
	"Kabab",
	"Chicken",
	"Soup",
	"Tofu",
	"Sushi",
	"Steak",
	"Soda",
	"*",
	"Salad",
	"#"
};
const char KEYS[12] = {
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'*',
	'0',
	'#'
};

int main(void)
{
	DDRB=0xFF;
	lcd_init();

	dis_print("Welcome");
	second_line();
	dis_print("to our resturant");
	_delay_ms(50);
	
	show_menu();	
	

	while(1)
	{	
		int k = press_key();
		if(k){
			if (k==9){
				lcd_clear();
				dis_print("press a key");
				continue;
			}else if(k==11){
				lcd_clear();
				dis_print("Finished");
				break;
			}
			lcd_clear();
			char str[16];
			first_line();
			sprintf(str,"%c: %s",KEYS[k],ITEMS[k]);
			dis_print(str);
			second_line();
			dis_print("*:add, #:finish");
			_delay_ms(10);

		}
		
	}
	
}

int press_key()
{
	unsigned char c,r;
	
	DDRA=0xf0;
	PORTA=0xff;
	int k[4][3]={{0,1,2},{3,4,5},{6,7,8},{9,10,11}};
	
	do{
		PORTA &= 0x0f;
		r = PINA & 0x0f;
	} while(r!=0x0f);
		
	do{
		r = PINA & 0x0f;
	} while(r==0x0f);
		
	while(1){
		PORTA = 0xef;
		r = PINA & 0x0f;
		if(r!=0x0f){
			c=0;
			break;
		}
		PORTA = 0xdf;
		r = PINA & 0x0f;
		if(r!=0x0f){
			c=1;
			break;
		}
		PORTA = 0xbf;
		r=PINA&0x0f;
		if(r!=0x0f)
		{
			c=2;
			break;
		}
	}
	if(r==0x0e) return k[0][c];
	else if(r==0x0d) return k[1][c];
	else if(r==0x0b) return k[2][c];
	else return k[3][c];
}

void show_menu(){
	lcd_clear();
	dis_print("***** MENU *****");
	_delay_ms(30);
	for(int i =0; i<9; i+=2){
		lcd_clear();
		char str[16];
		sprintf(str,"%s: press %c",ITEMS[i],KEYS[i]);
		dis_print(str);
		second_line();
		if(i==8)i++;
		sprintf(str,"%s: press %c",ITEMS[i+1],KEYS[i+1]);
		 dis_print(str);
		_delay_ms(40);
		first_line();
	}
}

void second_line(){
	dis_cmd(0xC0);
}

void first_line(){
	dis_cmd(0x80);
}

void lcd_clear_animation(){
	for(int x=0;x<16;x++)
	{
		dis_cmd(0x1c);
		_delay_ms(5);
	}
	lcd_clear();
}

void lcd_clear(){
	dis_cmd(0x01);
}

void lcd_init()
{
	dis_cmd(0x02);		// to initialize LCD in 4-bit mode.
	dis_cmd(0x28);
	dis_cmd(0x0C);
	dis_cmd(0x06);
	dis_cmd(0x83);
}

void dis_cmd(char cmd_value)
{
	char cmd_value1;
	
	cmd_value1 = cmd_value & 0xF0;		//mask lower nibble because PA4-PA7 pins are used.
	lcdcmd(cmd_value1);			// send to LCD
	
	cmd_value1 = ((cmd_value<<4) & 0xF0);	//shift 4-bit and mask
	lcdcmd(cmd_value1);			// send to LCD
}

void dis_data(char data_value)
{
	char data_value1;
	
	data_value1=data_value&0xF0;
	lcddata(data_value1);
	
	data_value1=((data_value<<4)&0xF0);
	lcddata(data_value1);
}

void lcdcmd(char cmdout)
{
	PORTB=cmdout;
	PORTB&=~(1<<rs);
	PORTB|=(1<<en);
	_delay_ms(1);
	PORTB&=~(1<<en);
}

void lcddata(char dataout)
{
	PORTB=dataout;
	PORTB|=(1<<rs);
	PORTB|=(1<<en);
	_delay_ms(1);
	PORTB&=~(1<<en);
}

void dis_print(char* p){
	while(*p){
		dis_data(*p++);
	}
}
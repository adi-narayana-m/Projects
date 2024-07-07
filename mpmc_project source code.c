#include <reg51.h>
#include<intrins.h>
#define lcd_data P2

sbit rs=P0^0;
sbit rw=P0^1;
sbit en=P0^2;
sbit d=P1^1;
sbit F=P1^0;
sbit L=P1^1;
sbit T=P1^7;
sfr16 DPTR=0x82;
sbit trig=P3^5;
sbit echo=P3^2;
unsigned char range=0;
char str;
char Charin=0;
void lcd_init();
void cmd(unsigned char a);
void dat(unsigned char b);
void show(unsigned char *s);
void lcd_delay();

void lcd_init()
{
    cmd(0x38);
    cmd(0x0e);
    cmd(0x01);
    cmd(0x06);
    cmd(0x0c);
    cmd(0x80);
}

void cmd(unsigned char a)
{
    lcd_data=a;
    rs=0;
    rw=0;
    en=1;
    lcd_delay();
    en=0;
}

void dat(unsigned char b)
{
    lcd_data=b;
    rs=1;
    rw=0;
    en=1;
    lcd_delay();
    en=0;
}

void show(unsigned char *s)
{
    while(*s) {
        dat(*s++);
    }
}


void delay(int time)

{

 unsigned int i,j;

 for(i=0;i<time;i++)

 for(j=0;j<1275;j++);

}


void Serialwrite(char byte)

{

  SBUF=byte;

  while(!TI);

  TI=0;

}


void Serialprintln(char *p)

{

  while(*p)

  {

    Serialwrite(*p);

    p++;

  }

  Serialwrite(0x0d);

}


void Serialbegin()

{

   TMOD=0x20;

   SCON=0x50;

   TH1=0xfd;

   TR1=1;

}

void lcd_delay()
{
  unsigned int i;
  for(i=0;i<=1000;i++);
}
void send_pulse(void) 
{
  TH0=0x00;TL0=0x00; 
   trig=1;
   _nop_();_nop_();_nop_();_nop_();_nop_();
   _nop_();_nop_();_nop_();_nop_();_nop_();
   trig=0;
}
unsigned char ultrasonic()
{
  unsigned char get;
  send_pulse();
  while(!echo);
  while(echo);
  DPH=TH0;
  DPL=TL0;
  TH0=TL0=0xff;
  if(DPTR<38000)
    get=DPTR/59;
  else
    get=0;
  return get;
}


int main()
{
	    
  P2=0x00;
  Serialbegin();
  Serialprintln("System Ready...");
  delay(50);
  
    lcd_init();
    while(1) {
    while(!RI);

    Charin=SBUF;

    str=Charin;

    RI=0;

      if(str=='1')

      {

        F=1;
				L=1;
				cmd(0x80);
				show("NORMAL MODE     ");
				cmd(0xc0);
				show("APPLIANCES OFF  ");

        Serialprintln(" ALL APPLIANCES OFF\n");

        delay(50);

      }
			      if(str=='2')

      {

        F=0;
				L=0;
				cmd(0x80);
				show("NORMAL MODE     ");
							cmd(0xc0);	
        show("APPLIANCES ON   ");
        Serialprintln(" ALL APPLIANCES ON\n");

        delay(50);

      }
			      if(str=='3')

      {

        //F=1;
				L=1;
				cmd(0x80);
				show("NORMAL MODE     ");
				cmd(0xc0);
				show("LIGHTS OFF      ");

        Serialprintln(" ALL LIGHTS OFF\n");

        delay(50);

      }
			      if(str=='4')

      {F=1;
				//L=1;
				cmd(0x80);
				show("NORMAL MODE     ");
				cmd(0xc0);
				show("FANS OFF        ");
        Serialprintln(" ALL FANS OFF\n");
        delay(50);
     }
			      if(str=='5')      {
        F=0;
				//L=0;
				cmd(0x80);
				show("NORMAL MODE     ");
				cmd(0xc0);
				show("ALL FANS ON     ");
				Serialprintln("ALL FANS ON\n");
        delay(50);
      }
			      if(str=='6')  {   
							//F=0;
				L=0;
				cmd(0x80);
				show("NORMAL MODE     ");
				cmd(0xc0);
				show("ALL LIGHTS ON   ");
        Serialprintln("ALL LIGHTS ON\n");
        delay(50);

      }
		        if(str=='7')      {
				cmd(0x80);
				show("NORMAL MODE     ");
				  TMOD=0x09;
  TH0=TL0=0;
  TR0=1;
  lcd_init();
		cmd(0xc0);
  show("DISTANC         ");
  P3|=(1<<2);
  while(1) {
    cmd(0x84);
    range=ultrasonic();
		dat((range/100)+48);
    dat(((range/10)%10)+48);
    dat((range%10)+48);
		if(range>10){
		T=1;
		}
		else{
			T=0;		}
 }
						}
    }
}







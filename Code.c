#include<reg52.h>

#define LCD_Data P2
#define keyport P1

sbit IR11 = P0^0;
sbit IR12 = P0^1;
sbit IR21 = P0^2;
sbit IR22 = P0^3;

sbit relay1 = P3^6;
sbit relay2 = P3^7;

sbit RS = P3^2; 
sbit RW = P3^3; 
sbit EN = P3^4; 

unsigned char keypad[4][4] = {{'1','2','3','A'},
															{'4','5','6','B'},
															{'7','8','9','C'},
															{'*','0','#','D'} };
unsigned char colloc, rowloc;
unsigned char key_detect();
unsigned char temp[10] = {'0','0','1','2','3','4','5','6','7','8'};
int i;

void Show(int, int, int, int);
void Serial(int, int, int, int, char *ptr1, char *ptr2);
void SERIAL_INIT(void);
void SERIAL_STRING_TX(unsigned char *tx_String);
void count1check(int, int);
void count2check(int, int);
void countfull(int, int ,int, int, char *ptr1, char *ptr2);
void delay(unsigned int);
void LCD_Command(char Command);
void LCD_Char(char Data);
void LCD_String(unsigned char *ptr);
void LCD_String_xy(unsigned char row,unsigned char column, unsigned char *);
void LCD_Init();
void main()
{	

	int count1 = 0;
  int count2 = 0;
	char A[3];
	char time1[7] = "xx xx";
	char time2[7] = "xx xx";
	int leave1 = 0;
	int leave2 = 0;
	unsigned char s;
	
  LCD_Init();
	Serial(count1, count2, leave1, leave2, time1, time2);
	Show(count1, count2, leave1, leave2);

  while(1) {
		DISP:
		
		keyport=0xF0;
		if(IR11 == 1 && IR12 == 0) {
      count1++;
			Serial(count1, count2, leave1, leave2, time1, time2);
			count1check(count1, leave1);
      delay(500);
    }
    else if(IR11 == 0 && IR12 == 1) {
			if(count1 != 0)
				count1--;
			Serial(count1, count2, leave1, leave2, time1, time2);
			count1check(count1, leave1);
				delay(500);
		}
    if(IR21 == 1 && IR22 == 0) {
			count2++;
			Serial(count1, count2, leave1, leave2, time1, time2);
			count2check(count2, leave2);		
      delay(500);
    }
    else if(IR21 == 0 && IR22 == 1) {
			if(count2 != 0) 
				count2--;
			Serial(count1, count2, leave1, leave2, time1, time2);
      count2check(count2, leave2);
			delay(500);
		}		

		
		if(keyport != 0xF0){
			LCD_Command(0x01);
			LCD_String_xy(1, 0, "STUDENTS PRESS 1");
			LCD_String_xy(2, 0, "TEACHERS PRESS 2");
			s = key_detect();
			if(s == '1') {
				LCD_Command(0x01);
				LCD_String_xy(1, 0, "ENTER PASSWORD");
				delay(30);
				for(i = 0;i < 3; i++){
					A[i] = key_detect();
					LCD_String_xy(2, i, "*");
				}
				if(A[0] == '1' && A[1] == '2' && A[2] == '3'){
					LCD_String_xy(3, 0, "CORRECT");
					delay(1000);
					LCD_Command(0x01);
					
					countfull(count1, count2, leave1, leave2, time1, time2);
					
					LCD_String_xy(3, 0, "PRESS 1 TO EXIT ");
					s = key_detect();
					if(s == '1'){
						Show(count1, count2, leave1, leave2);
						goto DISP;
					}
				}
				else{
					LCD_String_xy(3, 0, "INCORRECT");
					delay(1000);
					Show(count1, count2, leave1, leave2);
					goto DISP;
				}
			}
			else if(s == '2') {
				
				LCD_Command(0x01);
				LCD_String_xy(1, 0, "ENTER PASSWORD");
				delay(30);
				for(i = 0;i < 3; i++){
					A[i] = key_detect();
					LCD_String_xy(2, i, "*");
				}
				if(A[0] == '4' && A[1] == '5' && A[2] == '6'){
					LCD_String_xy(3, 0, "CORRECT");
					delay(1000);
				
					LCD_Command(0x01);
					LCD_String_xy(1, 0, "T1 PRESS 1");
					LCD_String_xy(2, 0, "T2 PRESS 2");
					
					s = key_detect();
					
					if(s == '1'){
						LCD_Command(0x01);
						LCD_String_xy(1, 0, "1 TIME ");
						LCD_String_xy(2, 0, "2 LEAVE");
						
						s = key_detect();
						
						if(s == '1'){
							LCD_Command(0x01);
							LCD_String_xy(1, 0, "ENTER TIME");
							LCD_Command(0xc0);
							for(i=0;i<5;i++){
								if(i==2){
									time1[i] = ' ';
								}
								else
									time1[i] = key_detect();
								LCD_Char(time1[i]);
							}
							delay(1000);
							Show(count1, count2, leave1, leave2);
							goto DISP;
						}
						else if(s == '2'){
							LCD_Command(0x01);
							leave1 = 1;
							LCD_String_xy(1, 0, "LEAVE CONFIRMED ");
							delay(1000);
							Show(count1, count2, leave1, leave2);
							goto DISP;
						}
					}
					
					else if(s == '2'){
						LCD_Command(0x01);
						LCD_String_xy(1, 0, "1 TIME ");
						LCD_String_xy(2, 0, "2 LEAVE");
						
						s = key_detect();
						
						if(s == '1'){
							LCD_Command(0x01);
							LCD_String_xy(1, 0, "ENTER TIME");
							LCD_Command(0xc0);
							for(i=0;i<5;i++){
								if(i==2){
									time2[i] = ' ';
								}
								else
									time2[i] = key_detect();
								LCD_Char(time2[i]);
							}
							delay(1000);
							Show(count1, count2, leave1, leave2);
							goto DISP;
						}
						else if(s == '2'){
							LCD_Command(0x01);
							leave2 = 1;
							LCD_String_xy(1, 0, "LEAVE CONFIRMED ");
							delay(1000);
							Show(count1, count2, leave1, leave2);
							goto DISP;
						}	
					}		        
				}
				else{
					LCD_String_xy(3, 0, "INCORRECT");
					delay(1000);
					Show(count1, count2, leave1, leave2);
					goto DISP;
				}		
			}
		}
	}	
}

void count1check(int count1, int leave1)
{
	if(leave1 == 1){
		relay1 = 1;
		LCD_String_xy(1, 0, "T1 LEAVE        ");
	}
	else{	
		if(count1 == 1){
			relay1 = 0;
			LCD_String_xy(1, 0, "T1 AVAILABLE    ");
			delay(30);
		}
		else if(count1 == 0){
			relay1 = 1;
			LCD_String_xy(1, 0, "T1 UNAVAILABLE  ");
			delay(30);
		}
		else if(count1 > 1){
			relay1 = 0;
			LCD_String_xy(1, 0, "T1 BUSY         ");
			LCD_Command(0x88);
			LCD_Char(temp[count1]);
			delay(30);
		}
	}
}

void count2check(int count2, int leave2)
{
	if(leave2 == 1){
		relay2 = 1;
		LCD_String_xy(2, 0, "T2 LEAVE        ");
	}
	else{
		if(count2 == 1){
			relay2 = 0;
			LCD_String_xy(2, 0, "T2 AVAILABLE    ");
			delay(30);
		}
		if(count2 == 0){
			relay2 = 1;
			LCD_String_xy(2, 0, "T2 UNAVAILABLE  ");
			delay(30);
		}
		else if(count2 > 1){
			relay2 = 0;
			LCD_String_xy(2, 0, "T2 BUSY         ");
			LCD_Command(0xc8);
			LCD_Char(temp[count2]);
			delay(30);
		}
	}
}

void countfull(int count1, int count2,int leave1,int leave2, char* time1, char* time2){
	LCD_Command(0x01);
	LCD_String_xy(1, 0, "T1 ");
	if(leave1 ==0){
		if(count1 ==0){
		LCD_String_xy(1, 3, "BACK BY ");
		LCD_String_xy(1, 12, time1);
		}
		else if(count1 == 1)
			LCD_String_xy(1, 3, "AVAILABLE       ");
		else
			LCD_String_xy(1, 3, "BUSY            ");
	}
	else
			LCD_String_xy(1, 3, "LEAVE           ");
	
	LCD_String_xy(2, 0, "T2 ");
	if(leave2 ==0){
		if(count2 ==0){
		LCD_String_xy(2, 3, "BACK BY ");
		LCD_String_xy(2, 12, time2);
		}
		else if(count2 == 1)
			LCD_String_xy(2, 3, "AVAILABLE       ");
		else
			LCD_String_xy(2, 3, "BUSY            ");
	}
	else
			LCD_String_xy(2, 3, "LEAVE           ");

}

void Serial(int count1, int count2, int leave1, int leave2, char* time1, char* time2)
{		
	SERIAL_INIT();
	if(leave1 == 1){
		SERIAL_STRING_TX("   T1 LEAVE \r\n");
		delay(100);
	}
	else{	
		if(count1 == 1){
			SERIAL_STRING_TX("   T1 AVAILABLE \r\n");
			delay(100);
		}
		else if(count1 == 0){
			SERIAL_STRING_TX("   T1 BACK BY ");
			SERIAL_STRING_TX(time1);
			SERIAL_STRING_TX("\r\n");
			delay(100);
		}
		else if(count1 > 1){
			SERIAL_STRING_TX("   T1 BUSY \r\n");
			delay(100);
		}
	}
	if(leave2 == 1){
		SERIAL_STRING_TX("   T2 LEAVE \r\n");
	}
	else{
		if(count2 == 1){
			SERIAL_STRING_TX("   T2 AVAILABLE \r\n");
			delay(100);
		}
		else if(count2 == 0){
			SERIAL_STRING_TX("   T2 BACK BY ");
			SERIAL_STRING_TX(time2);
			SERIAL_STRING_TX("\r\n");
			delay(100);
		}
		else if(count2 > 1){
			SERIAL_STRING_TX("T2 BUSY \r\n");
			delay(100);
		}
	}	
}

void SERIAL_INIT(void){
	SCON=0x50;
	TMOD=0x20;
	TH1=TL1=0xfd;
	//TH1=0xFF;   // setting buadrate to 115200 bps
	TR1=1;
}

	*/

void SERIAL_STRING_TX(unsigned char *tx_String){ 
	while(*tx_String) {
		SBUF=*tx_String++;
    while(TI==0);
    TI=0;
	}
}

void delay(unsigned int k)
{
	int i,j;
	for (i=0;i<k;i++)
		for (j=0;j<112;j++);
}

void LCD_Command(char Command)
{
	LCD_Data = Command;
	RS=0;
	RW=0;
	EN=1;
	delay(30);
	EN=0;
	delay(30);
}
void LCD_Char(char Data)
{
	LCD_Data = Data;
	RS=1;
	RW=0;
	EN=1;
	delay(30);
	EN=0;
	delay(30);
}

void LCD_String(unsigned char *str)
{
	int i;
	for(i=0;str[i]!=0;i++)							
	{
		LCD_Char (str[i]);							
	}
}

void LCD_String_xy (unsigned char row, unsigned char pos, unsigned char *str)	
{
	if (row == 1)
		LCD_Command((pos & 0x0F)|0x80);				
	else if (row == 2)
		LCD_Command((pos & 0x0F)|0xC0);				
	else if (row == 3)
		LCD_Command((pos & 0x0F)|0x94);				
	else if (row == 4)
		LCD_Command((pos & 0x0F)|0xD4);
	LCD_String(str);								
	
}

void Show(int count1, int count2, int leave1, int leave2)
{
		LCD_Command(0x01);
    if(leave1 == 1){
		LCD_String_xy(1, 0, "T1 LEAVE        ");
	}
	else{	
		if(count1 == 1){
			LCD_String_xy(1, 0, "T1 AVAILABLE    ");
			delay(30);
		}
		else if(count1 == 0){
			LCD_String_xy(1, 0, "T1 UNAVAILABLE  ");
			delay(30);
		}
		else if(count1 > 1){
			LCD_String_xy(1, 0, "T1 BUSY         ");
			delay(30);
		}
	}
	
	if(leave2 == 1){
		LCD_String_xy(2, 0, "T2 LEAVE        ");
	}
	else{
		if(count2 == 1){
			LCD_String_xy(2, 0, "T2 AVAILABLE    ");
			delay(30);
		}
		if(count2 == 0){
			LCD_String_xy(2, 0, "T2 UNAVAILABLE  ");
			delay(30);
		}
		else if(count2 > 1){
			LCD_String_xy(2, 0, "T2 BUSY         ");
			delay(30);
		}
	}	
	
	LCD_String_xy(3, 0, "PRESS ANY KEY TO");
    delay(30);
    LCD_String_xy(4, 0, "LOGIN");
    delay(30);
}

void LCD_Init()
{
	delay(50);
	LCD_Command(0x38);
	LCD_Command(0x0C);
	LCD_Command(0x01);
	LCD_Command(0x06);
	LCD_Command(0x80);
}

unsigned char key_detect()
{
	keyport=0xF0;			
	do
	{
		keyport = 0xF0;				
		colloc = keyport;
		colloc&= 0xF0;	
	}while(colloc != 0xF0);		
	
	do
	{
		do
		{
			delay(30);	
			colloc = (keyport & 0xF0);	
		}while(colloc == 0xF0);
		
		delay(30);
		colloc = (keyport & 0xF0);
	}while(colloc == 0xF0);
			
	while(1)
	{
		/* now check for rows */
		keyport= 0xFE;											
		colloc = (keyport & 0xF0);
		if(colloc != 0xF0)
		{
			rowloc = 0;
			break;
		}

		keyport = 0xFD;									
		colloc = (keyport & 0xF0);
		if(colloc != 0xF0)
		{
			rowloc = 1;
			break;
		}
			
		keyport = 0xFB;			
		colloc = (keyport & 0xF0);
		if(colloc != 0xF0)
		{
			rowloc = 2;
			break;
		}

		keyport = 0xF7;			
		colloc = (keyport & 0xF0);
		if(colloc != 0xF0)
		{
			rowloc = 3;
			break;
		}	
	}
	
	if(colloc == 0xE0)
	{
		return(keypad[rowloc][0]);
	}		
	else if(colloc == 0xD0)
	{
		return(keypad[rowloc][1]);
	}	
	else if(colloc == 0xB0)
	{
		return(keypad[rowloc][2]);
	}	
	else
	{
		return(keypad[rowloc][3]);
	}	
}

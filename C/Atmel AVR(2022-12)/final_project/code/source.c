#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#define OVERFLOW 256
#define TICKS_PER_SEC 1000

unsigned int tick_time;
unsigned char seg_pat[10]={0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
void Seg_out(int num);
void Seg_error();
void Buzzer_main();
volatile unsigned int counter;
void Test();
volatile unsigned int j = 0;
int k;
int tmp;


ISR(TIMER0_COMP_vect)
{
	tmp = 0;
	tick_time++;
	if(tick_time == TICKS_PER_SEC){
		tick_time = 0;
	}
}
ISR(INT4_vect)
{
	Test(); //스위치 1번에 대한 설정(프로그램 시작)
}
ISR(INT5_vect)
{
	counter++; //스위치 2번에 대한 설정(점수 카운팅)
}


int main()
{
	EICRA = 0x00;
	EICRB = 0x0A;
	EIMSK = 0x30;
	sei();

	while(1);
	return (1);
}

void Test()
{
	unsigned int buzzer_Hz[10];
	double tmp[10];
	volatile unsigned int i;


	
	counter = 0;

	TIMSK = 0x02;
	sei();

	
	DDRA = 0xFF;
	DDRB |= (1 << DDB4);
	DDRE = DDRE & 0xCF;
	DDRC = 0xFF;
	DDRG |= 0x0F;
	PORTA = 0xFF;
	
	for(i=0;i< 10;i++)
	{
		buzzer_Hz[i] = rand() % (16000-20+1)+20;
		tmp[i] = (double)1/(buzzer_Hz[i]*0.000008);
	}

	k = rand () % 10;
	buzzer_Hz[k] = 50000;
	tmp[k] = (double)1/(buzzer_Hz[k]*0.000008);

	while(1)
	{
		if(j == 10)
		break;
		OCR0 = tmp[j];
		TCNT0 = 0;
		TIMSK = 0x02;
		TCCR0 = 0x1D;
		PORTA = ~PORTA;
		if(counter > j)
		break;
		j++;
		_delay_ms(55000); //소리 주기에 대한 설정(5초)
	}
	OCR0 = 0;
	if(counter > j)
		Seg_error(); //에러 발생시 에러 함수 출력
	else
		Seg_out(counter);//프로그램 정상 작동 시 점수 출력(최대 9점)
	PORTA = 0xFF;
}


void Seg_out(int num)
{
	DDRC = 0xFF;
	DDRG |= 0x0F;
	if(num < 10){
		PORTG = 0x01;
		PORTC = seg_pat[num];
	}
}
void Seg_error()
{
	DDRA = 0xFF;
	DDRC = 0xFF;
	DDRG |= 0x0F;

	PORTA = 0xFF;
	PORTC = 0xFF;
	PORTG = 0x7F;
}


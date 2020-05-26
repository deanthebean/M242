#include <avr/io.h>
#include <Atmega328P.h>
#include <avrlib.h>
#include <RegisterAccess.h>
#define F_CPU 16000000
#include <util/delay.h>
	
void OverflowImpl(void);
void CompareMatchImpl(void);


void ConfigurePins(void)
{
	SetRegister(PortB.DDR, (PIN_3, DdrOutput), (PIN_4, DdrOutput), (PIN_5, DdrInput));
	SetRegister(PortB.PORT, (PIN_4, 0), (PIN_3,1));
}


void ConfigureTimerCounter(void)
{
	// configure timer counter 1
	SetRegister(Tcnt0.TCCRA, (TCCRA_WGM, ClrTmrOnCmpMatch));
	SetRegister(Tcnt0.TCCRB, (TCCRB_CS, CsT1_Div1024));
	Tcnt0.OCRA = 41;

	// configure timer counter 2
	SetRegister(Tcnt2.TCCRA, (TCCRA_WGM, Normal));
	SetRegister(Tcnt2.TCCRB, (TCCRB_CS, CsT2_Div1024));

	// enable interrupts
	SetRegister(Timsk0, (TIMSK_OCIEA, True) ); // enable compare match interrupts for tcnt0
	SetRegister(Timsk2, (TIMSK_TOIE, True));   // enable overflow interrutp for tcnt1
}

int main(void)
{
	Usart_Init(250000); // higher is to fast; cannot be consumed reliably anymore!
	
	Usart_Trace0(1);

	ConfigurePins();
	ConfigureTimerCounter();
	while (True);
	
	return 0;
}


ISR_Tcnt2Overflow()
{
	Usart_Trace0(2);
	UpdateRegister(PortD.PORT, (PIN_6, True));
	_delay_us(5);
	UpdateRegister(PortD.PORT, (PIN_6, False));
}


ISR_Tcnt0CompareMatchA()
{
	Usart_Trace0(3);
	UpdateRegister(PortD.PORT, (PIN_7, True));
	_delay_us(5);
	UpdateRegister(PortD.PORT, (PIN_7, False));
}
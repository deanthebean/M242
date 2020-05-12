#include <avr\io.h>
#include <Atmega328P.h>
#include <avrlib.h>
#include <RegisterAccess.h>
#define F_CPU 16000000
#include <util\delay.h>
#include <inttypes.h>
#include <avr/interrupt.h>

void ConfigPins(Bool status) {
	SetRegister(PortB.DDR, (PIN_3, DdrOutput), (PIN_4, DdrOutput), (PIN_5, DdrInput));

	SetRegister(PortB.PORT, (PIN_3, status), (PIN_4, 0));
}

int main(void)
{
	Usart_Init(250000);

	Bool status = False;

	ConfigPins(status);

	while (True)
	{
		TRACE("Hello world 1");

		status = !status;

		UpdateRegister(PortB.PORT, (PIN_3, status));

		_delay_ms(1000);
	}

	return 0;
}
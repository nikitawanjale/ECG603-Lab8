#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/flash.h"
#include "driverlib/eeprom.h"

int main(void)
{
	uint32_t pui32Data[2];
	uint32_t pui32Read[2];
	pui32Data[0] = 0x12345678;
	pui32Data[1] = 0x56789abc;

	//Set the system clock
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	//Enable peripheral GPIOF and set the output pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00); //Write 0x00 to the pins
	SysCtlDelay(20000000);

	//Erase the flash
	FlashErase(0x10000);
	FlashProgram(pui32Data, 0x10000, sizeof(pui32Data));
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x02);
	SysCtlDelay(20000000);

	//Enable EEPROM peripheral
	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
	EEPROMInit(); //Recovery of power fail during previous write operation
	EEPROMMassErase(); //Erase entire EEPROM
	EEPROMRead(pui32Read, 0x0, sizeof(pui32Read)); //Read the erased values
	EEPROMProgram(pui32Data, 0x0, sizeof(pui32Data)); //Program the data array
	EEPROMRead(pui32Read, 0x0, sizeof(pui32Read)); //Read the data into the array
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x04); //Turn on blue led

	while(1)
	{
	}
}

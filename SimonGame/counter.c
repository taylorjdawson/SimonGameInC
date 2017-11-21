//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------

#include "counter.h"
#include "spi.h"
#include "sam.h"
#include <stdint.h>

//------------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//
//------------------------------------------------------------------------------
#define LED_GSCLK (PORT_PA15)
#define LED_GSCLK_GROUP (0)
#define LED_GSCLK_PIN (PIN_PA15%32)

#define LED_LAT (PORT_PA14)
#define LED_LAT_GROUP (0)
#define LED_LAT_PIN (PIN_PA14%32)

#define LED_BLANK (PORT_PA07)
#define LED_BLANK_GROUP (0)
#define LED_BLANK_PIN (PIN_PA07%32)
//------------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                __          __        ___  __
//     \  /  /\  |__) |  /\  |__) |    |__  /__`
//      \/  /~~\ |  \ | /~~\ |__) |___ |___ .__/
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//
//------------------------------------------------------------------------------
static void pulse_blank();
static void pulse_latch();
//------------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//------------------------------------------------------------------------------

//==============================================================================
void counter_init()
{
	PORT->Group[LED_LAT_GROUP].DIRSET.reg = LED_LAT;
	PORT->Group[LED_BLANK_GROUP].DIRSET.reg = LED_BLANK;
	PORT->Group[LED_GSCLK_GROUP].DIRSET.reg = LED_GSCLK;
	
	PORT->Group[LED_LAT_GROUP].OUTCLR.reg = LED_LAT;
	PORT->Group[LED_BLANK_GROUP].OUTCLR.reg = LED_BLANK;
	PORT->Group[LED_GSCLK_GROUP].OUTCLR.reg = LED_GSCLK;
	
  	// Enable the bus clk for the peripheral
  	PM->APBCMASK.bit.TC3_ = 1;
  	
	// Configure the General Clock with the 48MHz clk
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_TCC2_TC3) |
	GCLK_CLKCTRL_GEN_GCLK0 |
	GCLK_CLKCTRL_CLKEN;
	
    // Wait for the GCLK to be synchronized
    while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);
  	
    counter_disable();
	
	// Put it in the 16-bit mode.
	TC3->COUNT16.CTRLA.bit.MODE = TC_CTRLA_MODE_COUNT16_Val;

	// Set up for match frequency mode (count to period)
	TC3->COUNT16.CTRLA.bit.WAVEGEN = TC_CTRLA_WAVEGEN_MFRQ_Val;
	
	// Setup count event inputs
	TC3->COUNT16.EVCTRL.bit.EVACT = TC_EVCTRL_EVACT_COUNT_Val;
	
	// Enable the event input
	TC3->COUNT16.EVCTRL.bit.TCEI = 1;
		
	// Enable the TC3 interrupt request
	//TC3 ->COUNT16.INTENSET.reg = TC_INTFLAG_OVF;
	TC3->COUNT16.INTENSET.bit.OVF = 1;
	while(TC3->COUNT16.INTENSET.reg & TC_STATUS_SYNCBUSY);
	
	// Enable the Interrupt
	NVIC_DisableIRQ(TC3_IRQn);
	NVIC_ClearPendingIRQ(TC3_IRQn);
	NVIC_SetPriority(TC3_IRQn, 0);
	NVIC_EnableIRQ(TC3_IRQn);
	
	// Set the Period to be 8192 Events
	counter_set(8191);

	// Enable TC3
	counter_enable();
}

//============================================================================
void counter_set(uint8_t value)
{
	TC3->COUNT16.CC[0].bit.CC	 = value;
	while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
}

//============================================================================
void counter_enable()
{
	TC3->COUNT16.CTRLA.bit.ENABLE = 1;
	while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
}

//============================================================================
void counter_disable()
{
	TC3->COUNT16.CTRLA.bit.ENABLE = 0;
	while (TC3->COUNT16.STATUS.bit.SYNCBUSY);
}

//------------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//------------------------------------------------------------------------------
static void pulse_latch()
{
	PORT->Group[LED_LAT_GROUP].OUTSET.reg = LED_LAT;
	PORT->Group[LED_LAT_GROUP].OUTCLR.reg = LED_LAT;
}

static void pulse_blank()
{
	PORT->Group[LED_BLANK_GROUP].OUTSET.reg = LED_BLANK;
	PORT->Group[LED_BLANK_GROUP].OUTCLR.reg = LED_BLANK;
}

//------------------------------------------------------------------------------
//      __                  __        __        __
//     /  `  /\  |    |    |__)  /\  /  ` |__/ /__`
//     \__, /~~\ |___ |___ |__) /~~\ \__, |  \ .__/
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//        __   __  , __
//     | /__` |__)  /__`   
//     | .__/ |  \  .__/
//
//------------------------------------------------------------------------------
void TC3_Handler()
{
	
	if (spi_latch())
	{
		pulse_latch();
		spi_latch_clr();
	}
	pulse_blank();
	

	TC3->COUNT16.INTFLAG.bit.OVF = 1;
}
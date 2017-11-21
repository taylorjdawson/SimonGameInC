//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------

#include "timer.h"
#include "sam.h"

//------------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//
//------------------------------------------------------------------------------

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

//------------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//------------------------------------------------------------------------------

//==============================================================================
void timer_init()
{
	// Enable the TCC bus clock (CLK_TCCx_APB)
	PM->APBCMASK.bit.TCC0_ = 1;
	
	// Configure the General Clock with the 48MHz clk
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCLK_CLKCTRL_ID_TCC0_TCC1) |
	                    GCLK_CLKCTRL_GEN_GCLK0 |
	                    GCLK_CLKCTRL_CLKEN;
    // Wait for the GCLK to be synchronized
    while(GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

	timer_disable();
		
	// Enable peripheral function F on  PA15
	PORT->Group[0].PINCFG[15].bit.PMUXEN = 1;
	PORT->Group[0].PMUX[7].bit.PMUXO = PORT_PMUX_PMUXO_F_Val;
	

	// Select the Waveform Generation mode
	TCC0->WAVE.bit.WAVEGEN = TCC_WAVE_WAVEGEN_MFRQ_Val; // match frequency mode
	while (TCC0->SYNCBUSY.bit.WAVE);                    // wait for synchronization

	// Set output matrix to output everything on channel CC0
	TCC0->WEXCTRL.bit.OTMX = 0x2;
	
	timer_set_per();

	// Set the prescaler to 0
	TCC0->CTRLA.bit.PRESCALER = 0;
	
	// Set up the output event - turn on the Match Compare Event Output - 0
	TCC0->EVCTRL.bit.MCEO0 = 1;

	// Enable TCC0
	//timer_enable();
}

//============================================================================
void timer_set_per()
{
	// Set the time to count and wait for synchronization.
	TCC0->CC[0].bit.CC = 100;
	while (TCC0->SYNCBUSY.bit.CC0);
}

//============================================================================
void timer_enable()
{
	TCC0->CTRLA.bit.ENABLE = 1;
}

//============================================================================
void timer_disable()
{
	TCC0->CTRLA.bit.ENABLE = 0;
}

//------------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//------------------------------------------------------------------------------

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
//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------

#include "buttons.h"
#include <sam.h>

//------------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//
//------------------------------------------------------------------------------
#define BUTTON_0 (PORT_PB09)
#define BUTTON_0_GROUP (1)
#define BUTTON_0_PIN (PIN_PB09%32)

#define BUTTON_1 (PORT_PA04)
#define BUTTON_1_GROUP (0)
#define BUTTON_1_PIN (PIN_PA04%32)

#define BUTTON_ACTIVE (PORT_PA08)
#define BUTTON_ACTIVE_GROUP (0)
#define BUTTON_ACTIVE_PIN (PIN_PA08%32)

#define JOYSTICK_BTN (PORT_PA20)
#define JOYSTICK_BTN_GROUP (0)
#define JOYSTICK_BTN_PIN (PIN_PA20%32)
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

//=============================================================================
void buttons_init(void)
{
	
  // Turn on the input enables for each button
  PORT->Group[BUTTON_ACTIVE_GROUP].PINCFG[BUTTON_ACTIVE_PIN].bit.INEN = 1;
  PORT->Group[BUTTON_0_GROUP].PINCFG[BUTTON_0_PIN].bit.INEN = 1;
  PORT->Group[BUTTON_1_GROUP].PINCFG[BUTTON_1_PIN].bit.INEN = 1;
  PORT->Group[JOYSTICK_BTN_GROUP].PINCFG[JOYSTICK_BTN_PIN].bit.INEN = 1;
  
  // Make sure the directions regs are configured as inputs
  PORT->Group[BUTTON_ACTIVE_GROUP].DIRCLR.reg = BUTTON_ACTIVE;
  PORT->Group[BUTTON_0_GROUP].DIRCLR.reg = BUTTON_0;
  PORT->Group[BUTTON_1_GROUP].DIRCLR.reg = BUTTON_1;
  PORT->Group[JOYSTICK_BTN_GROUP].DIRCLR.reg = JOYSTICK_BTN;
}

//==============================================================================
// Buttons are active low
// Button 0 is on PD2
// Button 1 is on PD3
//==============================================================================
int8_t buttons_get()
{
	int8_t retval = -1;
	if (PORT->Group[BUTTON_ACTIVE_GROUP].IN.reg & BUTTON_ACTIVE)
	{
		uint8_t btn_0 = (PORT->Group[BUTTON_0_GROUP].IN.reg & BUTTON_0) ? 1 : 0;
		uint8_t btn_1 = (PORT->Group[BUTTON_1_GROUP].IN.reg & BUTTON_1) ? 2 : 0;	  
		retval =  btn_1 + btn_0;
	}
	return retval;
}

int8_t joystick_pressed()
{
	return (PORT->Group[JOYSTICK_BTN_GROUP].IN.reg & JOYSTICK_BTN) ? 0 : 1;
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
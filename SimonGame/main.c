//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------

#include "sam.h"
#include "buttons.h"
#include "font.h"
#include "video.h"
#include "delay.h"
#include <math.h>
//-----------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//
//-----------------------------------------------------------------------------
#define START (0)
#define PLAY_BUTTON_PRESSED (1)
#define BEGIN_GAME (2)
#define PLAY_TONES (3)
#define PLAYER_TURN (4)
#define LOSER (5)

#define LOW (1)
#define ACTIVE (250)
//-----------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//                __          __        ___  __
//     \  /  /\  |__) |  /\  |__) |    |__  /__`
//      \/  /~~\ |  \ | /~~\ |__) |___ |___ .__/
//
//-----------------------------------------------------------------------------
static volatile uint64_t millis;
uint16_t led_values[16];

//-----------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//
//-----------------------------------------------------------------------------
void red_set(uint8_t light_select, uint8_t led_value);
void green_set(uint8_t light_select, uint8_t led_value);
void blue_set(uint8_t light_select, uint8_t led_value);
void bit_conversion(uint16_t* array, uint8_t* data_array);
void rgb_set(uint8_t light_select, uint8_t red, uint8_t green, uint8_t blue);
void led_active(uint8_t light_select);
static inline clear_led_values();
//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//-----------------------------------------------------------------------------

//=============================================================================
int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	SysTick_Config(48000);
	
	// Initialize the board
	//adc_init();
	
	timer_init();
	counter_init();
	video_init();
	//spi_init();
	
	event_init();
	
	buttons_init();
	counter_enable();
	timer_enable();
	
	font_t *font1, *font2, *font3, *font4;
	uint8_t state = START;
	uint8_t button = -1;
	uint8_t data[24];
	uint64_t current_time = 0;
	uint64_t delay;
	uint64_t previous_time = 0;
	uint64_t led_timer = 0;
	uint64_t video_timer = 0;
	uint64_t play_seq_timer = 0;
	uint64_t button_time = 0;
	uint64_t seed_time = 0;
	
	/*********************
	CHANGE BACK TO 0
	********************/
	uint16_t round = 5;
	
	uint8_t sequence = 0;
	uint8_t current_round = 1;
	
	//clear_led_values();
	
	// Get the font pointers
	font1 = font_get(FONT_6x8);
	font2 = font_get(FONT_8x8);
	font3 = font_get(FONT_8x12);
	font4 = font_get(FONT_12x16);

	// Write black background
	video_paint_rect(0, 0, 176, 220, 0x0000);
	enable_spi_interrupt();
	clear_led_values();
	
	// Turn on leds to low
	rgb_set(1, 0, LOW, 0);   // GREEN
	rgb_set(2, LOW, 0, 0);   // RED
	rgb_set(3, 0, 0, LOW);   // BLUE
	rgb_set(4, LOW, LOW, 0); // YELLOW
	bit_conversion(led_values, data);
	spi_write_led(data);
	DelayMs(10);
	disable_spi_interrupt();
	
	
	// Welcome message
	video_print_string("Welcome to the Simon Game", font3, 10, 20, 0xffff, 0x0000);
	video_print_string("Game press button S0 to", font3, 10, 40, 0xffff, 0x0000);
	video_print_string("begin...", font3, 10, 60, 0xffff, 0x0000);
	
    /* Replace with your application code */
    while (1) 
    {
		current_time = millis;
		/*
		if ( (current_time - led_timer) > 16 )
		{
			bit_conversion(led_values, data);
			spi_write_led(data);
			led_timer = current_time;
		}
		*/
		

		
		// Notes: random number between 0-3 tones and colours correspond to 0-3
		
		// Regardless of state (I think) buttons should switch light from low to active
		//if ( (button = buttons_get()) > -1) {
		//	led_active(button);
		//}
		
		if ( (current_time - button_time) > 20 ){
			if (button > -1)
			{	
				button = buttons_get();
			}
			button_time = current_time;
		}
		
		
		switch(state)
		{
			case START:
				// If button S0 is pressed state = PLAY_BUTTON_PRESSED;
				if ( (button > -1) && (button == 0) ) {
					state = PLAY_BUTTON_PRESSED;
				}
			break;
			
			case PLAY_BUTTON_PRESSED:
				if (button == 0)
				{
					// Get the length of time in this state to seed the random number generator
					seed_time += current_time; // We'll just create this number by coninuously adding the current time to the seed_time
				}
				else {
					
					state = BEGIN_GAME;
				}
			break;
			
			case BEGIN_GAME:
				//Clear the screen 
				video_paint_rect(0, 0, 176, 220, 0x0000);
				
				// Print the instructions and score board on the video screen; must reconfig spi (I think?)
					// Call external paint_rect function
				// 
				//current_sequenc = 0;
				uint8_t first_time = 1;
				uint8_t toggle = 0;
				state = PLAY_TONES;
				
			break;
			
			case PLAY_TONES:
				if(first_time) {
					disable_spi_interrupt();
					video_print_string("My turn...", font3, 10, 60, 0xffff, 0x0000);
					srand(seed_time);
					first_time = 0;
					enable_spi_interrupt();
				}
				if ( (current_time - play_seq_timer) > 1000)
				{
					if (current_round < round)
					{
						sequence = rand() % 4;  // value 0-3 indicating which tone/light
						led_active(sequence);
						bit_conversion(led_values, data);
						spi_write_led(data);
						//play_tone( sequence );
						current_round++;

					}
					else {
						current_round = 0;
						first_time = 1;
						disable_spi_interrupt();
						state = PLAYER_TURN;
					}
					play_seq_timer = current_time;
				}
			break;
			
			case PLAYER_TURN:
			
				if (first_time) {
					disable_spi_interrupt();
					video_print_string("Your turn...", font3, 10, 60, 0xffff, 0x0000);
					enable_spi_interrupt();
					srand(seed_time);
					sequence = rand() % 4;
					first_time = 0;
				}
				
				led_active(button);
				bit_conversion(led_values, data);
				spi_write_led(data);
				
				if (current_round < round)
				{
					if ((button > -1) && (button == sequence))
					{
						current_round++;
						sequence = rand() % 4;
					}
					else
					{
						state = LOSER;
					}
				}
				else {
					current_round = 0;
					uint8_t first_time = 1;
					state = PLAY_TONES;
				}

			break;
			
			case LOSER:
				if (first_time) {
					disable_spi_interrupt();
					video_print_string("YOU LOSE SUCKA!", font3, 10, 60, 0xffff, 0x0000);
					enable_spi_interrupt();
					srand(seed_time);
					sequence = rand() % 4;
					first_time = 0;
				}
				rgb_set(1, ACTIVE, 0, 0);   // RED
				rgb_set(2, ACTIVE, 0, 0);   // RED
				rgb_set(3, ACTIVE, 0, 0);   // RED
				rgb_set(4, ACTIVE, 0, 0);   // RED
				bit_conversion(led_values, data);
				spi_write_led(data);
			break;
			
			// seed_time back to 0 at some point
			
		}
		
		
    }
}

//-----------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//-----------------------------------------------------------------------------
void led_active(uint8_t light_select)
{
		// Turn on leds to low
	rgb_set(1, 0, LOW, 0);   // GREEN
	rgb_set(2, LOW, 0, 0);   // RED
	rgb_set(3, 0, 0, LOW);   // BLUE
	rgb_set(4, LOW, LOW, 0); // YELLOW
	
	switch(light_select){
		case 0:
		rgb_set(1, 0, ACTIVE, 0); // GREEN high
		break;
		case 1:
		rgb_set(2, ACTIVE, 0, 0); // GREEN high
		break;
		case 2:
		rgb_set(3, 0, 0, ACTIVE); // GREEN high
		break;
		case 3:
		rgb_set(4, ACTIVE, ACTIVE, 0); // GREEN high
		break;
	}
}
//==============================================================================
void bit_conversion(uint16_t* array, uint8_t* data_array)
{
	uint8_t ind = 0;
	
	for (int i = 8; i > 0; i--)
	{
		data_array[ind++] = (array[i * 2 - 1] & 0x0FF0) >> 4;
		data_array[ind++] = ((array[i * 2 - 1] & 0x000F) << 4) | ((array[i * 2 - 2] & 0x0F00) >> 8);
		data_array[ind++] = array[i * 2 - 2] & 0x00FF;
	}
}

//==============================================================================
void rgb_set(uint8_t light_select, uint8_t red, uint8_t green, uint8_t blue)
{
	red_set(light_select, red);
	green_set(light_select, green);
	blue_set(light_select, blue);
}

//==============================================================================
void red_set(uint8_t light_select, uint8_t led_value)
{
	led_values[light_select*3-3] = led_value * 16;
}

//==============================================================================
void green_set(uint8_t light_select, uint8_t led_value)
{
	led_values[light_select*3-2] = led_value * 16;
}

//==============================================================================
void blue_set(uint8_t light_select, uint8_t led_value)
{
	led_values[light_select*3-1] = led_value * 16;
}

//==============================================================================
static inline clear_led_values()
{
	for( int i = 0; i < 16; i++)
	{
		led_values[i] = 0;
	}
}

//-----------------------------------------------------------------------------
//        __   __   __
//     | /__` |__) /__`
//     | .__/ |  \ .__/
//
//-----------------------------------------------------------------------------
void SysTick_Handler()
{
	millis++;
}
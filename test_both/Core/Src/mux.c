 /*
 * mux.c
 *
 *  Created on: Oct 28, 2025
 *      Author: javis
 */

#include "mux.h"
#include "gpio.h"
#include "buttons.h"
#include "ws2812.h"


static inline uint8_t mux4051_read_pc0(void)
{
    return (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0) == GPIO_PIN_SET);
}
static inline void mux4051_settle(void)
{
    // 4051 przełącza w mikrosekundach; 10–50 µs zwykle wystarczy.
    // Jeśli nie masz timera µs, 1 ms też zadziała:
    // HAL_Delay(1);
    for (volatile int i=0; i<800; ++i) __NOP(); // ~10–20 µs @ 84 MHz
}

int mux_2_state(void) {
	// PA4
	return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == GPIO_PIN_RESET;  // Wyjscie mux aktywne, stan niski

}

// Funkcja ustawiająca odpowiednią kombinację bitów na PC10, PC11, PC12
static inline void mux4051_select(uint8_t ch)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, (ch & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET); // A
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, (ch & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET); // B
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, (ch & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET); // C
}

// Funkcja sterująca, która będzie przechodzić po wszystkich wejściach multipleksera
void cycle_mux_channels(void)
{
	 uint8_t stop = 0;

	    while (!stop)
	    {
	        // Najpierw wyczyść całą taśmę (ramkę)
	        ws2812_set_all(0,0,0);

	        for (uint8_t ch = 0; ch < 8; ++ch)
	        {
	            mux4051_select(ch);
	            mux4051_settle();

	            uint8_t high = mux4051_read_pc0();  // 1 gdy kontaktron zamknięty (stan wysoki)

	            if (high) {
	                // zapal LED na pozycji odpowiadającej kanałowi
	                // (tu zielony; zmień wedle uznania)
	                ws2812_set(7-ch, 0, 255, 0);
	            } else {
	                // zgaś — nic nie ustawiamy, bo ramka już wyczyszczona
	                // ws2812_set(ch, 0, 0, 0);
	            }
	        }

	        // pokaż całą ramkę naraz (mniej migotania)
	        ws2812_show_async();

	        // tempo odświeżania (np. 50 ms)
	        HAL_Delay(50);

	        stop = button4_pressed();
	    }


}

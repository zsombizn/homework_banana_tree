/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/


/***************************************************************************//**
 * @file     app.c
 * @brief    Banana Tree game
 *
 * @details  This application uses the Segment LCD and capacitive touch sensors
 *           of the EFM32GG-STK3700 board. Intended as a homework project of
 *           the subject VIMIA347.
 *
 * Created on:  2025. 10. 04.
 *
 * Author:      Zador-Nagy Zsombor (I5R3IK)
 ******************************************************************************/


/***************************************************************************//**
 * Includes
 ******************************************************************************/
#include "app.h"
/*
 * Header for the SegmentLCD driver extension
 */
#include "segmentlcd_individual.h"

/*
 * "segmentlcd.h" is also required, as the SegmentLCD driver extension does not
 * provide any initialization functions. We use the base SegmentLCD driver to
 * to initialize the display.
 */
#include "segmentlcd.h"

/*
 * "sl_udelay.h" is used only by the demo functions to slow things down.
 * Otherwise it is not required to use the SegmentLCD driver extension.
 */
#include <sl_udelay.h>

/*
 * Header for the capacitive sensor
 */
#include "caplesense.h"

/*
 * Header for device peripheral description
 */
#include "em_device.h"
#include "em_cmu.h"

#include "sl_simple_button_instances.h"

#include <sl_string.h>
#include <string.h>
#include <stdbool.h>


/***************************************************************************//**
 * Globals
 ******************************************************************************/

/*
 * SegmentLCD_UpperSegments() and SegmentLCD_LowerSegments() are used to update
 * the display with new data. Display data is expected by these functions to be
 * located in arrays with SegmentLCD_UpperCharSegments_TypeDef and
 * SegmentLCD_LowerCharSegments_TypeDef elements, respectively.
 */
SegmentLCD_UpperCharSegments_TypeDef upperCharSegments[SEGMENT_LCD_NUM_OF_UPPER_CHARS];
SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];


volatile uint32_t msTicks; /* counts 1ms timeTicks */
int sliderPos;
int sliderDownsc = 0, sliderDownscOld = 0;
int game_ticks = 0;

bool reset_text = false;

/***************************************************************************//**
 * Function definitions
 ******************************************************************************/

/***************************************************************************//**
 * @brief SysTick_Handler
 *   Interrupt Service Routine for system tick counter
 * @note
 *   No wrap around protection
 ******************************************************************************/
void SysTick_Handler(void)
{
  msTicks++;       /* increment counter necessary in Delay()*/
}

/***************************************************************************//**
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 ******************************************************************************/
void Delay(uint32_t dlyTicks)
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) ;
}


/***************************************************************************//**
 * @brief Displays a text, scrolls if bigger than screen
 *
 * @param text The text itself
 * @param speed Game ticks per character
 ******************************************************************************/
void display_text(const char text[TEXT_LENGTH], int speed);


/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void)
{
  /*
   * The SegmentLCD driver extension does not provide an initialization
   * functions. However, we need to call the initialization function of the
   * underlying base SegmentLCD driver.
   */

  /* Enable LCD without voltage boost */
  SegmentLCD_Init(false);

  CAPLESENSE_Init(false);

  sl_button_enable(&sl_button_btn1);

  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE)/1000)) {
    while (1) ;
  }

}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  static sl_button_state_t last = 1;

  uint32_t curTicks = msTicks;
  game_ticks++;

  const char txt_press[TEXT_LENGTH] = "Pressed";
  const char txt_rel[TEXT_LENGTH] = "Released";

  /*
   * Poll the button in every game tick, debouncing is set in config,
   * meaning it changes state after the last n inputs are consistent
   * (stable for GAME_TICK_INTERVAL*n ms)
   */
  sl_button_poll_step(&sl_button_btn1);

  if (sl_button_get_state(&sl_button_btn1) == 1 ) {
      if (last != 1) reset_text = true;
      display_text(txt_press, 20);

  } else {
      if(last == 1) reset_text = true;
      display_text(txt_rel, 20);
  }

  last = sl_button_get_state(&sl_button_btn1);


  while ((msTicks - curTicks) < GAME_TICK_INTERVAL) ;

  return;
}

/*
 * Displays a scrolling text
 */
void display_text(const char text[TEXT_LENGTH], int speed) {
  static int pos = 0;
  static int t = 0;

  static char shifted[TEXT_LENGTH + 7];

  int len = sl_strlen(text);

  if (reset_text){
      // padding
      for(int i = 0; i < DISPLAY_WIDTH; i++) {
          shifted[i] = ' ';
      }

      sl_strcpy_s(&shifted[DISPLAY_WIDTH], TEXT_LENGTH, text);

      t = 0;
      pos = 0;
      reset_text = false;
  }

  if (len <= DISPLAY_WIDTH){
      SegmentLCD_Write(text);
      return;
  }

  SegmentLCD_Write(&shifted[pos]);

  if (t % speed == 0) pos = (pos < len+7) ? pos+1  : 0;

  t++;

  return;
}

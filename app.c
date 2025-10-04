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
 * @brief Prints hello on screen
 ******************************************************************************/

void test_LCD(void);



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

  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE)/1000)) {
    while (1) ;
  }

  test_LCD();
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  return;
}


/*
 * Hello function on LCD
 */
void test_LCD(void) {
  SegmentLCD_Number(42);

  // H
  lowerCharSegments[0].b = 1;
  lowerCharSegments[0].c = 1;
  lowerCharSegments[0].e = 1;
  lowerCharSegments[0].f = 1;
  lowerCharSegments[0].g = 1;
  lowerCharSegments[0].m = 1;
  // E
  lowerCharSegments[1].a = 1;
  lowerCharSegments[1].d = 1;
  lowerCharSegments[1].e = 1;
  lowerCharSegments[1].f = 1;
  lowerCharSegments[1].g = 1;
  lowerCharSegments[1].m = 1;
  // L
  lowerCharSegments[2].d = 1;
  lowerCharSegments[2].e = 1;
  lowerCharSegments[2].f = 1;
  // L
  lowerCharSegments[3].d = 1;
  lowerCharSegments[3].e = 1;
  lowerCharSegments[3].f = 1;
  // O
  lowerCharSegments[4].a = 1;
  lowerCharSegments[4].b = 1;
  lowerCharSegments[4].c = 1;
  lowerCharSegments[4].d = 1;
  lowerCharSegments[4].e = 1;
  lowerCharSegments[4].f = 1;

  // draw LCD
  SegmentLCD_LowerSegments(lowerCharSegments);
}

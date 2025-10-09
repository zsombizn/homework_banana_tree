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

uint32_t game_ticks = 0;

ScrollTextConfigType scrollTextConfig;

const char * const textConstants[] = TEXT_CONSTANTS;

GameConfigType GameConfig;

GameStateType GameState;

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
 * @brief Displays a text, scrolls if bigger than screen, based on config
 *
 ******************************************************************************/
void displayScrollText(void);


/***************************************************************************//**
 * @brief Initialize scrolltext configuration with default values
 *
 * @return ScrollTExtConfigType
 ******************************************************************************/
ScrollTextConfigType initScrollTextConfig(void);


/***************************************************************************//**
 * @brief Return a random int with a 2 bit range
 *
 ******************************************************************************/
int rand_4(void);


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

  sl_led_toggle(&sl_led_led0);

  /* Enable LCD without voltage boost */
  SegmentLCD_Init(false);

  LCD_ContrastSet(25);

  CAPLESENSE_Init(false);

  sl_button_enable(&sl_button_btn1);

  scrollTextConfig = initScrollTextConfig();


  /* Set up random generation */

  psa_status_t s;

  s = psa_crypto_init();
  if (s != PSA_SUCCESS) {
      sl_led_turn_on(&sl_led_led0);
      sl_led_turn_on(&sl_led_led1);
      while (true) {

      }
  }




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

  /*
   * Poll the button in every game tick, debouncing is set in config,
   * meaning it changes state after the last n inputs are consistent
   * (stable for GAME_TICK_INTERVAL*n ms)
   */
  sl_button_poll_step(&sl_button_btn1);

  if (sl_button_get_state(&sl_button_btn1) == 1 ) {
      if (last != 1) scrollTextConfig.reset = true;
      sl_strcpy_s(scrollTextConfig.text, TEXT_LENGTH, textConstants[TXT_PRES]);
      if (last != 1) {
          sl_led_toggle(&sl_led_led0);
          SegmentLCD_Number(rand_4());
      }

  } else {
      if(last == 1) scrollTextConfig.reset = true;
      sl_strcpy_s(scrollTextConfig.text, TEXT_LENGTH, textConstants[TXT_REL]);
  }

  last = sl_button_get_state(&sl_button_btn1);

  displayScrollText();


  while ((msTicks - curTicks) < GAME_TICK_INTERVAL) ;

  return;
}


int rand_4(void) {
  uint8_t res = 0;
  psa_status_t s;
  s = psa_generate_random(&res, 1);

  if (s != PSA_SUCCESS) {
        sl_led_turn_on(&sl_led_led0);
        sl_led_turn_on(&sl_led_led1);
        while (true) {

        }
    }

  res &= 0x03;
  return res;
}


/*
 * Initialize ScrollTextConfigType with defaults
 */
ScrollTextConfigType initScrollTextConfig(void)
{
  ScrollTextConfigType res;

  for (int i = 0; i < TEXT_LENGTH - 1; i++) {
      res.text[i] = ' ';
  }

  res.text[TEXT_LENGTH - 1] = '\0';
  res.speed = 20;
  res.reset = true;

  return res;
}

/*
 * Displays a scrolling text
 */
void displayScrollText(void)
{
  static int pos = 0;
  static int t = 0;

  static char shifted[TEXT_LENGTH + 7];

  int len = sl_strlen(scrollTextConfig.text);

  if (scrollTextConfig.reset){
      // padding
      for(int i = 0; i < DISPLAY_WIDTH; i++) {
          shifted[i] = ' ';
      }

      sl_strcpy_s(&shifted[DISPLAY_WIDTH], TEXT_LENGTH, scrollTextConfig.text);

      t = 0;
      pos = 0;
      scrollTextConfig.reset = false;
  }

  if (len <= DISPLAY_WIDTH){
      SegmentLCD_Write(scrollTextConfig.text);
      return;
  }

  SegmentLCD_Write(&shifted[pos]);

  if (t % scrollTextConfig.speed == 0) pos = (pos < len+7) ? pos+1  : 0;

  t++;

  return;
}

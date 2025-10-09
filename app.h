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
#include <stdbool.h>

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

#include "sl_simple_led.h"
#include "sl_simple_led_instances.h"

#ifndef APP_H
#define APP_H

// In ms, used for button polling as well
#define GAME_TICK_INTERVAL 20

#define TEXT_LENGTH 35
#define DISPLAY_WIDTH 7

typedef enum {
    TXT_REL,
    TXT_PRES,
    TXT_GAME_OVER,
    TXT_COUNT
} TextId;

#define TEXT_CONSTANTS {     \
  [TXT_REL] = "Released",    \
  [TXT_PRES] = "Pressed",    \
  [TXT_GAME_OVER] = "GAME OVER!" \
  }

typedef struct {
  char text[TEXT_LENGTH];
  int speed;
  bool reset;
} ScrollTextConfigType;


typedef const struct {
  int difficulty;
  int n_bananas;
} GameConfigType;

typedef int BananaState;

typedef enum {
  BANANA_CREATED,
  BANANA_FALLING,
  BANANA_LANDED
} BananaStateId;

typedef struct {
  int n_catched;
  int next_banana;

} GameStateType;


/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
void app_init(void);

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void);

#endif  // APP_H

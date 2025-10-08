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
#include <stdint.h>

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


/**
 * @brief Stores the configuration of a scrolling text on the LCD.
 */
typedef struct {
  char text[TEXT_LENGTH];
  int speed;              /** In Game tick/character */
  bool reset;             /** If flag is set, animation will reset*/
} ScrollTextConfigType;


/**
 * @brief Represents game config, like difficulty, and total number of bananas.
 */
typedef const struct {
  int difficulty;
  int n_bananas;
} GameConfigType;


/**
 * @brief Represents the current game state
 *
 * This structure holds information about the number of catched bananas,
 * and the current status of bananas in the game.
 *
 */
typedef struct {
  int n_catched;
  int bananas[4];
  uint32_t next_update[4];

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

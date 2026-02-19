#ifndef ST7789H2_H
#define ST7789H2_H

#include "stm32f4xx_hal.h"

/* Adjust these for your wiring */
#define ST7789H2_SPI          hspi1
#define ST7789H2_DC_GPIO      GPIOB
#define ST7789H2_DC_PIN       GPIO_PIN_5
#define ST7789H2_RST_GPIO     GPIOA
#define ST7789H2_RST_PIN      GPIO_PIN_8
#define ST7789H2_CS_GPIO      GPIOA
#define ST7789H2_CS_PIN       GPIO_PIN_4

#define ST7789H2_WIDTH   240
#define ST7789H2_HEIGHT  240

void ST7789H2_Init(void);
void ST7789H2_Fill(uint16_t color);
void ST7789H2_DrawPixel(uint16_t x, uint16_t y, uint16_t color);

#endif

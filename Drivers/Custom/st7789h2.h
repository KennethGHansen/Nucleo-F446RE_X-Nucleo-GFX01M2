#ifndef ST7789H2_H
#define ST7789H2_H

#include "stm32f4xx_hal.h"

/* Adjust these for your wiring */
#define ST7789H2_SPI          hspi1
#define ST7789H2_DC_GPIO      GPIOB
#define ST7789H2_DC_PIN       GPIO_PIN_5
#define ST7789H2_RST_GPIO     GPIOA
#define ST7789H2_RST_PIN      GPIO_PIN_1
#define ST7789H2_CS_GPIO      GPIOA
#define ST7789H2_CS_PIN       GPIO_PIN_4


void ST7789H2_Init(void);
void ST7789H2_Fill(uint16_t color);
void ST7789H2_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ST7789H2_DrawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg);
void ST7789H2_DrawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg);
void ST7789H2_DrawCharScaled(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t scale);
void ST7789H2_DrawStringScaled(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t scale);

#endif
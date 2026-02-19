#include "st7789h2.h"

/* Shortcuts */
#define CS_LOW()   HAL_GPIO_WritePin(ST7789H2_CS_GPIO, ST7789H2_CS_PIN, GPIO_PIN_RESET)
#define CS_HIGH()  HAL_GPIO_WritePin(ST7789H2_CS_GPIO, ST7789H2_CS_PIN, GPIO_PIN_SET)
#define DC_LOW()   HAL_GPIO_WritePin(ST7789H2_DC_GPIO, ST7789H2_DC_PIN, GPIO_PIN_RESET)
#define DC_HIGH()  HAL_GPIO_WritePin(ST7789H2_DC_GPIO, ST7789H2_DC_PIN, GPIO_PIN_SET)
#define RST_LOW()  HAL_GPIO_WritePin(ST7789H2_RST_GPIO, ST7789H2_RST_PIN, GPIO_PIN_RESET)
#define RST_HIGH() HAL_GPIO_WritePin(ST7789H2_RST_GPIO, ST7789H2_RST_PIN, GPIO_PIN_SET)

/* Send command */
static void ST7789H2_WriteCmd(uint8_t cmd)
{
    DC_LOW();
    CS_LOW();
    HAL_SPI_Transmit(&ST7789H2_SPI, &cmd, 1, HAL_MAX_DELAY);
    CS_HIGH();
}

/* Send data */
static void ST7789H2_WriteData(uint8_t *data, uint16_t len)
{
    DC_HIGH();
    CS_LOW();
    HAL_SPI_Transmit(&ST7789H2_SPI, data, len, HAL_MAX_DELAY);
    CS_HIGH();
}

/* Set drawing window */
static void ST7789H2_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint8_t data[4];

    ST7789H2_WriteCmd(0x2A);
    data[0] = x0 >> 8; data[1] = x0;
    data[2] = x1 >> 8; data[3] = x1;
    ST7789H2_WriteData(data, 4);

    ST7789H2_WriteCmd(0x2B);
    data[0] = y0 >> 8; data[1] = y0;
    data[2] = y1 >> 8; data[3] = y1;
    ST7789H2_WriteData(data, 4);

    ST7789H2_WriteCmd(0x2C);
}

/* Basic reset */
static void ST7789H2_Reset(void)
{
    RST_LOW();
    HAL_Delay(20);
    RST_HIGH();
    HAL_Delay(20);
}

void ST7789H2_Init(void)
{
    ST7789H2_Reset();

    /* Startup sequence for ST7789H2 (minimal version) */
    ST7789H2_WriteCmd(0x36);   // MADCTL
    uint8_t rot = 0x00;        // Top-left origin
    ST7789H2_WriteData(&rot, 1);

    ST7789H2_WriteCmd(0x3A);   // COLMOD = 16-bit color
    uint8_t col = 0x55;
    ST7789H2_WriteData(&col, 1);

    ST7789H2_WriteCmd(0x11);   // Sleep out
    HAL_Delay(120);

    ST7789H2_WriteCmd(0x29);   // Display ON
    HAL_Delay(20);

    ST7789H2_Fill(0x0000);
}

void ST7789H2_Fill(uint16_t color)
{
    uint8_t data[2] = { color >> 8, color & 0xFF };

    ST7789H2_SetAddressWindow(0, 0, ST7789H2_WIDTH - 1, ST7789H2_HEIGHT - 1);

    for (uint32_t i = 0; i < ST7789H2_WIDTH * ST7789H2_HEIGHT; i++)
    {
        ST7789H2_WriteData(data, 2);
    }
}

void ST7789H2_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= ST7789H2_WIDTH || y >= ST7789H2_HEIGHT) return;

    uint8_t data[2] = { color >> 8, color & 0xFF };
    ST7789H2_SetAddressWindow(x, y, x, y);
    ST7789H2_WriteData(data, 2);
}
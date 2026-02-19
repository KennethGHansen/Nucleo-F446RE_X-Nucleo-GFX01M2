#include "st7789h2.h"
#include "spi.h"
#include "font5x7.h"

/* Shortcuts */
//SPI line definitions
#define CS_LOW()   HAL_GPIO_WritePin(ST7789H2_CS_GPIO, ST7789H2_CS_PIN, GPIO_PIN_RESET)
#define CS_HIGH()  HAL_GPIO_WritePin(ST7789H2_CS_GPIO, ST7789H2_CS_PIN, GPIO_PIN_SET)
#define DC_LOW()   HAL_GPIO_WritePin(ST7789H2_DC_GPIO, ST7789H2_DC_PIN, GPIO_PIN_RESET)
#define DC_HIGH()  HAL_GPIO_WritePin(ST7789H2_DC_GPIO, ST7789H2_DC_PIN, GPIO_PIN_SET)
#define RST_LOW()  HAL_GPIO_WritePin(ST7789H2_RST_GPIO, ST7789H2_RST_PIN, GPIO_PIN_RESET)
#define RST_HIGH() HAL_GPIO_WritePin(ST7789H2_RST_GPIO, ST7789H2_RST_PIN, GPIO_PIN_SET)

//Display offset definitions
#define X_OFFSET 0
#define Y_OFFSET 0

//Display size definitions
#define ST7789H2_WIDTH   240
#define ST7789H2_HEIGHT  320

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
    
		//using offset if necessary
		x0 += X_OFFSET;
    x1 += X_OFFSET;
    y0 += Y_OFFSET;
    y1 += Y_OFFSET;

		// CASET: Column
    ST7789H2_WriteCmd(0x2A);
    data[0] = 0x00; data[1] = x0;
    data[2] = 0x00; data[3] = x1;
    ST7789H2_WriteData(data, 4);

    // RASET: Row
    ST7789H2_WriteCmd(0x2B);
    data[0] = 0x00; data[1] = y0;
    data[2] = 0x01; data[3] = (y1 & 0xFF);
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

/* Display initialization */
void ST7789H2_Init(void)
{
    // MADCTL - Memory Access Control
    // BGR = bit 3 (0x08) - fixes the BGR - > RGB for correct control of color 
		// MY = 0x80 -> Vertical flip (mirror top ? bottom)
		// MX = 0x40 -> Horizontal flip (mirror left ? right)
		// MV = 0x20 -> Swap X/Y (rotate 90°)
		// ML = 0x10 -> Vertical refresh order
		// MH = 0x04 -> Horizontal refresh order
		// RGB/GBR = 0x08 -> Color order
	
		ST7789H2_Reset();

    /* Startup sequence for ST7789H2 (minimal version) */
    ST7789H2_WriteCmd(0x36);   // MADCTL
    uint8_t rot = 0x40 | 0x08 ;  // M? + BGR   // BGR (bit 3) (Fixed incorrect RGB color order)
    ST7789H2_WriteData(&rot, 1);

    ST7789H2_WriteCmd(0x3A);   // COLMOD = 16-bit color
    uint8_t col = 0x55;
    ST7789H2_WriteData(&col, 1);

    ST7789H2_WriteCmd(0x11);   // Sleep out
    HAL_Delay(120);

    ST7789H2_WriteCmd(0x29);   // Display ON
    HAL_Delay(20);

    ST7789H2_Fill(0x0000);		// Clear screen (black)
}

// ------------------------------------------------------
// Fill entire visible display
// ------------------------------------------------------
void ST7789H2_Fill(uint16_t color)
{
    uint8_t data[2] = { color >> 8, color & 0xFF };

    ST7789H2_SetAddressWindow(0, 0, ST7789H2_WIDTH - 1, ST7789H2_HEIGHT - 1);

    for (uint32_t i = 0; i < ST7789H2_WIDTH * ST7789H2_HEIGHT; i++)
    {
        ST7789H2_WriteData(data, 2);
    }
}

// ------------------------------------------------------
// Single Pixel draw
// ------------------------------------------------------
void ST7789H2_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if (x >= ST7789H2_WIDTH || y >= ST7789H2_HEIGHT) return;

    uint8_t data[2] = { color >> 8, color & 0xFF };

    ST7789H2_SetAddressWindow(x, y, x, y);
    ST7789H2_WriteData(data, 2);
}


// ------------------------------------------------------
// ASCII Character draw
// ------------------------------------------------------
void ST7789H2_DrawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg)
{
    if (c < 32 || c > 126) return;
    c -= 32;

    for (uint8_t col = 0; col < 5; col++)
    {
        uint8_t line = Font5x7[(uint8_t)c][col];

        for (uint8_t row = 0; row < 7; row++)
        {
            uint16_t px = (line & 0x01) ? color : bg;
            ST7789H2_DrawPixel(x + col, y + row, px);
            line >>= 1;
        }
    }
}

// ------------------------------------------------------
// ASCII String draw
// ------------------------------------------------------
void ST7789H2_DrawString(uint16_t x, uint16_t y,
                       const char *str, uint16_t color, uint16_t bg)
{
    while (*str)
    {
        ST7789H2_DrawChar(x, y, *str, color, bg);
        x += 6;   // 5px char + 1px spacing
        str++;
    }
}


// ------------------------------------------------------
// ASCII Character with scaling draw
// ------------------------------------------------------

void ST7789H2_DrawCharScaled(uint16_t x, uint16_t y, char c,
                           uint16_t color, uint16_t bg, uint8_t scale)
{
    if (c < 32 || c > 126) return;
    c -= 32;

    for (uint8_t col = 0; col < 5; col++)
    {
        uint8_t line = Font5x7[(uint8_t)c][col];

        for (uint8_t row = 0; row < 7; row++)
        {
            uint16_t pixel_color = (line & 0x01) ? color : bg;

            // Draw a scale×scale block
            for (uint8_t dx = 0; dx < scale; dx++)
            for (uint8_t dy = 0; dy < scale; dy++)
            {
                ST7789H2_DrawPixel(x + col*scale + dx,
                                 y + row*scale + dy,
                                 pixel_color);
            }

            line >>= 1;
        }
    }
}


// ------------------------------------------------------
// ASCII String with scaling draw 
// ------------------------------------------------------
void ST7789H2_DrawStringScaled(uint16_t x, uint16_t y,
                             const char *str,
                             uint16_t color, uint16_t bg,
                             uint8_t scale)
{
	
    uint16_t orig_x = x;   // Remember starting X for line breaks
    uint16_t line_height = 8 * scale;  // 7 px font + 1 px spacing

	
    while (*str)
    {
			if (*str == '\n')
        {
            y += line_height;   // Move down one line
            x = orig_x;         // Reset to left margin
            str++;
            continue;
        }

        ST7789H2_DrawCharScaled(x, y, *str, color, bg, scale);
        x += 6 * scale;        // 5px + 1px spacing, scaled
        str++;
    }
}

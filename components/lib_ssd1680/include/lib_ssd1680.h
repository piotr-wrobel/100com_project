#pragma once

#define DEBUG

#include <stdint.h>

#include <driver/spi_master.h>
#include <driver/gpio.h>

enum ssd1680_refresh_mode{
  FULL_REFRESH = 0xF7,		/**< Refresh whole screen in a slow robust flickery way */
  PARTIAL_REFRESH = 0xFF,	/**< Refresh updated region in a slow robust flickery way */
  FAST_FULL_REFRESH = 0xC7,	/**< Refresh whole screen in a fast way */
  FAST_PARTIAL_REFRESH = 0xCF	/**< Refresh updated region in a fast way */
};

enum ssd1680_tmp_sensor_ctrl{
	TMP_INTERNAL_SENSOR = 0x80,
	TMP_EXTERNAL_SENSOR = 0x48
};

typedef enum {
    SSD1680_BLACK = 0b00,
    SSD1680_WHITE = 0b01,
    SSD1680_RED = 0b10,
} ssd1680_color_t;

typedef enum {
    SSD1680_NORMAL,
    SSD1680_90_DEG,
    SSD1680_180_DEG,
    SSD1680_270_DEG,
} ssd1680_orientation_t;

typedef enum {
	SSD1680_REVERSE_FALSE,
	SSD1680_REVERSE_TRUE
} ssd1680_reverse_t;

typedef struct {
    gpio_num_t busy;
    gpio_num_t reset;
    gpio_num_t dc;
    gpio_num_t cs;
} ssd1680_pinmap_t;

typedef struct {
    spi_host_device_t   spi_host;
    spi_device_handle_t spi_device;

    ssd1680_pinmap_t      pinmap;
    ssd1680_orientation_t orientation;

    uint8_t* framebuffer_bw;
    uint8_t* framebuffer_red;
    uint32_t framebuffer_size;
    uint16_t pos_x, pos_y;
    uint16_t res_x, res_y;
    uint16_t rows_cnt, clmn_cnt;
} ssd1680_t;

typedef struct {
	uint16_t x;
	uint16_t y;
} ssd1680_cursor_t;

typedef struct {
	const uint8_t x_size;
	const uint8_t y_size;
	const uint8_t bytes_per_row;
	const uint8_t bytes_per_char;
	const unsigned char data[];
} ssd1680_font_t;

ssd1680_t *ssd1680_init(spi_host_device_t spi_host, ssd1680_pinmap_t pinmap, uint16_t res_x, uint16_t res_y, ssd1680_orientation_t orientation);
void ssd1680_deinit(ssd1680_t* disp);

void ssd1680_sleep(ssd1680_t *disp);
void ssd1680_wakeup(ssd1680_t *disp);

void ssd1680_fill(ssd1680_t *disp, ssd1680_color_t color);
void ssd1680_set_pixel(ssd1680_t *disp, uint16_t x, uint16_t y, ssd1680_color_t color);
void ssd1680_draw_line(ssd1680_t *disp, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, ssd1680_color_t color); //Bresenham’s Line Drawing Algorithm
void ssd1680_set_area(ssd1680_t *disp, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t* area, uint16_t area_size, ssd1680_color_t color, ssd1680_reverse_t reverse_byte, ssd1680_reverse_t reverse_bits);
uint16_t ssd1680_display_char(ssd1680_t *disp, ssd1680_font_t * font, uint16_t x, uint16_t y, uint8_t character, ssd1680_color_t color);
ssd1680_cursor_t ssd1680_display_string(ssd1680_t *disp, ssd1680_font_t * font, uint16_t x, uint16_t y, char * string, ssd1680_color_t color);
void ssd1680_send_framebuffer(ssd1680_t *disp);

void ssd1680_set_refresh_window(ssd1680_t *disp, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ssd1680_refresh(ssd1680_t *disp, uint8_t mode);


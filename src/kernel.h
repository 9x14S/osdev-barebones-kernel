#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

enum VGA_COLORS;

static inline uint8_t vga_entry_color( enum VGA_COLORS, enum VGA_COLORS);
static inline uint16_t vga_entry(unsigned char, uint8_t);
void terminal_initialize(void);
void terminal_setcolor(uint8_t);
void terminal_putentryat(char, uint8_t, size_t, size_t);
void terminal_putchar(char);
void terminal_write(const char *, size_t);
void terminal_writestring(const char *);
void kernel_main(void);

/*
 * Style rationale:
 *  Due to this kernel just being a learning experience, I decided to add a lot of comments
 *  to even obvious things.
 *
 *  `Internal` variables have been named verbosely, and almost never changed into short forms.
 *    (`Internal` = related to the kernel's function and not to the language, that's why I still
 *    use `i` as the accumulator for `for` loops.)
 *    Some exceptions apply; for example, for enumerations.
 *
 *  I also added many `TODO: FMO` (FMO = Figure Me Out) on stuff that I didn't understand
 *  at the moment.
 *
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "kernel_utility.h"

#ifdef __linux__
# error "Wtf we're not compiling for linux!!!!"
#endif

#ifndef __i386__
# error "Wtf we ARE compiling for i386!!!"
#endif

// The tutorial has different names for this enum's members
enum VGA_COLORS {
  C_BLACK = 0,
  C_BLUE,
  C_GREEN,
  C_CYAN,
  C_RED,
  C_MAGENTA,
  C_BROWN,
  C_LIGHT_GREY,
  C_DARK_GREY,
  C_LIGHT_BLUE,
  C_LIGHT_GREEN,
  C_LIGHT_CYAN,
  C_LIGHT_RED,
  C_LIGHT_MAGENTA,
  C_LIGHT_BROWN,
  C_WHITE = 15,
};

// TODO: FMO (why inline and static?)
// Returns a color byte
static inline uint8_t vga_entry_color( enum VGA_COLORS foreground, enum VGA_COLORS background)
{
  // Fuse both the foreground color and background into a single byte
  return foreground | background << 4;
}

// Fuses a color byte and a codepoint byte into a VGA-printable short
static inline uint16_t vga_entry(unsigned char codepoint, uint8_t color)
{
  return (uint16_t) codepoint | (uint16_t) color << 8;
}

#define VGA_BUFFER_ADDR 0xB8000

// TODO: FMO (why static on variable? And why const and not a macro?)
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t    terminal_column; // x
size_t    terminal_row;    // y
uint8_t   terminal_color;
uint16_t *terminal_buffer;

void terminal_initialize(void)
{
  // Start at top left (global variables)
  terminal_row = 0;
  terminal_column = 0;

  // TODO: FMO (what if I use C_WHITE instead of C_LIGHT_GREY? What if I emulate Windoze's 
  //  BSOD?)
  // Set light grey on black as default
  terminal_color = vga_entry_color(C_LIGHT_GREY, C_BLACK);
  terminal_buffer = (uint16_t *) VGA_BUFFER_ADDR;

  // Clear the terminal buffer
  for (int y = 0; y < VGA_HEIGHT; y++)
    for (int x = 0; x < VGA_WIDTH; x++) {
      // The tutorial had a `const` here
      size_t index = y * VGA_WIDTH + x;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
}

void terminal_setcolor(uint8_t color)
{
  terminal_color = color;
}

void terminal_putentryat(char character, uint8_t color, size_t x, size_t y)
{
  const size_t index = y * VGA_WIDTH + x;
  terminal_buffer[index] = vga_entry(character, color);
}

void terminal_putchar(char character)
{
  if (character != '\n')
    // Uses the global variables to set the context of the character
    terminal_putentryat(character, terminal_color, terminal_column, terminal_row);
  else
    terminal_column = VGA_WIDTH;

  // If reached the end of the line, start back at position 0 on the next row
  //  jump back to 0,0
    // The tutorial just did `==`
  if (++terminal_column >= VGA_WIDTH)
  {
    terminal_column = 0;

    if (++terminal_row >= VGA_HEIGHT)
      terminal_row = 0;
  }
}

void terminal_write(const char *data, size_t size)
{
  for (int i = 0; i < size; i++)
    terminal_putchar(data[i]);
}

void terminal_writestring(const char *string)
{
  terminal_write(string, strlen(string));
}

// The boot.s kernel stub jumps to here!!!
void kernel_main(void)
{
  terminal_initialize();

  terminal_writestring("The kernel works!\n");
}

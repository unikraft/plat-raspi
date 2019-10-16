/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#include <raspi/gpio.h>
#include <raspi/mbox.h>
#include <raspi/delays.h>

/* PL011 UART registers */
#define UART0_DR        ((volatile unsigned int*)(MMIO_BASE+0x00201000))
#define UART0_FR        ((volatile unsigned int*)(MMIO_BASE+0x00201018))
#define UART0_IBRD      ((volatile unsigned int*)(MMIO_BASE+0x00201024))
#define UART0_FBRD      ((volatile unsigned int*)(MMIO_BASE+0x00201028))
#define UART0_LCRH      ((volatile unsigned int*)(MMIO_BASE+0x0020102C))
#define UART0_CR        ((volatile unsigned int*)(MMIO_BASE+0x00201030))
#define UART0_IMSC      ((volatile unsigned int*)(MMIO_BASE+0x00201038))
#define UART0_ICR       ((volatile unsigned int*)(MMIO_BASE+0x00201044))

static char prev_sent_char = '\0';

/**
 * Set baud rate and characteristics (115200 8N1) and map to GPIO
 */
void _libraspiplat_init_serial_console()
{
    register unsigned int r;

    /* initialize UART */
    *UART0_CR = 0;         // turn off UART0

    /* set up clock for consistent divisor values */
    mbox[0] = 9*4;
    mbox[1] = MBOX_REQUEST;
    mbox[2] = MBOX_TAG_SETCLKRATE; // set clock rate
    mbox[3] = 12;
    mbox[4] = 8;
    mbox[5] = 2;           // UART clock
    mbox[6] = 4000000;     // 4Mhz
    mbox[7] = 0;           // clear turbo
    mbox[8] = MBOX_TAG_LAST;
    mbox_call(MBOX_CH_PROP);

    /* map UART0 to GPIO pins */
    r=*GPFSEL1;
    r&=~((7<<12)|(7<<15)); // gpio14, gpio15
    r|=(4<<12)|(4<<15);    // alt0
    *GPFSEL1 = r;
    *GPPUD = 0;            // enable pins 14 and 15
    wait_cycles(150);
    *GPPUDCLK0 = (1<<14)|(1<<15);
    wait_cycles(150);
    *GPPUDCLK0 = 0;        // flush GPIO setup

    *UART0_ICR = 0x7FF;    // clear interrupts
    *UART0_IBRD = 2;       // 115200 baud
    *UART0_FBRD = 0xB;
    *UART0_LCRH = 0b11<<5; // 8n1
    *UART0_CR = 0x301;     // enable Tx, Rx, FIFO
}

unsigned int serial_tx_buffer_full(void)
{
	return *UART0_FR&0x20;
}

/**
 * Send a character
 */
void _libraspiplat_serial_putc(char c) {
	if ((c == '\n') && (prev_sent_char != '\r'))
		_libraspiplat_serial_putc('\r');

    /* wait until we can send */
    do{
		asm volatile("nop");
	} while (serial_tx_buffer_full());

    /* write the character to the buffer */
    *UART0_DR = c;
	prev_sent_char = c;
}

unsigned int serial_rx_buffer_empty(void)
{
	return *UART0_FR&0x10;
}

/**
 * Receive a character
 */
int  _libraspiplat_serial_getc(void) {
	if (serial_rx_buffer_empty())
		return -1;

    char r;
    r = (char)(*UART0_DR);
    return (int)r;
}

void serial_console_send_string(char *str) {
	while(*str) {
        _libraspiplat_serial_putc(*str++);
    }

	do{
		asm volatile("nop");
	} while (serial_tx_buffer_full());
}

void serial_console_send_string_i(char *str, int value) {
	while(*str) {
		if (*str == '%') {
			str++;
			if (*str == 'd') {
				int lenght = 1;
				int divisor = 1;
				int valueCopy = value;
				valueCopy /= 10;
				while (valueCopy > 0) {
					valueCopy /= 10;
					divisor *= 10;
					lenght++;
				}

				_libraspiplat_serial_putc('0' + value / divisor);
				while (divisor > 1) {
					value %= divisor;
					divisor /= 10;
					_libraspiplat_serial_putc('0' + value / divisor);
				}
			} else {
				str--;
			}
		}
        _libraspiplat_serial_putc(*str++);
    }

	do{
		asm volatile("nop");
	} while (serial_tx_buffer_full());
}

void serial_console_send_string_u(char *str, unsigned int value) {
	while(*str) {
		if (*str == '%') {
			str++;
			if (*str == 'u') {
				unsigned int  lenght = 1;
				unsigned int divisor = 1;
				unsigned int valueCopy = value;
				valueCopy /= 10;
				while (valueCopy > 0) {
					valueCopy /= 10;
					divisor *= 10;
					lenght++;
				}

				_libraspiplat_serial_putc('0' + value / divisor);
				while (divisor > 1) {
					value %= divisor;
					divisor /= 10;
					_libraspiplat_serial_putc('0' + value / divisor);
				}
			} else {
				str--;
			}
		}
        _libraspiplat_serial_putc(*str++);
    }

	do{
		asm volatile("nop");
	} while (serial_tx_buffer_full());
}

void serial_console_send_string_lu(char *str, unsigned long value) {
	while(*str) {
		if (*str == '%') {
			str++;
			if (*str == 'l') {
				str++;
				if (*str == 'u') {
					unsigned long lenght = 1;
					unsigned long divisor = 1;
					unsigned long valueCopy = value;
					valueCopy /= 10;
					while (valueCopy > 0) {
						valueCopy /= 10;
						divisor *= 10;
						lenght++;
					}

					_libraspiplat_serial_putc('0' + value / divisor);
					while (divisor > 1) {
						value %= divisor;
						divisor /= 10;
						_libraspiplat_serial_putc('0' + value / divisor);
					}
				} else {
					str--;
					str--;
				}
			} else {
				str--;
			}
		}
        _libraspiplat_serial_putc(*str++);
    }

	do{
		asm volatile("nop");
	} while (serial_tx_buffer_full());
}

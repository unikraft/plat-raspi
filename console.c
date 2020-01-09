/* SPDX-License-Identifier: ISC */
/*
 * Authors: Dan Williams
 *          Martin Lucina
 *          Felipe Huici <felipe.huici@neclab.eu>
 *          Florian Schmidt <florian.schmidt@neclab.eu>
 *          Simon Kuenzer <simon.kuenzer@neclab.eu>
 *          Santiago Pagani <santiagopagani@gmail.com>
 *
 * Copyright (c) 2015-2017 IBM
 * Copyright (c) 2016-2017 Docker, Inc.
 * Copyright (c) 2020, NEC Laboratories Europe GmbH, NEC Corporation.
 *                     All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose with or without fee is hereby granted, provided
 * that the above copyright notice and this permission notice appear
 * in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <uk/plat/console.h>
#include <uk/config.h>
#include <uk/essentials.h>
#if (CONFIG_RASPI_PRINTF_SERIAL_CONSOLE || CONFIG_RASPI_DEBUG_SERIAL_CONSOLE || CONFIG_RASPI_KERNEL_SERIAL_CONSOLE)
#include <raspi/serial_console.h>
#endif

void _libraspiplat_init_console(void)
{
#if (CONFIG_RASPI_PRINTF_SERIAL_CONSOLE || CONFIG_RASPI_DEBUG_SERIAL_CONSOLE || CONFIG_RASPI_KERNEL_SERIAL_CONSOLE)
	_libraspiplat_init_serial_console();
#endif
}

int ukplat_coutd(const char *buf __maybe_unused, unsigned int len)
{
	for (unsigned int i = 0; i < len; i++) {
#if (CONFIG_RASPI_PRINTF_SERIAL_CONSOLE || CONFIG_RASPI_DEBUG_SERIAL_CONSOLE)
		_libraspiplat_serial_putc(buf[i]);
#endif
	}
	return len;
}

int ukplat_coutk(const char *buf __maybe_unused, unsigned int len)
{
	for (unsigned int i = 0; i < len; i++) {
#if (CONFIG_RASPI_PRINTF_SERIAL_CONSOLE || CONFIG_RASPI_KERNEL_SERIAL_CONSOLE)
		_libraspiplat_serial_putc(buf[i]);
#endif
	}
	return len;
}

int ukplat_cink(char *buf __maybe_unused, unsigned int maxlen __maybe_unused)
{
	int ret __maybe_unused;
	unsigned int num = 0;

#if (CONFIG_RASPI_PRINTF_SERIAL_CONSOLE || CONFIG_RASPI_KERNEL_SERIAL_CONSOLE)
	while (num < maxlen
	       && (ret = _libraspiplat_serial_getc()) >= 0) {
		*(buf++) = (char) ret;
		num++;
	}
#endif
	return (int) num;
}

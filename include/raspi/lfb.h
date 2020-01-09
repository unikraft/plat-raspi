/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *               https://github.com/bztsrc/raspi3-tutorial
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

#ifndef __RASPI_LFB_H__
#define __RASPI_LFB_H__

#define LFB_LIB_SIZE_BYTES	(1708040)

void lfb_init(void);

void lfb_show_unikraft_logo(void);
void lfb_show_letter(unsigned int x_pos, unsigned y_pos, char letter);
void lfb_show_string(unsigned int x_pos, unsigned y_pos, char *str);
void lfb_show_string_u(unsigned int x_pos, unsigned y_pos, char *str, unsigned int value);

#endif /* __RASPI_LFB_H__ */

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

#include "uart.h"
#include "mbox.h"
#include "pixmaps.h"

#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	480
#define PIXEL_MEM_SIZE	4

unsigned int width, height, pitch;
unsigned char *lfb;

/**
 * Set screen resolution to 800x480
 */
void lfb_init()
{
    mbox[0] = 35*4;
    mbox[1] = MBOX_REQUEST;

    mbox[2] = 0x48003;  //set phy wh
    mbox[3] = 8;
    mbox[4] = 8;
    mbox[5] = SCREEN_WIDTH;         //FrameBufferInfo.width
    mbox[6] = SCREEN_HEIGHT;          //FrameBufferInfo.height

    mbox[7] = 0x48004;  //set virt wh
    mbox[8] = 8;
    mbox[9] = 8;
    mbox[10] = SCREEN_WIDTH;        //FrameBufferInfo.virtual_width
    mbox[11] = SCREEN_HEIGHT;         //FrameBufferInfo.virtual_height
    
    mbox[12] = 0x48009; //set virt offset
    mbox[13] = 8;
    mbox[14] = 8;
    mbox[15] = 0;           //FrameBufferInfo.x_offset
    mbox[16] = 0;           //FrameBufferInfo.y.offset
    
    mbox[17] = 0x48005; //set depth
    mbox[18] = 4;
    mbox[19] = 4;
    mbox[20] = 32;          //FrameBufferInfo.depth

    mbox[21] = 0x48006; //set pixel order
    mbox[22] = 4;
    mbox[23] = 4;
    mbox[24] = 1;           //RGB, not BGR preferably

    mbox[25] = 0x40001; //get framebuffer, gets alignment on request
    mbox[26] = 8;
    mbox[27] = 8;
    mbox[28] = 4096;        //FrameBufferInfo.pointer
    mbox[29] = 0;           //FrameBufferInfo.size

    mbox[30] = 0x40008; //get pitch
    mbox[31] = 4;
    mbox[32] = 4;
    mbox[33] = 0;           //FrameBufferInfo.pitch

    mbox[34] = MBOX_TAG_LAST;

    if(mbox_call(MBOX_CH_PROP) && mbox[20]==32 && mbox[28]!=0) {
        mbox[28]&=0x3FFFFFFF;
        width=mbox[5];
        height=mbox[6];
        pitch=mbox[33];
        lfb=(void*)((unsigned long)mbox[28]);
    } else {
        uart_puts("Unable to set screen resolution to 800x480x32\n");
    }


	letters_data [0x20 - LETTER_MIN] = letter_space_data;	// 0x20
	letters_data [0x21 - LETTER_MIN] = letter_space_data;	// 0x21
	letters_data [0x22 - LETTER_MIN] = letter_space_data;	// 0x22
	letters_data [0x23 - LETTER_MIN] = letter_space_data;	// 0x23
	letters_data [0x24 - LETTER_MIN] = letter_space_data;	// 0x24
	letters_data [0x25 - LETTER_MIN] = letter_space_data;	// 0x25
	letters_data [0x26 - LETTER_MIN] = letter_space_data;	// 0x26
	letters_data [0x27 - LETTER_MIN] = letter_space_data;	// 0x27
	letters_data [0x28 - LETTER_MIN] = letter_space_data;	// 0x28
	letters_data [0x29 - LETTER_MIN] = letter_space_data;	// 0x29
	letters_data [0x2A - LETTER_MIN] = letter_space_data;	// 0x2A
	letters_data [0x2B - LETTER_MIN] = letter_space_data;	// 0x2B
	letters_data [0x2C - LETTER_MIN] = letter_space_data;	// 0x2C
	letters_data [0x2D - LETTER_MIN] = letter_space_data;	// 0x2D
	letters_data [0x2E - LETTER_MIN] = letter_space_data;	// 0x2E
	letters_data [0x2F - LETTER_MIN] = letter_space_data;	// 0x2F
	letters_data [0x30 - LETTER_MIN] = letter_0_data;		// 0x30
	letters_data [0x31 - LETTER_MIN] = letter_1_data;		// 0x31
	letters_data [0x32 - LETTER_MIN] = letter_2_data;		// 0x32
	letters_data [0x33 - LETTER_MIN] = letter_3_data;		// 0x33
	letters_data [0x34 - LETTER_MIN] = letter_4_data;		// 0x34
	letters_data [0x35 - LETTER_MIN] = letter_5_data;		// 0x35
	letters_data [0x36 - LETTER_MIN] = letter_6_data;		// 0x36
	letters_data [0x37 - LETTER_MIN] = letter_7_data;		// 0x37
	letters_data [0x38 - LETTER_MIN] = letter_8_data;		// 0x38
	letters_data [0x39 - LETTER_MIN] = letter_9_data;		// 0x39
	letters_data [0x3A - LETTER_MIN] = letter_space_data;	// 0x3A
	letters_data [0x3B - LETTER_MIN] = letter_space_data;	// 0x3B
	letters_data [0x3C - LETTER_MIN] = letter_space_data;	// 0x3C
	letters_data [0x3D - LETTER_MIN] = letter_space_data;	// 0x3D
	letters_data [0x3E - LETTER_MIN] = letter_space_data;	// 0x3E
	letters_data [0x3F - LETTER_MIN] = letter_space_data;	// 0x3F
	letters_data [0x40 - LETTER_MIN] = letter_space_data;	// 0x40
	letters_data [0x41 - LETTER_MIN] = letter_A_data;		// 0x41
	letters_data [0x42 - LETTER_MIN] = letter_B_data;		// 0x42
	letters_data [0x43 - LETTER_MIN] = letter_C_data;		// 0x43
	letters_data [0x44 - LETTER_MIN] = letter_D_data;		// 0x44
	letters_data [0x45 - LETTER_MIN] = letter_E_data;		// 0x45
	letters_data [0x46 - LETTER_MIN] = letter_F_data;		// 0x46
	letters_data [0x47 - LETTER_MIN] = letter_G_data;		// 0x47
	letters_data [0x48 - LETTER_MIN] = letter_H_data;		// 0x48
	letters_data [0x49 - LETTER_MIN] = letter_I_data;		// 0x49
	letters_data [0x4A - LETTER_MIN] = letter_J_data;		// 0x4A
	letters_data [0x4B - LETTER_MIN] = letter_K_data;		// 0x4B
	letters_data [0x4C - LETTER_MIN] = letter_L_data;		// 0x4C
	letters_data [0x4D - LETTER_MIN] = letter_M_data;		// 0x4D
	letters_data [0x4E - LETTER_MIN] = letter_N_data;		// 0x4E
	letters_data [0x4F - LETTER_MIN] = letter_O_data;		// 0x4F
	letters_data [0x50 - LETTER_MIN] = letter_P_data;		// 0x50
	letters_data [0x51 - LETTER_MIN] = letter_Q_data;		// 0x51
	letters_data [0x52 - LETTER_MIN] = letter_R_data;		// 0x52
	letters_data [0x53 - LETTER_MIN] = letter_S_data;		// 0x53
	letters_data [0x54 - LETTER_MIN] = letter_T_data;		// 0x54
	letters_data [0x55 - LETTER_MIN] = letter_U_data;		// 0x55
	letters_data [0x56 - LETTER_MIN] = letter_V_data;		// 0x56
	letters_data [0x57 - LETTER_MIN] = letter_W_data;		// 0x57
	letters_data [0x58 - LETTER_MIN] = letter_X_data;		// 0x58
	letters_data [0x59 - LETTER_MIN] = letter_Y_data;		// 0x59
	letters_data [0x5A - LETTER_MIN] = letter_Z_data;		// 0x5A
	letters_data [0x5B - LETTER_MIN] = letter_space_data;	// 0x5B
	letters_data [0x5C - LETTER_MIN] = letter_space_data;	// 0x5C
	letters_data [0x5D - LETTER_MIN] = letter_space_data;	// 0x5D
	letters_data [0x5E - LETTER_MIN] = letter_space_data;	// 0x5E
	letters_data [0x5F - LETTER_MIN] = letter_space_data;	// 0x5F
	letters_data [0x60 - LETTER_MIN] = letter_space_data;	// 0x60
	letters_data [0x61 - LETTER_MIN] = letter_a_data;		// 0x61
	letters_data [0x62 - LETTER_MIN] = letter_b_data;		// 0x62
	letters_data [0x63 - LETTER_MIN] = letter_c_data;		// 0x63
	letters_data [0x64 - LETTER_MIN] = letter_d_data;		// 0x64
	letters_data [0x65 - LETTER_MIN] = letter_e_data;		// 0x65
	letters_data [0x66 - LETTER_MIN] = letter_f_data;		// 0x66
	letters_data [0x67 - LETTER_MIN] = letter_g_data;		// 0x67
	letters_data [0x68 - LETTER_MIN] = letter_h_data;		// 0x68
	letters_data [0x69 - LETTER_MIN] = letter_i_data;		// 0x69
	letters_data [0x6A - LETTER_MIN] = letter_j_data;		// 0x6A
	letters_data [0x6B - LETTER_MIN] = letter_k_data;		// 0x6B
	letters_data [0x6C - LETTER_MIN] = letter_l_data;		// 0x6C
	letters_data [0x6D - LETTER_MIN] = letter_m_data;		// 0x6D
	letters_data [0x6E - LETTER_MIN] = letter_n_data;		// 0x6E
	letters_data [0x6F - LETTER_MIN] = letter_o_data;		// 0x6F
	letters_data [0x70 - LETTER_MIN] = letter_p_data;		// 0x70
	letters_data [0x71 - LETTER_MIN] = letter_q_data;		// 0x71
	letters_data [0x72 - LETTER_MIN] = letter_r_data;		// 0x72
	letters_data [0x73 - LETTER_MIN] = letter_s_data;		// 0x73
	letters_data [0x74 - LETTER_MIN] = letter_t_data;		// 0x74
	letters_data [0x75 - LETTER_MIN] = letter_u_data;		// 0x75
	letters_data [0x76 - LETTER_MIN] = letter_v_data;		// 0x76
	letters_data [0x77 - LETTER_MIN] = letter_w_data;		// 0x77
	letters_data [0x78 - LETTER_MIN] = letter_x_data;		// 0x78
	letters_data [0x79 - LETTER_MIN] = letter_y_data;		// 0x79
	letters_data [0x7A - LETTER_MIN] = letter_z_data;		// 0x7A
	letters_data [0x7B - LETTER_MIN] = letter_space_data;	// 0x7B
	letters_data [0x7C - LETTER_MIN] = letter_space_data;	// 0x7C
	letters_data [0x7D - LETTER_MIN] = letter_space_data;	// 0x7D
	letters_data [0x7E - LETTER_MIN] = letter_space_data;	// 0x7E
	letters_data [0x7F - LETTER_MIN] = letter_space_data;	// 0x7F
}

/**
 * Show a picture
 */
void lfb_show_unikraft_logo()
{
    unsigned char *ptr=lfb;
    char pixel[4];

    ptr += (height-unikraft_logo_height)/2*pitch + (width-unikraft_logo_width)*2;
    for(unsigned int y=0; y<unikraft_logo_height; y++) {
        for(unsigned int x=0; x<unikraft_logo_width; x++) {
            HEADER_PIXEL(unikraft_logo_data, pixel);
            *((unsigned int*)ptr)=*((unsigned int *)&pixel);
            ptr+=4;
        }
        ptr+=pitch-unikraft_logo_width*4;
    }
}

void lfb_show_letter(char letter, unsigned int x_pos, unsigned y_pos) {
	if ((letter < LETTER_MIN) || (letter > LETTER_MAX))
		return;

    unsigned char *ptr=lfb;
	char pixel[PIXEL_MEM_SIZE];
	char *data = letters_data[letter - LETTER_MIN];

    ptr += y_pos*pitch + x_pos*PIXEL_MEM_SIZE;
    for (unsigned int y=0; y<letter_height; y++) {
        for (unsigned int x=0; x<letter_width; x++) {
			if ((x < SCREEN_WIDTH) && (y < SCREEN_HEIGHT)) {
            	HEADER_PIXEL(data, pixel);
            	*((unsigned int*)ptr)=*((unsigned int *)&pixel);
			}
            ptr+=PIXEL_MEM_SIZE;
        }
        ptr+=pitch-letter_width*PIXEL_MEM_SIZE;
    }
}

void lfb_show_string(char *str, unsigned int x_pos, unsigned y_pos) {
	unsigned int x = x_pos;

	while (*str != '\0') {
		if (*str == '\n') {
			x = x_pos;
			y_pos += letter_height;

			if (y_pos > SCREEN_HEIGHT)
				return;
		} else {
			if (x < SCREEN_WIDTH) {
				lfb_show_letter(*str, x, y_pos);
				x += letter_width;
			}
		}
		str++;
	}
}
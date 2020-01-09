/*
 * Driver for memory based ft5406 touchscreen
 *
 * Copyright (C) 2015, 2017 Raspberry Pi
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <uk/print.h>
#include <string.h>
#include <raspi/mbox.h>
#include <raspi/touchscreen.h>

#define LV_CONF_INCLUDE_SIMPLE
#include <lvgl.h>

static unsigned char *touchscreen;
static int known_ids = 0;

#define MAXIMUM_SUPPORTED_POINTS 10
#define FTS_TOUCH_DOWN      0
#define FTS_TOUCH_UP        1
#define FTS_TOUCH_CONTACT   2

#define ID_TO_BIT(a) (1 << a)

struct ft5406_regs {
	uint8_t device_mode;
	uint8_t gesture_id;
	uint8_t num_points;
	struct ft5406_touch {
		uint8_t xh;
		uint8_t xl;
		uint8_t yh;
		uint8_t yl;
		uint8_t pressure;
		uint8_t area;
	} point[MAXIMUM_SUPPORTED_POINTS];
};

static lv_coord_t last_x[MAXIMUM_SUPPORTED_POINTS];
static lv_coord_t last_y[MAXIMUM_SUPPORTED_POINTS];
static lv_indev_drv_t indev_drv;

bool touchscreen_update(lv_indev_drv_t * indev_driver __unused, lv_indev_data_t * data)
{
	struct ft5406_regs regs;

	memcpy(&regs, touchscreen, sizeof(struct ft5406_regs));

	unsigned char *numpoints = touchscreen + offsetof(struct ft5406_regs, num_points);
	*numpoints = 99;
	data->state = LV_INDEV_STATE_REL;
	data->point.x = last_x[0];
	data->point.y = last_y[0];
	
	if (!(regs.num_points == 99 || (regs.num_points == 0 && known_ids == 0))) {
		int modified_ids = 0, released_ids;

		for (int i = 0; i < regs.num_points; i++) {
			int x = (((int) regs.point[i].xh & 0xf) << 8) + regs.point[i].xl;
			int y = (((int) regs.point[i].yh & 0xf) << 8) + regs.point[i].yl;
			int touchid = (regs.point[i].yh >> 4) & 0xf;
			int event_type = (regs.point[i].xh >> 6) & 0x03;

			modified_ids |= ID_TO_BIT(touchid);

			if (event_type == FTS_TOUCH_DOWN || event_type == FTS_TOUCH_CONTACT) {
				if (i == 0) {
					data->state = LV_INDEV_STATE_PR;
					data->point.x = last_x[i] = (lv_coord_t) x;
					data->point.y = last_y[i] = (lv_coord_t) y;
				}
				if (!((ID_TO_BIT(touchid)) & known_ids)) {
					uk_pr_debug("Pressed  (point %d): x = %d, y = %d\n", i, x, y);
				} else {
					uk_pr_debug("Held     (point %d): x = %d, y = %d\n", i, x, y);
				}
			}
		}

		released_ids = known_ids & ~modified_ids;
		for (int i = 0; released_ids && i < MAXIMUM_SUPPORTED_POINTS; i++) {
			if (released_ids & (1 << i)) {
				uk_pr_debug("Released (point %d): x = %d, y = %d\n", i, last_x[i], last_y[i]);
				modified_ids &= ~(ID_TO_BIT(i));
			}
		}
		known_ids = modified_ids;
	}

	return false;
}

void touchscreen_init()
{
    mbox[0] = 8*4;
    mbox[1] = MBOX_REQUEST;

    mbox[2] = 0x4801f;
    mbox[3] = 4;
    mbox[4] = 4;
    mbox[5] = 0;
    mbox[6] = 0;

    mbox[7] = MBOX_TAG_LAST;

	// #TODO: I believe we should be receiving the pointer to the touchscreen buffer in mbox[5] and the buffer size in
	// mbox[6]. However for some reason I'm receiving those two bytes as 0. The most curious thing is that then the
	// touchscreen actually works when reading from address 0. Maybe that is the actual address of the touchscreen buffer?
	// If so, it still does not make sense why the size we are reading is 0. Until we figure this out, because it is working
	// when reading from address 0, we do not compare against an address that is non-zero to say that the mailbox command
	// succeded.
    //if(mbox_call(MBOX_CH_PROP) && mbox[5] != 0) {
	if(mbox_call(MBOX_CH_PROP)) {
		touchscreen = (void*)((unsigned long)mbox[5]);
    } else {
        uk_pr_err("Unable to get touchscreen buffer\n");
    }

	lv_indev_drv_init(&indev_drv);				// Descriptor of a input device driver
	indev_drv.type = LV_INDEV_TYPE_POINTER;		// Touch pad is a pointer-like device
	indev_drv.read_cb = touchscreen_update;		// Set your driver function
	lv_indev_drv_register(&indev_drv);			// Finally register the driver
}

/*
 * Copyright (C) 2016 Rockchip Electronics Co.Ltd
 * Authors:
 *	Zhiqin Wei <wzq@rock-chips.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#ifndef _rk_drm_rga_
#define _rk_drm_rga_

#include <stdint.h>
#include <sys/cdefs.h>
#include "rga.h"

/*****************************************************************************/

/* for compatibility */
#define DRM_RGA_MODULE_API_VERSION      HWC_MODULE_API_VERSION_0_1
#define DRM_RGA_DEVICE_API_VERSION      HWC_DEVICE_API_VERSION_0_1
#define DRM_RGA_API_VERSION             HWC_DEVICE_API_VERSION

#define DRM_RGA_TRANSFORM_ROT_MASK      0x0000000F
#define DRM_RGA_TRANSFORM_ROT_0         0x00000000
#define DRM_RGA_TRANSFORM_ROT_90        HAL_TRANSFORM_ROT_90
#define DRM_RGA_TRANSFORM_ROT_180       HAL_TRANSFORM_ROT_180
#define DRM_RGA_TRANSFORM_ROT_270       HAL_TRANSFORM_ROT_270

#define DRM_RGA_TRANSFORM_FLIP_MASK     0x00000003
#define DRM_RGA_TRANSFORM_FLIP_H        HAL_TRANSFORM_FLIP_H
#define DRM_RGA_TRANSFORM_FLIP_V        HAL_TRANSFORM_FLIP_V

enum {
	AWIDTH                      = 0,
	AHEIGHT,
	ASTRIDE,
	AFORMAT,
	ASIZE,
	ATYPE,
};
/*****************************************************************************/


typedef struct bo {
    int fd;
    void *ptr;
    size_t size;
    size_t offset;
    size_t pitch;
    unsigned handle;
 }bo_t;//外部数据结构

/*
   @value size:     user not need care about.For avoid read/write out of memory
 */
typedef struct rga_rect {
	int xoffset;
	int yoffset;
	int width;
	int height;
	int wstride;
	int hstride;
	int format;
	int size;
} rga_rect_t;

/*
   @value fd:     use fd to share memory, it can be ion shard fd,and dma fd.
   @value virAddr:userspace address
   @value phyAddr:use phy address
   @value hnd:    use buffer_handle_t
 */
typedef struct rga_info {
	int fd;
	void *virAddr;
	void *phyAddr;
	unsigned hnd;
	int format;
	rga_rect_t rect;
	unsigned int blend;
	int bufferSize;
	int rotation;
	int color;
	int testLog;
	int mmuFlag;
	int reserve[128];
} rga_info_t;


typedef struct drm_rga {
	rga_rect_t src;
	rga_rect_t dst;
} drm_rga_t;

/*
   @fun rga_set_rect:For use to set the rects esayly

   @param rect:The rect user want to set,like setting the src rect:
   drm_rga_t rects;
   rga_set_rect(rects.src,0,0,1920,1080,1920,NV12);
   mean to set the src rect to the value.
 */
static inline int rga_set_rect(rga_rect_t *rect,
		int x, int y, int w, int h, int sw, int sh, int f)
{
	if (!rect)
		return -EINVAL;

	rect->xoffset = x;
	rect->yoffset = y;
	rect->width = w;
	rect->height = h;
	rect->wstride = sw;
	rect->hstride = sh;
	rect->format = f;

	return 0;
}
/*****************************************************************************/

#endif

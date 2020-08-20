/*
 * Copyright 201=9 Rockchip Electronics S.LSI Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef __ROCKCHIP_DRM_H__
#define __ROCKCHIP_DRM_H__

#include <linux/videodev2.h>
#include <drm.h>
#include <xf86drm.h>

typedef struct _DrmBuffer {
    int fd;
    unsigned int handle;
    void *ptr;
    size_t size;
    unsigned long phys;
} DrmBuffer, CmaBuffer;

int RockchipDrmOpen();
void RockchipDrmClose(int fd);

/* Drm Memory Allocator: physical address is non-contiguous */
DrmBuffer *RockchipDrmAlloc(int fd, __u32 v4l2Format, __u32 width, __u32 height);
void RockchipDrmFree(int fd, DrmBuffer *buf);

/* Contiguous Memory Allocator: physical address is contiguous */
CmaBuffer *RockchipCmaAlloc(int fd, size_t size);
void RockchipCmaFree(int fd, CmaBuffer *buf);

#endif

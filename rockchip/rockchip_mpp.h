/*
 * Copyright 2017 Rockchip Electronics S.LSI Co. LTD
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

#ifndef __ROCKCHIP_MPP_H__
#define __ROCKCHIP_MPP_H__

#include <linux/videodev2.h>
#include "mpp_buffer.h"
#include "mpp_err.h"
#include "mpp_frame.h"
#include "mpp_meta.h"
#include "mpp_packet.h"
#include "mpp_task.h"
#include "rk_mpi.h"
#include "rk_type.h"
#include "vpu_api.h"

/*************** Rockchip MPP Decoder *******************/
struct _MppDecoder;

/*
 * Decode type support
 */
typedef enum {
    DECODE_TYPE_H264 = 0,
    DECODE_TYPE_H265,
    DECODE_TYPE_MJPEG,
    DECODE_TYPE_MAX,
} DecodeType;
 
typedef struct _DecFrame {
    MppFrame mppFrame;
    /* Decode format will only be V4L2_PIX_FMT_NV12. */
    __u32 v4l2Format;
    __u32 width;
    __u32 height;

    __u32 coded_width;
    __u32 coded_height;

    int fd;
    void *data;
    size_t size;
    MppBufferGroup frameGroup;
    MppBuffer frameBuf;
} DecFrame;

typedef struct _DecOps {
    int (*enqueue)(struct _MppDecoder *dec, void *data, size_t size);
    /* Function: nonblock */
    DecFrame *(*dequeue)(struct _MppDecoder *dec);
    /* Function: Block until timeout
     * Parameter: timeout_ms
     *   1)  0: nonblock
     *   2) -1: block
     *   3)  n: timeout value in ms
     */
    DecFrame *(*dequeue_timeout)(struct _MppDecoder *dec, int timeout_ms);
    DecFrame *(*decode)(struct _MppDecoder *dec, void *data, size_t size);
    void (*freeFrame)(DecFrame *dec);
} DecOps;

typedef struct _MppDecoder {
    MppApi *mpi;
    MppCtx mctx;
    DecOps *ops;
    DecodeType type;
} MppDecoder;

MppDecoder *MppDecoderCreate(DecodeType type);
void MppDecoderDestroy(MppDecoder *dec);

/*************** Rockchip MPP Encoder *******************/
#define ENCODE_BUFFER_COUNT	16
struct _MppEncoder;

/*
 * Encode type support
 */
typedef enum {
    ENCODE_TYPE_H264 = 0,
    ENCODE_TYPE_MJPEG,
    ENCODE_TYPE_MAX,
} EncodeType;
 
/*
 * Encode format support
 */
static int checkV4l2Format(__u32 v4l2Format)
{
    switch(v4l2Format) {
    case V4L2_PIX_FMT_YUV420:
    case V4L2_PIX_FMT_NV12:
    case V4L2_PIX_FMT_NV21:
    case V4L2_PIX_FMT_YUV422P:
    case V4L2_PIX_FMT_NV16:
    case V4L2_PIX_FMT_YUYV:
    case V4L2_PIX_FMT_UYVY:
    case V4L2_PIX_FMT_RGB565:
    case V4L2_PIX_FMT_RGB24:
    case V4L2_PIX_FMT_BGR24:
    case V4L2_PIX_FMT_RGB32:
    case V4L2_PIX_FMT_BGR32:
        return 0;
    default:
        return -1;
    }
}

/*
 * RcMode - rate control mode, default CBR
 * 0 - cbr mode, Constant bit rate
 * 1 - vbr mode, variable bit rate
 */
typedef enum _EncodeRcMode {
    ENCODE_RC_MODE_CBR = 0,
    ENCODE_RC_MODE_VBR,
} EncodeRcMode;

/*
 * Quality - quality parameter, default CQP
 * mpp does not give the direct parameter in different protocol.
 * mpp provide total 6 quality level 0 ~ 5
 * 0 - cqp
 * 1 - worst
 * 2 - worse
 * 3 - medium
 * 4 - better
 * 5 - best
 */
typedef enum _EncodeQuality {
    ENCODE_QUALITY_CQP = 0,
    ENCODE_QUALITY_WORST,
    ENCODE_QUALITY_WORSE,
    ENCODE_QUALITY_MEDIUM,
    ENCODE_QUALITY_BETTER,
    ENCODE_QUALITY_BEST,
} EncodeQuality;

/*
 * Profile -profile parameter, default HIGH
 * 66  - Baseline Profile
 * 77  - Main Profile
 * 100 - High Profile 
 */
typedef enum _EncodeProfile {
    ENCODE_PROFILE_BASELINE = 66,
    ENCODE_PROFILE_MAIN = 77,
    ENCODE_PROFILE_HIGH = 100,
} EncodeProfile;

/* Quant - JPEG Quantization, default 10
 */

typedef struct _EncPacket {
    MppPacket mppPacket;
    int fd;
    void *data;
    size_t size;
    int is_intra;
} EncPacket;

typedef struct _EncCtx {
    __u32 v4l2Format;
    __u32 width;
    __u32 height;
    size_t size;

    int fps;
    int bps;
    int gop;
    EncodeRcMode mode;
    EncodeQuality quality;
    union {
        int profile;
        int quant;
    };
} EncCtx;

typedef struct _EncOps {
    /* Import external drm buffer fd */
    int (*importBufferFd)(struct _MppEncoder *enc, int fd, int index);
    /* Get sps/pps for H.264 */
    EncPacket *(*getExtraData)(struct _MppEncoder *enc);
    int (*enqueue)(struct _MppEncoder *enc, int index);
    EncPacket *(*dequeue)(struct _MppEncoder *enc);
    void (*freePacket)(EncPacket *packet);
} EncOps;

typedef struct _MppEncoder {
    MppCtx mctx;
    MppApi *mpi;

    EncCtx ctx;
    EncOps *ops;
    
    MppBuffer buf[ENCODE_BUFFER_COUNT];
} MppEncoder;

MppEncoder *MppEncoderCreate(EncCtx ctx, EncodeType type);
void MppEncoderDestroy(MppEncoder *enc);

#endif

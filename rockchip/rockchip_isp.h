#ifndef __ROCKCHIP_ISP_H__
#define __ROCKCHIP_ISP_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef HAL_AF_MODE_NOT_SET
enum HAL_AF_MODE {
	HAL_AF_MODE_NOT_SET = -1,
	HAL_AF_MODE_AUTO,
	HAL_AF_MODE_MACRO,
	HAL_AF_MODE_INFINITY,
	HAL_AF_MODE_FIXED,
	HAL_AF_MODE_EDOF,
	HAL_AF_MODE_CONTINUOUS_VIDEO,
	HAL_AF_MODE_CONTINUOUS_PICTURE,
};
#endif

int rkisp_start(void* &engine, int vidFd, const char* ispNode, const char* tuningFile);

int rkisp_stop(void* &engine);

int rkisp_setFocusMode(void* &engine, enum HAL_AF_MODE fcMode);

#ifdef __cplusplus
}
#endif

#endif


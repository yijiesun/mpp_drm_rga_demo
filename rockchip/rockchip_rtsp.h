#ifndef ROCKCHIP_RTSP_H
#define ROCKCHIP_RTSP_H
#include <functional>
#include <string>

typedef std::function<void(unsigned char *, size_t)> FRtspCallBack;

class RtspClient {
public:
    RtspClient(std::string url, std::string username = "", std::string password = "");
    ~RtspClient();
    void setDataCallback(FRtspCallBack callBack);
    void enable();
    void disable();
    size_t onTransmitIn(unsigned char *buf, size_t size, int index, int flag);

private:
    void *mRtspClient;
    void *mRtpH26X;
    void *mCallBackEP;
    FRtspCallBack mCallback;
};


#endif //ROCKCHIP_RTSP_H

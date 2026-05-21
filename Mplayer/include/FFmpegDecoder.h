#pragma once

extern "C" {

    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>
}
#include <string>
#include <iostream>
#include <vector>

class FFmpegDecoder {

public:

    FFmpegDecoder();
    ~FFmpegDecoder();

    bool openFile(const std::string& path);
    bool decodeFrame();

    int getWidth();

    int getHeight();
    AVFrame* getRGBFrame();
    double getfps();
private:

    AVFormatContext* formatContext;
    AVCodecContext* codecContext;
    SwsContext* swsContext = NULL;

    AVFrame* frame;
    AVFrame* rgbframe;
    AVPacket* packet;
    
    std::vector<uint8_t> buffer;
    int videoStreamIndex;
};
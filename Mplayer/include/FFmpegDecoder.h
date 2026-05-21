#pragma once

extern "C" {

    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}
#include <string>
#include <iostream>

class FFmpegDecoder {

public:

    FFmpegDecoder();
    ~FFmpegDecoder();

    bool openFile(const std::string& path);
    bool decodeFrame();

   

private:

    AVFormatContext* formatContext;
    AVCodecContext* codecContext;

    AVFrame* frame;
    AVPacket* packet;

    int videoStreamIndex;
};
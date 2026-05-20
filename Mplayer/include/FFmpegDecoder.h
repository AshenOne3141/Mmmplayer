#pragma once



    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>


#include <string>

class FFmpegDecoder {

public:

    FFmpegDecoder();
    ~FFmpegDecoder();

    bool openFile(const std::string& path);

    bool decodeOneFrame();

private:

    AVFormatContext* formatContext;
    AVCodecContext* codecContext;

    AVFrame* frame;
    AVPacket* packet;

    int videoStreamIndex;
};
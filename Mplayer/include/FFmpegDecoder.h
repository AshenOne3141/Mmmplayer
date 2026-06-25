#pragma once

extern "C" { 
    #include <libavcodec/avcodec.h> 
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>
    #include <libswresample/swresample.h>
    #include <libavutil/opt.h>
    #include <libavutil/frame.h>
    #include <libavutil/channel_layout.h>
}
#include <string>
#include <iostream>
#include <vector>
#include "./PacketQueue.h"
#include <thread>
#include <atomic>
class FFmpegDecoder {

public:

    FFmpegDecoder();
    ~FFmpegDecoder();

    bool openFile(const std::string& path);
    bool readPacket();
    bool processAudio();
    bool processFrame();

    int getWidth();

    int getHeight();
    AVFrame* getRGBFrame();
    double getfps();
    void togglePause();
    void rewind();
    void forward();
    void startDecoder();
    void stopDecoder();
    void decodeLoop();
private:
    std::thread decodeThread;
    std::atomic<bool> running;
    AVFormatContext* formatContext;
    AVCodecContext* codecContext;
    AVCodecContext* acodecContext;
    SwsContext* swsContext = NULL;
    SwrContext* swrContext = NULL;
    AVFrame* frame;
    AVFrame* rgbframe;
    AVPacket* packet;
    AVFrame* audioframe;
    
    std::vector<uint8_t> audioBuffer;
    int audioSize;
    bool audioReady;
    
    std::vector<uint8_t> buffer;
    int videoStreamIndex;
    int audioStreamIndex;
    bool paused;
    PacketQueue videoQueue;
    PacketQueue audioQueue;
};
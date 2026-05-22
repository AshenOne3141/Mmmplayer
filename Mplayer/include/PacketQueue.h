#pragma once

#include <iostream>
extern "C" {

    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>
}
#include<queue>
#include<mutex>
#include <SDL3/SDL.h>

class PacketQueue {

private:

    std::queue<AVPacket> queue;

    SDL_Mutex* mutex;
    SDL_Condition* cond;

public:

    void push(AVPacket* pkt);

    bool pop(AVPacket* pkt);
};
#include <iostream>
extern "C" {

    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>
}
#include<queue>
#include<mutex>
#include "../include/PacketQueue.h"
#include <SDL3/SDL.h>


void PacketQueue::push(AVPacket* pkt) {
    
    SDL_LockMutex(mutex);
    AVPacket copy = { 0 };
    av_packet_ref(&copy, pkt);
    queue.push(copy);
    SDL_SignalCondition(cond);
    SDL_UnlockMutex(mutex);
    


}
bool PacketQueue::pop(AVPacket* pkt) {

    SDL_LockMutex(mutex);
    while (queue.empty()) {

        SDL_WaitCondition(cond, mutex);
    }


    *pkt = queue.front();

    queue.pop();
    SDL_UnlockMutex(mutex);

    return true;
}
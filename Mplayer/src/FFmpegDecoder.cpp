extern "C" {

    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>
}
#include<vector>
#include <string>
#include <iostream>

#include "../include/FFmpegDecoder.h"

FFmpegDecoder::FFmpegDecoder()
    : formatContext(nullptr),
    codecContext(nullptr),
    frame(av_frame_alloc()),
    packet(av_packet_alloc()),
    videoStreamIndex(-1),
    rgbframe(av_frame_alloc()),
    swsContext(nullptr)
{
}

FFmpegDecoder::~FFmpegDecoder() {

    if (frame)
        av_frame_free(&frame);

    if (packet)
        av_packet_free(&packet);

    if (codecContext)
        avcodec_free_context(&codecContext);

    if (formatContext)
        avformat_close_input(&formatContext);
    if (rgbframe)
        av_frame_free(&rgbframe);
    if (swsContext)
        sws_freeContext(swsContext);
}

bool FFmpegDecoder::openFile(const std::string& path) {

    if (avformat_open_input(&formatContext, path.c_str(), nullptr, nullptr) != 0) {

        std::cout << "Could not open file\n";

        return false;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {

        std::cout << "Could not find stream info\n";

        return false;
    }

    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {

        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {

            videoStreamIndex = i;

            break;
        }
    }

    if (videoStreamIndex == -1) {

        std::cout << "No video stream found\n";

        return false;
    }

    const AVCodec* codec =
        avcodec_find_decoder(formatContext->streams[videoStreamIndex]->codecpar->codec_id);

    if (codec == nullptr) {

        std::cout << "Unsupported Codec\n";

        return false;
    }

    codecContext = avcodec_alloc_context3(codec);

    if (!codecContext) {

        std::cout << "Failed to allocate codec context\n";

        return false;
    }

    if (avcodec_parameters_to_context(codecContext,formatContext->streams[videoStreamIndex]->codecpar) < 0) {

        std::cout << "Copying to Context Failed\n";

        return false;
    }

    if (avcodec_open2(codecContext, codec, nullptr) < 0) {

        std::cout << "Failed to open codec\n";

        return false;
    }

    std::cout << "Decoder initialized\n";

    return true;
}
bool FFmpegDecoder::decodeFrame() {
    std::cout << "decodeFrame called\n";
    if (frame == NULL) {
        std::cout << "No Frame Allocated\n";
        return 0;
    }
    if (codecContext == NULL) {
        std::cout << "No Codex Context\n";
        return 0;
    }

    
    int ByteSize;
    ByteSize = av_image_get_buffer_size(AV_PIX_FMT_RGB24,  codecContext->width, codecContext->height, 1);
    buffer.resize(ByteSize);
    av_image_fill_arrays(rgbframe->data, rgbframe->linesize, buffer.data(), AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 1);
    swsContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt, codecContext->width, codecContext->height, AV_PIX_FMT_RGB24,SWS_BILINEAR,NULL,NULL,NULL);
   
    while (av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == videoStreamIndex) {
            if (avcodec_send_packet(codecContext, packet) < 0)return 0;
            while (!avcodec_receive_frame(codecContext, frame)) { 
                std::cout << "Frame Decoded\n";
                sws_scale(swsContext, frame->data, frame->linesize, 0, codecContext->height, rgbframe->data, rgbframe->linesize);



               
            }

        }
        av_packet_unref(packet);

        return true;
    }



}
int FFmpegDecoder::getWidth() {

    return codecContext->width;
}

int FFmpegDecoder::getHeight() {

    return codecContext->height;
}
AVFrame* FFmpegDecoder::getRGBFrame() {

    return rgbframe;
}
double FFmpegDecoder::getfps() {
    return av_q2d(formatContext
        ->streams[videoStreamIndex]
        ->avg_frame_rate);
}
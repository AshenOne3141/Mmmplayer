extern "C" {

    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>

}

#include <string>
#include <iostream>

#include "../include/FFmpegDecoder.h"

FFmpegDecoder::FFmpegDecoder()
    : formatContext(nullptr),
    codecContext(nullptr),
    frame(av_frame_alloc()),
    packet(av_packet_alloc()),
    videoStreamIndex(-1)
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

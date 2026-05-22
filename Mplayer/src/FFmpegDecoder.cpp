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
    acodecContext(nullptr),
    frame(av_frame_alloc()),
    packet(av_packet_alloc()),
    videoStreamIndex(-1),
    audioStreamIndex(-1),
    rgbframe(av_frame_alloc()),
    swsContext(nullptr),
    paused(false)
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

        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIndex<0) {

            videoStreamIndex = i;

            
        }

        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO &&audioStreamIndex<0) {

            audioStreamIndex = i;

        }
    }

    
    if (videoStreamIndex == -1) {

        std::cout << "No video stream found\n";

        return false;
    }

    if (audioStreamIndex == -1) {
        std::cout << "No audio stream found\n";

        return false;
    }

    const AVCodec* codec =avcodec_find_decoder(formatContext->streams[videoStreamIndex]->codecpar->codec_id);

    const AVCodec* audioCodec= avcodec_find_decoder(formatContext->streams[audioStreamIndex]->codecpar->codec_id);

    if (codec == nullptr) {

        std::cout << "Unsupported Codec\n";

        return false;
    }

    if (audioCodec == nullptr) {

        std::cout << "Unsupported Codec\n";

        return false;
    }

    codecContext = avcodec_alloc_context3(codec);
    acodecContext = avcodec_alloc_context3(audioCodec);

    if (!codecContext) {

        std::cout << "Failed to allocate video codec context\n";

        return false;
    }
    if (!acodecContext) {

        std::cout << "Failed to allocate audio codec context\n";

        return false;
    }

    if (avcodec_parameters_to_context(codecContext,formatContext->streams[videoStreamIndex]->codecpar) < 0) {

        std::cout << "Copying to Video Context Failed\n";

        return false;
    }
    if (avcodec_parameters_to_context(acodecContext, formatContext->streams[audioStreamIndex]->codecpar) < 0) {

        std::cout << "Copying to Audio Context Failed\n";

        return false;
    }

    if (avcodec_open2(codecContext, codec, nullptr) < 0) {

        std::cout << "Failed to open video codec\n";

        return false;
    }
    if (avcodec_open2(acodecContext, audioCodec, nullptr) < 0) {

        std::cout << "Failed to open audio codec\n";

        return false;
    }


    std::cout << "Decoder initialized\n";
       int ByteSize;
    ByteSize = av_image_get_buffer_size(AV_PIX_FMT_RGB24,  codecContext->width, codecContext->height, 1);
    buffer.resize(ByteSize);
    av_image_fill_arrays(rgbframe->data, rgbframe->linesize, buffer.data(), AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 1);
    swsContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt, codecContext->width, codecContext->height, AV_PIX_FMT_RGB24,SWS_BILINEAR,NULL,NULL,NULL);

    return true;
}







bool FFmpegDecoder::decode() {
    std::cout << "decodeFrame called\n";
    if (frame == NULL) {
        std::cout << "No Frame Allocated\n";
        return 0;
    }
    if (codecContext == NULL) {
        std::cout << "No Codex Context\n";
        return 0;
    }

    
 

    
    int ret = avcodec_receive_frame(codecContext, frame);
   /* if (ret == 0) {
        sws_scale(swsContext, frame->data, frame->linesize, 0,
            codecContext->height, rgbframe->data, rgbframe->linesize);
        return true;
    }*/
    if (paused)return true;
    while (av_read_frame(formatContext, packet) >= 0) {

        if (packet->stream_index == videoStreamIndex) {
            return processFrame();
            

        }
        /*else if (packet->stream_index == audioStreamIndex) {
            return processAudio();

        }*/
        else {
            av_packet_unref(packet);

        }

        
    }

    return false;

}

bool FFmpegDecoder::processFrame() {
    if (avcodec_send_packet(codecContext, packet) < 0) {
        av_packet_unref(packet); return 0;
    }
    av_packet_unref(packet);

    while (!avcodec_receive_frame(codecContext, frame)) {


        sws_scale(swsContext, frame->data, frame->linesize, 0, codecContext->height, rgbframe->data, rgbframe->linesize);


        std::cout << "Frame Decoded\n";
        return true;


    }
    return false;
}

bool FFmpegDecoder::processAudio() {
    

   
        
        if (avcodec_send_packet(acodecContext, packet) < 0) {
                av_packet_unref(packet); return 0;
        }

        
        else {
           
            av_packet_unref(packet);
            return true;

        }


    

    return false;

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
void FFmpegDecoder::togglePause() {
    paused = !paused;
}

void FFmpegDecoder::rewind() {
    int64_t offset = av_rescale_q(10, AVRational { 1, 1 }, formatContext->streams[videoStreamIndex]->time_base);
    
    av_seek_frame(formatContext, videoStreamIndex, frame->pts-offset, AVSEEK_FLAG_BACKWARD);

    avcodec_flush_buffers(codecContext);
}
void FFmpegDecoder::forward() {
    int64_t offset = av_rescale_q(10, AVRational{ 1, 1 }, formatContext->streams[videoStreamIndex]->time_base);

    av_seek_frame(formatContext, videoStreamIndex, frame->pts + offset, 0);

    avcodec_flush_buffers(codecContext);
}

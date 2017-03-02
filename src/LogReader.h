/*
 * This file is modified from
 *  ElasticFusion/GUI/src/Tools/LogReader.h
 *  Logger2/src/Logger2.h
 *
 * Format:
 * int32_t at file beginning for frame count
 *
 * For each frame:
 * int64_t: timestamp
 * int32_t: depthSize
 * int32_t: imageSize
 * depthSize * unsigned char: depth_compress_buf
 * imageSize * unsigned char: encodedImage->data.ptr
 *
 */

#ifndef LOGREADER_H
#define LOGREADER_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <inttypes.h>

#include <opencv2/opencv.hpp>

#include "zlib.h"
#include "JPEGLoader.h"

class LogReader
{
    public:
        LogReader(std::string filename, int width, int height);
        virtual ~LogReader();

        void processNextFrame();
        void writePng(std::string rgbDir, std::string depthDir);

        int64_t timestamp;
        unsigned short * depth;
        unsigned char * rgb;
        int numProcessedFrames;

        Bytef * depthDecompressionBuffer;
        Bytef * rgbDecompressionBuffer;
        unsigned char * depthReadBuffer;
        unsigned char * rgbReadBuffer;
        int32_t rgbSize;
        int32_t depthSize;
        std::string filename;
        FILE * logFile;
        int32_t numFrames;

    private:
        int width;
        int height;

        JPEGLoader jpeg;

};

#endif // LOGREADER_H

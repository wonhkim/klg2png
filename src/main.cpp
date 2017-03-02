#include <cassert>
#include <iomanip>
#include <time.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <sys/stat.h>

#include <opencv2/opencv.hpp>

#include "LogReader.h"

int main(int argc, char ** argv)
{
    
    // Process arguments
    if (argc != 6)
    {
        std::cerr 
            << std::endl
            << "Usage: ./klg2png <input_klg_file> <output_rgb_dir> <output_depth_dir> <width> <height>"
            << std::endl;

        return EXIT_FAILURE;
    }
    
    std::string klgFilename = std::string(argv[1]);
    std::string rgbDir = std::string(argv[2]);
    std::string depthDir = std::string(argv[3]);
    struct stat st;
    if (stat(rgbDir.c_str(), &st) != 0) {
        std::cerr << rgbDir << " does not exist!\n";
        return EXIT_FAILURE;
    }
    if (stat(depthDir.c_str(), &st) != 0) {
        std::cerr << depthDir << " deos not exist!\n";
        return EXIT_FAILURE;
    }
    int w = std::atoi(argv[4]);
    int h = std::atoi(argv[5]);
    if (w <= 0 || h <= 0) {
        std::cerr << "Invalid image size.\n";
        return EXIT_FAILURE;
    }
        
    
    std::cout << "-------" << std::endl;
    std::cout << "Start reading log file ..." << std::endl;
    

    // Read klg log file
    LogReader * logreader = new LogReader(klgFilename, w, h);
    while (logreader->numProcessedFrames < logreader->numFrames)
    {
        logreader->processNextFrame();
        logreader->writePng(rgbDir, depthDir);
    }

    std::cout << "-------" << std::endl << std::endl;
    
    delete logreader;

    return 0;
}




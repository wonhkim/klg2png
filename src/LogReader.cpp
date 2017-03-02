#include "LogReader.h"

LogReader::LogReader(std::string filename, int width, int height)
    : timestamp(0),
      depth(0),
      rgb(0),
      numProcessedFrames(0),
      filename(filename),
      width(width),
      height(height)
{
    assert(logFile = fopen(filename.c_str(), "rb"));
    assert(fread(&numFrames, sizeof(int32_t), 1, logFile));
    std::cout << filename << " opened." << std::endl;
    std::cout << "numFrames: " << numFrames << std::endl;

    depthReadBuffer = new unsigned char[width * height * 2];
    rgbReadBuffer   = new unsigned char[width * height * 3];
    depthDecompressionBuffer = new Bytef[width * height * 2];
    rgbDecompressionBuffer   = new Bytef[width * height * 3];
}

LogReader::~LogReader()
{
    delete [] depthReadBuffer;
    delete [] rgbReadBuffer;
    delete [] depthDecompressionBuffer;
    delete [] rgbDecompressionBuffer;
    
    fclose(logFile);
}

void LogReader::processNextFrame()
{
    assert(fread(&timestamp, sizeof(int64_t), 1, logFile));
    assert(fread(&depthSize, sizeof(int32_t), 1, logFile));
    assert(fread(&rgbSize, sizeof(int32_t), 1, logFile));
    assert(fread(depthReadBuffer, depthSize, 1, logFile));
    if (rgbSize > 0) {
        assert(fread(rgbReadBuffer, rgbSize, 1, logFile));
    }
    if (depthSize == width * height * 2) {
        memcpy(&depthDecompressionBuffer[0], depthReadBuffer, width * height * 2);
    }
    else {
        unsigned long decompLength = width * height * 2;
        uncompress(
            &depthDecompressionBuffer[0],
            (unsigned long *)&decompLength,
            (const Bytef *)depthReadBuffer,
            depthSize);
    }

    if (rgbSize == width * height * 3) {
        memcpy(&rgbDecompressionBuffer[0], rgbReadBuffer, width * height * 3);
    }
    else if (rgbSize > 0) {
        jpeg.readData(rgbReadBuffer, rgbSize, (unsigned char *)&rgbDecompressionBuffer[0]);
    }
    else {
        memset(&rgbDecompressionBuffer[0], 0, width * height * 3);
    }
    depth = (unsigned short *)depthDecompressionBuffer;
    rgb = (unsigned char *)&rgbDecompressionBuffer[0];

    numProcessedFrames++;
}

void LogReader::writePng(std::string rgbDir, std::string depthDir)
{
    std::string id_str;
    if (timestamp > 1000000) // TUM format
    {
        std::ostringstream out;
        out << std::setprecision(6) << (double)timestamp / 1000000.0;
        id_str = out.str();
    }
    else
    {
        id_str = std::to_string(timestamp);
    }
    
    std::string depthFilename = depthDir;
    std::string rgbFilename = rgbDir;
    if (depthFilename.find_last_of("/") != depthFilename.length()-1) { depthFilename += "/"; }
    if (rgbFilename.find_last_of("/")   != rgbFilename.length()-1)   { rgbFilename += "/"; }
    depthFilename += id_str + ".png";
    rgbFilename += id_str + ".png";
    
    cv::Mat matDepth(height, width, CV_16UC1, depth);
    cv::Mat matRGB = cv::Mat(height, width, CV_8UC3, rgb);
    cv::cvtColor(matRGB, matRGB, CV_BGR2RGB);
    
    cv::imwrite(depthFilename, matDepth);
    cv::imwrite(rgbFilename, matRGB);
    std::cout
        << "[" << timestamp << "] "
        << depthFilename << ", " << rgbFilename << " written.\n";
}

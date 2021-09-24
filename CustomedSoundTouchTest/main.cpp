//
//  main.cpp
//  CustomedSoundTouchTest
//
//  Created by lilyhpeng on 2021/9/24.
//

#include <iostream>
#include "SoundTouch.h"

using namespace txrtmp_soundtouch;

void init_soundtouch(SoundTouch *pSoundTouch, int sampleRate, int channels) {
    pSoundTouch->setSampleRate(sampleRate);
    pSoundTouch->setChannels(channels);

    // 下面参数需要实时地变化，所以不在初始化的时候设值
    //pSoundTouch->setTempoChange(tempoDelta);
    //pSoundTouch->setPitchSemiTones(pitchDelta);
    //pSoundTouch->setRateChange(rateDelta);

    //pSoundTouch->setSetting(SETTING_USE_QUICKSEEK, quick);
    //pSoundTouch->setSetting(SETTING_USE_AA_FILTER, !(noAntiAlias));
}

int main(int argc, const char * argv[]) {
    // 配置的参数：
    std::string inputFileName = "./audio/Chf1.wav";
    std::string outputFileName = "./audio/Chf1_out.wav";
    int sampleRate = 16000;
    int channels = 2;
    float tempo = -50;
    
    SoundTouch soundtouch;
    init_soundtouch(&soundtouch, sampleRate, channels);
    // 每次送入SoundTouch的采样点数为960，每次从SoundTouch读出的采样点数为480
    int sampleNumIn = 960;
    int sampleNumOut = 480;
    
    // 打开文件：
    FILE* fpInputfile = NULL;
    FILE* fpoutputfile = NULL;
    fpInputfile = fopen(inputFileName.c_str(),"rb");
    if (fpInputfile == NULL)
    {
        printf("open input file failed\n");
        exit(-1);
    }
    fpoutputfile = fopen(outputFileName.c_str(),"wb");
    if (fpoutputfile == NULL)
    {
        printf("open output file failed\n");
        exit(-1);
    }
    short *pInputBuf;  // 存输入soundtouch中的音频数据
    short pOutPutTmp[1024] = {0};  // 存从soundtouch中取出的音频数据
    pInputBuf = new short[10*sampleNumIn];  // 开一个缓存空间，大小是10倍的采样点个数
    memset(pInputBuf,0,sizeof(short)*10*sampleNumIn);  //内存初始化函数，将某一块内存的值都初始化为相同的值
    
    int frameIdx = 0;  // 读取一个历史帧
    fseek(fpInputfile, frameIdx * sampleNumIn * sizeof(short), 0);  // 将文件读写指针移动到当前帧的首地址来
    int nSamples; // 每次从soundtouch中取出的样点数
    // Process samples read from the input file
    while(sampleNumIn == fread(pInputBuf,sizeof(short),sampleNumIn,fpInputfile))
    {

        // Read a chunk of samples from the input file
        // ......

        // Feed the samples into SoundTouch processor
        soundtouch.putSamples(pInputBuf, sampleNumIn);

        // Read ready samples from SoundTouch processor & write them output file.
        // NOTES:
        // - 'receiveSamples' doesn't necessarily return any samples at all
        //   during some rounds!
        // - On the other hand, during some round 'receiveSamples' may have more
        //   ready samples than would fit into 'sampleBuffer', and for this reason
        //   the 'receiveSamples' call is iterated for as many times as it
        //   outputs samples.
        nSamples = sampleNumIn;
        do
        {
            nSamples = soundtouch.receiveSamples(pOutputBuf, sampleNumOut);
            fwrite(pOutputBuf,sizeof(short),nSamples, fpoutputfile);
            
        } while (nSamples != 0);
    }

    // Now the input file is processed, yet 'flush' few last samples that are
    // hiding in the SoundTouch's internal processing pipeline.
    soundtouch.flush();
    do
    {
        nSamples = soundtouch.receiveSamples(pOutputBuf, sampleNumOut);
        fwrite(pOutputBuf,sizeof(short),nSamples, fpoutputfile);
    } while (nSamples != 0);
    
    
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}

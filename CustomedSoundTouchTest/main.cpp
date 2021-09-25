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
}

float get_speed_ratio() {
    float speed_ratio;
    // 整个文件采用固定的speed_ratio
    speed_ratio = 1.5;
    // 音频文件的每帧都调整一下speed_ratio，测试是否会出现乱序的问题
    float speed_ratio_list[7] = {0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6};
    int idx = rand() % 7;
    speed_ratio = speed_ratio_list[idx];
    return speed_ratio;
}

int main(int argc, const char * argv[]) {
    // 配置的参数：
    std::string inputFileName = "/Users/lilyhpeng/Documents/SoundTouchTest/audio/Chf1.pcm";
    std::string outputFileName = "/Users/lilyhpeng/Documents/SoundTouchTest/audio/Chf1_out2.pcm";
    int sampleRate = 16000;
    int channels = 1;
    
    // ｜sampleNumIn｜每次送入SoundTouch的采样点数为960，
    // ｜sampleNumOut｜每次从SoundTouch读出的采样点数为480
    // ｜pInputBuf｜存储每次从文件中读取的音频样点的缓存区
    int sampleNumIn = 960;
    int sampleNumOut = 480;
    FILE* fpInputfile = NULL;
    FILE* fpoutputfile = NULL;
    short *pInputBuf = new short[2*sampleNumIn];
    SoundTouch soundtouch;
    
    // step 01: 打开文件
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
    
    // step 02：初始化soundtouch的相关参数
    init_soundtouch(&soundtouch, sampleRate, channels);
    
    // while循环中每次需要从soundtouch中取出sampleNumOut个样点，
    bool file_end = false;
    int read_frame_cnt = 0;
    float speed_ratio;
    while (!file_end) {
        int len = sampleNumOut;
        short *pOutputBuf2 = new short[2*sampleNumOut];  // 存从soundtouch中一次取出的音频数据
        short *pCurPosition = pOutputBuf2;
        while (len > 0) {
            // 设置当前的播放速度：
            speed_ratio = get_speed_ratio();
            soundtouch.setTempoChange((speed_ratio - 1.0f) * 100.0f);
            
            // 从soundtouch的缓存中读取数据，直到取空
            int read_samples = 0, ret_samples = 0;
            do {
                ret_samples = soundtouch.receiveSamples(pCurPosition + read_samples, len - read_samples);
                read_samples += ret_samples;
            } while (ret_samples);
            // 挪动缓存区指针到现有数据的末尾，待读取数据的长度减去这次从soundtouch中读出的数据长度
            pCurPosition += read_samples;
            len -= read_samples;
            
            if (file_end) {
                break;
            }
            
            // 如果soundtouch中的数据不够，需要从文件中读取960个样点数据存入soundtouch中
            if (sampleNumIn != fread(pInputBuf,sizeof(short),sampleNumIn,fpInputfile)) {
                file_end = true;
            }
            // 注意：无论从文件中读入的数据长度是否为sampleNumIn，都需要将这些数据送入soundtouch中去处理
            soundtouch.putSamples(pInputBuf, sampleNumIn);
            
        }
        
        // 注意：这里将变速处理后的数据写入文件时，写入长度是sampleNumOut-len而不是sampleNumOut，是因为可能读取的数据
        // 是大于sampleNumOut的（此时len为负数）
        fwrite(pOutputBuf2, sizeof(short), sampleNumOut-len, fpoutputfile);
        read_frame_cnt ++;
        std::cout << "read_frame_idx:" << read_frame_cnt << ", speed_ratio:" << speed_ratio << std::endl;
        delete [] pOutputBuf2;
    }
    

    // 到这里输入文件中的数据已全部送入soundtouch中，但是soundtouch中可能还有一些数据存在其内部的各种缓存区中（等不到足够数据去进行变速处理了）
    // 通过flush操作将这些soundtouch内部缓存的数据提取出来，再通过receiveSamples读出来
    soundtouch.flush();
    short *pOutputBuf = new short[2*sampleNumOut];  // 存从soundtouch中一次取出的音频数据
    int nSamples;
    do
    {
        nSamples = soundtouch.receiveSamples(pOutputBuf, sampleNumOut);
        fwrite(pOutputBuf, sizeof(short), nSamples, fpoutputfile);
    } while (nSamples != 0);
    
    // 释放动态数组
    delete [] pInputBuf;
    delete [] pOutputBuf;
    
    return 0;
}

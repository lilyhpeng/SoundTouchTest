## 一. 简介

SoundTouch是一个对音频进行加速、变调等操作的开源库，这个项目使用这个开源库对音频进行变速：（1）对整个音频文件进行变速（不变调）（2）对音频文件分帧，每帧采用不同的加速系数对音频文件进行处理，模拟实时音视频流中的变速操作。

## 二. 使用

使用Xcode打开项目，项目中包含两个target，其中：

* SoundTouchTest是使用源码中的SoundStretch实现对整个音频文件进行变速操作，需配置命令行参数，参数说明参照https://gitlab.com/soundtouch/soundtouch/-/tree/master；
* CustomedSoundTouchTest实现流式地动态调整音频播放速度，调试参数直接在代码中修改。

项目中的audio文件夹用于存放各种音频文件，SoundTouch文件夹下则为源码，audio和SoundTouch文件夹为两个target共有。
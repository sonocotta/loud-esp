## Introduction

This demo shows how to use the touch screen together with audio output of Loud-ESP. Most obvious combination of two is the sound board. Due to internal memory capacity limits it makes sense to also involve sdcard to keep audio files and images.

## SD-CARD preparation

Since my screen is 4:3 aspect ratio, it makes sense to split it into 12 equal parts. I will prepare 12 folders on the sdcard accordingly, each having one file for audio, one for 80x80 px image, and text file for caption. Since we have plenty of space on the card, I converted images into `bmp` files and audio into `wav` files. These are not size friendly, but implement no compression whatsoever, making ESP32's job easier. `ffmpeg` will make it easy for you

```
find ./ -name "*.og?" -exec ffmpeg -acodec libvorbis -i "{}" -acodec pcm_s16le {}.wav \;
find ./ -name "*.jpg" -exec ffmpeg -i {} -q 0 -vf scale="'if(gt(iw,ih),-1,80):if(gt(iw,ih),80,-1)', crop=80:80:exact=1" {}.bmp \;
```

## What to play

I end up with the bird songs. But it is rather a question of personal choice, since you just need to replace content on the sdcard. You can go with car engines, ambient sounds, animals or whatever else springs into your mind.

## Looks


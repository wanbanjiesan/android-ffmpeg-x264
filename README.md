# android_ffmpeg_x264
android mac环境下的ffmpeg编译

编译环境是：
* Mac 10.12.4 (16E195)
* Android Studio 2.3.1
* NDK android-ndk-r14b
* ffmpeg-3.3.2   
* x264 lastest   

下载地址：<br>
ffmpeg：git clone https://git.ffmpeg.org/ffmpeg.git ffmpeg<br>
x264: git clone http://git.videolan.org/git/x264.git<br>
ndk: https://developer.android.com/ndk/downloads/index.html   需要注意系统，而且不需要配置环境变量<br>

在下载过程中可以去编译脚本<br>
新建脚本: build_with_x264.sh<br>
```Javascript
cd x264

export TEP=/Users/you/Desktop/ffmpeg
export PLATFORM_VERSION=android-9
export ANDROID_NDK=/Users/you/Desktop/android-ndk-r14b #ndk 目录根据你的安装目录
export TOOLCHAIN=$TEP/fftoolchain #toolchain 安装目录
export SYSROOT=$TOOLCHAIN/sysroot/
export PLATFORM=$ANDROID_NDK/platforms/$PLATFORM_VERSION/arch-arm
export PREFIX=$TEP/android-lib #编译结果的目录
export OUT_PREFIX=$TEP/../264fflib

#生成toolchain目录，这一段可以在第一次运行后注释掉
$ANDROID_NDK/build/tools/make-standalone-toolchain.sh \
    --toolchain=arm-linux-androideabi-4.9 \
    --platform=$PLATFORM_VERSION --install-dir=$TOOLCHAIN 

#
./configure \
    --prefix=$PREFIX \
    --enable-static \
    --enable-shared \
    --enable-pic \
    --disable-asm \
    --disable-cli \
    --host=arm-linux \
    --cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
    --sysroot=$PLATFORM

make -j8
make install

cd ..

# 加入x264编译库
EXTRA_CFLAGS="-I./android-lib/include" 
EXTRA_LDFLAGS="-L./android-lib/lib"


./configure \
    --target-os=linux \
    --prefix=$OUT_PREFIX \
    --enable-cross-compile \
    --enable-runtime-cpudetect \
    --disable-asm \
    --disable-doc \
    --arch=arm \
    --cc=$TOOLCHAIN/bin/arm-linux-androideabi-gcc \
    --cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
    --disable-stripping \
    --nm=$TOOLCHAIN/bin/arm-linux-androideabi-nm \
    --sysroot=$PLATFORM \
    --enable-gpl \
    --enable-static \
    --disable-shared \
    --enable-version3 \
    --enable-small \
    --disable-vda \
    --disable-iconv \
    --disable-encoders \
    --enable-libx264 \
    --enable-encoder=libx264 \
    --disable-muxers \
    --enable-muxer=mov \
    --enable-muxer=ipod \
    --enable-muxer=psp \
    --enable-muxer=mp4 \
    --enable-muxer=avi \
    --disable-decoders \
    --enable-decoder=aac \
    --enable-decoder=aac_latm \
    --enable-decoder=h264 \
    --enable-decoder=mpeg4 \
    --disable-demuxers \
    --enable-demuxer=h264 \
    --enable-demuxer=mov \
    --disable-parsers \
    --enable-parser=aac \
    --enable-parser=ac3 \
    --enable-parser=h264 \
    --disable-protocols \
    --enable-protocol=file \
    --enable-protocol=rtmp \
    --disable-bsfs \
    --enable-bsf=aac_adtstoasc \
    --enable-bsf=h264_mp4toannexb \
    --disable-indevs \
    --enable-zlib \
    --disable-outdevs \
    --disable-ffprobe \
    --disable-ffplay \
    --disable-ffmpeg \
    --disable-ffserver \
    --disable-debug \
    --extra-cflags=$EXTRA_CFLAGS \
    --extra-ldflags=$EXTRA_LDFLAGS


make clean 
make -j8
make install

# 这段解释见后文
$TOOLCHAIN/bin/arm-linux-androideabi-ld -rpath-link=$PLATFORM/usr/lib -L$PLATFORM/usr/lib -L$OUT_PREFIX/lib -soname libffmpeg.so -shared -nostdlib -Bsymbolic --whole-archive --no-undefined -o $OUT_PREFIX/libffmpeg.so \
    android-lib/lib/libx264.a \
    libavcodec/libavcodec.a \
    libavfilter/libavfilter.a \
    libswresample/libswresample.a \
    libavformat/libavformat.a \
    libavutil/libavutil.a \
    libswscale/libswscale.a \
    libpostproc/libpostproc.a \
    libavdevice/libavdevice.a \
    -lc -lm -lz -ldl -llog --dynamic-linker=/system/bin/linker $TOOLCHAIN/lib/gcc/arm-linux-androideabi/4.9.x/libgcc.a   
```

等待前面的2个源码下载完成后，修改下文件结构<br>
ffmpeg<br>
--x264<br>
--build_with_x264.sh<br>

至此，准备工作完成，接下来是重点，开始编译...<br>
cd ffmpeg<br>
chmod +x build_with_x264.sh<br>
sh ./build_with_x264.sh<br>

......<br>

中间可能出现多次warning，无视就好。。。<br>
最终会出现一下代码结构：<br>

说明编译ok











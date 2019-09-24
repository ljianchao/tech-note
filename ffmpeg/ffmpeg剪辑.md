# 转码

常用转换命令如下：

```shell
    ffmpeg -i out.ogv -vcodec h264 out.mp4 
    ffmpeg -i out.ogv -vcodec mpeg4 out.mp4 
    ffmpeg -i out.ogv -vcodec libxvid out.mp4 
    ffmpeg -i out.mp4 -vcodec wmv1 out.wmv 
    ffmpeg -i out.mp4 -vcodec wmv2 out.wmv 
```

`-i`后面是输入文件名。`-vcodec`后面是编码格式，`h264`最佳，但Windows系统默认不安装。如果是要插入`ppt`的视频，选择`wmv1`或`wmv2`基本上万无一失。

附加选项：`-r`指定帧率，`-s`指定分辨率，`-b`指定比特率；于此同时可以对声道进行转码，`-acodec`指定音频编码，`-ab`指定音频比特率，`-ac`指定声道数，例如
> ffmpeg -i out.ogv -s 640x480 -b 500k -vcodec h264 -r 29.97 -acodec libfaac -ab 48k -ac 2 out.mp4

# 剪切

用`-ss`和`-t`选项， 从第30秒开始，向后截取10秒的视频，并保存：
```shell
    ffmpeg -i input.wmv -ss 00:00:30.0 -c copy -t 00:00:10.0 output.wmv 
    ffmpeg -i input.wmv -ss 30 -c copy -t 10 output.wmv 
``` 

达成相同效果，也可以用`-ss`和`-to`选项， 从第30秒截取到第40秒：
```shell
ffmpeg -i input.wmv -ss 30 -c copy -to 40 output.wmv 
```

`-c copy`可以保持视频和音频和源完全一致的配置。

只设置开始时间，则截取到最后
```shell
    ffmpeg -i input.mp4 -ss 00:00:30.0 -c:v copy -c:a copy output.mp4
```

值得注意的是`ffmpeg`为了加速，会使用关键帧技术，所以有时剪切出来的结果在起止时间上未必准确。通常来说，把`-ss`选项放在`-i`之前，会使用关键帧技术；把`-ss`选项放在`-i`之后，则不使用关键帧技术。 如果要使用关键帧技术又要保留时间戳，可以加上`-copyts`选项：
```shellffmpeg -ss 00:01:00 -i video.mp4 -to 00:02:00 -c copy -copyts cut.mp4 
```

# 合并

`FFmpeg concat`分离器方法成功率很高，也是最好的，但是需要`FFmpeg 1.1`以上版本。先创建一个文本文件`filelist.txt`：

```
file 'input1.mkv'
file 'input2.mkv'
file 'input3.mkv'
```

然后：
```shell
    ffmpeg -f concat -i filelist.txt -c copy output.mkv
```

注意：
> 使用`FFmpeg concat`分离器时，如果文件名有奇怪的字符，要在`filelist.txt`中转义。

# 参考

- [ffmpeg Documenation](http://ffmpeg.org/ffmpeg.html)
- [使用FFmpeg将字幕文件集成到视频文件](https://www.yaosansi.com/post/ffmpeg-burn-subtitles-into-video/)
- [使用ffmpeg为视频嵌入字幕](https://www.jianshu.com/p/ba8670f09df0)
- [ffmpeg命令行map参数的使用](https://blog.csdn.net/xiaoluer/article/details/81136478)
- [ffmpeg基础使用](https://www.jianshu.com/p/ddafe46827b7)
- [FFmpeg：视频转码、剪切、合并、播放速调整](https://fzheng.me/2016/01/08/ffmpeg/)
- [ffmpeg filter过滤器 基础实例及全面解析](https://blog.csdn.net/newchenxf/article/details/51364105)
- [FFMpeg无损合并视频的多种方法](https://www.jianshu.com/p/a9bccc12229b)
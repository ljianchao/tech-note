# 字幕文件转换

字幕文件有很多种，常见的有 `.srt` , `.ass` 文件等,下面使用FFmpeg进行相互转换。

将`.srt`文件转换成`.ass`文件
```shell
    ffmpeg -i subtitle.srt subtitle.ass
```

将`.ass`文件转换成`.srt`文件
```shell
    ffmpeg -i subtitle.ass subtitle.srt
```

# 集成字幕，选择播放

这种字幕集成比较简单，播放时需要在播放器中选择相应的字幕文件。适用于`mp4`格式的视频外挂字幕合并时操作。
```shell
    ffmpeg -i input.mp4 -i subtitles.srt -c:s mov_text -c:v copy -c:a copy output.mp4
```

# 嵌入SRT字幕到视频文件

## 单独SRT字幕

字幕文件为`subtitle.srt`
```shell
    ffmpeg -i video.avi -vf subtitles=subtitle.srt out.avi
```

无损嵌入字幕，重点是`copy`选项
```shell
    ffmpeg -i movie.mkv -i sub.srt -c:s copy -c:v copy -c:a copy output.mkv
```

有时源视频中可能存在别的字幕，可以使用自己的字幕文件进行替换
```shell
    ffmpeg -i movie.mkv -i sub.srt -map 0:v -map 0:a -map 1:s -c copy output.mkv 
```

其中`movie.mkv`的编号为`#0`，`sub.srt`的是`#1`，`v`代表视频流，`a`代表音频流，`s`则代表字幕流，程序会从`#0`中抽取视频流和音频流、从`#1`中抽取字幕流，复制到`output.mkv`中。
另外，最后的`copy`参数一定要加上，否则`ffmpeg`会对音视频流进行重新编码，将极大增加处理时间。

## 嵌入在MKV等容器的字幕

将`video.mkv`中的字幕（默认）嵌入到`out.avi`文件
```shell
    ffmpeg -i video.mkv -vf subtitles=video.mkv out.avi
```

将`video.mkv`中的字幕（第二个）嵌入到`out.avi`文件
```shell
    ffmpeg -i video.mkv -vf subtitles=video.mkv:si=1 out.avi
```

## 嵌入ASS字幕到视频文件
```shell
    ffmpeg -i video.avi -vf "ass=subtitle.ass" out.avi
```

## 字幕文件修复
```shell
    ffmpeg -fix_sub_duration -i sub.srt sub2.srt
```

# 参考

- [ffmpeg Documenation](http://ffmpeg.org/ffmpeg.html)
- [使用FFmpeg将字幕文件集成到视频文件](https://www.yaosansi.com/post/ffmpeg-burn-subtitles-into-video/)
- [使用ffmpeg为视频嵌入字幕](https://www.jianshu.com/p/ba8670f09df0)
- [ffmpeg命令行map参数的使用](https://blog.csdn.net/xiaoluer/article/details/81136478)
- [ffmpeg基础使用](https://www.jianshu.com/p/ddafe46827b7)
- [FFmpeg：视频转码、剪切、合并、播放速调整](https://fzheng.me/2016/01/08/ffmpeg/)
- [ffmpeg filter过滤器 基础实例及全面解析](https://blog.csdn.net/newchenxf/article/details/51364105)
- [FFMpeg无损合并视频的多种方法](https://www.jianshu.com/p/a9bccc12229b)




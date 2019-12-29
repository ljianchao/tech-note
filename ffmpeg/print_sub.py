import videocr

if __name__ == '__main__':
    videocr.save_subtitles_to_file('E:\download\ChallengeGameC.mkv', file_path='E:\download\ChallengeGamemkv.srt', lang='chi_tra', 
                                time_start='0:00', time_end='',
                                conf_threshold=30, sim_threshold=90, use_fullframe=False)
import os
import random
import tempfile
import time
import traceback
import serial

from aiy.voice.audio import AudioFormat, play_wav, record_file

AIY_CARDS = {
    'sndrpigooglevoi': 'Voice HAT (v1)',
    'aiyvoicebonnet': 'Voice Bonnet (v2)'
}


def play_random_file_from_dir(dirpath):
    file_to_play = random.choice(os.listdir(dirpath))
    fpath=os.path.join(dirpath,file_to_play)
    print("attempting to play: {}".format(fpath))
    play_wav(fpath)


def listen_for_and_play_message():
    ser = None
    while not ser:
        try:    
            ser = serial.Serial('/dev/ttyACM0', 9600)
        except Exception as e:
            print("Serial at /dev/ttyACM0 still not available")
            time.sleep(5)
    while True:
        msg = ser.readline()
        #print("received msg: {}".format(msg))
        if  "periodic_sound" in str(msg):
            play_random_file_from_dir('/home/pi/voicefiles/periodic_callout')
        elif "closer_sound" in str(msg):
            play_random_file_from_dir('/home/pi/voicefiles/getting_closer')
        elif "close_sound" in str(msg):
            play_random_file_from_dir('/home/pi/voicefiles/close')
        elif "away_sound" in str(msg):
            play_random_file_from_dir('/home/pi/voicefiles/gone')
        msg = None

def main():
    listen_for_and_play_message()


if __name__ == '__main__':
    try:
        main()
    except:
        traceback.print_exc()


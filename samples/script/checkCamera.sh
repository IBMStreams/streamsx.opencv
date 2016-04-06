#!/bin/bash

die() { echo ; echo -e "\033[1;31m$*\033[0m" >&2 ; echo "//////////////////////////////////////////////////////////////////////" ; exit 1 ; }
step() { echo ; echo -e "\033[1m$*\033[0m" ; }

step "these USB devices are attached to this machine:"
lsusb | sort 

step "these cameras are attached to this machine:"
ls -1 /dev/video*

camera=/dev/video$1
[ ! -c $camera ] && echo "sorry, '$camera' is not connected right now" && exit -1

step "checking camera $camera ..."

#v4l2-ctl -d $camera --list-formats
#v4l2-ctl -d $camera --list-formats-ext

#v4l2-ctl -d $camera --set-fmt-video=width=320,height=240,pixelformat=MJPG
#v4l2-ctl -d $camera --set-fmt-video=width=352,height=288,pixelformat=MJPG
#v4l2-ctl -d $camera --set-fmt-video=width=640,height=480,pixelformat=MJPG
#v4l2-ctl -d $camera --set-fmt-video=width=960,height=720,pixelformat=MJPG

#v4l2-ctl -d $camera -p 5

#v4l2-ctl -d $camera --all

#ffplay -f v4l2 -i $camera -list_formats all

#ffplay -f v4l2 -i $camera
#ffplay -f v4l2 -i $camera -video_size 640x480 -framerate 30
#ffplay -f v4l2 -i $camera -input_format yuyv422 -video_size 640x480 -framerate 30 
ffplay -f v4l2 -i $camera -input_format mjpeg -video_size 640x480 -framerate 30 
#ffplay -f v4l2 -i $camera -input_format mjpeg -video_size 960x720 -framerate 30 

exit 0

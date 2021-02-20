# Script to generate an mp4 from png files of the form $1%d.png (as created by recording, for example)
# $1 is the common prefix of all png's and $2 is the output name
set -e
test "${PWD##*/}" == "out"
ffmpeg -r 60 -f image2 -s 1920x1080 -i $1%d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p $2

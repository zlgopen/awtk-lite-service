cd ffmpeg && ./configure && make && cd -

cd x264 && ./configure && make && cd - 

find . -name \*.a -exec cp -fv {} ../../lib \;


MP3 TAG READER & EDITOR

Compile:
gcc *.c

View:
./a.out -v sample.mp3

Edit examples:
./a.out -e -t "New Title" sample.mp3
./a.out -e -a "New Artist" sample.mp3
./a.out -e -A "New Album" sample.mp3
./a.out -e -y 2001 sample.mp3
./a.out -e -m Rock sample.mp3
./a.out -e -c "New Comment" sample.mp3

The included sample.mp3 is a small test MP3 with ID3v2.3 metadata.
It is included only so the project can be tested immediately.
Replace it with your own MP3 when you want to read/edit a real song.

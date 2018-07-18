# sys-play
- Background audio playing sysmodule for Nintendo Switch!

## How to use it
- Add the kip file to hekate INI (something like `kip1=sys-play.kip`)
- Create a text file on the root of your SD named `sys-play.txt`.
- Add there the names of the audio files to play, ordered as you want.

TXT file example: 
```
audio1.mp3
track.wav
music.ogg
```
- Then, place all those audio files under `sdmc:/media`, because sys-play will try to find them there.
- Enjoy your music on home menu, or anywhere!
- NXPlay support is planned, as a tool to pause/skip audios. Sorry, but controlling cannot be used with sysmodules.

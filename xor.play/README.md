# ModuleMania sysmodule - `xor:play`

## How do I use this sysmodule?

  - First of all, DON'T RUN THIS KIP ALONG WITH LAYEREDFS!
  - Save the kip and add it to your own CFW. I guess you'll already know how to add kips to the CFW you use.
  - Create a file on the root of the SD card named `xor.play.json`.
  Add there the Title IDs and the audio files to play when that title is opened.
  - Here's an example with BotW and Home Menu:
  ``` json
  {
      "0100000000001000" : "sdmc:/myaudios/play.wav",
      "01007EF00011E000" : "sdmc:/aud.mp3"
  }
  ```
  - This way, when there's nothing opened and you're in Home Menu `play.wav` file would be played, and while BotW is opened `aud.mp3` would be played (even if you're on Home Menu and the app is minimized)
  - The number of applets which support this is unknown, be free to test it. All games should work.
  - Any problems you have, ask them on my Discord server ([Nintendo H&H server](https://discord.gg/Qqnndqd)), or ask them on the release GBAtemp thread of ModuleMania.
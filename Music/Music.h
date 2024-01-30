/*
 *  Class Music
 *  use operating single music
 *  dependent audio library is BASS library
 * */

#ifndef MUSIC_H
#define MUSIC_H

#include "../bass/bass.h"
#include <string>

class Music {
public:
  // default volume is 100%
  Music(unsigned volume = 100);
  ~Music();

  // load music by musicname
  void load(const char *musicname);

  // play(play from the beginning) and pause and start(stop pause) music
  void play();
  void pause();
  void start();
  // music whether stopped, pause and mute and loop
  bool isStopped() const {
    return BASS_ChannelIsActive(_music) == BASS_ACTIVE_STOPPED;
  }
  bool isPaused() const noexcept { return _pause; }
  bool isMute() const noexcept { return _mute; }
  bool isLoop() const noexcept { return _loop; }

  // end current music
  void end();

  // increase and decrease and mute and set volume
  void increaseVolume(unsigned num = 1);
  void decreaseVolume(unsigned num = 1);
  void muteVolume();
  void setVolume(unsigned volume);

  // music progress forward and backword
  void progressForward(unsigned num = 1);
  void progressBackward(unsigned num = 1);

  // whether loop music
  void loop();

  // get information
  const char *getMusicName() const noexcept { return _musicName.c_str(); }
  double getTime() const noexcept { return _time; }
  double getCurrentTime() const {
    return BASS_ChannelBytes2Seconds(
        _music, BASS_ChannelGetPosition(_music, BASS_POS_BYTE));
  }
  int getVolume() const noexcept { return _volume; }

private:

  HSTREAM _music;
  std::string _musicName;
  int _volume;
  QWORD _length;
  double _time;
  bool _pause = true;
  bool _mute = false;
  bool _loop = false;
};

inline void Music::setVolume(unsigned volume) {
  BASS_ChannelSetAttribute(_music, BASS_ATTRIB_VOL, 0.01 * volume);
}

bool isSupportedMusicType(const std::string &musicName);

#endif

#include "Music.h"
#include "../bass/bass.h"
#include "../bass/bassflac.h"
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ncurses.h>

// Currently support music type
const char *supportType[] = {"MP3",  "mp3",  "MP2",  "mp2", "MP1",
                             "mp1",  "OGG",  "ogg",  "WAV", "wav",
                             "AIFF", "aiff", "FLAC", "flac"};

bool isSupportedMusicType(const std::string &musicName) {
  auto pos = musicName.find_last_of('.');
  if (pos == std::string::npos) {
    if (!isendwin())
      endwin();
    fprintf(stderr,
            "This is not a music name. The music name should have its type.\n");
    std::exit(EXIT_FAILURE);
  }
  const char *type = musicName.c_str() + pos + 1;
  if (std::find_if(std::begin(supportType), std::end(supportType),
                   [=](const char *str) { return !std::strcmp(type, str); }) !=
      std::end(supportType)) {
    return true;
  } else
    return false;
}

Music::Music(unsigned volume) : _volume(volume) {
  BASS_Init(-1, 44100, 0, 0, 0);
  BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, volume * 100);
}

Music::~Music() { BASS_Free(); }

void Music::load(const char *name) {
  _musicName = name;
  // check music type and execute correspond function
  const char *type = _musicName.c_str() + _musicName.find_last_of('.') + 1;
  if (isSupportedMusicType(_musicName)) {
    if (!std::strcmp(type, "FLAC") || !std::strcmp(type, "flac")) {
      _music = BASS_FLAC_StreamCreateFile(FALSE, _musicName.c_str(), 0, 0,
                                          BASS_STREAM_AUTOFREE);
    } else {
      _music = BASS_StreamCreateFile(FALSE, _musicName.c_str(), 0, 0,
                                     BASS_STREAM_AUTOFREE);
    }
  } else {
    if (!isendwin())
      endwin();
    fprintf(stderr, "This music type (%s) is unsupported!\n", type);
    fprintf(stderr, "Support Type as follow:\n");
    std::for_each(std::begin(supportType), std::end(supportType),
                  [](const char *type) { fprintf(stderr, "%s ", type); });
    fprintf(stderr, "\n");
    std::exit(EXIT_FAILURE);
  }

  _length = BASS_ChannelGetLength(_music, BASS_POS_BYTE);
  _time = BASS_ChannelBytes2Seconds(_music, _length);
  _loop = false;
  _mute = false;
}

void Music::play() {
  BASS_ChannelPlay(_music, TRUE);
  _pause = false;
}

void Music::pause() {
  BASS_ChannelPause(_music);
  _pause = true;
}

void Music::start() {
  BASS_ChannelStart(_music);
  _pause = false;
}

void Music::end() {
  BASS_StreamFree(_music);
  _musicName.clear();
  _length = 0;
  _time = 0.0;
}

void Music::increaseVolume(unsigned num) {
  if (_volume < 100) {
    _volume += num;
    if (_volume > 100)
      _volume = 100;
    if (!_mute)
      setVolume(_volume);
  }
}

void Music::decreaseVolume(unsigned num) {
  if (_volume > 0) {
    _volume -= num;
    if (_volume < 0)
      _volume = 0;
    if (!_mute)
      setVolume(_volume);
  }
}

void Music::muteVolume() {
  if (_mute) {
    setVolume(_volume);
    _mute = false;
  } else {
    setVolume(0);
    _mute = true;
  }
}

void Music::progressForward(unsigned num) {
  QWORD pos = BASS_ChannelGetPosition(_music, BASS_POS_BYTE);
  pos += BASS_ChannelSeconds2Bytes(_music, num);
  if (pos > _length)
    pos = _length - 1;
  BASS_ChannelSetPosition(_music, pos, BASS_POS_BYTE);
}

void Music::progressBackward(unsigned num) {
  QWORD pos = BASS_ChannelGetPosition(_music, BASS_POS_BYTE);
  QWORD size = BASS_ChannelSeconds2Bytes(_music, num);
  if (pos < size)
    pos = 0;
  else
    pos -= size;
  BASS_ChannelSetPosition(_music, pos, BASS_POS_BYTE);
}

void Music::loop() {
  if (_loop) {
    BASS_ChannelFlags(_music, 0, BASS_SAMPLE_LOOP);
    _loop = false;
  } else {
    BASS_ChannelFlags(_music, BASS_SAMPLE_LOOP, BASS_SAMPLE_LOOP);
    _loop = true;
  }
}

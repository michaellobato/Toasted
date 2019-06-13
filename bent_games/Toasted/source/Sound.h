
#ifndef SOUND_H
#define SOUND_H

/* include fmod */
#include "../fmod/fmod.h"
#include <stdlib.h>
#include <stdio.h>

/*! \brief Sound struct to be used by all loaded sounds. */
typedef struct {
	/* the fmod sound */
	FSOUND_SAMPLE *sound;
	/* the fmod channel in use by this sound */
	int currentChannel;
}SOUND;

/* fmod variables */
/*FMOD_SYSTEM *soundSystem;*/


/* global sounds to be used often */
SOUND soundMenu, soundGame, soundGameOver, soundBullet,
	soundMenuChange, soundMenuSelect, soundLaser, soundSniper;

/* error checking - unused */
/*FMOD_RESULT result;*/

int soundsEnabled;

void loadSoundSystem();
void updateSoundSystem();
void unloadSoundSystem();
void playSound(SOUND *currentSound);
void playSoundCopy(SOUND *currentSound, int volume);
void stopSound(SOUND *currentSound);
void pauseSound(SOUND *currentSound, int pause);

#endif

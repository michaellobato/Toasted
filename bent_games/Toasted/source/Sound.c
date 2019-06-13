/*! ************************************************************************
\file Sound.c
\author Sheridan Beebe
\par email: sbeebe@digipen.edu
\par Game: Toasted
\date Feburuary 6th, 2007
\brief  Contains all the code to make the sound work correctly
mainly to be used in the driver.c file
***************************************************************************/

/* include the sound header */
#include "Sound.h"

/* must be called everytime the game starts to load the sound system */
void loadSoundSystem()
{
	/* create the fmod sound system */
	/*FMOD_System_Create(&soundSystem); */


	/* initialize the fmod sound system */
	/*FMOD_System_Init(soundSystem, 32, FMOD_INIT_NORMAL, NULL);*/
	FSOUND_Init(44100, 32, 0);

	/* load up some sounds  to be used often - make sure to unload*/
	
	soundMenu.sound = FSOUND_Sample_Load(FSOUND_FREE, "sounds/music/GameMenu.wav", FSOUND_NORMAL | FSOUND_2D, 0, 0);
	FSOUND_Sample_SetMode(soundMenu.sound, FSOUND_LOOP_NORMAL);
	soundMenu.currentChannel = -1;

	soundGame.sound = FSOUND_Sample_Load(FSOUND_FREE, "sounds/music/GameLoop.wav", FSOUND_NORMAL | FSOUND_2D, 0, 0);
	FSOUND_Sample_SetMode(soundGame.sound, FSOUND_LOOP_NORMAL);
	soundGame.currentChannel = -1;

	soundGameOver.sound = FSOUND_Sample_Load(FSOUND_FREE, "sounds/music/GameOver.wav", FSOUND_NORMAL | FSOUND_2D, 0, 0);
	FSOUND_Sample_SetMode(soundGameOver.sound, FSOUND_LOOP_NORMAL);
	soundGameOver.currentChannel = -1;
	
	soundBullet.sound = FSOUND_Sample_Load(FSOUND_FREE, "sounds/effects/Gun.wav", FSOUND_NORMAL | FSOUND_2D, 0, 0);
	FSOUND_Sample_SetMode(soundBullet.sound, FSOUND_LOOP_OFF);
	soundBullet.currentChannel = -1;

	soundSniper.sound = FSOUND_Sample_Load(FSOUND_FREE, "sounds/effects/SniperRifle.wav", FSOUND_NORMAL | FSOUND_2D, 0, 0);
	FSOUND_Sample_SetMode(soundSniper.sound, FSOUND_LOOP_OFF);
	soundSniper.currentChannel = -1;

	soundMenuChange.sound = FSOUND_Sample_Load(FSOUND_FREE, "sounds/effects/MenuChange.wav", FSOUND_NORMAL | FSOUND_2D, 0, 0);
	FSOUND_Sample_SetMode(soundMenuChange.sound, FSOUND_LOOP_OFF);
	soundMenuChange.currentChannel = -1;

	soundMenuSelect.sound = FSOUND_Sample_Load(FSOUND_FREE, "sounds/effects/MenuSelect.wav", FSOUND_NORMAL | FSOUND_2D, 0, 0);
	FSOUND_Sample_SetMode(soundMenuSelect.sound, FSOUND_LOOP_OFF);
	soundMenuSelect.currentChannel = -1;

	soundLaser.sound = FSOUND_Sample_Load(FSOUND_FREE, "sounds/effects/BigLaser2.wav", FSOUND_NORMAL | FSOUND_2D, 0, 0);
	FSOUND_Sample_SetMode(soundLaser.sound, FSOUND_LOOP_NORMAL);
	soundLaser.currentChannel = -1;

	/* play the laser sound and pause it */
	playSound(&soundLaser);
	FSOUND_SetPaused(soundLaser.currentChannel, 1);
	/* set laser volume */
	FSOUND_SetVolume(soundLaser.currentChannel, 80);

	/* end sound loading */

	/* set the sound modes */
	/*FMOD_Sound_SetMode(soundMenu.sound, FMOD_LOOP_NORMAL);
	FMOD_Sound_SetMode(soundGame.sound, FMOD_LOOP_NORMAL);
	FMOD_Sound_SetMode(soundGameOver.sound, FMOD_LOOP_OFF);
	FMOD_Sound_SetMode(soundBullet.sound, FMOD_LOOP_OFF);*/

	/* set volumes */
	/*FMOD_Channel_SetVolume(soundBullet.currentChannel, 10);*/
}

/* update the sound system, must be called every game loop */
void updateSoundSystem()
{
	/* update the fmod system */
	FSOUND_Update();
}

/* unload the fmod system, must be called before the program exits */
void unloadSoundSystem()
{
	/* release all the sounds we loaded */

	FSOUND_Sample_Free(soundMenu.sound);
	FSOUND_Sample_Free(soundGameOver.sound);
	FSOUND_Sample_Free(soundGame.sound);
	FSOUND_Sample_Free(soundBullet.sound);
	FSOUND_Sample_Free(soundMenuSelect.sound);
	FSOUND_Sample_Free(soundMenuChange.sound);

	/*FMOD_Sound_Release(soundMenu.sound);
	FMOD_Sound_Release(soundGameOver.sound);
	FMOD_Sound_Release(soundGame.sound);
	FMOD_Sound_Release(soundBullet.sound);
	FMOD_Sound_Release(soundMenuSelect.sound);
	FMOD_Sound_Release(soundMenuChange.sound);*/

	/* close the sound system */
	/*FMOD_System_Close(soundSystem);*/

	/* release the sound system */
	/*FMOD_System_Release(soundSystem);*/
}

/* play a sound - used for looping music, will play till
the music is stopped with stopSound() */
void playSound(SOUND *currentSound)
{
	if(soundsEnabled)
	{
		/* make sure the sound isn't playing */
		if(currentSound->currentChannel == -1)
		{
			/*FMOD_System_PlaySound(soundSystem, FMOD_CHANNEL_FREE, currentSound->sound, 
				0, &(currentSound->currentChannel));*/
			currentSound->currentChannel = FSOUND_PlaySound(FSOUND_FREE, currentSound->sound);
		}
	}
}

/* stop a sound - used to stop looping music that is
currently playing */
void stopSound(SOUND *currentSound)
{
	/* make sure the sound is playing */
	if(currentSound->currentChannel != -1)
	{
		/*FMOD_Channel_Stop(currentSound->currentChannel);
		currentSound->currentChannel = NULL;*/
		FSOUND_StopSound(currentSound->currentChannel);
		currentSound->currentChannel = -1;
	}
}

/* this plays a sound like playSound, but uses a "copy" of the
sound being passed in. This way it can be played multiple times, for things
like bullets or explosions, and most other sound effects */
void playSoundCopy(SOUND *currentSound, int volume)
{
	if(soundsEnabled)
	{
		int channel = FSOUND_PlaySound(FSOUND_FREE, currentSound->sound);
		FSOUND_SetVolume(channel, volume);

	}
}
void pauseSound(SOUND *currentSound, int pause)
{
	FSOUND_SetPaused(currentSound->currentChannel, (char)pause);
}

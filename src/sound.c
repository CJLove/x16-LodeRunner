#include <unistd.h>
#include "ym2151.h"
#include "levels.h"
#include "sound.h"


static struct soundFx_t goldFx = {
    0, 0,
    0x1,    // Channel 1
    {
        YM_OP_CTRL, YM_RL_ENABLE|YM_CON_ALL_PL,
        YM_KS_AR, 0xf1,
        YM_AMS_EN_D1R, 0xa0,
        YM_D1L_RR, 0xff,
        YM_KC, 0x51,         // Octave 5 D
        YM_KEY_ON, YM_CH_1|YM_SN_ALL,
        //FX_DELAY_REG, 1,
        YM_KC, 0x53,         // Octave 5 E
        YM_KEY_ON, YM_CH_1|YM_SN_ALL,
        //FX_DELAY_REG, 1,
        YM_KC, 0x58,         // Octave 5 G
        YM_KEY_ON, YM_CH_1|YM_SN_ALL,
        //FX_DELAY_REG, 1,
        YM_KEY_ON, 0x00,
        FX_DONE_REG, 0        
    }
};

static struct soundFx_t deadFx = {
    0, 0,
    0x2,    // Channel 2
    {
        YM_OP_CTRL+YM_CH_2, YM_RL_ENABLE|YM_CON_ALL_PL,
        YM_KS_AR+YM_CH_2, 0xf1,
        YM_AMS_EN_D1R+YM_CH_2, 0xa0,
        YM_D1L_RR+YM_CH_2, 0xff,
        YM_KC+YM_CH_2, 0x44,
        YM_KEY_ON, YM_CH_2|YM_SN_ALL,
        FX_DELAY_REG,3,
        YM_KC+YM_CH_2, 0x42,
        YM_KEY_ON, YM_CH_2|YM_SN_ALL,
        FX_DELAY_REG,3,
        YM_KC+YM_CH_2, 0x40,
        YM_KEY_ON, YM_CH_2|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KEY_ON, YM_CH_2,
        FX_DONE_REG, 0        
    }
};

static struct soundFx_t digFx = {
    0, 0,
    0xc,    // Channels 3 & 4
    {
        YM_OP_CTRL+YM_CH_3, YM_RL_ENABLE|YM_CON_ALL_PL,
        YM_OP_CTRL+YM_CH_4, YM_RL_ENABLE|YM_CON_ALL_PL,
        YM_KS_AR+YM_CH_3, 0xf1,
        YM_AMS_EN_D1R+YM_CH_3, 0xa0,
        YM_D1L_RR+YM_CH_3, 0xff,
        YM_KS_AR+YM_CH_4, 0xf1,
        YM_AMS_EN_D1R+YM_CH_4, 0xa0,
        YM_D1L_RR+YM_CH_4, 0xff,

        YM_KEY_ON, YM_CH_3,
        YM_KEY_ON, YM_CH_4,
        YM_KC+YM_CH_3, 0x56,
        YM_KC+YM_CH_4, 0x58,        
        YM_KEY_ON, YM_CH_3|YM_SN_ALL,
        YM_KEY_ON, YM_CH_4|YM_SN_ALL,
        FX_DELAY_REG,2,
        YM_KEY_ON, YM_CH_3,
        YM_KEY_ON, YM_CH_4,
        FX_DELAY_REG,1,
        YM_KEY_ON, YM_CH_3|YM_SN_ALL,
        YM_KEY_ON, YM_CH_4|YM_SN_ALL,
        FX_DELAY_REG,2,
        YM_KEY_ON, YM_CH_3,
        YM_KEY_ON, YM_CH_4,
        FX_DELAY_REG,1,
        YM_KEY_ON, YM_CH_3|YM_SN_ALL,
        YM_KEY_ON, YM_CH_4|YM_SN_ALL,
        FX_DELAY_REG,2,
        YM_KEY_ON, YM_CH_3,
        YM_KEY_ON, YM_CH_4,
        FX_DONE_REG, 0        
    }
};

static struct soundFx_t fallFx = {
    0,0,
    0x10,   // Channel 5
    {
        YM_OP_CTRL+YM_CH_5, YM_RL_ENABLE|YM_CON_ALL_PL,
        YM_KS_AR+YM_CH_5, 0xf1,
        YM_AMS_EN_D1R+YM_CH_5, 0xa0,
        YM_D1L_RR+YM_CH_5, 0xff,
        YM_KC+YM_CH_5, YM_KC_OCT7|YM_KC_C,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT7|YM_KC_B,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT7|0xd,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT7|0xc,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT7|0xb,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT7|0xa,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT7|0x9,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT7|0x8,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT7|0x7,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT7|0x6,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT7|0x5,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT7|0x4,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT7|0x3,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT7|0x2,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT7|0x1,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|YM_KC_C,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|YM_KC_B,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|0xd,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|0xc,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|0xb,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|0xa,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|0x9,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|0x8,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|0x7,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|0x6,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|0x5,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|0x4,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|0x3,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|0x2,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT6|0x1,    
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|YM_KC_C,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|YM_KC_B,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|0xd,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|0xc,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|0xb,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|0xa,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|0x9,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|0x8,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|0x7,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|0x6,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|0x5,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|0x4,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|0x3,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|0x2,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT5|0x1,    
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT4|0xd,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT4|0xc,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT4|0xb,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT4|0xa,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT4|0x9,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT4|0x8,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT4|0x7,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT4|0x6,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        YM_KC+YM_CH_5, YM_KC_OCT4|0x5,
        YM_KEY_ON, YM_CH_5,
        FX_DONE_REG, 0,
    }
};

static struct soundFx_t passFx = {
    0,0,
    0x10,   // Channel 5
    {
        YM_OP_CTRL+YM_CH_5, YM_RL_ENABLE|YM_CON_ALL_PL,
        YM_KS_AR+YM_CH_5, 0xf1,
        YM_AMS_EN_D1R+YM_CH_5, 0xa0,
        YM_D1L_RR+YM_CH_5, 0xff,
        YM_KC+YM_CH_5, YM_KC_OCT5|0xe,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KC+YM_CH_5, YM_KC_OCT6|0x1,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KC+YM_CH_5, YM_KC_OCT6|0x4,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KC+YM_CH_5, YM_KC_OCT6|0x5,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KC+YM_CH_5, YM_KC_OCT6|0x8,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KC+YM_CH_5, YM_KC_OCT6|0xa,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KC+YM_CH_5, YM_KC_OCT6|0xd,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KC+YM_CH_5, YM_KC_OCT6|0xe,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KC+YM_CH_5, YM_KC_OCT7|0x1,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KC+YM_CH_5, YM_KC_OCT7|0x4,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KC+YM_CH_5, YM_KC_OCT7|0x5,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KC+YM_CH_5, YM_KC_OCT7|0x8,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KC+YM_CH_5, YM_KC_OCT7|0xa,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KC+YM_CH_5, YM_KC_OCT7|0xd,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KC+YM_CH_5, YM_KC_OCT7|0xe,
        YM_KEY_ON, YM_CH_5|YM_SN_ALL,
        FX_DELAY_REG,1,
        YM_KEY_ON, YM_CH_5,
        FX_DONE_REG, 0,
    }
};

// Bitmasks for sound effects
#define PLAY_FALL 0x1
#define PLAY_GOLD 0x2
#define PLAY_DIG  0x4
#define PLAY_DEAD 0x8
#define PLAY_PASS 0x10

static uint16_t soundMask = 0;

void playSoundFx(void)
{
    // If sound is disabled then do nothing
    if (currentGame.sound == SOUND_OFF) return;

    if (soundMask & PLAY_FALL) {
        if (playFx(&fallFx)) {
            soundMask &= ~PLAY_FALL;
        }
    }
    if (soundMask & PLAY_GOLD) {
        if (playFx(&goldFx)) {
            soundMask &= ~PLAY_GOLD;
        }
    }
    if (soundMask & PLAY_DIG) {
        if (playFx(&digFx)) {
            soundMask &= ~PLAY_DIG;
        }
    }
    if (soundMask & PLAY_DEAD) {
        if (playFx(&deadFx)) {
            soundMask &= ~PLAY_DEAD;
        }
    }
    if (soundMask & PLAY_PASS) {
        if (playFx(&passFx)) {
            soundMask &= ~PLAY_PASS;
        }
    }
}

void stopAllSoundFx(void)
{
    stopFx(&fallFx);
    stopFx(&goldFx);
    stopFx(&digFx);
    stopFx(&deadFx);
    stopFx(&passFx);
    soundMask = 0;
}

// Functions to play/preempt the falling sound effect
void playFallingFx(void)
{
    soundMask |= PLAY_FALL;
}

void stopFallingFx(void)
{
    stopFx(&fallFx);
    soundMask &= ~PLAY_FALL;
}

// Function to play the gold sound effect
void playGoldFx(void)
{
    stopFx(&goldFx);
    soundMask |= PLAY_GOLD;
}

// Functions to play/preempt the digging sound effect
void playDiggingFx(void)
{
    stopFx(&digFx);
    soundMask |= PLAY_DIG;
}

void stopDiggingFx(void)
{
    stopFx(&digFx);
    soundMask &= ~PLAY_DIG;
}

// Function to play the runner death sound effect
void playDeadFx(void)
{
    if (currentGame.sound == SOUND_ON)
        playFxSync(&deadFx);
    else
        sleep(3);
}

// Functions to play/preempt the level completion scoring sound effect
void playScoringFx(void)
{
    soundMask |= PLAY_PASS;
}

void stopScoringFx(void)
{
    stopFx(&passFx);
    soundMask &= ~PLAY_PASS;
}
#include <stm32f031x6.h>
#include "musical_notes.h"
#include "sound.h"

extern void delay(volatile uint32_t dly);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);

// ── Core timer control ────────────────────────────────────────────────────────

void playNote(uint32_t Freq)
{
    TIM14->ARR  = (uint32_t)65536 / (uint32_t)Freq;
    TIM14->CCR1 = TIM14->ARR / 2;
    TIM14->CNT  = 0;
    TIM14->CR1 |= (1 << 0);
}

void stopSound(void)
{
    TIM14->CR1 &= ~(1 << 0);
}

void initSound(void)
{
    RCC->APB1ENR |= (1 << 8);
    pinMode(GPIOB, 1, 2);
    GPIOB->AFR[0] &= ~(0x0fu << 4);
    TIM14->CR1   = 0;
    TIM14->CCMR1 = (1 << 6) | (1 << 5);
    TIM14->CCER |= (1 << 0);
    TIM14->PSC   = 48000000UL / 65536UL;
    TIM14->ARR   = (48000000UL / (uint32_t)(TIM14->PSC)) / (uint32_t)C4;
    TIM14->CCR1  = TIM14->ARR / 2;
    TIM14->CNT   = 0;
}

// ── Game sound effects ────────────────────────────────────────────────────────

void sfx_menu_move(void)
{
    playNote(A5);  delay(30);
    stopSound();
}

void sfx_confirm(void)
{
    playNote(E5);  delay(60);
    playNote(B5);  delay(80);
    stopSound();
}

void sfx_title_jingle(void)
{
    playNote(C5);  delay(100);
    playNote(E5);  delay(100);
    playNote(G5);  delay(100);
    playNote(E5);  delay(80);
    playNote(C6);  delay(180);
    stopSound();
}

void sfx_battle_start(void)
{
    playNote(C4);  delay(80);
    playNote(E4);  delay(80);
    playNote(G4);  delay(80);
    playNote(C5);  delay(150);
    stopSound();   delay(40);
    playNote(C5);  delay(200);
    stopSound();
}

void sfx_player_attack(void)
{
    playNote(A5);  delay(40);
    playNote(E5);  delay(40);
    playNote(A4);  delay(60);
    stopSound();
}

void sfx_enemy_attack(void)
{
    playNote(G3);  delay(50);
    playNote(D3);  delay(50);
    playNote(G2);  delay(80);
    stopSound();
}

void sfx_enemy_ko(void)
{
    playNote(G4);  delay(80);
    playNote(B4);  delay(80);
    playNote(D5);  delay(80);
    playNote(G5);  delay(200);
    stopSound();
}

void sfx_player_ko(void)
{
    playNote(G4);  delay(100);
    playNote(E4);  delay(100);
    playNote(C4);  delay(100);
    playNote(G3);  delay(200);
    stopSound();
}

void sfx_victory(void)
{
    playNote(C5);       delay(100);
    playNote(C5);       delay(100);
    playNote(C5);       delay(100);
    playNote(C5);       delay(150);
    stopSound();        delay(20);
    playNote(GS4_Ab4);  delay(150);
    playNote(AS4_Bb4);  delay(150);
    playNote(C5);       delay(100);
    stopSound();        delay(30);
    playNote(AS4_Bb4);  delay(100);
    playNote(C5);       delay(300);
    stopSound();
}

void sfx_defeat(void)
{
    playNote(E4);       delay(150);
    playNote(DS4_Eb4);  delay(150);
    playNote(D4);       delay(150);
    playNote(CS4_Db4);  delay(150);
    playNote(C4);       delay(300);
    stopSound();
}

// ── Background music engine ───────────────────────────────────────────────────

#define REST 0

const MusicNote track_menu[] = {
    {C5,120},{E5,120},{G5,120},{E5,120},
    {C5,120},{G4,180},{REST,60},
    {F5,120},{A5,120},{C6,120},{A5,120},
    {F5,120},{C5,180},{REST,80},
    {G5,120},{B5,120},{D6,120},{B5,120},
    {G5,120},{D5,180},{REST,60},
    {E5,120},{G5,120},{C6,180},{REST,100},
};

const MusicNote track_battle[] = {
    {A4,80},{A4,80},{REST,40},{A4,80},{REST,40},{F4,80},{A4,80},
    {REST,60},{G4,80},{REST,40},{G4,80},{REST,40},{E4,80},{G4,80},
    {REST,60},{F4,80},{G4,40},{F4,80},{E4,80},{D4,80},{REST,60},
    {A4,80},{REST,40},{A4,80},{C5,120},{REST,80},
    {B4,80},{B4,80},{REST,40},{B4,80},{REST,40},{G4,80},{B4,80},
    {REST,60},{A4,160},{REST,60},{A4,80},{B4,80},
    {C5,80},{B4,80},{A4,80},{G4,80},{F4,160},{REST,80},
};

const MusicNote track_mystery[] = {
    {DS4_Eb4,200},{REST,80},{GS4_Ab4,200},{REST,80},
    {G4,160},{REST,60},{F4,160},{REST,60},
    {DS4_Eb4,200},{CS4_Db4,200},{REST,100},
    {C4,300},{REST,120},
    {GS3_Ab3,200},{REST,80},{AS3_Bb3,160},{REST,60},
    {G3,200},{REST,80},{F3,160},{REST,80},
    {DS3_Eb3,300},{REST,150},
};

const MusicNote track_godmode[] = {
    {C5,80},{E5,80},{G5,80},{C6,80},{REST,40},
    {C6,80},{G5,80},{E5,80},{C5,80},{REST,40},
    {G5,80},{B5,80},{D6,80},{G6,80},{REST,40},
    {G6,160},{REST,60},{G6,80},{A6,80},{REST,40},
    {A6,80},{G6,80},{F6,80},{E6,80},{D6,80},{C6,80},
    {E6,160},{G6,160},{C7,240},{REST,100},
};

const uint16_t len_track_menu    = sizeof(track_menu)    / sizeof(track_menu[0]);
const uint16_t len_track_battle  = sizeof(track_battle)  / sizeof(track_battle[0]);
const uint16_t len_track_mystery = sizeof(track_mystery) / sizeof(track_mystery[0]);
const uint16_t len_track_godmode = sizeof(track_godmode) / sizeof(track_godmode[0]);

static const MusicNote *current_track = 0;
static uint16_t         track_len     = 0;
static uint16_t         track_pos     = 0;
static uint32_t         note_end_ms   = 0;
static uint8_t          music_looping = 0;

void music_play(const MusicNote *track, uint16_t len, uint8_t loop)
{
    current_track = track;
    track_len     = len;
    track_pos     = 0;
    music_looping = loop;
    note_end_ms   = 0;
}

void music_stop(void)
{
    current_track = 0;
    stopSound();
}

void music_tick(uint32_t now_ms)
{
    if (!current_track) return;
    if ((int32_t)(now_ms - note_end_ms) < 0) return;

    const MusicNote *n = &current_track[track_pos];
    if (n->freq == REST) stopSound();
    else                 playNote(n->freq);

    note_end_ms = now_ms + n->dur_ms;
    track_pos++;
    if (track_pos >= track_len) {
        if (music_looping) track_pos = 0;
        else               music_stop();
    }
}
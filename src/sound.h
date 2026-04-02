#pragma once
#include <stdint.h>
#include "musical_notes.h"

typedef struct { uint32_t freq; uint16_t dur_ms; } MusicNote;

void initSound(void);
void playNote(uint32_t Freq);
void stopSound(void);

// SFX
void sfx_menu_move(void);
void sfx_confirm(void);
void sfx_title_jingle(void);
void sfx_battle_start(void);
void sfx_player_attack(void);
void sfx_enemy_attack(void);
void sfx_enemy_ko(void);
void sfx_player_ko(void);
void sfx_victory(void);
void sfx_defeat(void);

// Music engine
void music_play(const MusicNote *track, uint16_t len, uint8_t loop);
void music_stop(void);
void music_tick(uint32_t now_ms);

extern const MusicNote track_menu[];
extern const MusicNote track_battle[];
extern const MusicNote track_mystery[];
extern const MusicNote track_godmode[];

extern const uint16_t len_track_menu;
extern const uint16_t len_track_battle;
extern const uint16_t len_track_mystery;
extern const uint16_t len_track_godmode;
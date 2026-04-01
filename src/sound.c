#include <stm32f031x6.h>
#include "musical_notes.h"
#include "sound.h"

// Provided by main.c — declared here so sound functions can use it
extern void delay(volatile uint32_t dly);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);

// ── Core timer control ────────────────────────────────────────────────────────

void playNote(uint32_t Freq)
{
	// Counter is running at 65536 Hz
	TIM14->ARR  = (uint32_t)65536 / (uint32_t)Freq;
	TIM14->CCR1 = TIM14->ARR / 2;
	TIM14->CNT  = 0;
	TIM14->CR1 |= (1 << 0); // enable counter
}

void stopSound(void)
{
	TIM14->CR1 &= ~(1 << 0); // disable counter (silence)
}

void initSound(void)
{
	RCC->APB1ENR |= (1 << 8);          // power up TIM14
	pinMode(GPIOB, 1, 2);              // PB1 = alternate function
	GPIOB->AFR[0] &= ~(0x0fu << 4);   // AF0 = TIM14_CH1
	TIM14->CR1   = 0;
	TIM14->CCMR1 = (1 << 6) | (1 << 5);
	TIM14->CCER |= (1 << 0);
	TIM14->PSC   = 48000000UL / 65536UL; // counter runs at 65536 Hz
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
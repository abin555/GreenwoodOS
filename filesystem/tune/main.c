#include "libc.h"

uint32_t *timer_ticks;
void wait_ticks(int ticks);

typedef enum {
    A0 = 27,
    B0 = 30,
    C1 = 32,
    D1 = 36,
    E1 = 41,
    F1 = 43,
    G1 = 48,
    A1 = 55,
    B1 = 61,
    C2 = 65,
    D2 = 73,
    E2 = 82,
    F2 = 87,
    G2 = 97,
    A2 = 110,
    B2 = 123,
    C3 = 130,
    D3 = 146,
    E3 = 164,
    F3 = 174,
    G3 = 196,
    A3 = 220,
    B3 = 246,
    C4 = 261,
    D4 = 293,
    E4 = 329,
    F4 = 349,
    G4 = 392,
    A4 = 440,
    B4 = 493,
    C5 = 523,
    D5 = 587,
    E5 = 659,
    F5 = 698,
    G5 = 783,
    A5 = 880,
    B5 = 987,
    C6 = 1046,
    D6 = 1174,
    E6 = 1318,
    F6 = 1396,
    G6 = 1568,
    A6 = 1760,
    B6 = 1975,
    C7 = 2093,
    D7 = 2349,
    E7 = 2637,
    F7 = 2793,
    G7 = 3136,
    A7 = 3520,
    B7 = 3951,
    C8 = 4186,
} Note;

struct PCSpeaker_Handle *speaker;

void play_note(Note note, int time);
void rest(int time);


int main(int argc, char **argv){
    print("[Tune Program]\n");
    speaker = getPCSpeaker();
    timer_ticks = getTimerTickHandle();
    for(int i = 0 ; i < 2; i++){
        play_note(F4, 2);
        rest(2);
        play_note(A4, 2);
        rest(1);
        play_note(B4, 4);
        rest(1);
        play_note(F4, 2);
        rest(2);
        play_note(A4, 2);
        rest(1);
        play_note(B4, 4);
        play_note(F4, 2);
        rest(1);
        play_note(A4, 2);
        rest(1);
        play_note(B4, 2);
        rest(1);
        play_note(E5, 2);
        rest(1);
        play_note(D5, 4);
        rest(1);
        play_note(B4, 2);
        rest(2);
        play_note(C5, 2);
        rest(1);
        play_note(B4, 2);
        rest(2);
        play_note(G4, 2);
        rest(1);
        play_note(E4, 12);
        rest(2);
        play_note(D4, 2);
        rest(1);
        play_note(E4, 4);
        play_note(G4, 2);
        rest(1);
        play_note(E4, 12);
    }
    return 0;
}


void wait_ticks(int ticks){
    ticks = ticks * 6;
    if(timer_ticks == NULL) return;
    uint32_t start_ticks = *timer_ticks;
    uint32_t target = start_ticks + ticks;
    while(*timer_ticks < target){}
}

void play_note(Note note, int time){
    speaker->play(note);
    wait_ticks(time);
    speaker->mute();
}
void rest(int time){
    wait_ticks(time);
}
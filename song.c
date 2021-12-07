/*
 *  song.c
 *
 *  Created on: Dec 6, 2021
 *  Author: James Minardi, Ainara Machargo
 */

#include "song.h"

unsigned char song_notes[7] = { 67, 66, 64, 62, 64, 0, 64 };
unsigned char song_duration[7] = { 45, 40, 45, 40, 15, 1, 15 };
unsigned char note_duration[1] = { 25 };
unsigned char hole_note[1] = { 60 };
unsigned char boundary_note[1] = { 55 };
unsigned char bump_note[1] = { 52 };

void song_play(oi_t *sensor_data)
{
    // G-F#-E-D-E-E
    oi_loadSong(1, 7, song_notes, song_duration);
    oi_play_song(1);
}

void hole_play(oi_t *sensor_data)
{
    // C
    oi_loadSong(1, 1, hole_note, note_duration);
    oi_play_song(1);
}

void boundary_play(oi_t *sensor_data)
{
    // G
    oi_loadSong(1, 1, boundary_note, note_duration);
    oi_play_song(1);
}


void bump_play(oi_t *sensor_data)
{
    // E
    oi_loadSong(1, 1, bump_note, note_duration);
    oi_play_song(1);
}


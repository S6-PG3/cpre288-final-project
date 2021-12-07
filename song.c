/*
 *  song.c
 *
 *  Created on: Dec 6, 2021
 *  Author: James Minardi, Ainara Machargo
 */

#include "song.h"

song_play(oi_t *sensor_data) {
    // G-F#-E-D-E-E
    oi_loadSong(1, 7, song_notes, song_duration);
    oi_play_song(1);
}

hole_play(oi_t *sensor_data) {
    // C
    oi_loadSong(1, 1, hole, note_duration);
    oi_play_song(1);
}

boundary_play(oi_t *sensor_data) {
    // G
    oi_loadSong(1, 1, boundary, note_duration);
    oi_play_song(1);
}

object_play(oi_t *sensor_data) {
    // E
    oi_loadSong(1, 1, object, note_duration);
    oi_play_song(1);
}



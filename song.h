/*
 *  song.h
 *
 *  Created on: Dec 6, 2021
 *  Author: Ainara Machargo, James Minardi
 */

#ifndef SONG_H_
#define SONG_H_

    unsigned char song_notes[7] = {67, 66, 64, 62, 64, 0, 64};
    unsigned char song_duration[7] =  {45, 40, 45, 40, 15, 1, 15};
    unsigned char note_duration[1] =  {25};
    unsigned char hole[1] = {60};
    unsigned char boundary[1] = {55};
    unsigned char object[1] = {52};

song_play(oi_t *sensor_data);

hole_play(oi_t *sensor_data);

boundary_play(oi_t *sensor_data);

object_play(oi_t *sensor_data);

#endif /* SONG_H_ */

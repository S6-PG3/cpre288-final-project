/*
 *  song.h
 *
 *  Created on: Dec 6, 2021
 *  Author: Ainara Machargo, James Minardi
 */

#ifndef SONG_H_
#define SONG_H_

#include "open_interface.h"

void song_play(oi_t *sensor_data);

void hole_play(oi_t *sensor_data);

void boundary_play(oi_t *sensor_data);

void bump_play(oi_t *sensor_data);

#endif /* SONG_H_ */

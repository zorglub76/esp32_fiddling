/*
 * Rotary.h
 *
 *  Created on: May 14, 2017
 *      Author: mare
 */

#include <driver/gpio.h>

#ifndef COMPONENTS_ROTARY_ENCODER_CSRC_ROTARY_H_
#define COMPONENTS_ROTARY_ENCODER_CSRC_ROTARY_H_

// Enable this to emit codes twice per step.
// #define HALF_STEP

// Enable weak pullups
#define ENABLE_PULLUPS

// Values returned by 'process'
// No complete step yet.
#define DIR_NONE 0x0
// Clockwise step.
#define DIR_CW 0x10
// Counter-clockwise step.
#define DIR_CCW 0x20

#endif /* COMPONENTS_ROTARY_ENCODER_CSRC_ROTARY_H_ */

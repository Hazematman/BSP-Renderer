#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} colour3UByte;

typedef struct {
	float x;
	float y;
	float z;
} vec3Float;

typedef struct {
	int16_t x;
	int16_t y;
	int16_t z;
} vec3Short;

typedef struct {
	vec3Float min;
	vec3Float max;
} BBoxFloat;

typedef struct {
	vec3Short min;
	vec3Short max;
} BBoxShort;

#endif

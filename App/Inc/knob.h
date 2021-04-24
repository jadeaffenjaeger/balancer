#ifndef KNOB_H_
#define KNOB_H_ 
#include <stdint.h>
#include <stdbool.h>
#include "adc.h"

typedef struct {
    float min;
    float max;
} knob_t;

void knob_init(knob_t *config);
float knob_read(knob_t *config);

#endif

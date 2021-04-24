#include "knob.h"


const uint32_t KNOB_RAW_MIN = 0;
const uint32_t KNOB_RAW_MAX = 4096;

static float mid_point = 0.0;
static float scale = 1.0;

void knob_init(knob_t *config) {
    scale = (float) (config->max - config->min)/4096;
}

float knob_read(knob_t *config) {
    float raw_value = (float) adc1_read();
    raw_value *= scale;
    raw_value += config->min;
    return raw_value;
}

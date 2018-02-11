#version 410

// Pseudo-random number generator: hacky & probably non-functional across drivers.
// It is based on The Book of Shaders, essentially, the idea is that if we bump up
// the modulation of the sine wave to something really big the sampling of it will
// give something that is a bit pseudo-random. We add a local state to it as well.

float sineprng_seed = 10e5;
float sineprng_state = 0.0;
float sineprng() {
    sineprng_state += 10e2;
    return fract(sin(sineprng_state)*sineprng_seed);
}

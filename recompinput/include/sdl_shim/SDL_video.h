// SDL2 ships SDL_video.h as a granular header. Code that includes it
// directly (rather than going through SDL.h) ends up here on Android. Our
// shim is monolithic — forward to SDL.h which carries every typedef and
// declaration we expose.
#pragma once
#include "SDL.h"

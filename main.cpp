#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_audio.h>
#include <OpenGL/glu.h>
#include "libs/maximilian.h"

const int AMPLITUDE = 28000;
const int SAMPLE_RATE = 44100;
const int BUFFER = 2048;

void init_sdl();
void init_audio();
void audio_callback();
void init_gl();
void render();
void quit();

SDL_Window* window = NULL;
SDL_GLContext context;
maxiOsc carrier, modulator;
int sample_num = 0;

void audio_callback(void *user_data, Uint8 *raw_buffer, int bytes)
{
  Sint16 *buffer = (Sint16*) raw_buffer;
  int length = bytes / 2; 
  int &sample_num(*(int*) user_data);

  for(int i = 0; i < length; i++, sample_num++)
  {
    // FM synthesis
    buffer[i] = AMPLITUDE * carrier.sinewave(440 + 
        (modulator.sinewave(1)*100)
        );
  }
}

void init_sdl() {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  window = SDL_CreateWindow("SDL_Maximilian",SDL_WINDOWPOS_CENTERED, 
      SDL_WINDOWPOS_CENTERED, 680, 480, 0);
  context = SDL_GL_CreateContext(window);
  SDL_GL_SetSwapInterval(1);
}

void init_audio()
{
  SDL_AudioSpec spec_want;
  SDL_AudioSpec spec_have; //for error checking, l8r

  spec_want.freq = SAMPLE_RATE; 
  spec_want.format = AUDIO_S16SYS; 
  spec_want.channels = 1; 
  spec_want.samples = BUFFER; 
  spec_want.callback = audio_callback; 
  spec_want.userdata = &sample_num;

  SDL_OpenAudio(&spec_want, &spec_have);
}

void init_gl()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glClearColor(0.f, 0.f, 0.f, 1.f);
}

void render()
{
  // Draw the modulator position
  float y = modulator.sinewave(1);
  glPointSize(10);
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_POINTS);
  glVertex2f(0, y);
  glEnd();
}

void quit()
{
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char* args[])
{
  bool should_quit = false;

  init_sdl();
  SDL_Event events;

  init_gl();
  init_audio();

  SDL_PauseAudio(0);

  while(!should_quit)
  {
    while(SDL_PollEvent(&events) != 0)
    {
      if(events.type == SDL_QUIT)
      {
        should_quit = true;
      }
    }

    render();
    SDL_GL_SwapWindow(window);
  }
  quit();
  return 0;
}

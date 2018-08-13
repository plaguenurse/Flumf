#ifndef __FLIBBLE_H
#define __FLIBBLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


typedef struct Image
{
	SDL_Rect destPos;
	SDL_Rect srcPos;
	SDL_Texture * image;
	int xAnim;
	int yAnim;
	int animPos;
	int animTime;
	int x;
	int y;
	int w;
	int h;
	double r;
} Image;

typedef struct Layer
{
	SDL_Texture * layerTarget;
	SDL_Rect scrollDest;
	SDL_Rect scrollSrc;
} Layer;

typedef struct EventChain
{
	short int mouse;
	short int window;
	short int close;
	int winwidth;
	int winheight;
	int x;
	int y;
	int scrolly;
	int vx;//virtual x and y relative to in game pixels
	int vy;
	short key[128];
	short arrows[4];// R - L - D - U
} EventChain;

typedef struct Font
{
	Image * image;
	int natHeight;
} Font;

typedef struct Game
{
	EventChain * chain;
	SDL_PixelFormat* fmt;
	SDL_Window* window;
	SDL_Renderer * renderer;
	int width;
	int height;
	int currLayer;
	int layerSize;
	int layerCapacity;
	Layer ** layerList;
	SDL_Rect boundingBox;
} Game;

// Font representation?
Font * newFont(Image * image);
int writeWords(char * words, Font * font, int size, Game * game, int x, int y);
// main game window

Game * setup(int width,int height);
void endGame(Game * game);
// general setup
void moveLayer(Game * game,int x, int y,int layer);
// event chain
EventChain * initEventChain(int width, int height);
void getEvents(EventChain * chain, Game * game);

// loading images with animation and offscreen scrolling

Image * loadImage(char * image, int xAnim, int yAnim, Game * game);
Image * imageCopy(Image * image);
void moveImage(Image * image, int x, int y);
void moveImageTo(Image * image, int x, int y);
void animate(Image * image);
void animateSpeed(Image * image,int speed);
void animateRange(Image * image, int min, int max);
void drawImage(Image * image, Game * game);
void setToFrame(Image * image, int x, int y);
void display(Game * game);
void setLayer(Game * game, int layer);
int addLayer(Game * game, int width, int height);
void clearLayer(Game * game);
void swapLayers(Game * game, int layer1, int layer2);
void resizeWindow(Game * game);
Layer * newLayer(Game * game, int width, int height);
void freeImage(Image * image);
#endif

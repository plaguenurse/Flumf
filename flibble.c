#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h> 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "flibble.h"


void swapLayers(Game * game, int layer1, int layer2)
{
	Layer * layer = game->layerList[layer1];
	game->layerList[layer1] = game->layerList[layer2];
	game->layerList[layer2] = layer;
}


void endGame(Game * game)
{
	free(game->chain);
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	free(game);
	Mix_Quit();
	SDL_Quit();
}


Layer * newLayer(Game * game, int width, int height)
{
	Layer * layer = malloc(sizeof(Layer));
	layer->layerTarget = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	layer->scrollSrc.x=0;
	layer->scrollSrc.y=0;
	layer->scrollSrc.w=width;
	layer->scrollSrc.h=height;
	layer->scrollDest.x=0;
	layer->scrollDest.y=0;
	layer->scrollDest.w=width;
	layer->scrollDest.h=height;
	SDL_SetTextureBlendMode(layer->layerTarget,SDL_BLENDMODE_BLEND);
	return layer;
}

Game * setup(int width,int height)
{
	int imgFlags = IMG_INIT_PNG;
	Game * game = malloc(sizeof(Game));
	game->width = width;
	game->height = height;
	
	game->layerSize =0;
	game->layerCapacity = 10;
	game->layerList = malloc(sizeof(Layer)*game->layerCapacity);
	
	srand(time(NULL));
	
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		fprintf(stderr,"%s\n",SDL_GetError());
		return NULL;
	}
	
	game->window = SDL_CreateWindow("GAME",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width,height,SDL_WINDOW_RESIZABLE);
	
	
	if (game->window == NULL)
	{
		fprintf(stderr,"%s\n",SDL_GetError());
		return NULL;
	}
	
	game->renderer = SDL_CreateRenderer(game->window, -1,SDL_RENDERER_ACCELERATED| SDL_RENDERER_TARGETTEXTURE |SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawBlendMode(game->renderer,SDL_BLENDMODE_BLEND);
	if(!(IMG_Init(imgFlags)&imgFlags)) 
	{ 
		fprintf(stderr,"%s\n",IMG_GetError());
		return NULL;
	}
	
	game->layerSize++;
	
	
	SDL_SetRenderDrawColor(game->renderer,0,0,0,0);
	
	game->boundingBox.w=width;
	game->boundingBox.h=height;
	game->boundingBox.x=0;
	game->boundingBox.y=0;
	
	game->layerList[0] = newLayer(game,width,height);

	SDL_RenderSetViewport(game->renderer,&game->boundingBox);

	if((MIX_INIT_MP3&Mix_Init(MIX_INIT_MP3)) != MIX_INIT_MP3)
	{
		fprintf(stderr,"%s\n",Mix_GetError());
	}
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) 
		fprintf(stderr,"%s\n",Mix_GetError());

	setLayer(game,0);
	clearLayer(game);
	game->chain = initEventChain(game->width,game->height);
	
	return game;
}

void resizeWindow(Game * game)
{
	SDL_SetRenderTarget(game->renderer,NULL);
	SDL_RenderSetScale(game->renderer,(game->chain->winheight*1.0)/(game->height*1.0),(game->chain->winheight*1.0)/(game->height*1.0));	
	game->boundingBox.w=game->chain->winwidth*((game->height*1.0)/(game->chain->winheight*1.0));
	game->boundingBox.h=game->chain->winheight*((game->height*1.0)/(game->chain->winheight*1.0));
	SDL_SetRenderTarget(game->renderer,game->layerList[game->currLayer]->layerTarget);
}


void moveLayer(Game * game,int x, int y,int layer)
{
	game->layerList[layer]->scrollDest.x += x;
	game->layerList[layer]->scrollDest.y += y;
}

Font * newFont(Image * image)
{
	Font * font = malloc(sizeof(Font));
	font->image = image;
	font->natHeight = font->image->h;
	return font;
}

int writeWords(char * words, Font * font, int size, Game * game, int x, int y)
{
	SDL_Rect temprect = {x,y,font->image->w*((double)size/(double)font->image->h),size};
	int i = 0;
	for(i = 0; words[i] != 0; i++)
	{
		if(words[i]>='A' && words[i]<='Z')
		{
			setToFrame(font->image,(words[i]-'A')%font->image->xAnim,((words[i]-'A')/font->image->xAnim));
		}
		else if(words[i]>='a' && words[i]<='z')
		{
			setToFrame(font->image,(words[i]-'a'+26)%font->image->xAnim,(words[i]-'a'+26)/font->image->xAnim);
		}
		else if(words[i]>='0' && words[i]<='9')
		{
			setToFrame(font->image,(words[i]-'0'+52)%font->image->xAnim,(words[i]-'0'+52)/font->image->xAnim);
		}
		else
		{
			temprect.x += temprect.w;
			continue;
		}
		SDL_RenderCopy(game->renderer,font->image->image,&font->image->srcPos,&temprect);
		temprect.x += temprect.w;
	}
	return temprect.x-x;
}

void getEvents(EventChain * chain, Game * game)
{
	SDL_Event event;
	chain->window = 0;
    while (SDL_PollEvent(&event))
    {
        if(event.type == SDL_WINDOWEVENT)
        {
			if(event.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				 chain->close = 1;
			}
			else if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				 chain->window = 1;
				 chain->winwidth = event.window.data1;
				 chain->winheight = event.window.data2;
				 
			}
		}
		else if(event.type == SDL_MOUSEBUTTONDOWN)
		{
			chain->mouse = 1;
			chain->x = event.button.x; 
			chain->y = event.button.y; 
		}
		else if(event.type == SDL_MOUSEBUTTONUP)
		{
			chain->mouse = 2;
			chain->x = event.button.x; 
			chain->y = event.button.y; 
		}
		else if(event.type == SDL_MOUSEMOTION)
		{
			chain->x = event.motion.x; 
			chain->y = event.motion.y; 
		}
		else if(event.type == SDL_KEYDOWN)
		{
			if(event.key.keysym.sym <= 127)
				chain->key[event.key.keysym.sym] = 1;
			else if(event.key.keysym.sym >= 1073741903 && event.key.keysym.sym <= 1073741906)
				chain->arrows[event.key.keysym.sym - 1073741903] = 1;
		}
		else if(event.type == SDL_KEYUP)
		{
			if(event.key.keysym.sym <= 127)
				chain->key[event.key.keysym.sym] = 0;
			else if(event.key.keysym.sym >= 1073741903 && event.key.keysym.sym <= 1073741906)
				chain->arrows[event.key.keysym.sym - 1073741903] = 0;
		}
		if(event.type == SDL_MOUSEWHEEL)
		{
			if(event.wheel.y > 0 || event.wheel.y < 0) 
			{
				 chain->scrolly += event.wheel.y;
			}
		}

		chain->vx = chain->x*(game->boundingBox.h*1.0/chain->winheight);
		chain->vy = chain->y*(game->boundingBox.h*1.0/chain->winheight);
	}
}

EventChain * initEventChain(int width, int height)
{
	int i = 0;
	EventChain * event = malloc(sizeof(EventChain));
	event->mouse = 0;
	event->window = 0;
	event->close = 0;
	event->winwidth = width;
	event->winheight = height;
	event->x = 0;
	event->y = 0;
	event->scrolly = 0;
	for(i=0;i<128;i++)
		event->key[i] = 0;
	for(i=0;i<4;i++)
		event->arrows[i] = 0;
	return event;
}



Image * loadImage(char * image, int xAnim, int yAnim, Game * game)
{
	SDL_Surface * temp;
	SDL_Texture *imageSurf;
	Image * retval = malloc(sizeof(Image));

	temp = IMG_Load(image);
	imageSurf = SDL_CreateTextureFromSurface(game->renderer,temp);

	retval->image = imageSurf;
	retval->xAnim = xAnim;
	retval->yAnim = yAnim;
	retval->srcPos.x = retval->srcPos.y = 0;
	retval->destPos.x = retval->destPos.y = 0;
	retval->w = retval->srcPos.w = retval->destPos.w = temp->w/xAnim;
	retval->h = retval->srcPos.h = retval->destPos.h = temp->h/yAnim;
	retval->animTime = 0;
	retval->animPos = 0;
	retval->x = 0;
	retval->y = 0;
	retval->r = 0;
	//SDL_FreeSurface(temp);
	return retval;
}
Image * imageCopy(Image * image)
{
	Image * retval = malloc(sizeof(Image));
	
	retval->image = image->image;
	retval->xAnim = image->xAnim;
	retval->yAnim = image->yAnim;
	retval->srcPos.x = image->srcPos.x;
	retval->srcPos.y = image->srcPos.y;
	retval->destPos.x = image->destPos.x; 
	retval->destPos.y = image->destPos.y;
	retval->w = image->w;
	retval->srcPos.w = image->srcPos.w;
	retval->destPos.w = image->destPos.w;
	retval->h = image->h;
	retval->srcPos.h = image->srcPos.h;
	retval->destPos.h = image->destPos.h;
	retval->animTime =  image->animTime;
	retval->animPos =  image->animPos;
	retval->x =  image->x;
	retval->y =  image->y;
	retval->r =  image->r;
	return retval;
}

void moveImage(Image * image, int x, int y)
{
	image->destPos.x+=x;
	image->destPos.y+=y;
	image->x+=x;
	image->y+=y;
}
void moveImageTo(Image * image, int x, int y)
{
	image->destPos.x=x;
	image->destPos.y=y;
	image->x=x;
	image->y=y;
}

void animate(Image * image)
{
	image->animTime = (image->animTime + 1)%60;
	if(image->animTime%3==0)
	{
		image->animPos = (image->animPos+1)%(image->xAnim*image->yAnim);
		image->srcPos.x = (image->w*image->animPos)%(image->w*image->xAnim) + image->srcPos.x%image->w;
		image->srcPos.y = (((image->w*image->animPos)/(image->w*image->xAnim))*image->h)%(image->h*image->yAnim) + image->srcPos.y%image->h;
		
	}
}
void animateSpeed(Image * image, int speed)
{
	image->animTime = (image->animTime + 1)%60;
	if(image->animTime%speed==0)
	{
		image->animPos = (image->animPos+1)%(image->xAnim*image->yAnim);
		image->srcPos.x = (image->w*image->animPos)%(image->w*image->xAnim) + image->srcPos.x%image->w;
		image->srcPos.y = (((image->w*image->animPos)/(image->w*image->xAnim))*image->h)%(image->h*image->yAnim) + image->srcPos.y%image->h;
		
	}
}

void animateRange(Image * image, int min, int max)
{
	image->animTime = (image->animTime + 1)%60;
	if(image->animTime%3==0)
	{
		if(image->animPos < min)
			image->animPos = min;
		if(image->animPos >= max)
			image->animPos = min;

		image->srcPos.x = (image->w*image->animPos)%(image->w*image->xAnim);
		image->srcPos.y = (((image->w*image->animPos)/(image->w*image->xAnim))*image->h)%(image->h*image->yAnim);
		image->animPos = image->animPos+1;	
	}
}

void setToFrame(Image * image, int x, int y)
{
	image->srcPos.x = image->w*x;
	image->srcPos.y = image->h*y;

}

void drawImage(Image * image, Game * game)
{
	if(image->r == 0)
		SDL_RenderCopy(game->renderer,image->image,&image->srcPos,&image->destPos);
	else
		SDL_RenderCopyEx(game->renderer,image->image,&image->srcPos,&image->destPos,image->r,NULL,SDL_FLIP_NONE);
}

void setLayer(Game * game, int layer)
{
	if(layer>= game->layerSize)
	{
		fprintf(stderr,"Invalid Layer\n");
	}
	game->currLayer=layer;
	SDL_SetRenderTarget(game->renderer,game->layerList[layer]->layerTarget);
}

int addLayer(Game * game, int width, int height)
{
	
	if(game->layerSize+1>=game->layerCapacity)
	{
		game->layerCapacity*=2;
		game->layerList = malloc(sizeof(Layer)*game->layerCapacity);
	}
	game->layerList[game->layerSize] = newLayer(game,width,height);
	
	SDL_SetRenderTarget(game->renderer,game->layerList[game->layerSize]->layerTarget);
	SDL_RenderClear(game->renderer);
	SDL_SetRenderTarget(game->renderer,game->layerList[game->currLayer]->layerTarget);
	
	game->layerSize++;
	return game->layerSize-1;
}

void clearLayer(Game * game)
{
	SDL_RenderClear(game->renderer);
}

void display(Game * game)
{
	int i = 0;
	SDL_SetRenderTarget(game->renderer,NULL);
	SDL_RenderClear(game->renderer);
	for(i=0;i<game->layerSize;i++)
	{
		SDL_RenderCopy(game->renderer,game->layerList[i]->layerTarget,&game->layerList[i]->scrollSrc,&game->layerList[i]->scrollDest);
	}
	SDL_RenderPresent(game->renderer);
	
	SDL_SetRenderTarget(game->renderer,game->layerList[game->currLayer]->layerTarget);
}
void freeImage(Image * image)
{
	SDL_DestroyTexture(image->image);
	free(image);
}

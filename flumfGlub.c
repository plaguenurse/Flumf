#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "flibble.h"
#include "flumfGlub.h"


Item * getItem(Object * object)
{
	Item * itemRet = malloc(sizeof(Item));
	itemRet->image = imageCopy(object->image);
	itemRet->name = malloc(sizeof(char)*(strlen(object->name)+1));
	strcpy(itemRet->name,object->name);
	itemRet->mass = object->mass;
	itemRet->value = object->value;
	itemRet->fabric = object->fabric;
	itemRet->metal = object->metal;
	itemRet->nature = object->nature;
	itemRet->plastic = object->plastic;
	itemRet->technology = object->technology;
	return itemRet;
}

Item * getItemByName(char * item, ObjectList * list)
{
		int i = 0, j = 0;
		Object * objTemp = NULL;
		Item * itemRet = NULL;
		itemRet = malloc(sizeof(Item));
		for(j=0;j<list->tiersize && objTemp == NULL;j++)
		{
			for(i=0;i<list->size[j] && objTemp == NULL;i++)
			{
				if(strcmp(item,list->list[j][i]->name) == 0)
				{
					objTemp = list->list[j][i];
				}
			}
		}
		if(objTemp == NULL)
			return NULL;
					
		itemRet->image = imageCopy(objTemp->image);
		itemRet->name = malloc(sizeof(char)*(strlen(objTemp->name)+1));
		strcpy(itemRet->name,objTemp->name);
		itemRet->mass = objTemp->mass;
		itemRet->value = objTemp->value;
		itemRet->fabric = objTemp->fabric;
		itemRet->metal = objTemp->metal;
		itemRet->nature = objTemp->nature;
		itemRet->plastic = objTemp->plastic;
		itemRet->technology = objTemp->technology;
		return itemRet;
}

Image * getImageByName(char * item, ObjectList * list)
{
		int i = 0, j = 0;
		Image * retval = NULL;
		for(j=0;j<list->tiersize && retval == NULL;j++)
		{
			for(i=0;i<list->size[j] && retval == NULL;i++)
			{
				if(strcmp(item,list->list[j][i]->name) == 0)
				{
					retval = list->list[j][i]->image;
				}
			}
		}
		return retval;
}



void drawImagePyramid(Image * image, Game * game , int amount, int midx, int y,int maxwidth)
{
	SDL_Rect tempRect;
	int currSize = maxwidth;
	int level = 1, i = 0;
	
	tempRect.w = image->w;
	tempRect.h = image->h;
	for(level=1;amount>0;level++)
	{
		if(maxwidth<ceil(amount/2.0))
		{
			tempRect.x = midx - (image->w*maxwidth)/(2);
			tempRect.y = y - (double)image->h*(double)level*.8;
		}
		else
		{	
			if(amount < currSize)
			{
				currSize = amount;
			}
			else if(currSize>2)
			{
				currSize--;
			}
			else if(amount==1)
			{
				currSize=1;
			}
			tempRect.x = midx - (image->w*currSize)/(2);
			tempRect.y = y - (double)image->h*(double)level*.8;

		}
		if(level%2)
			tempRect.x+=3;
		
		for(i=0;i<currSize && i<maxwidth;i++)
		{
			SDL_RenderCopy(game->renderer,image->image,&image->srcPos,&tempRect);
			tempRect.x+=image->w;
		}
		if(maxwidth<ceil(amount/2.0))
		{
			amount -= maxwidth;
		}
		else
			amount -= currSize;
	}
}

void setToLoop(ObjectList* list,int loop)
{
	int i;
	int j;
	for(i=0;i<list->tiersize;i++)
	{
		for(j=0;j<list->size[i];j++)
		{
			setToFrame(list->list[i][j]->image,loop,0);
		}
	}
}

ObjectList* getList(char * fileLocation,Game * game)
{
	ObjectList* objectList;
	Object * tempobj;
	char buffer[512] = {0};
	int i = 0;
	int tiers = 10;// non hardcoded? might be necesary 
	Uint8 listSize=0;
	char * temp;
	FILE * file=fopen(fileLocation,"r");
	fscanf(file,"%s\n",buffer);
	listSize++;
	while(!feof(file))
	{
		fscanf(file,"%s\n",buffer);
		listSize++;
	}
	rewind(file);
	objectList = malloc(sizeof(ObjectList));
	objectList->list = malloc(sizeof(Object**)*tiers);
	for(i = 0;i< tiers;i++)
		objectList->list[i] = malloc(sizeof(Object*)*listSize);
	objectList->size = calloc(tiers,sizeof(int));
	objectList->tiersize = tiers;
	fgets(buffer,512,file);
	i = 0;
	while(!feof(file) && i<listSize)
	{
		tempobj = malloc(sizeof(Object));	
		temp = strtok(buffer,",");
		tempobj->name = malloc(sizeof(char)*(strlen(temp)+1));
		strcpy(tempobj->name,temp);
		tempobj->image = loadImage(strtok(NULL,","),4,1,game);
		tempobj->rarity = atoi(strtok(NULL,","));
		tempobj->mass = atoi(strtok(NULL,","));
		tempobj->value = atoi(strtok(NULL,","));
		tempobj->plastic = atoi(strtok(NULL,","));
		tempobj->fabric = atoi(strtok(NULL,","));
		tempobj->metal = atoi(strtok(NULL,","));
		tempobj->nature = atoi(strtok(NULL,","));
		tempobj->technology = atoi(strtok(NULL,","));
		objectList->list[tempobj->rarity][objectList->size[tempobj->rarity]] = tempobj;
		objectList->size[tempobj->rarity]++;
		i++;
		
		fgets(buffer,512,file);
	}
	fclose(file);
	
	return objectList;
}
Item * getItemGroup(ObjectList * list, int tier, int chutzpahPool)
{
	int point = 1;
	if(tier<0)
		tier = 0;
	point = getIndexPointforgrabby(list,tier,chutzpahPool);
	while(point == 0)
	{
		tier--;
		if(tier<0)
			tier = 0;
		point = getIndexPointforgrabby(list,tier,chutzpahPool);
	}
	return getItem(list->list[tier][rand()%(point)]);
}

int getIndexPointforgrabby(ObjectList * list,int tier, int chutzpah)
{
	int i = 0;
	while(i < list->size[tier] && list->list[tier][i]->mass< chutzpah)
		i++;
	return i;
	
}

int drawRecipes(RecipeList * recipeList,Game * game,Font * font,Image * recipeBackground,Image * recipeBlack,ObjectList * items, Pile * pile, int * scrolly, int * canScroll, Image * tinyResources, int * levels, int * scrollmax, int* clickable)
{
	int i = 0, j = 0, k =0,width,height, doubletemp,retval = 0, maxwidth;
	int active = 0, recipeJump = 0;
	char numText[16] = "x0";
	SDL_Rect tempRect,bgRect;
	Image * tempImg;
	
	*canScroll = 0;
	if(*scrolly < 0)
		*canScroll+=1;
	tempRect.x = 20;
	tempRect.y = *scrolly;
	bgRect.w = recipeBackground->w;
	bgRect.h = recipeBackground->h;
	for(i=0;i<recipeList->size;i++)
	{
		if(recipeList->list[i]->unlocked==1)
		{
			active++;
			recipeJump = 0;
			bgRect.w = recipeBackground->w;
			bgRect.h = recipeBackground->h;
			width=40+recipeList->list[i]->image->w;
			maxwidth=40+recipeList->list[i]->image->w;
			height = 130;
			for(j=0;j<recipeList->list[i]->size;j++)
			{
				tempImg = getImageByName(recipeList->list[i]->nameList[j],items);
				
				if(width+ tempImg->w>game->boundingBox.w*3/4)
				{
					height+=100;
					if(width > maxwidth)
						maxwidth = width;
					width = 40;
				}
				width +=tempImg->w+20;
			}
			if(width > maxwidth)
						maxwidth = width;
			
			if((recipeList->list[i]->levels[0]>0 || recipeList->list[i]->levels[1]>0 || recipeList->list[i]->levels[2]>0 || recipeList->list[i]->levels[3]>0 || recipeList->list[i]->levels[4]>0))
			{
				if(width + getStatWidth(recipeList->list[i]->levels,100000,1000000,game,font,tinyResources,350) > game->boundingBox.w*3/4)
				{
					height+=getStatHeight(recipeList->list[i]->levels,tinyResources)+40;
					recipeJump=1;
					width = 40+getStatWidth(recipeList->list[i]->levels,tempRect.x,tempRect.y,game,font,tinyResources,350);
					if(width > maxwidth)
						maxwidth = width;
				}
				else 
				{
					width+= getStatWidth(recipeList->list[i]->levels,tempRect.x,tempRect.y,game,font,tinyResources,350);
					if(width > maxwidth)
						maxwidth = width;
				}
			}
			bgRect.y = tempRect.y;
			bgRect.x = 0;
			for(j=0;j<maxwidth;j+=recipeBackground->w)
			{
				if(j==0)
					setToFrame(recipeBackground,0,0);
				else 
					setToFrame(recipeBackground,1+(j/recipeBackground->w)%6,0);
				SDL_RenderCopy(game->renderer,recipeBackground->image,&recipeBackground->srcPos,&bgRect);
				
				doubletemp = bgRect.y;
				bgRect.y+=recipeBackground->h;
				for(k=recipeBackground->h;k<height;k+=recipeBackground->h)
				{
					if(j==0)
					{
						setToFrame(recipeBackground,0,1+(k/recipeBackground->h)%4);
					}
					else
					{
						setToFrame(recipeBackground,1+(j/recipeBackground->w)%6,1+(k/recipeBackground->h)%4);
					}
					SDL_RenderCopy(game->renderer,recipeBackground->image,&recipeBackground->srcPos,&bgRect);
					bgRect.y+=recipeBackground->h;
				}
				if(j==0)
					setToFrame(recipeBackground,0,5);
				else 
					setToFrame(recipeBackground,1+(j/recipeBackground->w)%6,5);
				SDL_RenderCopy(game->renderer,recipeBackground->image,&recipeBackground->srcPos,&bgRect);	
				bgRect.x+=recipeBackground->w;
				bgRect.y = doubletemp;
				
			}
			setToFrame(recipeBackground,7,0);
			SDL_RenderCopy(game->renderer,recipeBackground->image,&recipeBackground->srcPos,&bgRect);
			doubletemp = bgRect.y;
			bgRect.y+=recipeBackground->h;
			for(k=recipeBackground->h;k<height;k+=recipeBackground->h)
			{
				setToFrame(recipeBackground,7,1+(k/recipeBackground->h)%4);
				SDL_RenderCopy(game->renderer,recipeBackground->image,&recipeBackground->srcPos,&bgRect);
				bgRect.y+=recipeBackground->h;
			}
			setToFrame(recipeBackground,7,5);
			SDL_RenderCopy(game->renderer,recipeBackground->image,&recipeBackground->srcPos,&bgRect);
			bgRect.y = doubletemp;
			
			
			writeWords(recipeList->list[i]->name,font,12,game,tempRect.x,tempRect.y + 20);
			
			tempRect.w = recipeList->list[i]->image->w;
			tempRect.h = recipeList->list[i]->image->h;
			doubletemp = tempRect.y;
			tempRect.y += 130 - tempRect.h;
			SDL_RenderCopy(game->renderer,recipeList->list[i]->image->image,&recipeList->list[i]->image->srcPos,&tempRect);
			tempRect.y = doubletemp;
			tempRect.x += 20+ tempRect.w;
			for(j=0;j<recipeList->list[i]->size;j++)
			{
				tempImg = getImageByName(recipeList->list[i]->nameList[j],items);
				tempRect.w = tempImg->w;
				tempRect.h = tempImg->h;
				
				
				if(tempRect.x + tempImg->w>game->boundingBox.w*3/4)
				{
					tempRect.y+=tempImg->h+20;
					tempRect.x = 40;
				}
				
				tempRect.y += 130 - tempRect.h;
				SDL_RenderCopy(game->renderer,tempImg->image,&tempImg->srcPos,&tempRect);
				
				tempRect.y -= 130 - tempRect.h;
	
				numText[1] = '0'+ recipeList->list[i]->numList[j];
				
				writeWords(numText,font,12,game,tempRect.x+tempImg->w,tempRect.y +130);
				

				tempRect.x +=20+ tempImg->w;
				
			}			
			if(recipeJump)
			{
			
				tempRect.y+=140;
				tempRect.x = 20;
			}
			else if((recipeList->list[i]->levels[0]>0 || recipeList->list[i]->levels[1]>0 || recipeList->list[i]->levels[2]>0 || recipeList->list[i]->levels[3]>0 || recipeList->list[i]->levels[4]>0))
			{
				tempRect.y+=110-getStatHeight(recipeList->list[i]->levels,tinyResources);
			
			}
			drawStats(recipeList->list[i]->levels,tempRect.x+10,tempRect.y+25,game,font,tinyResources, 350);
			if(recipeJump)
			{
				tempRect.y-=140;
			}
			else if((recipeList->list[i]->levels[0]>0 || recipeList->list[i]->levels[1]>0 || recipeList->list[i]->levels[2]>0 || recipeList->list[i]->levels[3]>0 || recipeList->list[i]->levels[4]>0))
			{
				tempRect.y-=110-getStatHeight(recipeList->list[i]->levels,tinyResources);
			}
			tempRect.x = 20;
			
			if(!canCraft(recipeList->list[i],pile,items,levels))
			{
				bgRect.x = 0;
				for(j=0;j<maxwidth;j+=recipeBlack->w)
				{
					if(j==0)
						setToFrame(recipeBlack,0,0);
					else 
						setToFrame(recipeBlack,1+(j/recipeBackground->w)%6,0);
					SDL_RenderCopy(game->renderer,recipeBlack->image,&recipeBlack->srcPos,&bgRect);
					
					doubletemp = bgRect.y;
					bgRect.y+=recipeBlack->h;
					for(k=recipeBlack->h;k<height;k+=recipeBlack->h)
					{
						if(j==0)
						{
							setToFrame(recipeBlack,0,1+(k/recipeBlack->h)%4);
						}
						else
						{
							setToFrame(recipeBlack,1+(j/recipeBlack->w)%6,1+(k/recipeBlack->h)%4);
						}
						SDL_RenderCopy(game->renderer,recipeBlack->image,&recipeBlack->srcPos,&bgRect);
						bgRect.y+=recipeBlack->h;
					}
					if(j==0)
						setToFrame(recipeBlack,0,5);
					else 
						setToFrame(recipeBlack,1+(j/recipeBlack->w)%6,5);
					SDL_RenderCopy(game->renderer,recipeBlack->image,&recipeBlack->srcPos,&bgRect);	
					bgRect.x+=recipeBlack->w;
					bgRect.y = doubletemp;
					
				}
				setToFrame(recipeBlack,7,0);
				SDL_RenderCopy(game->renderer,recipeBlack->image,&recipeBlack->srcPos,&bgRect);
				doubletemp = bgRect.y;
				bgRect.y+=recipeBlack->h;
				for(k=recipeBlack->h;k<height;k+=recipeBlack->h)
				{
					setToFrame(recipeBlack,7,1+(k/recipeBlack->h)%4);
					SDL_RenderCopy(game->renderer,recipeBlack->image,&recipeBlack->srcPos,&bgRect);
					bgRect.y+=recipeBlack->h;
				}
				setToFrame(recipeBlack,7,5);
				SDL_RenderCopy(game->renderer,recipeBlack->image,&recipeBlack->srcPos,&bgRect);
			}
			else
			{
				bgRect.y = tempRect.y;
				bgRect.w = (maxwidth/50+1)*50;
				bgRect.x =0;
				bgRect.h = (height/50+1)*50;
				if(isClicked(&bgRect,game,game->currLayer))
				{
					if(game->chain->mouse == 1)
					{
						retval = i+1;
						game->chain->mouse = 2;
					}
					*clickable = 1;
					
				}
			}
			tempRect.y+=height+60;
		}

	}
	if(tempRect.y > game->boundingBox.h)
	{
		*canScroll += 2;
	}
	*scrollmax = *scrolly - tempRect.y+ game->boundingBox.h;
	return retval;
}

int canCraft(Recipe * recipe, Pile * pile, ObjectList * list, int*levels)
{
	PileItem * item = NULL;
	int i,j;
	for(i =0; i< recipe->size;i++)
	{
		item = pile->firstAlpha;
		
		while(item!= NULL && strcmp(recipe->nameList[i],item->item->name)!=0 )
		{
			item = item->nextAlpha;
		}
		if(item->size < recipe->numList[i])
		{
			return 0;
		}		
	}
	for(j=0;j<5;j++)
	{
		if(levels[j]<recipe->levels[j])
			return 0;
	}
	return 1;
}



void unlockTier(int tier, RecipeList * list)
{
	int i;
	for(i=0;i<list->size;i++)
	{
		if(list->list[i]->type == tier)
		{
			list->list[i]->unlocked = 1;
		}
	}
}

void lockTier(int tier, RecipeList * list)
{
	int i;
	for(i=0;i<list->size;i++)
	{
		if(list->list[i]->type == tier)
		{
			list->list[i]->unlocked = 0;
		}
	}
}

RecipeList* getRecipeList(char * fileLocation, Game * game)
{
	RecipeList* recipeList;
	char buffer[512] = {0};
	int i = 0, j =0;
	int listSize=0;
	char * temp;
	FILE * file=fopen(fileLocation,"r");
	fgets(buffer,512,file);
	while(!feof(file))
	{
		fgets(buffer,512,file);
		listSize++;
	}
	rewind(file);
	recipeList = malloc(sizeof(RecipeList));
	recipeList->list = malloc(sizeof(Recipe*)*listSize);
	recipeList->size = listSize;
	fgets(buffer,512,file);
	while(!feof(file) && i<listSize)
	{
		recipeList->list[i] = malloc(sizeof(Recipe));	
		temp = strtok(buffer,",");
		recipeList->list[i]->name = malloc(sizeof(char)*(strlen(temp)+1));
		strcpy(recipeList->list[i]->name,temp);
		temp = strtok(NULL,",");
		recipeList->list[i]->image = loadImage(temp,3,1,game);
		recipeList->list[i]->type = atoi(strtok(NULL,","));
		recipeList->list[i]->nameList = NULL;
		recipeList->list[i]->numList = NULL;
		recipeList->list[i]->levels[0] = 0;
		recipeList->list[i]->levels[1] = 0;
		recipeList->list[i]->levels[2] = 0;
		recipeList->list[i]->levels[3] = 0;
		recipeList->list[i]->levels[4] = 0;
		temp = strtok(NULL,",");

		j = 0;
		while(temp != NULL)
		{
			if(strcmp("%levels",temp)==0)
			{
				recipeList->list[i]->levels[0]=atoi(strtok(NULL,","));
				recipeList->list[i]->levels[1]=atoi(strtok(NULL,","));
				recipeList->list[i]->levels[2]=atoi(strtok(NULL,","));
				recipeList->list[i]->levels[3]=atoi(strtok(NULL,","));
				recipeList->list[i]->levels[4]=atoi(strtok(NULL,","));
			}
			else
			{
				j++;
				recipeList->list[i]->nameList = realloc(recipeList->list[i]->nameList,sizeof(char*)*j);
				recipeList->list[i]->nameList[j-1] = malloc(sizeof(char)*(strlen(temp)+2));
				strcpy(recipeList->list[i]->nameList[j-1],temp);
				recipeList->list[i]->numList = realloc(recipeList->list[i]->numList,sizeof(int)*j);
				recipeList->list[i]->numList[j-1] = atoi(strtok(NULL,","));
			}
			temp = strtok(NULL,",");
		}
		if(recipeList->list[i]->type == 1)
		{
			recipeList->list[i]->unlocked = 1;
		}
		else
		{
			recipeList->list[i]->unlocked = 0;
		}
		recipeList->list[i]->size = j;
		i++;
		fgets(buffer,512,file);
	}
	fclose(file);
	return recipeList;
}

ObjectList* freeList(ObjectList* list)
{
	int i = 0,j = 0;
	if(list == NULL)
		return NULL;
	for(j=0;j<list->tiersize;j++)
	{
		for(i = 0; i<list->size[j];i++)
		{
			//SDL_FreeSurface(list->list[j][i]->image);
			free(list->list[j][i]->name);
			free(list->list[j][i]);
		}
		free(list->list[j]);
	}
	free(list->list);
	free(list->size);
	free(list);
	return NULL;
}

Pile * initPile(Image * image, int x, int y,ObjectList * items)
{
	Pile * pile = malloc (sizeof(Pile));
	pile->firstAlpha = NULL;
	pile->firstMass = NULL;
	pile->firstSize = NULL;
	pile->size = 0;
	pile->total = 0;
	pile->image = image;
	moveImageTo(pile->image,x,y-pile->image->h);
	insertAllItems(items,pile);
	return pile;
}

void drawPile(Pile * pile, Game * game)
{
	int size = 0, minHeight = 0, baseHeight = 0,i = 0;
	int x =pile->image->x,y=pile->image->y,w=pile->image->w,h=pile->image->h;

	PileItem * traverse = pile->firstSize;
	while(traverse!=NULL)
	{
		if(traverse->size>15)
		{
			for(i=0;i<15;i++)
			{
				if(baseHeight > game->boundingBox.h*1.5)
				{
					baseHeight = 0;
				}
				if((size + traverse->item->image->w )> w)
				{
					moveImageTo(traverse->item->image,x + w-traverse->item->image->w, y + h -traverse->item->image->h - baseHeight);
				
					if(minHeight > (traverse->item->image->h*9)/10)
						minHeight = (traverse->item->image->h*9)/10;
					else if(minHeight == 0)
						minHeight = (traverse->item->image->h*9)/10;
					size = 0;
					baseHeight += minHeight;
				}
				else
				{
					moveImageTo(traverse->item->image,x + size, y + h -traverse->item->image->h - baseHeight);
					
					size += (traverse->item->image->w*9)/10;
					
					if(minHeight > (traverse->item->image->h*9)/10)
						minHeight = (traverse->item->image->h*9)/10;
					else if(minHeight == 0)
						minHeight = (traverse->item->image->h*9)/10;
				}
				drawImage(traverse->item->image,game);
			}
		}
		else 
		{
			for(i=0;i<traverse->size;i++)
			{
				if(baseHeight > game->boundingBox.h*1.5)
				{
					baseHeight = 0;
				}
				if((size + traverse->item->image->w )> w)
				{
					moveImageTo(traverse->item->image,x + w-traverse->item->image->w, y + h -traverse->item->image->h - baseHeight);
				
					if(minHeight > (traverse->item->image->h*9)/10)
						minHeight = (traverse->item->image->h*9)/10;
					else if(minHeight == 0)
						minHeight = (traverse->item->image->h*9)/10;
					size = 0;
					baseHeight += minHeight;
				}
				else
				{
					moveImageTo(traverse->item->image,x + size, y + h -traverse->item->image->h - baseHeight);
					
					size += (traverse->item->image->w*9)/10;
					
					if(minHeight > (traverse->item->image->h*9)/10)
						minHeight = (traverse->item->image->h*9)/10;
					else if(minHeight == 0)
						minHeight = (traverse->item->image->h*9)/10;
				}
				drawImage(traverse->item->image,game);
			}
		}
		traverse = traverse->nextSize;
	}
	drawImage(pile->image,game);
}

int isClicked(SDL_Rect * box, Game * game, int layer)
{
	int x = game->chain->x*(game->boundingBox.h*1.0/game->chain->winheight);
	int y = game->chain->y*(game->boundingBox.h*1.0/game->chain->winheight);
		
	if(x>(box->x + game->layerList[layer]->scrollDest.x) &&
	 x< (box->x + box->w +game->layerList[layer]->scrollDest.x) &&
	  y>(box->y +game->layerList[layer]->scrollDest.y) &&
	   y < (box->y + box->h + game->layerList[layer]->scrollDest.y))
	{
		return 1;
	}
	return 0;
}

Adventurer * newAdventurer(int type, Game * game)
{
	Adventurer * pal = NULL;
	pal = malloc(sizeof(Adventurer));
	pal->dir = 1;
	pal->class = 0;
	pal->timer = 0;
	pal->delay = 0;
	pal->negpos = 0;
	pal->posTo = 0;
	pal->posToX = 0;
	pal->posToY = 0;
	
	pal->moxie = rand()%3 + 1;
	pal->burdenedmoxie = pal->moxie;
	pal->chutzpah = 50 + rand() % 35;
	pal->wonder = 1 + (rand()%100>=80);

	pal->advancedMove = 0;
	if(type == 1)
		pal->image = loadImage("images/pals/1.png",8,3,game);	
	else if (type ==2)
		pal->image = loadImage("images/pals/2.png",8,3,game);
	pal->status = 0;

	pal->type = type;
	pal->item = NULL;
	return pal;
}
void giveAdventurerNewItem(Adventurer * pal, Object * item)
{
	pal->item = getItem(item);
	moveImageTo(pal->item->image,pal->image->x - (pal->item->image->w)/2 + pal->image->x/2,pal->image->y - pal->item->image->h);
}
void giveAdventurerItem(Adventurer * pal, Item * item)
{
	pal->item = item;
	moveImageTo(item->image,pal->image->x,pal->image->y - item->image->h);
	
}
void giveAdventurerNewItemByName(Adventurer * pal, char * itemName, ObjectList * list)
{
	pal->item = getItemByName(itemName,list);
	moveImageTo(pal->item->image,pal->image->x - (pal->item->image->w)/2 + pal->image->x/2,pal->image->y - pal->item->image->h);
}

void drawAdventurer(Adventurer * pal, Game * game, Image * palImage)
{
	if(pal!=NULL)
	{
		if(pal->image->h!=palImage->h)
		{
			pal->image->h = palImage->h;
			pal->image->srcPos.h = palImage->srcPos.h;
			pal->image->destPos.h = palImage->destPos.h;
			
		}
		SDL_RenderCopy(game->renderer,palImage->image,&pal->image->srcPos,&pal->image->destPos);
		if(pal->item !=NULL )
		{
			drawImage(pal->item->image,game);
		}
	}
}
void drawAdventurerAtPoint(Adventurer * pal, Game * game,int x, int y)
{
	SDL_Rect temprect, base;
	base.x=0;
	base.y=0;
	base.w=pal->image->w;
	base.h=pal->image->h;
	temprect.w = pal->image->w;
	temprect.h = pal->image->h;
	temprect.x=x;
	temprect.y=y;
	if(pal!=NULL)
	{
		SDL_RenderCopy(game->renderer,pal->image->image,&base,&temprect);
	}
}

void moveAdventurer(Adventurer * pal, int x, int y)
{
	if(y!=0)
	{
		
	}
	else if(pal->dir==-1 && x>0)
	{
		setToFrame(pal->image,0,0);
		
		pal->dir = 1;
	}
	else if(pal->dir==1 && x<0)
	{
		setToFrame(pal->image,0,1);
			
		pal->dir = -1;
	}
	
	if(y!=0)
	{
		animateRange(pal->image,16,23);
	}
	else if(pal->dir == 1)
	{
		animateRange(pal->image,0,7);
	}
	else
	{
		animateRange(pal->image,8,15);
		
	}
	moveImage(pal->image,x,y);
	
	if(pal->item!=NULL)
	{
		moveImage(pal->item->image,x,y);
	}
	
}

void adventurerDo(Adventurer * pal, Game * game, ObjectList * list, int x, int y,int* currOutPals, char * redraw, int* eggs, int moxieBonus, int scrollmin,AmbulationTowerList * ambulationList, Image * palImage)
{
	int ambulation = 0;
	if(pal->type ==1)
	{
		
		if (pal->status == 1  && pal->image->x >= -pal->image->w*4)
		{
			if(pal->delay == 0)
			{
				ambulation =canAmbulate(ambulationList,pal->image->x, pal->image->y, pal->posToX,pal->posToY);
				if(ambulation !=0)
				{
					if(ambulation == 2)
					{
						moveAdventurer(pal,0,(pal->image->y<pal->posToY)?3:-3);
					}
					else
					{
						moveAdventurer(pal,getMoxie(pal,moxieBonus)*ambulation,0);
					}
					drawAdventurer(pal,game,palImage);
				}
				else
				{
					moveAdventurer(pal,getMoxie(pal,moxieBonus)*(pal->image->x-pal->posToX<0?1:-1),0);
					drawAdventurer(pal,game,palImage);
				}
			}
			else
			{
				pal->delay--;
			}
		}
		else if (pal->status == 2)
		{
			ambulation =canAmbulate(ambulationList,pal->image->x, pal->image->y, pal->posToX,pal->posToY);
			if(ambulation !=0)
			{
				if(ambulation == 2)
				{
					moveAdventurer(pal,0,(pal->image->y<pal->posToY)?3:-3);
				}
				else
				{
					moveAdventurer(pal,pal->burdenedmoxie*ambulation,0);
				}
				
			}
			else
			{
				moveAdventurer(pal,getMoxie(pal,moxieBonus)*(pal->image->x-pal->posToX<0?1:-1),0);
				drawAdventurer(pal,game,palImage);
			}
			drawAdventurer(pal,game,palImage);
		}
		else if (pal->status == 3)
		{
			if(pal->advancedMove !=0)
			{
				ambulation =canAmbulate(ambulationList,pal->image->x, pal->image->y, pal->image->x,640-30-pal->image->h);
				if ((abs(pal->image->y-640+30+pal->image->h)<3 || (abs(pal->image->y-pal->posToY)<3)) && canAmbulate(ambulationList,pal->image->x, pal->image->y, pal->posToX,pal->posToY)!=0)
				{
					pal->advancedMove = 0;
					drawAdventurer(pal,game,palImage);
				}
				else if (ambulation !=0)
				{
					if(ambulation == 2)
					{
						moveAdventurer(pal,0,(pal->image->y<640-30-pal->image->h)?3:-3);
					}
					else
					{
						moveAdventurer(pal,getMoxie(pal,moxieBonus)*ambulation,0);
					}
					drawAdventurer(pal,game,palImage);
				}
				else
				{
					moveAdventurer(pal,0,(pal->image->y<640-30-pal->image->h)?3:-3);
					drawAdventurer(pal,game,palImage);
				}
				
			}
			else
			{
				ambulation =canAmbulate(ambulationList,pal->image->x, pal->image->y, pal->posToX,pal->posToY);

				if(pal->item != NULL)
					pal->item = NULL;
				if(ambulation !=0)
				{
					if(ambulation == 2)
					{
						moveAdventurer(pal,0,(pal->image->y<pal->posToY)?3:-3);
					}
					else
					{
						moveAdventurer(pal,getMoxie(pal,moxieBonus)*ambulation,0);
					}
					drawAdventurer(pal,game,palImage);
				}
				else if(abs(pal->image->x-pal->posToX)<getMoxie(pal,moxieBonus)+20)
				{
					pal->status = 0;
					(*currOutPals)--;
				}
				else
				{
					moveAdventurer(pal,getMoxie(pal,moxieBonus)*(pal->image->x-pal->posToX<0?1:-1),0);
					drawAdventurer(pal,game,palImage);
				}
			}
		}
		else if (pal->status == 4)
		{
			ambulation =canAmbulate(ambulationList,pal->image->x, pal->image->y, pal->posToX,pal->posToY);

			
			if(pal->delay == 0)
			{
				if(ambulation !=0)
				{
					if(ambulation == 2)
					{
						moveAdventurer(pal,0,(pal->image->y<pal->posToY)?3:-3);
					}
					else
					{
						moveAdventurer(pal,getMoxie(pal,moxieBonus)*ambulation,0);
					}
					drawAdventurer(pal,game,palImage);
				}
				else if(pal->image->x+20 >= pal->posToX)
				{
					pal->status = 5;
				}
				else
				{
					moveAdventurer(pal,getMoxie(pal,moxieBonus)*(pal->image->x-pal->posToX<0?1:-1),0);
					drawAdventurer(pal,game,palImage);
				}
	
			}
			else
			{
				pal->delay--;
			}
			
		}
		else if (pal->status == 5)
		{
			if(pal->item!=NULL)
			{
				
				ambulation =canAmbulate(ambulationList,pal->image->x, pal->image->y, pal->posToX,pal->posToY);
				
				if(ambulation !=0)
				{
					if(ambulation == 2)
					{
						moveAdventurer(pal,0,(pal->image->y<pal->posToY)?3:-3);
					}
					else
					{
						moveAdventurer(pal,getMoxie(pal,moxieBonus)*ambulation,0);
					}
					drawAdventurer(pal,game,palImage);
				}
				else if(abs(pal->image->x-pal->posToX)<getMoxie(pal,moxieBonus)+20)
				{
					pal->status = 6;
				}
				else
				{
					moveAdventurer(pal,getMoxie(pal,moxieBonus)*(pal->image->x-pal->posToX<0?1:-1),0);
					drawAdventurer(pal,game,palImage);
				}
			}
		}
		else if (pal->status==7)
		{
			if(pal->delay == 0)
			{
				ambulation =canAmbulate(ambulationList,pal->image->x, pal->image->y, pal->posToX,pal->posToY);

				if(ambulation !=0)
				{
					if(ambulation == 2)
					{
						moveAdventurer(pal,0,(pal->image->y<pal->posToY)?3:-3);
					}
					else
					{
						moveAdventurer(pal,getMoxie(pal,moxieBonus)*ambulation,0);
					}
				
				}
				else
				{
					moveAdventurer(pal,getMoxie(pal,moxieBonus)*(pal->image->x-pal->posToX<0?1:-1),0);
					drawAdventurer(pal,game,palImage);
				}
				drawAdventurer(pal,game,palImage);
			}
			else
			{
				pal->delay--;
			}
			if(pal->image->x+20 >=pal->posToX)
			{
				(*eggs)--;
				*redraw = 1;
				pal->status = 18;
			}
		}
		else if(pal->status==8)
		{
			ambulation =canAmbulate(ambulationList,pal->image->x, pal->image->y, pal->posToX,pal->posToY);
			if(ambulation !=0)
			{
				if(ambulation == 2)
				{
					moveAdventurer(pal,0,(pal->image->y<pal->posToY)?3:-3);
				}
				else
				{
					moveAdventurer(pal,getMoxie(pal,moxieBonus)*ambulation,0);
				}
				drawAdventurer(pal,game,palImage);
			}
			else 
			{
				if(abs(pal->image->x-pal->posToX)<getMoxie(pal,moxieBonus)+20)
				{
					pal->status = 9;
				}
				else
				{
					moveAdventurer(pal,getMoxie(pal,moxieBonus)*(pal->image->x-pal->posToX<0?1:-1),0);
					drawAdventurer(pal,game,palImage);
				}
			}
		}
		else if(pal->status==10)
		{
			ambulation =canAmbulate(ambulationList,pal->image->x, pal->image->y, pal->posToX,pal->posToY);

			if(ambulation !=0)
			{
				if(ambulation == 2)
				{
					moveAdventurer(pal,0,(pal->image->y<pal->posToY)?3:-3);
				}
				else
				{
					moveAdventurer(pal,getMoxie(pal,moxieBonus)*ambulation,0);
				}
				drawAdventurer(pal,game,palImage);
			}
			else
			{
				moveAdventurer(pal,getMoxie(pal,moxieBonus)*(pal->image->x-pal->posToX<0?1:-1),0);
				drawAdventurer(pal,game,palImage);
			
				if(abs(pal->image->x-pal->posToX)<getMoxie(pal,moxieBonus))
				{
					pal->status = 11;
					pal->burdenedmoxie = getMoxie(pal,moxieBonus);
				}
			}
		}
	}
	else if(pal->type == 2)
	{
		if (pal->status == 1  && pal->image->x >= -pal->image->w*4)
		{
			if(pal->delay == 0)
			{
				ambulation =canAmbulate(ambulationList,pal->image->x, pal->image->y, pal->posToX,pal->posToY);
				if(ambulation !=0)
				{
					if(ambulation == 2)
					{
						moveAdventurer(pal,0,(pal->image->y<pal->posToY)?3:-3);
					}
					else
					{
						moveAdventurer(pal,getMoxie(pal,moxieBonus)*ambulation,0);
					}
					drawAdventurer(pal,game,palImage);
				}
				else
				{
					moveAdventurer(pal,getMoxie(pal,moxieBonus)*(pal->image->x-pal->posToX<0?1:-1),0);
					drawAdventurer(pal,game,palImage);
				}
				drawAdventurer(pal,game,palImage);
			}
			else
			{
				pal->delay--;
			}
		}
		else if (pal->status == 2)
		{
			if(pal->advancedMove !=0)
			{
				ambulation =canAmbulate(ambulationList,pal->image->x, pal->image->y, pal->image->x,640-30-pal->image->h);
				if ((abs(pal->image->y-640+30+pal->image->h)<3 || (abs(pal->image->y-pal->posToY)<3)) && canAmbulate(ambulationList,pal->image->x, pal->image->y, pal->posToX,pal->posToY)!=0)
				{
					pal->advancedMove = 0;
					drawAdventurer(pal,game,palImage);
				}
				else if (ambulation !=0)
				{
					if(ambulation == 2)
					{
						moveAdventurer(pal,0,(pal->image->y<640-30-pal->image->h)?3:-3);
					}
					else
					{
						moveAdventurer(pal,getMoxie(pal,moxieBonus)*ambulation,0);
					}
					drawAdventurer(pal,game,palImage);
				}
				else
				{
					moveAdventurer(pal,0,(pal->image->y<640-30-pal->image->h)?3:-3);
					drawAdventurer(pal,game,palImage);
				}
				
			}
			else
			{
				ambulation =canAmbulate(ambulationList,pal->image->x, pal->image->y, pal->posToX,pal->posToY);

				if(pal->item != NULL)
					pal->item = NULL;
				if(abs(pal->image->y-pal->posToY)<5 && abs(pal->image->x-pal->posToX)<22)
				{
					pal->status = 0;
					(*currOutPals)--;
				}
				else if(ambulation !=0)
				{
					if(ambulation == 2)
					{
						moveAdventurer(pal,0,(pal->image->y<pal->posToY)?3:-3);
					}
					else
					{
						moveAdventurer(pal,getMoxie(pal,moxieBonus)*ambulation,0);
					}
					drawAdventurer(pal,game,palImage);
				}
				else
				{
					moveAdventurer(pal,getMoxie(pal,moxieBonus)*(pal->image->x-pal->posToX<0?1:-1),0);
					drawAdventurer(pal,game,palImage);
				}
			}
		}
	}
}

House * newHouseByName(Game * game, int x, int y, char * name)
{
	House * house = malloc(sizeof(House));
	house->posX = x;
	house->posY = y;
	if(strcmp(name,"Forge")==0)
	{
		house->image = loadImage("images/buildings/forge.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
		house->type = 1;
	}
	else if(strcmp(name,"Shack")==0)
	{
		house->image = loadImage("images/buildings/shack.png",3,1,game);
		house->capacity = 50;
		house->size = 0;
		house->pals = malloc(sizeof(Adventurer)*house->capacity);
		house->type = 2;
	}
	else if(strcmp(name,"House")==0)
	{
		house->image = loadImage("images/buildings/house.png",3,1,game);
		house->capacity = 150;
		house->size = 0;
		house->pals = malloc(sizeof(Adventurer)*house->capacity);
		house->type = 3;
	}
	else if(strcmp(name,"Gumball_Machine")==0)
	{
		house->image = loadImage("images/buildings/gumball.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
		house->type = 4;
	}
	else if(strcmp(name,"Clothing_Station")==0)
	{
		house->image = loadImage("images/buildings/clothes.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
		house->type = 5;
	}
	else if(strcmp(name,"Farm")==0)
	{
		house->image = loadImage("images/buildings/farm.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
		house->type = 6;
	}
	else if(strcmp(name,"Oven")==0)
	{
		house->image = loadImage("images/buildings/oven.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
		house->type = 7;
	}
	else if(strcmp(name,"Upgrade")==0)
	{
		house->image = loadImage("images/buildings/upgrade.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
		house->type = 8;
	}
	else if(strcmp(name,"Mine")==0)
	{
		house->image = loadImage("images/buildings/mine.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
		house->type = 9;
	}
	else if(strcmp(name,"Attack_Tower")==0)
	{
		house->image = loadImage("images/buildings/attack.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
		house->type = 10;
	}
	else if(strcmp(name,"Game_Station")==0)
	{
		house->image = loadImage("images/buildings/game-station.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
		house->type = 11;
	}
	moveImageTo(house->image,x,y);
	house->status = 0;
	house->pending = 0;
	return house;
}

House * newHouse(Game * game, int x, int y, int type)
{
	House * house = malloc(sizeof(House));
	house->posX = x;
	house->posY = y;
	if(type == 1)
	{
		house->image = loadImage("images/buildings/forge.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
	}
	else if(type == 2)
	{
		house->image = loadImage("images/buildings/shack.png",3,1,game);
		house->capacity = 50;
		house->size = 0;
		house->pals = malloc(sizeof(Adventurer)*house->capacity);
	}
	else if(type == 3)
	{
		house->image = loadImage("images/buildings/house.png",3,1,game);
		house->capacity = 150;
		house->size = 0;
		house->pals = malloc(sizeof(Adventurer)*house->capacity);
	}
	else if(type == 4)
	{
		house->image = loadImage("images/buildings/gumball.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
	}
	else if(type == 5)
	{
		house->image = loadImage("images/buildings/clothes.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
	}
	else if(type == 6)
	{
		house->image = loadImage("images/buildings/farm.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
	}
	else if(type == 7)
	{
		house->image = loadImage("images/buildings/oven.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
	}
	else if(type == 8)
	{
		house->image = loadImage("images/buildings/upgrade.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
	}
	else if(type == 9)
	{
		house->image = loadImage("images/buildings/mine.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
	}
	else if(type == 10)
	{
		house->image = loadImage("images/buildings/attack.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
	}
	else if(type == 11)
	{
		house->image = loadImage("images/buildings/game-station.png",3,1,game);
		house->capacity = 0;
		house->size = 0;
		house->pals = NULL;
	}
	moveImageTo(house->image,x,y);
	house->type = type;
	house->status = 0;
	house->pending = 0;
	return house;
}

char * houseFromType(int type)
{
	if(type == 1)
	{
		return "Forge";
	}
	else if(type == 2)
	{
		
		return "Shack";
	}
	else if(type == 3)
	{
		return "House";
		
	}
	else if(type == 4)
	{
		return "Gumball_Machine";
	}
	else if(type == 5)
	{
		return "Clothing_Station";
	}
	else if(type == 6)
	{
		return "Farm";
	}
	else if(type == 7)
	{
		return "Oven";
	}
	else if(type == 8)
	{
		return "Upgrade";
	}
	else if(type == 9)
	{
		return "Mine";
		
	}
	else if(type == 10)
	{
		return "Attack_Tower";
	}
	else if(type == 11)
	{
		return "Game_Station";
	}
	return NULL;
}


void addPaltoHouse(House * house, Adventurer * pal, Game * game)
{
	if(house->size +1 > house->capacity)
		return;
	
	house->pals[house->size] = pal;
	house->size++;
	moveImageTo(pal->image,house->image->x + house->image->w/2 - pal->image->x/2, pal->image->y);
	moveImageTo(pal->image,pal->image->x,house->image->y+house->image->h-pal->image->h);
	pal->posToX = house->image->x;
	pal->posToY = house->image->y;
}



void freeItem(Item * item)
{
	freeImage(item->image);
	free(item->name);
	free(item);
}


void putInPile(Pile * pile,Item * item)
{
	PileItem* curr = pile->firstAlpha;
	if(item == NULL)
		return;
	while(strcmp(item->name,curr->item->name)!=0)
	{
		curr = curr->nextAlpha;
	}
	curr->size++;
	pile->total++;
}
void putMultInPile(Pile * pile,Item * item,int num)
{
	PileItem* curr = pile->firstAlpha;
	if(item == NULL)
		return;
	while(strcmp(item->name,curr->item->name)!=0)
	{
		curr = curr->nextAlpha;
	}
	curr->size+=num;
	pile->total+=num;
}





int drawStatsItem(Item * item,int x, int y,Game * game,Font * font,Image* tinyResources, int maxWidth)
{
	int levels[5]={item->plastic,item->fabric,item->metal,item->nature,item->technology};
	return drawStats(levels,x+10,y,game,font,tinyResources,maxWidth);
}
int getStatHeight(int * levels, Image * tinyResources)
{
	int retVal = 0;
	int j = 0;
	for(j = 0;j<5;j++)
	{
		if (levels[j]>0)
			retVal+=tinyResources->h;
	}
	return retVal;
}

int getStatWidth(int * levels,int x, int y,Game * game,Font * font,Image* tinyResources, int maxWidth)
{
	char buffer [80];
	int offx=0,j=0;
	int maxoffx=0;
	double ratio = 0;
	for(j = 0;j<5;j++)
	{
		if (levels[j]>ratio)
			ratio = levels[j];
	}
	ratio=maxWidth/ratio;
	
	if(levels[0]>0)
	{		
		offx=0;
		sprintf(buffer,"Plastics:%d",levels[0]);
		offx+=strlen(buffer)*font->image->w;
		
		
		offx+=tinyResources->w;
		offx+=tinyResources->w;
		for(j=50;j<levels[0]*ratio;j+=50)
		{
			offx+=tinyResources->w;
		}
		if(maxoffx<offx+font->image->w*(strlen("Plastics:")+1+log10(levels[0])))
			maxoffx=offx+font->image->w*(strlen("Plastics:")+1+log10(levels[0]));
		y+=18;
	}	
	if(levels[1]>0)
	{
		offx=0;
		sprintf(buffer,"Fabrics:%d",levels[1]);
		offx+=strlen(buffer)*font->image->w;
		
		
		offx+=tinyResources->w;
		offx+=tinyResources->w;
		for(j=50;j<levels[0]*ratio;j+=50)
		{
			offx+=tinyResources->w;
		}
		if(maxoffx<offx+font->image->w*(strlen("Fabrics:")+1+log10(levels[1])))
			maxoffx=offx+font->image->w*(strlen("Fabrics:")+1+log10(levels[1]));
		y+=18;
	}	
	if(levels[2]>0)
	{
		offx=0;
		sprintf(buffer,"Metals:%d",levels[2]);
		offx+=strlen(buffer)*font->image->w;
		
		
		offx+=tinyResources->w;
		offx+=tinyResources->w;
		for(j=50;j<levels[0]*ratio;j+=50)
		{
			offx+=tinyResources->w;
		}
		if(maxoffx<offx+font->image->w*(strlen("Metals:")+1+log10(levels[2])))
			maxoffx=offx+font->image->w*(strlen("Metals:")+1+log10(levels[2]));
		
		y+=18;
	}	
	if(levels[3]>0)
	{
		offx=0;
		sprintf(buffer,"Natures:%d",levels[3]);
		offx+=strlen(buffer)*font->image->w;
		
		
		offx+=tinyResources->w;
		offx+=tinyResources->w;
		for(j=50;j<levels[0]*ratio;j+=50)
		{
			offx+=tinyResources->w;
		}
		if(maxoffx<offx+font->image->w*(strlen("Natures:")+1+log10(levels[3])))
			maxoffx=offx+font->image->w*(strlen("Natures:")+1+log10(levels[3]));
		y+=18;
	}	
	if(levels[4]>0)
	{
		offx=0;
		sprintf(buffer,"Techs:%d",levels[4]);
		offx+=strlen(buffer)*font->image->w;
		
		
		offx+=tinyResources->w;
		offx+=tinyResources->w;
		for(j=50;j<levels[0]*ratio;j+=50)
		{
			offx+=tinyResources->w;
		}
		if(maxoffx<offx+font->image->w*(strlen("Techs:")+1+log10(levels[4])))
			maxoffx=offx+font->image->w*(strlen("Techs:")+1+log10(levels[4]));
		y+=18;
	}

	return maxoffx;
}
int drawStats(int * levels,int x, int y,Game * game,Font * font,Image* tinyResources, int maxWidth)
{
	char buffer [80];
	int offx=0,j=0;
	int maxoffx=0;
	double ratio = 0;
	for(j = 0;j<5;j++)
	{
		if (levels[j]>ratio)
			ratio = levels[j];
	}
	ratio=maxWidth/ratio;
	
	if(levels[0]>0)
	{		
		offx=0;
		sprintf(buffer,"Plastics:%d",levels[0]);
		offx+=writeWords(buffer,font,14,game,x+offx,y);
		
		setToFrame(tinyResources,0,0);
		moveImageTo(tinyResources,x+offx,y);
		offx+=tinyResources->w;
		j=0;
		drawImage(tinyResources,game);
		moveImage(tinyResources,tinyResources->w,0);
		offx+=tinyResources->w;
		setToFrame(tinyResources,1,0);
		for(j=50;j<levels[0]*ratio;j+=50)
		{
			drawImage(tinyResources,game);
			moveImage(tinyResources,tinyResources->w,0);
			offx+=tinyResources->w;
		}
		setToFrame(tinyResources,2,0);
		drawImage(tinyResources,game);
		if(maxoffx<offx+font->image->w*(strlen("Plastics:")+1+log10(levels[0])))
			maxoffx=offx+font->image->w*(strlen("Plastics:")+1+log10(levels[0]));
		y+=18;
	}	
	if(levels[1]>0)
	{
		offx=0;
		sprintf(buffer,"Fabrics:%d",levels[1]);
		offx+=writeWords(buffer,font,14,game,x+offx,y);
		setToFrame(tinyResources,0,1);
		moveImageTo(tinyResources,x+offx,y);
		offx+=tinyResources->w;
		j=0;
		drawImage(tinyResources,game);
		moveImage(tinyResources,tinyResources->w,0);
		offx+=tinyResources->w;
		setToFrame(tinyResources,1,1);
		for(j=50;j<levels[1]*ratio;j+=50)
		{
			drawImage(tinyResources,game);
			moveImage(tinyResources,tinyResources->w,0);
			offx+=tinyResources->w;
		}
		setToFrame(tinyResources,2,1);
		drawImage(tinyResources,game);
		
		if(maxoffx<offx+font->image->w*(strlen("Fabrics:")+1+log10(levels[1])))
			maxoffx=offx+font->image->w*(strlen("Fabrics:")+1+log10(levels[1]));
		y+=18;
	}	
	if(levels[2]>0)
	{
		if(maxoffx<offx+20*font->image->w)
			maxoffx=offx+20*font->image->w;
		offx=0;
		sprintf(buffer,"Metals:%d",levels[2]);
		offx+=writeWords(buffer,font,14,game,x+offx,y);
		
		setToFrame(tinyResources,0,2);
		moveImageTo(tinyResources,x+offx,y);
		offx+=tinyResources->w;
		j=0;
		drawImage(tinyResources,game);
		moveImage(tinyResources,tinyResources->w,0);
		offx+=tinyResources->w;
		setToFrame(tinyResources,1,2);
		for(j=50;j<levels[2]*ratio;j+=50)
		{
			drawImage(tinyResources,game);
			moveImage(tinyResources,tinyResources->w,0);
			offx+=tinyResources->w;
		}
		setToFrame(tinyResources,2,2);
		drawImage(tinyResources,game);
		
		if(maxoffx<offx+font->image->w*(strlen("Metals:")+1+log10(levels[2])))
			maxoffx=offx+font->image->w*(strlen("Metals:")+1+log10(levels[2]));
		
		y+=18;
	}	
	if(levels[3]>0)
	{
		offx=0;
		sprintf(buffer,"Natures:%d",levels[3]);
		offx+=writeWords(buffer,font,14,game,x+offx,y);
		
		setToFrame(tinyResources,0,3);
		moveImageTo(tinyResources,x+offx,y);
		offx+=tinyResources->w;
		j=0;
		drawImage(tinyResources,game);
		moveImage(tinyResources,tinyResources->w,0);
		offx+=tinyResources->w;
		setToFrame(tinyResources,1,3);
		for(j=50;j<levels[3]*ratio;j+=50)
		{
			drawImage(tinyResources,game);
			moveImage(tinyResources,tinyResources->w,0);
			offx+=tinyResources->w;
		}
		setToFrame(tinyResources,2,3);
		drawImage(tinyResources,game);
		if(maxoffx<offx+font->image->w*(strlen("Natures:")+1+log10(levels[3])))
			maxoffx=offx+font->image->w*(strlen("Natures:")+1+log10(levels[3]));
		y+=18;
	}	
	if(levels[4]>0)
	{
		offx=0;
		sprintf(buffer,"Techs:%d",levels[4]);
		offx+=writeWords(buffer,font,14,game,x+offx,y);
		
		setToFrame(tinyResources,0,4);
		moveImageTo(tinyResources,x+offx,y);
		offx+=tinyResources->w;
		j=0;
		drawImage(tinyResources,game);
		moveImage(tinyResources,tinyResources->w,0);
		offx+=tinyResources->w;
		setToFrame(tinyResources,1,4);
		for(j=50;j<levels[4]*ratio;j+=50)
		{
			drawImage(tinyResources,game);
			moveImage(tinyResources,tinyResources->w,0);
			offx+=tinyResources->w;
		}
		setToFrame(tinyResources,2,4);
		drawImage(tinyResources,game);
		y+=18;
		if(maxoffx<offx+font->image->w*(strlen("Techs:")+1+log10(levels[4])))
			maxoffx=offx+font->image->w*(strlen("Techs:")+1+log10(levels[4]));
	}

	return maxoffx;
}

PileItem * drawPileDetail(Pile * pile, Game * game, Font * font, int * offsety, int * canScroll, int forge, Image* tinyResources, int * scrollMax, int* clickable, int * selection,Image *selectionMenu,Image * ordering)
{
	PileItem * curr = NULL, *retval = NULL;
	int  i = 0,scrollhold, tempX,doubletemp;
	SDL_Rect rect;
	rect.x = 30;
	rect.y = *offsety;
	*canScroll=0;
	if(*offsety<0)
		*canScroll=1;
	rect.w = 0;
	rect.h = 0;
	
	if((*selection&4)==4)
		curr= pile->firstAlpha;
	else
		curr= pile->firstSize;
	while(curr!=NULL)
	{
		if(curr->size>0)
		{
			tempX= printNumber(rect.x-24,rect.y,game,font,curr->size) -18;
			writeWords(curr->item->name,font,12,game,rect.x+tempX, rect.y);
			rect.y+=18;
			rect.w = curr->item->image->w;
			rect.h = curr->item->image->h;
			rect.x+=4*(curr->size);
			scrollhold = rect.y;
			for(i = 0;i<curr->size && rect.x>=30;i++)
			{
				doubletemp = rect.x;
				if(rect.x>130)
				{
					rect.x=130;
				}
				SDL_RenderCopy(game->renderer,curr->item->image->image,&curr->item->image->srcPos,&rect);
				rect.x = doubletemp;
				rect.x-=4;
				rect.y = scrollhold;
			}

			rect.w=130;

			if(isClicked(&rect,game,game->currLayer))
			{
				if (forge)
					drawStatsItem(curr->item,rect.x+rect.w+10,rect.y+5,game,font,tinyResources, game->boundingBox.w*3/4);
				if(game->chain->mouse == 1 )
				{
					game->chain->mouse = 2;
					retval = curr;
				}
				*clickable = 1;
			}
			
			
			rect.x=30;
			rect.y += curr->item->image->h+11;
			
		}
	
		rect.x = 30;
		//selector for type
		{
			if((*selection&4)==4)
				curr= curr->nextAlpha;
			else
				curr= curr->nextSize;
		}
	}
	*scrollMax = *offsety-rect.y +game->boundingBox.h-20;
	if(rect.y > *offsety + game->boundingBox.h-20)
	{
		*canScroll += 2;
	}
	
	for(i=0;i<2;i++)
	{
		moveImageTo(ordering,game->boundingBox.w/2-7-2*ordering->w+i*ordering->w,game->boundingBox.h-ordering->h);
		setToFrame(ordering,i,((*selection&4)==(i*4)));
		drawImage(ordering,game);
		if(isClicked(&ordering->destPos,game,game->currLayer))
		{
			if(game->chain->mouse == 1 )
			{
				game->chain->mouse = 2;
				*offsety = 0;
				*selection = (*selection & ~4)+(i)*4;
			}
			*clickable = 1;
		}
	}
	for(i=0;i<3;i++)
	{
		moveImageTo(selectionMenu,game->boundingBox.w/2+7+i*selectionMenu->w,game->boundingBox.h-selectionMenu->h);
		setToFrame(selectionMenu,i,((*selection&3)==(i+1)));
		drawImage(selectionMenu,game);
		if(isClicked(&selectionMenu->destPos,game,game->currLayer))
		{
			if(game->chain->mouse == 1 )
			{
				game->chain->mouse = 2;
				*selection = (*selection & ~3)+i+1;
			}
			*clickable = 1;
		}
	}
	
	
	return retval;
}

PileItem * drawPileDetailSingle(Pile * pile, Game * game, Font * font, int * offsety, int * canScroll, int forge, Image* tinyResources, int * scrollMax, int* clickable, int * selection,Image *selectionMenu,Image * ordering, char* name)
{
	PileItem * curr = NULL, *retval = NULL;
	int  i = 0,scrollhold, tempX,doubletemp;
	SDL_Rect rect;
	rect.x = 30;
	rect.y = *offsety;
	*canScroll=0;
	if(*offsety<0)
		*canScroll=1;
	rect.w = 0;
	rect.h = 0;

	curr= pile->firstAlpha;

	while(curr!=NULL)
	{
		if(curr->size>0 && strcmp(name,curr->item->name)==0)
		{
			tempX= printNumber(rect.x-24,rect.y,game,font,curr->size) -18;
			writeWords(curr->item->name,font,12,game,rect.x+tempX, rect.y);
			rect.y+=18;
			rect.w = curr->item->image->w;
			rect.h = curr->item->image->h;
			rect.x+=4*(curr->size);
			scrollhold = rect.y;
			for(i = 0;i<curr->size && rect.x>=30;i++)
			{
				doubletemp = rect.x;
				if(rect.x>130)
				{
					rect.x=130;
				}
				SDL_RenderCopy(game->renderer,curr->item->image->image,&curr->item->image->srcPos,&rect);
				rect.x = doubletemp;
				rect.x-=4;
				rect.y = scrollhold;
			}

			rect.w=130;

			if(isClicked(&rect,game,game->currLayer))
			{
				if (forge)
					drawStatsItem(curr->item,rect.x+rect.w+10,rect.y+5,game,font,tinyResources, game->boundingBox.w*3/4);
				if(game->chain->mouse == 1 )
				{
					game->chain->mouse = 2;
					retval = curr;
				}
				*clickable = 1;
			}
			
			
			rect.x=30;
			rect.y += curr->item->image->h+11;
			
		}
	
		rect.x = 30;
		//selector for type
		{
			curr= curr->nextAlpha;
	
		}
	}
	
	for(i=0;i<3;i++)
	{
		moveImageTo(selectionMenu,game->boundingBox.w/2+7+i*selectionMenu->w,game->boundingBox.h-selectionMenu->h);
		setToFrame(selectionMenu,i,((*selection&3)==(i+1)));
		drawImage(selectionMenu,game);
		if(isClicked(&selectionMenu->destPos,game,game->currLayer))
		{
			if(game->chain->mouse == 1 )
			{
				game->chain->mouse = 2;
				*selection = (*selection & ~3)+i+1;
			}
			*clickable = 1;
		}
	}
	
	*scrollMax = *offsety-rect.y +game->boundingBox.h-20;
	if(rect.y > *offsety + game->boundingBox.h-20)
	{
		*canScroll += 2;
	}
	
	
	return retval;
}

int printNumber(int x, int y, Game * game, Font * font, int num)
{
	char words[20];
	sprintf(words,"x%d",num);
	return writeWords(words,font,12,game,x,y);	
}

void deleteItemsRecipe(Recipe * recipe, Pile * pile)
{
	PileItem * item = NULL;
	int i;
	for(i =0; i< recipe->size;i++)
	{
		item = pile->firstAlpha;

		while(item!=NULL && strcmp(recipe->nameList[i],item->item->name)!=0)
		{
			item = item->nextAlpha;
		}
		if(item!=NULL)
		{
			item->size -= recipe->numList[i];
			pile->total-= recipe->numList[i];
		}
	}
	
}


void deletePileItem(PileItem * item)
{
	item->size--;
	
}




PileItem * drawPileDetailSub(Pile * pile, Game * game, Font * font, int * offsety, int * canScroll, int forge, Image * tinyResources,char plastic,char fabric,char metal,char nature, char tech,int * scrollMax, int* clickable, int * selection,Image *selectionMenu,Image * ordering)
{
	PileItem * curr = NULL, *retval = NULL;
	int  i = 0,scrollhold, tempX,doubletemp;
	SDL_Rect rect;
	rect.x = 30;
	rect.y = *offsety;
	*canScroll=0;
	if(*offsety<0)
		*canScroll=1;
	rect.w = 0;
	rect.h = 0;
	
	if((*selection&4)==4)
		curr= pile->firstAlpha;
	else
		curr= pile->firstSize;
	while(curr!=NULL)
	{
		if(curr->size>0 && (curr->item->plastic>0)==plastic && (curr->item->fabric>0)==fabric && (curr->item->metal>0)==metal && (curr->item->nature>0)==nature && (curr->item->technology>0)==tech)
		{
			tempX= printNumber(rect.x-24,rect.y,game,font,curr->size) -18;
			writeWords(curr->item->name,font,12,game,rect.x+tempX, rect.y);
			rect.y+=18;
			rect.w = curr->item->image->w;
			rect.h = curr->item->image->h;
			rect.x+=4*(curr->size);
			scrollhold = rect.y;
			for(i = 0;i<curr->size;i++)
			{
				doubletemp = rect.x;
				while(rect.x>130)
				{
					rect.y+=6;
					rect.x-=104;
				}
				SDL_RenderCopy(game->renderer,curr->item->image->image,&curr->item->image->srcPos,&rect);
				rect.x = doubletemp;
				rect.x-=4;
				rect.y = scrollhold;
			}
			if(4*(curr->size)>130)
			{
				rect.w+=130-rect.x;
				rect.h+=6*(4*curr->size/130+1);
			}
			else 
			{
				rect.w+=4*(curr->size);
			}
			if(isClicked(&rect,game,game->currLayer))
			{
				if (forge)
					drawStatsItem(curr->item,rect.x+rect.w+10,rect.y+5,game,font,tinyResources, game->boundingBox.w*3/4);
				if(game->chain->mouse == 1 )
				{
					game->chain->mouse = 2;
					retval = curr;
				}
				*clickable = 1;
			}
			
			rect.w-=4*curr->size;
			rect.x+=4*curr->size;
			
			rect.y += curr->item->image->h+5;
			
		}
	
		rect.x = 30;
		//selector for type
		{
			if((*selection&4)==4)
				curr= curr->nextAlpha;
			else
				curr= curr->nextSize;
		}
	}
	*scrollMax = *offsety-rect.y +game->boundingBox.h-20;
	if(rect.y > *offsety + game->boundingBox.h-20)
	{
		*canScroll += 2;
	}
	
	for(i=0;i<2;i++)
	{
		moveImageTo(ordering,game->boundingBox.w/2-7-2*ordering->w+i*ordering->w,game->boundingBox.h-ordering->h);
		setToFrame(ordering,i,((*selection&4)==(i*4)));
		drawImage(ordering,game);
		if(isClicked(&ordering->destPos,game,game->currLayer))
		{
			if(game->chain->mouse == 1 )
			{
				game->chain->mouse = 2;
				*offsety = 0;
				*selection = (*selection & ~4)+(i)*4;
			}
			*clickable = 1;
		}
	}
	for(i=0;i<3;i++)
	{
		moveImageTo(selectionMenu,game->boundingBox.w/2+7+i*selectionMenu->w,game->boundingBox.h-selectionMenu->h);
		setToFrame(selectionMenu,i,((*selection&3)==(i+1)));
		drawImage(selectionMenu,game);
		if(isClicked(&selectionMenu->destPos,game,game->currLayer))
		{
			if(game->chain->mouse == 1 )
			{
				game->chain->mouse = 2;
				*selection = (*selection & ~3)+i+1;
			}
			*clickable = 1;
		}
	}
	
	
	return retval;
}

void drawBackgroundTile(Game * game, Image * tile)
{
	int i = 0, j = 0;
	SDL_Rect temprect;
	temprect.x = 0;
	temprect.y = 0;
	temprect.w = tile->w;
	temprect.h = tile->h;
	for(i=0;i<game->boundingBox.w;i+= tile->w)
	{
		for(j=0;j<game->boundingBox.h;j+= tile->h)
		{
			SDL_RenderCopy(game->renderer,tile->image,&tile->srcPos,&temprect);
			temprect.y+=tile->h;
		}
		temprect.x+=tile->w;
		temprect.y=0;
	}	
}

void drawFeedBackground(Game * game, Image * bkgrnd, Image * flumf, Image * menuleft, int anim,int delay)
{
	int i = 0, j = 0;
	SDL_Rect temprect;
	
		temprect.x = 0;
		temprect.y = 0;
		temprect.w = bkgrnd->w;
		temprect.h = bkgrnd->h;
		for(i=0;i<game->boundingBox.w;i+= bkgrnd->w)
		{
			for(j=0;j<game->boundingBox.h;j+= bkgrnd->h)
			{
				SDL_RenderCopy(game->renderer,bkgrnd->image,&bkgrnd->srcPos,&temprect);
				temprect.y+=bkgrnd->h;
			}
			temprect.x+=bkgrnd->w;
			temprect.y=0;
		}

		if(anim > 0 && delay==0)
		{
			animateRange(flumf,1,6);
		}
		else
		{
			setToFrame(flumf,0,0);
		}
		moveImageTo(flumf,game->boundingBox.w - flumf->w,game->boundingBox.h - flumf->h);
		moveImageTo(menuleft,0,0);
		SDL_RenderCopy(game->renderer,flumf->image,&flumf->srcPos,&flumf->destPos);
		SDL_RenderCopy(game->renderer,menuleft->image,&menuleft->srcPos,&menuleft->destPos);
	
}


int dirtyTwoExponent(int power)
{
	return 1 << power;
}

int dirtyLogTwo(int value)
{
	int retVal = 0;
	long one = 1;
	while(value > one << retVal)
		retVal ++;
	return retVal;
}

int generateRandomLogVal(int maxVal)
{
	return maxVal - dirtyLogTwo((rand()%dirtyTwoExponent(maxVal-1)));
}

int getWonder(Adventurer * pal)
{
	int retval = 0;
	retval+=pal->wonder;

	
	return retval;
}

int getMoxie(Adventurer * pal, int moxieBonus)
{
	int retval = 0;
	retval+=pal->moxie;

	retval+=moxieBonus;
	return retval;
}

int getChutzpah(Adventurer * pal)
{
	int retval = 0;
	retval+=pal->chutzpah;
	
	return retval;
}

ProductionQueue * initProdQueue(void)
{
	ProductionQueue * queue = malloc(sizeof(ProductionQueue));
	queue->size =0;
	queue->queue = NULL;
	return queue;
}

void addHouseToQueue(ProductionQueue * queue, House * house,Recipe * recipe, Pile * pile, ObjectList * list)
{
	PileItem * item = NULL;
	int i,j;
	PQueue * tempQueue = malloc(sizeof(PQueue));

	
	tempQueue->type = 1;
	tempQueue->returnnum=1;
	tempQueue->items = NULL;
	tempQueue->itemstatus = NULL;
	tempQueue->queued = house;
	tempQueue->itemsize = 0;

	for(i =0; i< recipe->size;i++)
	{
		item = pile->firstAlpha;
		
		while(item!= NULL && strcmp(recipe->nameList[i],item->item->name)!=0 )
		{
			item = item->nextAlpha;
		}
		for(j=0;j<recipe->numList[i];j++)
		{
			tempQueue->itemsize++;
			tempQueue->items = realloc(tempQueue->items,sizeof(Item*)*(tempQueue->itemsize));
			tempQueue->itemstatus = realloc(tempQueue->itemstatus,sizeof(int)*(tempQueue->itemsize));
			tempQueue->items[tempQueue->itemsize-1] = copyItem(item->item);
			tempQueue->itemstatus[tempQueue->itemsize-1] = 0;
		}
	}
	

	queue->size++;
	queue->queue = realloc(queue->queue,sizeof(PQueue*)*queue->size);
	queue->queue[queue->size-1] = tempQueue;
	

}

void removeFromQueue(ProductionQueue * queue,int i)
{
	int j;
	for(j = i;j<queue->size-1;j++)
	{
		queue->queue[j] = queue->queue[j+1];
	}
	queue->size--;
	queue->queue = realloc(queue->queue,sizeof(PQueue *)*queue->size);
}

void removeItemFromQueue(PQueue * queue,int i)
{
	int j;
	for(j = i;j<queue->itemsize-1;j++)
	{
		queue->items[j] = queue->items[j+1];
		queue->itemstatus[j] = queue->itemstatus[j+1];
	}
	queue->itemsize--;
}

int removeNextItemQueue(ProductionQueue * queue,Item * item)
{
	int i,j;
	for(i = 0;i<queue->size;i++)
	{
		for(j = 0;j<queue->queue[i]->itemsize;j++)
		{
			if(queue->queue[i]->items[j] == item)
			{
				removeItemFromQueue(queue->queue[i],j);
				return i;
			}
		}
	}
	return -1;
}

House ** checkQueueHouse(ProductionQueue * queue, House ** houseList, int * houseNum)
{
	int i = 0;
	for(i = 0;i<queue->size;i++)
	{
		if(queue->queue[i]->type==1 && queue->queue[i]->itemsize == 0)
		{
			(*houseNum)++;
			
			houseList = realloc(houseList,sizeof(House*)*(*houseNum));
			houseList[(*houseNum)-1] = queue->queue[i]->queued;
		}
	}
	return houseList;
}

void itemSave(Pile * pile, FILE * savefile)
{
	PileItem * trav= pile->firstAlpha;
	while(trav!=NULL)
	{

		fprintf(savefile,"item %s %d\n",trav->item->name,trav->size);

		trav = trav->nextAlpha;
	}
}

char usefulstrcmp(char* input, char * match)
{
	int i,end = strlen(match);
	for(i=0;i<end;i++)
	{
		if(input[i]!=match[i])
			return 0;
	}
	return 1;
}


Item * dupeItem(Item * item, ObjectList * list)
{
	return getItemByName(item->name,list);
	/*
	Item * itemRet = malloc(sizeof(Item));
	itemRet->image = imageCopy(item->image);
	itemRet->name = malloc(sizeof(char)*(strlen(item->name)+1));
	strcpy(itemRet->name,item->name);
	itemRet->mass = item->mass;
	itemRet->value = item->value;
	itemRet->fabric = item->fabric;
	itemRet->metal = item->metal;
	itemRet->nature = item->nature;
	itemRet->plastic = item->plastic;
	itemRet->technology = item->technology;
	return itemRet;*/
}

int maximum(int v1, int v2)
{
	if(v1>v2)
		return v1;
	return v2;
}

void drawLevels(int* levels, Image * levelBar, Game * game, Image * background, Image* bottomInfo, Font * font)
{
	int drawLevels[5];
	char buffer[80];
	int i,j;
	int max = maximum(maximum(maximum(levels[0],levels[1]),maximum(levels[2],levels[3])),levels[4]);
	int step = 0;
	SDL_Rect temprect;
	temprect.x = 0;
	temprect.y = 0;
	temprect.w = background->w;
	temprect.h = background->h;
	for(i=0;i<game->boundingBox.w;i+= background->w)
	{
		SDL_RenderCopy(game->renderer,background->image,&background->srcPos,&temprect);
		temprect.x+=background->w;
	}
	if(game->boundingBox.w-265>bottomInfo->w*5)
	{
		step=(game->boundingBox.w-265)/5-bottomInfo->w;
	}
	for(i=0;i<5;i++)
	{	
		if(max==0)
			drawLevels[i] = 0;
		else
			drawLevels[i] = levels[i]*6/max;
		setToFrame(levelBar,i,1);
		setToFrame(bottomInfo,i,0);
		moveImageTo(bottomInfo,265+(levelBar->w+step)*i,game->boundingBox.h-46-bottomInfo->h);
		moveImageTo(levelBar,265+(levelBar->w+step)*i,game->boundingBox.h-46-bottomInfo->h-levelBar->h);
		for(j=0;j<drawLevels[i];j++)
		{
			drawImage(levelBar,game);
			moveImage(levelBar,0,-levelBar->h);
		}
		setToFrame(levelBar,i,0);
		drawImage(bottomInfo,game);
		drawImage(levelBar,game);
		sprintf(buffer,"%d",levels[i]);
		writeWords(buffer,font,13,game,levelBar->x+6,levelBar->y+26);
	}
}

void drawStatusBar(int amount, int total, int width, int height, int x, int y,char dir,Game * game)
{
	SDL_Rect bigBar;
	bigBar.x=x;
	bigBar.y=y;
	bigBar.w=width;
	bigBar.h=height;
	SDL_SetRenderDrawColor(game->renderer,0,0,0,255);
	SDL_RenderDrawRect(game->renderer,&bigBar);
	bigBar.x+=1;
	bigBar.w-=2;
	bigBar.y+=1;
	bigBar.h-=2;
	SDL_SetRenderDrawColor(game->renderer,84,84,84,255);
	SDL_RenderFillRect(game->renderer,&bigBar);
	SDL_SetRenderDrawColor(game->renderer,148,148,148,255);
	if(amount>0 && total > 0)
	{
		if(dir=='h')
		{
			bigBar.w =(width*amount)/total;
		}
		else
		{
			bigBar.h = (height*amount)/total -2;
			bigBar.y+= height- bigBar.h-2;
		}
		SDL_RenderFillRect(game->renderer,&bigBar);
	}
	else
	{
		fprintf(stderr,"Error dividing %d/%d\n",amount,total);
	}
	SDL_SetRenderDrawColor(game->renderer,0,0,0,0);
}

int canPlace(House ** houseList, int houseNum,House * movingHouse, int mapHeight)
{
	int i = 0,place = 0;
	if (movingHouse->image->y < 0)
	{
		return 0;
	}
	for(i=0;i<houseNum;i++)
	{
		if(houseList[i]!=movingHouse)
		{
			if(houseList[i]->image->y - 89 == movingHouse->image->y)
			{
				if(movingHouse->image->x>houseList[i]->image->x-houseList[i]->image->w/2 && movingHouse->image->x<houseList[i]->image->x+houseList[i]->image->w/2)
				{
					place = 1;
				}
			}
			else if(houseList[i]->image->y == movingHouse->image->y)
			{
				if(movingHouse->image->x>houseList[i]->image->x-houseList[i]->image->w/2 && movingHouse->image->x<houseList[i]->image->x+houseList[i]->image->w/2)
				{
					return 0;
				}
			}
		}
	}
	if(movingHouse->image->y==mapHeight-119)
	{
		return 1;
	}
	return place;
}

int canRemove(House ** houseList, int houseNum,House * movingHouse)
{
	int i = 0,place = 0;
	
	for(i=0;i<houseNum;i++)
	{
		if(houseList[i]->image->y + 89 == movingHouse->image->y)
		{
			if(movingHouse->image->x>houseList[i]->image->x-houseList[i]->image->w/2 && movingHouse->image->x<houseList[i]->image->x+houseList[i]->image->w/2)
			{
				return 0;
			}
		}
		else if(houseList[i]->image->y == movingHouse->image->y)
		{
			if(movingHouse->image->x>houseList[i]->image->x-houseList[i]->image->w/2 && movingHouse->image->x<houseList[i]->image->x+houseList[i]->image->w/2)
			{
				place = 1;
			}
		}
	}
	return place;
}

void enemyDo(Enemy * foe, Game * game, ObjectList * list, int x, int y, char * redraw, int* eggs,AmbulationTowerList * ambulationList)
{
	int ambulation;
	if(foe->type ==1)
	{
		if (foe->status == 1)
		{
			if(foe->delay == 0)
			{
				moveEnemy(foe,foe->speed,0);
				drawEnemy(foe,game);
			}
			else
			{
				foe->delay--;
			}
		}
		else if (foe->status == 2)
		{
			moveEnemy(foe,-foe->speed,0);
			drawEnemy(foe,game);
		}
	}
	else if(foe->type ==2)
	{
		ambulation =canAmbulate(ambulationList,foe->image->x, foe->image->y, foe->posToX,foe->posToY);
		if(ambulation !=0)
		{
			if(ambulation == 2)
			{
				moveEnemy(foe,0,(foe->image->y<foe->posToY)?3:-3);
			}
			else
			{
				moveEnemy(foe,foe->speed*ambulation,0);
			}
			
			
		}
		drawEnemy(foe,game);
	}
}
void drawEnemy(Enemy * foe, Game * game)
{
	if(foe!=NULL)
	{
		drawImage(foe->image,game);
		if(foe->item !=NULL )
		{
			drawImage(foe->item->image,game);
		}
	}
}
void moveEnemy(Enemy * foe, int x, int y)
{
	if(foe->dir==-1 && x>0)
	{
		setToFrame(foe->image,0,0);

		foe->dir = 1;
	}
	else if(foe->dir==1 && x<0)
	{
		setToFrame(foe->image,0,1);
		
		foe->dir = -1;
	}

	if(foe->dir == 1)
	{
		animateRange(foe->image,0,7);
	}
	else
	{
		animateRange(foe->image,8,15);
	}
	moveImage(foe->image,x,y);
	
	if(foe->item!=NULL)
	{
		moveImage(foe->item->image,x,y);
	}
}
Enemy * newEnemy(int type, Game * game)
{
	Enemy * foe = NULL;
	foe = malloc(sizeof(Enemy));
	foe->dir = 1;
	foe->class = 0;
	foe->timer = 0;
	foe->delay = 0;
	foe->negpos = 0;
	foe->posTo = 0;
	if(type == 1)
	{
		foe->speed = 2;
		foe->health = (rand()%7)*10 + 30;
		foe->image = loadImage("images/enemies/1.png",8,2,game);	
		foe->status = 0;
	}
	else if(type == 2)
	{
		foe->speed = 1;
		foe->health = (rand()%7)*10 + 100;
		foe->image = loadImage("images/enemies/2.png",8,3,game);	
		foe->status = 0;
	}
	
	foe->type = type;
	foe->item = NULL;
	return foe;
}
void clearEnemy(Enemy* foe)
{
	freeImage(foe->image);
	if(foe->item!=NULL)
	{
		freeItem(foe->item);
	}
	free(foe);
}
Item * takeItemFromPile(Pile * pile)
{
	if (pile->total == 0) 
		return NULL;
	int randval = rand()%pile->total,i =0;
	Item* retval;

	PileItem * temp = pile->firstAlpha;
	for(i=0;i<randval;i+=temp->size)
	{
		
		temp = temp->nextAlpha;
	}
	if(temp->size<=0)
		return NULL;
	temp->size--;
	pile->total--;
	retval = copyItem(temp->item);
	return retval;
}

Item * copyItem(Item * item)
{
	Item * retval = malloc(sizeof(Item));
	{
		retval->image = imageCopy(item->image);
		retval->name = malloc(sizeof(char)*(strlen(item->name)+1));
		strcpy(retval->name,item->name);
		retval->mass = item->mass;
		retval->value = item->value;
		retval->fabric = item->fabric;
		retval->metal = item->metal;
		retval->nature = item->nature;
		retval->plastic = item->plastic;
		retval->technology = item->technology;
	}
	return retval;
}


UpgradeList* getUpgradeList(char * fileLocation, Game * game)
{
	UpgradeList* upgradeList;
	char buffer[1024] = {0};
	int i = 0, j =0;
	int listSize=0;
	char * temp;
	FILE * file=fopen(fileLocation,"r");
	fgets(buffer,1024,file);
	while(!feof(file))
	{
		fgets(buffer,1024,file);
		listSize++;
	}
	rewind(file);
	upgradeList = malloc(sizeof(UpgradeList));
	upgradeList->list = malloc(sizeof(Upgrade*)*listSize);
	upgradeList->size = listSize;
	fgets(buffer,1024,file);
	while(!feof(file) && i<listSize)
	{
		upgradeList->list[i] = malloc(sizeof(Upgrade));	
		temp = strtok(buffer,",");
		upgradeList->list[i]->name = malloc(sizeof(char)*(strlen(temp)+1));
		
		strcpy(upgradeList->list[i]->name,temp);
		temp = strtok(NULL,",");
		upgradeList->list[i]->desc = malloc(sizeof(char)*(strlen(temp)+1));
		strcpy(upgradeList->list[i]->desc,temp);
		upgradeList->list[i]->tier = atoi(strtok(NULL,","));
		upgradeList->list[i]->nameList = NULL;
		upgradeList->list[i]->numList = NULL;
		upgradeList->list[i]->image = NULL;
		upgradeList->list[i]->levels[0] = 0;
		upgradeList->list[i]->levels[1] = 0;
		upgradeList->list[i]->levels[2] = 0;
		upgradeList->list[i]->levels[3] = 0;
		upgradeList->list[i]->levels[4] = 0;
		temp = strtok(NULL,",");

		j = 0;
		while(temp != NULL)
		{
			if(strcmp("%levels",temp)==0)
			{
				upgradeList->list[i]->levels[0]=atoi(strtok(NULL,","));
				upgradeList->list[i]->levels[1]=atoi(strtok(NULL,","));
				upgradeList->list[i]->levels[2]=atoi(strtok(NULL,","));
				upgradeList->list[i]->levels[3]=atoi(strtok(NULL,","));
				upgradeList->list[i]->levels[4]=atoi(strtok(NULL,","));
			}
			else if(strcmp("%image",temp)==0)
			{
				temp = strtok(NULL,",");
				upgradeList->list[i]->image = loadImage(temp,1,1,game);
			}
			else
			{
				j++;
				upgradeList->list[i]->nameList = realloc(upgradeList->list[i]->nameList,sizeof(char*)*j);
				upgradeList->list[i]->nameList[j-1] = malloc(sizeof(char)*(strlen(temp)+2));
				strcpy(upgradeList->list[i]->nameList[j-1],temp);
				upgradeList->list[i]->numList = realloc(upgradeList->list[i]->numList,sizeof(int)*j);
				upgradeList->list[i]->numList[j-1] = atoi(strtok(NULL,","));
			}
			temp = strtok(NULL,",");
		}
		if(upgradeList->list[i]->tier == 1)
		{
			upgradeList->list[i]->unlocked = 1;
		}
		else
		{
			upgradeList->list[i]->unlocked = 0;
		}
		upgradeList->list[i]->size = j;
		i++;
		fgets(buffer,1024,file);
	}
	fclose(file);
	return upgradeList;
}
void unlockUpgradeTier(int tier, UpgradeList * list)
{
	int i;
	for(i=0;i<list->size;i++)
	{
		if(list->list[i]->tier == tier)
		{
			list->list[i]->unlocked = 1;
		}
	}
}
void deleteItemsUpgrade(Upgrade * upgrade, Pile * pile, ObjectList * list)
{
	PileItem * item = NULL;
	int i;
	for(i =0; i< upgrade->size;i++)
	{
		item = pile->firstAlpha;

		while(item!=NULL && strcmp(upgrade->nameList[i],item->item->name)!=0)
		{
			item = item->nextAlpha;
		}
		if(item!=NULL)
		{
			item->size -= upgrade->numList[i];
			pile->total -= upgrade->numList[i];
		}
	}
}

int canUpgradeCraft(Upgrade * upgrade, Pile * pile, ObjectList * list, int*levels)
{
	PileItem * item = NULL;

	int i,j;
	for(i =0; i< upgrade->size;i++)
	{
		item = pile->firstAlpha;
		
		while(item!= NULL && strcmp(upgrade->nameList[i],item->item->name)!=0 )
		{
			item = item->nextAlpha;
		}
		if(item->size < upgrade->numList[i])
		{
			return 0;
		}		
	}
	for(j=0;j<5;j++)
	{
		if(levels[j]<upgrade->levels[j])
			return 0;
	}
	return 1;
}

int drawUpgrades(UpgradeList * upgradeList,Game * game,Font * font,Image * upgradeBackground,Image * upgradeBlack,ObjectList * items, Pile * pile, int * scrolly, int * canScroll, Image * tinyResources, int * levels, int * scrollmax, int* clickable)
{
	int i = 0, j = 0, k =0,width,height, doubletemp,retval = 0, maxwidth;
	int active = 0, upgradeJump = 0;
	char numText[16] = "x0";
	SDL_Rect tempRect,bgRect;
	Image * tempImg;
	
	*canScroll = 0;
	if(*scrolly < 0)
		*canScroll+=1;
	tempRect.x = 20;
	tempRect.y = *scrolly;
	bgRect.w = upgradeBackground->w;
	bgRect.h = upgradeBackground->h;
	for(i=0;i<upgradeList->size;i++)
	{
		if(upgradeList->list[i]->unlocked==1)
		{
			active++;
			upgradeJump = 0;
			bgRect.w = upgradeBackground->w;
			bgRect.h = upgradeBackground->h;
			width=190;
			maxwidth=190;
			height = 180;
			for(j=0;j<upgradeList->list[i]->size;j++)
			{
				tempImg = getImageByName(upgradeList->list[i]->nameList[j],items);
				
				if(width+tempImg->w>game->boundingBox.w*3/4)
				{
					height+=100;
					if(width > maxwidth)
						maxwidth = width;
					width = 40;
				}
				width +=tempImg->w+20;
			}
			if(width > maxwidth)
						maxwidth = width;
			
			if((upgradeList->list[i]->levels[0]>0 || upgradeList->list[i]->levels[1]>0 || upgradeList->list[i]->levels[2]>0 || upgradeList->list[i]->levels[3]>0 || upgradeList->list[i]->levels[4]>0))
			{
				if(width + getStatWidth(upgradeList->list[i]->levels,100000,1000000,game,font,tinyResources,350) > game->boundingBox.w*3/4)
				{
					height+=getStatHeight(upgradeList->list[i]->levels,tinyResources)+40;
					upgradeJump=1;
					width = 40+getStatWidth(upgradeList->list[i]->levels,tempRect.x,tempRect.y,game,font,tinyResources,350);
					if(width > maxwidth)
						maxwidth = width;
				}
				else 
				{
					width+= getStatWidth(upgradeList->list[i]->levels,tempRect.x,tempRect.y,game,font,tinyResources,350);
					if(width > maxwidth)
						maxwidth = width;
				}
			}
			bgRect.y = tempRect.y;
			bgRect.x = 0;
			for(j=0;j<maxwidth;j+=upgradeBackground->w)
			{
				if(j==0)
					setToFrame(upgradeBackground,0,0);
				else 
					setToFrame(upgradeBackground,1+(j/upgradeBackground->w)%6,0);
				SDL_RenderCopy(game->renderer,upgradeBackground->image,&upgradeBackground->srcPos,&bgRect);
				
				doubletemp = bgRect.y;
				bgRect.y+=upgradeBackground->h;
				for(k=upgradeBackground->h;k<height;k+=upgradeBackground->h)
				{
					if(j==0)
					{
						setToFrame(upgradeBackground,0,1+(k/upgradeBackground->h)%4);
					}
					else
					{
						setToFrame(upgradeBackground,1+(j/upgradeBackground->w)%6,1+(k/upgradeBackground->h)%4);
					}
					SDL_RenderCopy(game->renderer,upgradeBackground->image,&upgradeBackground->srcPos,&bgRect);
					bgRect.y+=upgradeBackground->h;
				}
				if(j==0)
					setToFrame(upgradeBackground,0,5);
				else 
					setToFrame(upgradeBackground,1+(j/upgradeBackground->w)%6,5);
				SDL_RenderCopy(game->renderer,upgradeBackground->image,&upgradeBackground->srcPos,&bgRect);	
				bgRect.x+=upgradeBackground->w;
				bgRect.y = doubletemp;
				
			}
			setToFrame(upgradeBackground,7,0);
			SDL_RenderCopy(game->renderer,upgradeBackground->image,&upgradeBackground->srcPos,&bgRect);
			doubletemp = bgRect.y;
			bgRect.y+=upgradeBackground->h;
			for(k=upgradeBackground->h;k<height;k+=upgradeBackground->h)
			{
				setToFrame(upgradeBackground,7,1+(k/upgradeBackground->h)%4);
				SDL_RenderCopy(game->renderer,upgradeBackground->image,&upgradeBackground->srcPos,&bgRect);
				bgRect.y+=upgradeBackground->h;
			}
			setToFrame(upgradeBackground,7,5);
			SDL_RenderCopy(game->renderer,upgradeBackground->image,&upgradeBackground->srcPos,&bgRect);
			bgRect.y = doubletemp;
			
			doubletemp = tempRect.y;
			
			writeWords(upgradeList->list[i]->name,font,12,game,tempRect.x,tempRect.y + 20);
			writeWordsMulti(upgradeList->list[i]->desc,font,12,game,tempRect.x,tempRect.y + 45,180);
			if(upgradeList->list[i]->image!=NULL)
			{
				moveImageTo(upgradeList->list[i]->image,tempRect.x,tempRect.y+50+13*(strlen(upgradeList->list[i]->desc)/(18)));
				drawImage(upgradeList->list[i]->image,game);
			}
			tempRect.y += 130 - tempRect.h;
			tempRect.y = doubletemp;
			tempRect.x += 190;
			for(j=0;j<upgradeList->list[i]->size;j++)
			{
				tempImg = getImageByName(upgradeList->list[i]->nameList[j],items);
				tempRect.w = tempImg->w;
				tempRect.h = tempImg->h;
				
				
				if(tempRect.x + tempImg->w>game->boundingBox.w*3/4)
				{
					tempRect.y+=tempImg->h+20;
					tempRect.x = 40;
				}
				
				tempRect.y += 130 - tempRect.h;
				SDL_RenderCopy(game->renderer,tempImg->image,&tempImg->srcPos,&tempRect);
				
				tempRect.y -= 130 - tempRect.h;
	
				numText[1] = '0'+ upgradeList->list[i]->numList[j];
				
				writeWords(numText,font,12,game,tempRect.x+tempImg->w,tempRect.y +130);
				

				tempRect.x +=20+ tempImg->w;
				
			}			
			if(upgradeJump)
			{
			
				tempRect.y+=140;
				tempRect.x = 20;
			}
			else if((upgradeList->list[i]->levels[0]>0 || upgradeList->list[i]->levels[1]>0 || upgradeList->list[i]->levels[2]>0 || upgradeList->list[i]->levels[3]>0 || upgradeList->list[i]->levels[4]>0))
			{
				tempRect.y+=110-getStatHeight(upgradeList->list[i]->levels,tinyResources);
			
			}
			drawStats(upgradeList->list[i]->levels,tempRect.x+10,tempRect.y+25,game,font,tinyResources, 350);
			if(upgradeJump)
			{
				tempRect.y-=140;
			}
			else if((upgradeList->list[i]->levels[0]>0 || upgradeList->list[i]->levels[1]>0 || upgradeList->list[i]->levels[2]>0 || upgradeList->list[i]->levels[3]>0 || upgradeList->list[i]->levels[4]>0))
			{
				tempRect.y-=110-getStatHeight(upgradeList->list[i]->levels,tinyResources);
			}
			tempRect.x = 20;
			
			if(!canUpgradeCraft(upgradeList->list[i],pile,items,levels))
			{
				bgRect.x = 0;
				for(j=0;j<maxwidth;j+=upgradeBlack->w)
				{
					if(j==0)
						setToFrame(upgradeBlack,0,0);
					else 
						setToFrame(upgradeBlack,1+(j/upgradeBackground->w)%6,0);
					SDL_RenderCopy(game->renderer,upgradeBlack->image,&upgradeBlack->srcPos,&bgRect);
					
					doubletemp = bgRect.y;
					bgRect.y+=upgradeBlack->h;
					for(k=upgradeBlack->h;k<height;k+=upgradeBlack->h)
					{
						if(j==0)
						{
							setToFrame(upgradeBlack,0,1+(k/upgradeBlack->h)%4);
						}
						else
						{
							setToFrame(upgradeBlack,1+(j/upgradeBlack->w)%6,1+(k/upgradeBlack->h)%4);
						}
						SDL_RenderCopy(game->renderer,upgradeBlack->image,&upgradeBlack->srcPos,&bgRect);
						bgRect.y+=upgradeBlack->h;
					}
					if(j==0)
						setToFrame(upgradeBlack,0,5);
					else 
						setToFrame(upgradeBlack,1+(j/upgradeBlack->w)%6,5);
					SDL_RenderCopy(game->renderer,upgradeBlack->image,&upgradeBlack->srcPos,&bgRect);	
					bgRect.x+=upgradeBlack->w;
					bgRect.y = doubletemp;
					
				}
				setToFrame(upgradeBlack,7,0);
				SDL_RenderCopy(game->renderer,upgradeBlack->image,&upgradeBlack->srcPos,&bgRect);
				doubletemp = bgRect.y;
				bgRect.y+=upgradeBlack->h;
				for(k=upgradeBlack->h;k<height;k+=upgradeBlack->h)
				{
					setToFrame(upgradeBlack,7,1+(k/upgradeBlack->h)%4);
					SDL_RenderCopy(game->renderer,upgradeBlack->image,&upgradeBlack->srcPos,&bgRect);
					bgRect.y+=upgradeBlack->h;
				}
				setToFrame(upgradeBlack,7,5);
				SDL_RenderCopy(game->renderer,upgradeBlack->image,&upgradeBlack->srcPos,&bgRect);
			}
			else
			{
				bgRect.y = tempRect.y;
				bgRect.w = (maxwidth/50+1)*50;
				bgRect.x =0;
				bgRect.h = (height/50+1)*50;
				if(isClicked(&bgRect,game,game->currLayer))
				{
					if(game->chain->mouse == 1)
					{
						retval = i+1;
						game->chain->mouse = 2;
					}
					*clickable = 1;
					
				}
			}
			tempRect.y+=height+60;
		}

	}
	if(tempRect.y > game->boundingBox.h)
	{
		*canScroll += 2;
	}
	*scrollmax = *scrolly - tempRect.y+ game->boundingBox.h;
	return retval;
}



int writeWordsMulti(char * words, Font * font, int size, Game * game, int x, int y, int wmax)
{
	SDL_Rect temprect = {x,y,font->image->w*((double)size/(double)font->image->h),size};
	char * phrase = malloc(sizeof(char)*(strlen(words)+1));
	char * temp = phrase;
	strcpy(temp,words);
	
	temp = strtok(temp," -");
	
	while (temp!=NULL)
	{
		if(temprect.x+ temprect.w*strlen(temp)>x+wmax)
		{
			temprect.x = x;
			temprect.y += temprect.h+3;
		}
		temprect.x += writeWords(temp,font,size,game,temprect.x,temprect.y);
		
		temprect.x += temprect.w;
		temp = strtok(NULL," -");
	}
	
	free(phrase);
	return temprect.x-x;
}
int upgradesAvailable(UpgradeList * upgradeList)
{
	int i = 0;
	for(i=0;i<upgradeList->size;i++)
	{
		if( upgradeList->list[i]->unlocked == 1)
			return 1;
	}
	return 0;
}

void insertAllItems(ObjectList * items, Pile * list)
{
	int i,j;
	PileItem* temp;
	for(i=0;i<10;i++)
	{
		for(j=0;j<items->size[i];j++)
		{
			
			temp = malloc(sizeof(PileItem));
			temp->item = getItem(items->list[i][j]);
			
			temp->size = 0;
			temp->volume = temp->item->image->w*temp->item->image->h;
			temp->nextAlpha = NULL;
			temp->nextSize = NULL;
			temp->nextMass = NULL;
			list->firstAlpha = recurseInsertAlpha(temp,list->firstAlpha);
			list->firstSize = recurseInsertSize(temp,list->firstSize);
			list->firstMass = recurseInsertMass(temp,list->firstMass);
			list->size++;
		}
	}
}
PileItem * recurseInsertAlpha(PileItem * object,PileItem * curr)
{
	if(curr == NULL)
	{
		return object;
	}
	else if(strcmp(curr->item->name,object->item->name)>0)
	{
		if(object->nextAlpha == NULL)
		{
			object->nextAlpha = curr;
			return object;
		}
		else
		{
			object->nextAlpha = recurseInsertAlpha(object->nextAlpha,curr);
			return object;
		}
	}
	else
	{
		curr->nextAlpha = recurseInsertAlpha(object,curr->nextAlpha);
		return curr;
	}
}
PileItem * recurseInsertMass(PileItem * object,PileItem * curr)
{
	if(curr == NULL)
	{
		return object;
	}
	else if(curr->item->mass<object->item->mass)
	{
		if(object->nextMass == NULL)
		{
			object->nextMass = curr;
			return object;
		}
		else
		{
			object->nextMass = recurseInsertMass(object->nextMass,curr);
			return object;
		}
	}
	else
	{
		curr->nextMass = recurseInsertMass(object,curr->nextMass);
		return curr;
	}
}
PileItem * recurseInsertSize(PileItem * object,PileItem * curr)
{
	if(curr == NULL)
	{
		return object;
	}
	else if(curr->volume<object->volume)
	{
		if(object->nextSize == NULL)
		{
			object->nextSize = curr;
			return object;
		}
		else
		{
			object->nextSize = recurseInsertSize(object->nextSize,curr);
			return object;
		}
	}
	else
	{
		curr->nextSize = recurseInsertSize(object,curr->nextSize);
		return curr;
	}
}

void sortList(ObjectList * objectList)
{
	int changed = 0, i = 0, size = objectList->size[0];
	int j = 0;
	Object * temp;
	for(j=0;j< objectList->tiersize;j++)
	{
		size = objectList->size[j];
		do
		{
			changed = 0;
			for(i=0;i<size-1;i++)
			{
				if (objectList->list[j][i]->mass>objectList->list[j][i+1]->mass)
				{
					temp = objectList->list[j][i];
					objectList->list[j][i] = objectList->list[j][i+1];
					objectList->list[j][i+1] = temp;
					changed = 1;
				}
			}
			size--;
		}
		while(changed);
	}
}


void unlockHouse(RecipeList * recipeList,char * name, char unlock)
{
	int i;
	for(i=0;i<recipeList->size;i++)
	{
		if(strcmp(recipeList->list[i]->name,name)==0)
		{
			recipeList->list[i]->unlocked = unlock;
		}
	}
}

looseItemList * initLooseList()
{
	looseItemList * list = malloc(sizeof(looseItemList));
	list->size = 0;
	list->capacity = 10;
	list->list = malloc(sizeof(looseItem)*list->capacity);
	return list;
}
void addItemToLooseList(looseItemList * list, Item * item, int type,int errata)
{
	if(list->size+1 >= list->capacity)
	{
		list->capacity *=2;
		list->list = realloc(list->list,sizeof(looseItem)*list->capacity);
	}
	list->list[list->size].item = item;
	list->list[list->size].type = type;
	list->list[list->size].errata = errata;
	list->size++;
}
void removeItemFromLooseList(looseItemList * list, Item * item)
{
	int i = 0;
	char grab = 0;
	for(i=0;i<list->size;i++)
	{
		if(grab == 1)
		{
			list->list[i-1].item = list->list[i].item;
			list->list[i-1].type = list->list[i].type;
			list->list[i-1].errata = list->list[i].errata;
		}
		else if (list->list[i].item == item)
		{
			grab = 1;
		}
	}
	list->size--;
}

void removeQueuedLooseItems(looseItemList * list,int id)
{
	int i = 0;
	char grab = 1;
	while (grab)
	{
		grab = 0;
		for(i=0;i<list->size;i++)
		{
			if(grab == 1)
			{
				list->list[i-1].item = list->list[i].item;
				list->list[i-1].type = list->list[i].type;
				list->list[i-1].errata = list->list[i].errata;
			}
			else if (list->list[i].type == 1 && list->list[i].errata == id)
			{
				grab = 1;
			}
		}
		if(grab)
			list->size--;
	}
	for(i=0;i<list->size;i++)
	{
		if(list->list[i].type == 1)
		{
			list->list[i].errata--;
		}
	}
}
Item * grabItemFromLoose(looseItemList * list, int pos)
{
	Item * retval = NULL;
	int i;
	for(i=0;i<list->size;i++)
	{
		if(list->list[i].type == 3 && pos == list->list[i].item->image->x)
		{
			retval = list->list[i].item;
			removeItemFromLooseList(list,retval);
			break;
		}
	}
	return retval;
}
int looseItemGrabbable(looseItemList * list)
{
	int i;
	for(i=0;i<list->size;i++)
	{
		if(list->list[i].type == 2)
		{
			return i;
		}
	}
	return -1;
}
int canAmbulate(AmbulationTowerList * list,int x, int y, int xTo, int yTo)
{
	int ambulation = 0;

	ambulation = canTraverseToLevel(list,(640-yTo-30)/(640/7),(640-y-30)/(640/7),xTo,x,0,3072);
	
	if((640-y-30)/(640/7)!=(640-yTo-30)/(640/7))
	{
		return ambulation;
	}
	else
	{
		if(abs(y-yTo)<4)
		{
			if (ambulation == 2)
				return 0;
			else
				return ambulation;
		}
		else return 2;
	}
}
AmbulationTowerList * createAmbulationList(House ** houseList, int houseNum)
{
	AmbulationTowerList * list = malloc(sizeof(AmbulationTowerList));
	list->tiers = malloc(sizeof(AmbulationPlatform*)*7);
	int i;
	int mapHeight = 640;
	for(i=0;i<7;i++)
	{
		list->tiers[i]=NULL;
	}
	list->tiers[0]= malloc(sizeof(AmbulationPlatform));
	list->tiers[0]->min=0;
	list->tiers[0]->max=3072;
	list->tiers[0]->next= NULL;
	for(i=0;i<houseNum;i++)
	{
		insertAmbulationPlatform(list,(640-houseList[i]->posY-30)/(mapHeight/7),houseList[i]->posX,houseList[i]->posX+houseList[i]->image->w);
		insertAmbulationPlatform(list,(640-houseList[i]->posY-30+houseList[i]->image->h)/(mapHeight/7),houseList[i]->posX,houseList[i]->posX+houseList[i]->image->w);
	}
	
	return list;
}
void insertAmbulationPlatform(AmbulationTowerList * list, int level, int min, int max)
{
	list->tiers[level] = recurseInsertAmbulationPlatform(list->tiers[level],level, min,max);
	postAmbulationInsertionRecalibration(list,level);
}
AmbulationPlatform * recurseInsertAmbulationPlatform(AmbulationPlatform * curr, int level, int min, int max)
{
	AmbulationPlatform * temp;
	if(curr == NULL)
	{
		temp= malloc(sizeof(AmbulationPlatform));
		temp->next = NULL;
		temp->min = min;
		temp->max = max;
		return temp;
	}
	else if(curr->min>max)
	{
		temp =  malloc(sizeof(AmbulationPlatform));
		temp->next = curr;
		temp->min = min;
		temp->max = max;
		return temp;
	}
	else if(curr->max>= min)
	{
		if(min<curr->min)
		{
			curr->min = min;
		}
		if(max> curr->max)
			curr->max = max;
		return curr;
	}
	else
	{
		curr->next = recurseInsertAmbulationPlatform(curr->next,level, min,max);
	}
	return curr;
}
void postAmbulationInsertionRecalibration(AmbulationTowerList * list, int level)
{
	AmbulationPlatform *curr,* temp;
	curr = list->tiers[level];
	while (curr != NULL && curr->next != NULL)
	{
		if(curr->max>curr->next->min)
		{
			temp = curr->next;
			curr->max = curr->next->max;
			curr->next = temp->next;
			free(temp);
		}
		curr = curr->next;
	}
}

/*void drawAmbulationPlatforms(AmbulationTowerList * list,Game * game)
{
	int i;
	AmbulationPlatform * curr;
	for(i=0;i<7;i++)
	{
		SDL_SetRenderDrawColor(game->renderer, 255, 0, 255, 128);
		curr = list->tiers[i];
		while(curr != NULL)
		{
			SDL_RenderDrawLine(game->renderer,curr->min,640-i*89-30,curr->max,640-i*89-30);
			curr = curr->next;
		}
	}
	SDL_SetRenderDrawColor(game->renderer,0,0,0,0);
}*/
int canTraverseToLevel(AmbulationTowerList * list, int levelTo, int currLevel, int xTo, int x, int min, int max)
{
	AmbulationPlatform * curr;
	if(levelTo <0 || levelTo > 6)
		return 0;
	if(currLevel <0 || currLevel > 6)
		return 0;
	curr = list->tiers[currLevel];
	while(curr != NULL)
	{
		
		if(levelTo == currLevel)
		{
			if(curr->max>min && curr->min <= max)
			{
				if (xTo<curr->min)
				{
					return 0;
				}
				else if (xTo>= curr->min && xTo < curr->max)
				{
					if(abs(x-xTo)<20)
						return 2;
					else if(x<xTo)
						return 1;
					else
						return -1;
				}
			}
			else if(curr->min > max)
				return 0;
		}
		else if(x>=curr->min && x<=curr->max)
		{
			if(levelTo<currLevel)
			{
				int temp = canTraverseToLevel(list,levelTo,currLevel-1,xTo,x,curr->min,curr->max);
				if(temp!=0)
				{
					
					if(abs(x-(curr->min+(curr->max-curr->min)/2))<20)
						return 2;
					else if(x<(curr->min+(curr->max-curr->min)/2))
						return 1;
					else
						return -1;
				}
			}
			else
			{
				int temp = canTraverseToLevel(list,levelTo,currLevel+1,xTo,x,curr->min,curr->max);
				if(temp!=0)
				{
					if(currLevel == 0)
						return temp;
					if(abs(x-(curr->min+(curr->max-curr->min)/2))<20)
						return 2;
					else if(x<(curr->min+(curr->max-curr->min)/2))
						return 1;
					else
						return -1;
				}
			}
		}
		curr = curr->next;
	}
	return 0;
}
Effect * newEffect(Effect * list,Image * image,int posX, int posY, int length)
{
	Effect * temp,*retval;
	
	temp = malloc(sizeof(Effect));
	temp->image = imageCopy(image);
	moveImageTo(temp->image,posX,posY+image->h);
	temp->timer = length;
	temp->microtimer = 5;
	temp->next = NULL;
	retval = list;
	if(list == NULL)
	{
		return temp;
	}
	else
	{
		temp->next = retval;
		return temp;
	}
}
Effect * processEffects(Effect * first,Game * game)
{
	Effect * temp;
	temp = first;
	while(temp!=NULL)
	{
		drawImage(temp->image,game);
		
		if(temp->microtimer==0)
		{
			animateSpeed(temp->image,1);
			temp->microtimer=5;
			temp->timer--;
		}
		temp->microtimer--;
		temp=temp->next;
	}
	return clearTimeredEffects(first);
}
Effect * clearTimeredEffects(Effect * effect)
{
	while(effect!=NULL && effect->timer == 0)
	{
		effect = effect->next;
	}
	if(effect!=NULL)
	{
		effect->next = clearTimeredEffects(effect->next);
		return effect;
	}
	else 
		return NULL;
}

FlyingItem * newFlyingItem(FlyingItem * list,Image * image,int startX, int startY,int endX, int endY, int delay, int dir)
{
	FlyingItem * temp,*retval;
	
	temp = malloc(sizeof(FlyingItem));
	temp->image = imageCopy(image);
	moveImageTo(temp->image,startX,startY);
	temp->next = NULL;
	temp->startX = startX;
	temp->startY = startY;
	temp->endX = endX;
	temp->delay = delay;
	temp->endY = endY;
	temp->dir = dir;
	moveImageTo(temp->image,startX,startY);
	retval = list;
	if(list == NULL)
	{
		return temp;
	}
	else
	{
		temp->next = retval;
		return temp;
	}
}
FlyingItem * processFlyingItems(FlyingItem * first,Game * game)
{
	FlyingItem * temp;
	temp = first;
	
	while(temp!=NULL)
	{
		if(temp->delay<=0)
		{
			
			rotateImage(temp->image,6);
			if(temp->dir == 0)
			{
				moveImage(temp->image,7,0);
				
				moveImageTo(temp->image,temp->image->x,temp->startY+(temp->endY-temp->startY)*((temp->image->x-temp->startX*1.0)/(temp->endX-temp->startX)*1.0));
			}
			else if(temp->dir == 1)
			{
				moveImage(temp->image,0,-4);
				
				moveImageTo(temp->image,temp->startX+(temp->endX-temp->startX)*((temp->image->y-temp->startY*1.0)/(temp->endY-temp->startY)*1.0),temp->image->y);
			}
			
			drawImage(temp->image,game);
		}
		else
		{
			temp->delay--;
		}
		temp=temp->next;
	}
	return clearFlyingItems(first);
}
FlyingItem * clearFlyingItems(FlyingItem * item)
{
	while(item!=NULL && abs(item->image->x-item->endX)<7 && item->dir == 0)
	{
		item = item->next;
	}
	while(item!=NULL && abs(item->image->y-item->endY)<7 && item->dir == 1)
	{
		item = item->next;
	}
	if(item!=NULL)
	{
		item->next = clearFlyingItems(item->next);
		return item;
	}
	else 
		return NULL;
}
miniCookieGame * initMiniCookieGame(int x, int y, Game * game)
{
	miniCookieGame * retval = malloc(sizeof(miniCookieGame));
	int i = 0, j = 0,r = 0,rVal;
	retval->x = x;
	retval->y = y;
	retval->w = 276;
	retval->h = 216;
	retval->dir = 0;
	retval->posX = -1;
	retval->posY = -1;
	retval->timer = 1000;
	retval->timerMax = 1000;
	retval->score = 0;
	retval->gameStatus= 0;
	retval->colRem = -1;
	retval->rowRem = -1;
	retval->winSound = Mix_LoadWAV("sounds/minigameWin.wav");
	retval->loseSound = Mix_LoadWAV("sounds/minigameLose.wav");
	retval->moveSound = Mix_LoadWAV("sounds/minigameMove.wav");
	retval->eatSound = Mix_LoadWAV("sounds/minigameEat.wav");
	retval->background = loadImage("images/mini-game/background.png",1,1,game);
	retval->foreground = loadImage("images/mini-game/foreground.png",1,1,game);
	retval->fuse = loadImage("images/mini-game/fuse.png",2,2,game);
	retval->scoreBoard = loadImage("images/mini-game/score.png",2,2,game);
	retval->cookies = loadImage("images/mini-game/cookies.png",5,2,game);
	retval->start =loadImage("images/mini-game/start.png",1,1,game);
	retval->win = loadImage("images/mini-game/win.png",1,1,game);
	retval->lose = loadImage("images/mini-game/lose.png",1,1,game);
	retval->menu = loadImage("images/mini-game/menu.png",1,1,game);
	retval->board = malloc(sizeof(char*)*5);
	for(i=0;i<5;i++)
	{
		retval->board[i]=calloc(sizeof(char),5);
		for(j=0;j<5;j++)
		{
			r=5;

			if(i>0)
			{
				if(j>0 && retval->board[i-1][j] != retval->board[i][j-1])
				{
					r-=2;
				}
				else 
					r--;
			}
			else if(j>0)
				r--;
			rVal = rand()%r;
			
			while((i>0 && rVal == retval->board[i-1][j]) || (j>0 && rVal==retval->board[i][j-1]))
			{
				rVal++;
			}
			retval->board[i][j] = rVal;
		}
	}
	return retval;
}
void processCookieGame(miniCookieGame * cookieGame,Game * game, int * clickable, int * grabable)
{
	int i = 0, j = 0, sigma = 2,tempx,tempy,add = 0;
	moveImageTo(cookieGame->cookies,72+cookieGame->x,69+cookieGame->y);
	moveImageTo(cookieGame->menu,cookieGame->x,cookieGame->y);
	moveImageTo(cookieGame->win,cookieGame->x,cookieGame->y);
	moveImageTo(cookieGame->lose,cookieGame->x,cookieGame->y);
	moveImageTo(cookieGame->background,cookieGame->x,cookieGame->y);
	moveImageTo(cookieGame->foreground,cookieGame->x,cookieGame->y);
	drawImage(cookieGame->background,game);
	if(cookieGame->gameStatus == 0)
	{
		drawImage(cookieGame->menu,game);
		moveImageTo(cookieGame->start,cookieGame->menu->x+155,cookieGame->menu->y+140);
		drawImage(cookieGame->start,game);
		if(isClicked(&cookieGame->start->destPos,game,game->currLayer))
		{
			*clickable = 1;
			if(game->chain->mouse == 1)
			{
				game->chain->mouse = 2;
				cookieGame->gameStatus = 1;
			}
		}
		
	}
	else if (cookieGame->gameStatus == 1)
	{
		if(game->chain->mouse == 3)
			*grabable = 1;
		for(i=0;i<5;i++)
		{
			if( cookieGame->board[i][0] >= 0 &&
				cookieGame->board[i][0] == cookieGame->board[i][1] &&
				cookieGame->board[i][2] == cookieGame->board[i][1] &&
				cookieGame->board[i][2] == cookieGame->board[i][3] &&
				cookieGame->board[i][4] == cookieGame->board[i][3])
			{
				cookieGame->colRem = i;
			}
			else if( cookieGame->board[0][i] >= 0 &&
				cookieGame->board[0][i] == cookieGame->board[1][i] &&
				cookieGame->board[2][i] == cookieGame->board[1][i] &&
				cookieGame->board[2][i] == cookieGame->board[3][i] &&
				cookieGame->board[4][i] == cookieGame->board[3][i])
			{
				cookieGame->rowRem = i;
				
			}
			if(cookieGame->dir == 0 && cookieGame->colRem==i)
			{
				cookieGame->colRem = -1;
				Mix_PlayChannel(1,cookieGame->eatSound, 0);
				cookieGame->board[i][0] = -1;
				cookieGame->board[i][1] = -1;
				cookieGame->board[i][2] = -1;
				cookieGame->board[i][3] = -1;
				cookieGame->board[i][4] = -1;
				cookieGame->score++;
				cookieGame->timer = cookieGame->timerMax;
				add = 1;
				
				
			}
			else if(cookieGame->dir == 0 && cookieGame->rowRem==i)
			{
				cookieGame->rowRem = -1;
				Mix_PlayChannel(1,cookieGame->eatSound, 0);
				cookieGame->board[0][i] = -1;
				cookieGame->board[1][i] = -1;
				cookieGame->board[2][i] = -1;
				cookieGame->board[3][i] = -1;
				cookieGame->board[4][i] = -1;
				cookieGame->score++;
				cookieGame->timer = cookieGame->timerMax;
				add = 1;
			}
		
		}
		if(add)
		{
			refillBoard(cookieGame);
		}
		if(game->chain->mouse == 3 && cookieGame->posX>=0 && cookieGame->posY>=0)
		{
			
			if(cookieGame->dir == 1 && (abs(game->chain->vx)>sigma || abs(game->chain->vy)>sigma))
			{
				if (abs(game->chain->vx-cookieGame->clickX)>abs(game->chain->vy-cookieGame->clickY))
				{
					cookieGame->dir = 2;//Horizontal
				}
				else 
				{
					cookieGame->dir = 3;//Vertical				
				}
			}
			if(cookieGame->dir == 2)
			{
				if (game->chain->vx-cookieGame->clickX > (cookieGame->cookies->w+3)/2)
				{
					Mix_PlayChannel(1,cookieGame->moveSound, 0);
					cookieGame->rowRem = -1;
					cookieGame->colRem = -1;
					
					cookieGame->posY = (cookieGame->posY-1);
					if(cookieGame->posY<0)
					{
						cookieGame->posY = 4;
					}
					cookieGame->clickX +=cookieGame->cookies->w+3;
					tempx = cookieGame->board[cookieGame->posX][4];
					cookieGame->board[cookieGame->posX][4] = cookieGame->board[cookieGame->posX][3];
					cookieGame->board[cookieGame->posX][3] = cookieGame->board[cookieGame->posX][2];
					cookieGame->board[cookieGame->posX][2] = cookieGame->board[cookieGame->posX][1];
					cookieGame->board[cookieGame->posX][1] = cookieGame->board[cookieGame->posX][0];
					cookieGame->board[cookieGame->posX][0] = tempx;
				}
				else if (game->chain->vx-cookieGame->clickX < -(cookieGame->cookies->w+3)/2)
				{
					Mix_PlayChannel(1,cookieGame->moveSound, 0);
					cookieGame->rowRem = -1;
					cookieGame->colRem = -1;
					cookieGame->posY = (cookieGame->posY+1);
					if(cookieGame->posY>4)
					{
						cookieGame->posY = 0;
					}
					cookieGame->clickX -=cookieGame->cookies->w+3;
					tempx = cookieGame->board[cookieGame->posX][0];
					cookieGame->board[cookieGame->posX][0] = cookieGame->board[cookieGame->posX][1];
					cookieGame->board[cookieGame->posX][1] = cookieGame->board[cookieGame->posX][2];
					cookieGame->board[cookieGame->posX][2] = cookieGame->board[cookieGame->posX][3];
					cookieGame->board[cookieGame->posX][3] = cookieGame->board[cookieGame->posX][4];
					cookieGame->board[cookieGame->posX][4] = tempx;
				}
			}
			else if(cookieGame->dir == 3)
			{
				if (game->chain->vy-cookieGame->clickY > (cookieGame->cookies->h+3)/2)
				{
					Mix_PlayChannel(1,cookieGame->moveSound, 0);
					cookieGame->rowRem = -1;
					cookieGame->colRem = -1;
					cookieGame->posX = (cookieGame->posX-1);
					if(cookieGame->posX<0)
					{
						cookieGame->posX = 4;
					}
					cookieGame->clickY +=cookieGame->cookies->h+3;
					tempx = cookieGame->board[4][cookieGame->posY];
					cookieGame->board[4][cookieGame->posY] = cookieGame->board[3][cookieGame->posY];
					cookieGame->board[3][cookieGame->posY] = cookieGame->board[2][cookieGame->posY];
					cookieGame->board[2][cookieGame->posY] = cookieGame->board[1][cookieGame->posY];
					cookieGame->board[1][cookieGame->posY] = cookieGame->board[0][cookieGame->posY];
					cookieGame->board[0][cookieGame->posY] = tempx;
				}
				else if (game->chain->vy-cookieGame->clickY < -(cookieGame->cookies->w+3)/2)
				{
					Mix_PlayChannel(1,cookieGame->moveSound, 0);
					cookieGame->rowRem = -1;
					cookieGame->colRem = -1;
					cookieGame->posX = (cookieGame->posX+1);
					if(cookieGame->posX>4)
					{
						cookieGame->posX = 0;
					}
					cookieGame->clickY -=cookieGame->cookies->h+3;
					tempx = cookieGame->board[0][cookieGame->posY];
					cookieGame->board[0][cookieGame->posY] = cookieGame->board[1][cookieGame->posY];
					cookieGame->board[1][cookieGame->posY] = cookieGame->board[2][cookieGame->posY];
					cookieGame->board[2][cookieGame->posY] = cookieGame->board[3][cookieGame->posY];
					cookieGame->board[3][cookieGame->posY] = cookieGame->board[4][cookieGame->posY];
					cookieGame->board[4][cookieGame->posY] = tempx;
				}
			}
			for(i=0;i<5;i++)
			{
				if(cookieGame->dir == 2 && cookieGame->posX==i)
						moveImage(cookieGame->cookies,game->chain->vx-cookieGame->clickX,0);
				for(j=0;j<5;j++)
				{
					if(cookieGame->dir == 3 && cookieGame->posY==j)
						moveImage(cookieGame->cookies,0,game->chain->vy-cookieGame->clickY);
					
					setToFrame(cookieGame->cookies,cookieGame->board[i][j],(cookieGame->colRem == i || cookieGame->rowRem == j));
					if(cookieGame->dir == 2 && cookieGame->cookies->x<72+cookieGame->x)
					{
						tempx = cookieGame->cookies->x;
						while (cookieGame->cookies->x<72+cookieGame->x)
							moveImage(cookieGame->cookies,(cookieGame->cookies->w+3)*5,0);
						drawImage(cookieGame->cookies,game);
						if(cookieGame->cookies->x>72+cookieGame->x+(cookieGame->cookies->w+3)*4)
						{
							moveImage(cookieGame->cookies,-(cookieGame->cookies->w+3)*5,0);
							drawImage(cookieGame->cookies,game);
						}
						moveImageTo(cookieGame->cookies,tempx,cookieGame->cookies->y);
					}
					else if(cookieGame->dir == 3 && cookieGame->cookies->y<69+cookieGame->y)
					{
						tempy = cookieGame->cookies->y;
						while (cookieGame->cookies->y<69+cookieGame->y)
							moveImage(cookieGame->cookies,0,(cookieGame->cookies->h+3)*5);
						drawImage(cookieGame->cookies,game);
						if(cookieGame->cookies->y>69+cookieGame->y+(cookieGame->cookies->h+3)*4)
						{
							moveImage(cookieGame->cookies,0,-(cookieGame->cookies->h+3)*5);
							drawImage(cookieGame->cookies,game);
						}
						moveImageTo(cookieGame->cookies,cookieGame->cookies->x,tempy);
					}
					else if(cookieGame->dir == 2 && cookieGame->cookies->x>72+cookieGame->x+(cookieGame->cookies->w+3)*5)
					{
						tempx = cookieGame->cookies->x;
						while (cookieGame->cookies->x>72+cookieGame->x+(cookieGame->cookies->w+3)*5)
							moveImage(cookieGame->cookies,-(cookieGame->cookies->w+3)*5,0);
						drawImage(cookieGame->cookies,game);
						if(cookieGame->cookies->x>72+cookieGame->x+(cookieGame->cookies->w+3)*4)
						{
							moveImage(cookieGame->cookies,-(cookieGame->cookies->w+3)*5,0);
							drawImage(cookieGame->cookies,game);
						}
						moveImageTo(cookieGame->cookies,tempx,cookieGame->cookies->y);
					}
					else if(cookieGame->dir == 3 && cookieGame->cookies->y>69+cookieGame->y+(cookieGame->cookies->h+3)*5)
					{
						tempy = cookieGame->cookies->y;
						while (cookieGame->cookies->y>69+cookieGame->y+(cookieGame->cookies->h+3)*5)
							moveImage(cookieGame->cookies,0,-(cookieGame->cookies->h+3)*5);
						drawImage(cookieGame->cookies,game);
						if(cookieGame->cookies->y>69+cookieGame->y+(cookieGame->cookies->h+3)*4)
						{
							moveImage(cookieGame->cookies,0,-(cookieGame->cookies->h+3)*5);
							drawImage(cookieGame->cookies,game);
						}
						moveImageTo(cookieGame->cookies,cookieGame->cookies->x,tempy);
					}
					else
						drawImage(cookieGame->cookies,game);
					if(cookieGame->dir == 2)
					{
						if(cookieGame->cookies->x>72+cookieGame->x+(cookieGame->cookies->w+3)*4)
						{
							moveImage(cookieGame->cookies,-(cookieGame->cookies->w+3)*5,0);
							drawImage(cookieGame->cookies,game);
							moveImage(cookieGame->cookies,(cookieGame->cookies->w+3)*5,0);
						}
					}
					else if(cookieGame->dir == 3)
					{
						if(cookieGame->cookies->y>69+cookieGame->y+(cookieGame->cookies->h+3)*4)
						{
							moveImage(cookieGame->cookies,0,-(cookieGame->cookies->h+3)*5);
							drawImage(cookieGame->cookies,game);
							moveImage(cookieGame->cookies,0,(cookieGame->cookies->h+3)*5);
						}
					}
					moveImage(cookieGame->cookies,cookieGame->cookies->w+3,0);
					if(cookieGame->dir == 3 && cookieGame->posY==j)
						moveImage(cookieGame->cookies,0,-game->chain->vy+cookieGame->clickY);
				}
				if(cookieGame->dir == 2 && cookieGame->posX==i)
					moveImage(cookieGame->cookies,-game->chain->vx+cookieGame->clickX,0);
				
				moveImage(cookieGame->cookies,-(cookieGame->cookies->w+3)*5,cookieGame->cookies->h+3);
			}
		}
		else
		{
			if(cookieGame->dir != 0)
			{
				if(cookieGame->dir == 2)
				{
					
				}
				else if (cookieGame->dir == 3)
				{
					
				}
				cookieGame->posX = -1;
				cookieGame->posY = -1;
				cookieGame->dir = 0;
			}
			
			
			for(i=0;i<5;i++)
			{
				
				for(j=0;j<5;j++)
				{
					if(isClicked(&cookieGame->cookies->destPos,game,game->currLayer))
					{
						*clickable = 1;
						if(game->chain->mouse == 1)
						{
							game->chain->mouse = 3;
							cookieGame->posX = i;
							cookieGame->posY = j;
							cookieGame->clickX = game->chain->vx;
							cookieGame->clickY = game->chain->vy;
							cookieGame->dir = 1;//Undecided
							
						}
						
					}
					if(cookieGame->board[i][j]>=0)
					{
						setToFrame(cookieGame->cookies,cookieGame->board[i][j],(cookieGame->colRem == i || cookieGame->rowRem == j));
						drawImage(cookieGame->cookies,game);
					}
					moveImage(cookieGame->cookies,cookieGame->cookies->w+3,0);
				}
				moveImage(cookieGame->cookies,-(cookieGame->cookies->w+3)*5,cookieGame->cookies->h+3);
			}
		}
		drawImage(cookieGame->foreground,game);
		moveImageTo(cookieGame->fuse,cookieGame->foreground->x+8,cookieGame->foreground->y+cookieGame->foreground->h-cookieGame->fuse->h);
		setToFrame(cookieGame->fuse,1,1);
		for(i=0;i<cookieGame->timer;i+=100)
		{
			drawImage(cookieGame->fuse,game);
			moveImage(cookieGame->fuse,0,-cookieGame->fuse->h);
		}
		moveImage(cookieGame->fuse,0,20-((cookieGame->timer-1)%100)/5);
		setToFrame(cookieGame->fuse,cookieGame->timer%10/5,0);
		drawImage(cookieGame->fuse,game);
		
		moveImageTo(cookieGame->scoreBoard,cookieGame->foreground->x+234,cookieGame->foreground->y+cookieGame->foreground->h-cookieGame->scoreBoard->h);
		for(i=0;i<15;i++)
		{
			setToFrame(cookieGame->scoreBoard,(cookieGame->score>= i+1),0);
			drawImage(cookieGame->scoreBoard,game);
			moveImage(cookieGame->scoreBoard,0,-cookieGame->scoreBoard->h);
		}
		setToFrame(cookieGame->scoreBoard,0,1);
		drawImage(cookieGame->scoreBoard,game);
		cookieGame->timer--;
		if(cookieGame->timer <=0)
		{
			cookieGame->gameStatus = 3;
			cookieGame->score = 0;
			cookieGame->timer = cookieGame->timerMax;
			Mix_PlayChannel(1,cookieGame->loseSound, 0);	
		}
		if(cookieGame->score == 15)
		{
			cookieGame->gameStatus = 2;
			cookieGame->score = 0;
			cookieGame->timer = cookieGame->timerMax;
			Mix_PlayChannel(1,cookieGame->winSound, 0);
		}
	}
	else if(cookieGame->gameStatus == 2)
	{
		drawImage(cookieGame->win,game);
		moveImageTo(cookieGame->start,cookieGame->menu->x+160,cookieGame->menu->y+130);
		if(isClicked(&cookieGame->start->destPos,game,game->currLayer))
		{
			*clickable = 1;
			if(game->chain->mouse == 1)
			{
				game->chain->mouse = 2;
				cookieGame->gameStatus = 1;
			}
		}
		drawImage(cookieGame->start,game);
	}
	else if(cookieGame->gameStatus == 3)
	{
		drawImage(cookieGame->lose,game);
		moveImageTo(cookieGame->start,cookieGame->menu->x+40,cookieGame->menu->y+150);
		if(isClicked(&cookieGame->start->destPos,game,game->currLayer))
		{
			*clickable = 1;
			if(game->chain->mouse == 1)
			{
				game->chain->mouse = 2;
				cookieGame->gameStatus = 1;
			}
		}
		drawImage(cookieGame->start,game);
	}
}
void refillBoard(miniCookieGame * cookieGame)
{
	int i, j;
	for(i=0;i<5;i++)
	{
		
		for(j=0;j<5;j++)
		{
			if (cookieGame->board[i][j]<0)
			{
				cookieGame->board[i][j]= rand()%5;
			}
		}
	}
}

void clearToLoop(Pile * pile,int loop)
{
	PileItem *temp= pile->firstAlpha;
	pile->total = 0;
	while(temp!=NULL)
	{
		temp->size = 0;
		setToFrame(temp->item->image,loop,0);
		temp = temp->nextAlpha;
	}
}

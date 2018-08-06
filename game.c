#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "flumfGlub.h"
#include "flibble.h"

int main(void)
{
	Game * game = setup(512,384);
	char temp [80];
	char tempItem [80];
	FILE* savefile;
	int selectionInfo = 1;
	int consumption = 0;
	Font * font;
	Image* background, *flumfImg, *menuImg, * recipeImg,* recipeBlack;
	Image* smolEgg, * bkgrnd, * flumfAnim, * error;
	Image *newGameImage, *continueGameImage, *mainMenu, *forgeInfo;
	Image * adventureButton,*craftButton,*leftArrow,*rightArrow,*upArrow,*downArrow, *close, *forgeButton, *natureButton;
	Image * statusButton, *flumfButton, *upgradeButton;
	Image * levelBar, *forgeSlice, *forgeLeftMenu, *tinyResources, *palLevelOutline, * ordering;
	double flumfAuto = 0;
	Image * dirtPile, *growing, *grown, *shadowLeftMenu;
	SDL_Rect tempRect;
	int growTimer = 0;
	Item * growItem = NULL;
	
	int levels[5]={0};

	Image * selectionMenu;

	char * activatedUpgrades;

	int scrollmin = 2048;
	
	double desireMods = 1;
	
	int enemyTimer = 400;
	int enemyTimerMax = 400;
	
	int playerDamage = 10;

	int maxscroll = 0;

	int moxieBonus = 0;
	
	Enemy ** enemylist;
	int enemylistsize = 0;
	int enemylistcapacity = 10;
	
	long int totalPals = 0;
	long int totalCapacity = 0;
	char hasForge = 0, hasUpgrade = 0;
	
	ProductionQueue * queue = initProdQueue();
	int newGame = -1;
	int  i = 0, j = 0, k = 0,l = 0, p = 0 , q = 0;
	int errorTimer = 0;
	ObjectList* itemList;
	
	int flumfEatActive = 0;
	Item * pendingItem = NULL;
	Pile * pile;
	PileItem* grabbed;
	House** houseList = NULL;
	int houseNum = 0;

	long long int flumfEntropy = 0;
	double flumfDesire = 16;
	Image * cursor;
	
	SDL_Rect temprect;
	
	int scrolly = 0, canScroll = 0, animWhistle = 0,returnTiming = 0;
	
	Adventurer **  queuedEggs = NULL;
	Adventurer **  palsPullingPals = NULL;
	
	int palpullcapacity = 10,palpullsize = 0;
	int eggSize = 0;
	int visEggSize = 0;
	
	RecipeList * recipeList;
	UpgradeList * upgradeList;
	
	int specialization = 0;
	int houseMove = 0;
	
	House * movingHouse = NULL;
	
	int clickable = 0;
	
	int inMenu = 0;
	int craft = 0; 
	int housex = 0;
	int housey = 0;
	int avgmoxie = 0;
	double wonderTeaSet = 0;
	int wonderModifier = 0;
	int currOutPals = 0;
	int currTotalPals = 0;
	int currExploringPals = 0;
	int chutzpahGravyBoat = 0, pendingChutzpah = 0;
	char redrawHouses = 0, redrawMenu = 0;
	
	srand(time(NULL));
	SDL_ShowCursor(SDL_DISABLE);

	recipeList = getRecipeList("lists/recipeList.txt",game);
	upgradeList = getUpgradeList("lists/upgradeList.txt",game);
	
	activatedUpgrades = calloc(sizeof(char),upgradeList->size);
	

	palsPullingPals = malloc(sizeof(Adventurer*)*palpullcapacity);
	
	
	
	background = loadImage("images/background.png",1,1,game);
	forgeInfo = loadImage("images/forge-types.png",5,1,game);
	
	palLevelOutline = loadImage("images/levels-outline.png",1,1,game);
	moveImageTo(palLevelOutline,0,10);
	
	levelBar = loadImage("images/levels.png",5,2,game);
	forgeSlice = loadImage("images/forge-slice.png",1,1,game);
	forgeLeftMenu = loadImage("images/forge-left-menu.png",1,1,game);
	tinyResources = loadImage("images/tiny-resources.png",3,5,game);

	bkgrnd = loadImage("images/background-tile.png",1,1,game);
	flumfImg = loadImage("images/flumf.png",1,1,game);
	menuImg = loadImage("images/menu.png",1,1,game);
	recipeImg = loadImage("images/recipeMenu.png",8,6,game);
	recipeBlack = loadImage("images/recipeBlack.png",8,6,game);
	smolEgg = loadImage("images/craftables/egg-smol.png",1,1,game);
	flumfAnim = loadImage("images/flumftestanim.png", 4, 2,game);
	error = loadImage("images/error.png", 1, 1,game);
	
	adventureButton = loadImage("images/buttons/whistleboy.png",2,2,game);

	close = loadImage("images/buttons/close.png",1,1,game);
	craftButton = loadImage("images/buttons/recipe.png",1,1,game);
	upgradeButton = loadImage("images/buttons/upgrade.png",1,1,game);
	forgeButton = loadImage("images/buttons/forge.png",1,1,game);
	statusButton = loadImage("images/buttons/party.png",1,1,game);
	natureButton = loadImage("images/buttons/nature.png",1,1,game);
	flumfButton = loadImage("images/buttons/flumf.png",1,1,game);
	leftArrow = loadImage("images/buttons/l-arrow.png", 1,1,game);
	rightArrow = loadImage("images/buttons/r-arrow.png", 1,1,game);
	upArrow = loadImage("images/buttons/u-arrow.png", 1,1,game);
	downArrow = loadImage("images/buttons/d-arrow.png", 1,1,game);
	
	cursor = loadImage("images/pointer.png", 2, 1,game);
	mainMenu = loadImage("images/flumf-menu.png", 1, 1,game);
	newGameImage = loadImage("images/newgame.png", 1, 1,game);
	continueGameImage = loadImage("images/continuegame.png", 1, 2,game);
	selectionMenu = loadImage("images/buttons/selection.png", 3, 2,game);
	ordering = loadImage("images/buttons/ordering.png", 2, 2,game);
	
	dirtPile = loadImage("images/dirt-pile.png", 1,1,game);
	growing = loadImage("images/leaf1.png", 1,1,game);
	grown = loadImage("images/leaf2.png", 1,1,game);
	
	shadowLeftMenu = loadImage("images/left-menu-overlay.png",1,1,game);

	font = newFont(loadImage("images/font.png",13,5,game));


	moveImageTo(flumfImg, background->w-flumfImg->w,background->h-30-flumfImg->h);
	
	
	moveImageTo(statusButton,statusButton->w+10,0);
	moveImageTo(craftButton,(craftButton->w+10)*2,0);
	moveImageTo(forgeButton,(forgeButton->w+10)*3,0);
	moveImageTo(upgradeButton,0,(upgradeButton->h+10));
	moveImageTo(natureButton,(natureButton->w+10),(natureButton->h+10));
	
	itemList = getList("lists/itemList.txt",game);
	housey = background->h-119;
	
	enemylist=malloc(sizeof(Enemy*)*enemylistcapacity);
	
	pile = initPile(loadImage("images/pile.png",1,1,game),2560,background->h-30,itemList);
	sortList(itemList);
	moveImageTo(newGameImage,game->boundingBox.w/4-newGameImage->w/2,game->boundingBox.h*1/8);
	moveImageTo(continueGameImage,game->boundingBox.w/4-continueGameImage->w/2,game->boundingBox.h*2/8);
	
	savefile = fopen("save0.sav","r");
	if(savefile==NULL)
	{
		setToFrame(continueGameImage,0,1);
	}
	addLayer(game,3072,640);
	addLayer(game,3072,640);
	addLayer(game,3072,640);
	addLayer(game,3072,640);
	setLayer(game,3);
	moveImageTo(background,0,-background->h+game->boundingBox.h);
	while(newGame==-1 && !game->chain->close)
	{
		clickable = 0;
		getEvents(game->chain, game);
		moveImageTo(cursor,game->chain->vx,game->chain->vy);

		drawImage(background,game);
		moveImageTo(mainMenu,game->boundingBox.w/2-mainMenu->w/2,0);
		drawImage(mainMenu,game);
		drawImage(newGameImage,game);
		drawImage(continueGameImage,game);
		if(isClicked(&(newGameImage->destPos),game,0))
		{
			if(game->chain->mouse ==1)
			{
				newGame=1;
				game->chain->mouse =2;
			}
			clickable = 1;
		}
		else if(savefile && isClicked(&(continueGameImage->destPos),game,0) )
		{
			if(game->chain->mouse ==1)
			{
				newGame=0;
				game->chain->mouse =2;
			}
			clickable = 1;
		}
		if(game->chain->window)
		{
			resizeWindow(game);
			moveImageTo(newGameImage,game->boundingBox.w/4-newGameImage->w/2,game->boundingBox.h*1/8);
			moveImageTo(continueGameImage,game->boundingBox.w/4-continueGameImage->w/2,game->boundingBox.h*2/8);
		}
		if(clickable)
		{
			setToFrame(cursor,1,0);
		}
		else
		{
			setToFrame(cursor,0,0);
		}
		drawImage(cursor,game);
		display(game);
	}
	clearLayer(game);
	swapLayers(game,0,3);
	moveLayer(game,0,-256,0);
	moveLayer(game,0,-256,1);

	
	

	housex = pile->image->x-100;
	if(newGame == 1)
	{
		houseNum++;
		houseList = realloc(houseList,sizeof(House*)*houseNum);
		houseList[houseNum-1] = newHouse(game,housex,housey,3);
		totalCapacity+=houseList[houseNum-1]->capacity;
		for(i=0;i<10;i++)
		{
			addPaltoHouse(houseList[houseNum-1],newAdventurer(1,game),game);
			currTotalPals++;
			totalPals++;
		}
		
	}
	else if(newGame == 0)
	{
		fgets(temp,80,savefile);
		while(!feof(savefile))
		{
			if(usefulstrcmp(temp,"eggs"))
			{
				sscanf(temp,"eggs %d",&eggSize);
				visEggSize = eggSize;
				queuedEggs = malloc(sizeof(Adventurer*)*eggSize);
				for(i=0;i<eggSize;i++)
				{
					queuedEggs[i] = newAdventurer(1,game);
				}
			}
			else if(usefulstrcmp(temp,"flumf"))
			{
						sscanf(temp,"flumf %lld %lf\n",&flumfEntropy,&flumfDesire);

			}
			else if(usefulstrcmp(temp,"resources"))
			{
				sscanf(temp,"resources %d %d %d %d %d",&(levels[0]),&(levels[1]),&(levels[2]),&(levels[3]),&(levels[4]));
			}
			else if(usefulstrcmp(temp,"upgrade"))
			{
				int upgrade;
				sscanf(temp,"upgrade %d",&upgrade);
				upgradeList->list[upgrade-1]->unlocked=0;
				if(upgrade==1)
				{
					moxieBonus += 2;	
				}
				else if(upgrade==2)
				{
					desireMods += .25;	
				}
				activatedUpgrades[upgrade-1] = 1;
			}
			else if(usefulstrcmp(temp,"item"))
			{
				sscanf(temp,"item %s",tempItem);
				putInPile(pile,getItemByName(tempItem,itemList));
			}
			else if(usefulstrcmp(temp,"house"))
			{
				char houseName[80];
				int numPalsinHouse;

				sscanf(temp,"house %s %d %d %d",houseName,&numPalsinHouse,&housex,&housey);
				houseNum++;
				houseList = realloc(houseList,sizeof(House*)*houseNum);
				houseList[houseNum-1] = newHouseByName(game,housex,housey,houseName);
				totalCapacity+=houseList[houseNum-1]->capacity;
				if(houseList[houseNum-1]->type==1)
				{
					unlockTier(2,recipeList);
					unlockTier(3,recipeList);
					unlockHouse(recipeList, houseName,0); 
					hasForge=1;
					wonderModifier++;
				}
				if(houseList[houseNum-1]->type == 4)
				{
					specialization = 1;
					lockTier(3,recipeList);
				}
				if(houseList[houseNum-1]->type == 5)
				{
					specialization = 2;
					lockTier(3,recipeList);
				}
				if(houseList[houseNum-1]->type == 6)
				{
					specialization = 4;
					lockTier(3,recipeList);
				}
				if(houseList[houseNum-1]->type == 9)
				{
					specialization = 3;
					lockTier(3,recipeList);
				}
				if(houseList[houseNum-1]->type == 8)
				{
					unlockHouse(recipeList, houseName,0);
					wonderModifier++;
					hasUpgrade = 1;
				}
			
			
				for(i = 0; i < numPalsinHouse;i++)
				{
					fgets(temp,80,savefile);
					if(feof(savefile))
						return(1);

					addPaltoHouse(houseList[houseNum-1],newAdventurer(1,game),game);
					totalPals++;
					currTotalPals++;
					sscanf(temp, "pal %d %d %d",&houseList[houseNum-1]->pals[i]->wonder,&houseList[houseNum-1]->pals[i]->chutzpah,
						&houseList[houseNum-1]->pals[i]->moxie);
				}
			}
			fgets(temp,80,savefile);
		}
		fclose(savefile);
	}
	
	//initial setup alignment	
	moveImageTo(background,0,0);
	moveImageTo(leftArrow,0,(game->boundingBox.h-leftArrow->h)/2);
	moveImageTo(rightArrow,game->boundingBox.w-rightArrow->w,(game->boundingBox.h-rightArrow->h)/2);
	moveImageTo(upArrow,game->boundingBox.w/2-upArrow->w/2,0);
	moveImageTo(downArrow,game->boundingBox.w/2-downArrow->w/2,(game->boundingBox.h-downArrow->h));
	moveImageTo(adventureButton, game->boundingBox.w - adventureButton->w,game->boundingBox.h - adventureButton->h);
	moveImageTo(close, game->boundingBox.w - close->w,0);
	
	
	moveLayer(game,-scrollmin,0,0);	
	moveLayer(game,-scrollmin,0,1);	
	moveLayer(game,-scrollmin,0,2);

	
	//Drawing background layer
	setLayer(game,0);
	drawImage(background,game);
	
	//Draw initial houses
	setLayer(game,1);
	for(i=0;i<houseNum;i++)
	{
		drawImage(houseList[i]->image, game);
	}
	drawImage(flumfImg,game);
	drawPile(pile,game);
	
	drawImagePyramid(smolEgg,game,visEggSize,flumfImg->x,background->h-30,7);
	//Drawing menu
	setLayer(game,3);
	drawImage(adventureButton,game);
	drawImage(craftButton,game);
	drawImage(flumfButton,game);
	if(hasUpgrade)
	drawImage(upgradeButton,game);
	if(hasForge)
		drawImage(forgeButton,game);
	drawImage(leftArrow,game);
	drawImage(rightArrow,game);
	drawImage(upArrow,game);
	drawImage(downArrow,game);


	//set to active movement layer
	setLayer(game,2);
	while(!game->chain->close)
	{
		clickable = 0;
		getEvents(game->chain, game);
		
		if(redrawHouses == 1)
		{
			setLayer(game,1);
			clearLayer(game);
			for(i=0;i<houseNum;i++)
			{
				drawImage(houseList[i]->image, game);
			}
			drawImage(flumfImg,game);
			drawPile(pile,game);
			drawImagePyramid(smolEgg,game,visEggSize,flumfImg->x,background->h-30,7);
			redrawHouses = 0;
			setLayer(game,2);
		}
		
		if(returnTiming > 0)
		{
			returnTiming--;
		}

		
		for(i=0;i<queue->size;i++)
		{
			if(queue->queue[i]->type == 1)
			{
				if(queue->queue[i]->itemsize == 0)
				{
					houseNum++;
					houseList = realloc(houseList,sizeof(House*)*houseNum);
					houseList[houseNum-1] = queue->queue[i]->queued;
					totalCapacity+=houseList[houseNum-1]->capacity;
					if(houseList[houseNum-1]->type==1)
						hasForge = 1;
					if(houseList[houseNum-1]->type==8)
						hasUpgrade = 1;
					removeFromQueue(queue,i);
					redrawHouses = 1;
				} 
			}
								
		}		
		
				
		if(!inMenu && isClicked(&adventureButton->destPos,game,3))
		{
			if(game->chain->mouse == 1)
			{
				game->chain->mouse =2;
				if(animWhistle == 0)
				{
					j=0;
					for(i=0;i<houseNum;i++)
					{
						for(k=0;k<houseList[i]->size;k++)
						{
							if(houseList[i]->pals[k]->status == 0)
							{
								houseList[i]->pals[k]->status = 1;
								houseList[i]->pals[k]->timer = 50;
								houseList[i]->pals[k]->delay = 5*j;
								j++;
								chutzpahGravyBoat += getChutzpah(houseList[i]->pals[k]);
								wonderTeaSet +=getWonder(houseList[i]->pals[k]);
								currOutPals++;
								currExploringPals++;
							}
						}
					}

					if(j==0)
					{
						setToFrame(adventureButton,0,1);
						redrawMenu = 1;
					}
					else
					{
						setToFrame(adventureButton,1,0);
						redrawMenu = 1;
					}
					animWhistle = 30;
				}
			}
			clickable = 1;
		}
		
		if(currOutPals/currTotalPals < flumfAuto)
		{

			for(i=0;i<houseNum;i++)
			{
				for(k=0;k<houseList[i]->size;k++)
				{
					if(houseList[i]->pals[k]->status == 0)
					{
						houseList[i]->pals[k]->status = 1;
						houseList[i]->pals[k]->timer = 50;
						houseList[i]->pals[k]->delay = 5*j;
						chutzpahGravyBoat += getChutzpah(houseList[i]->pals[k]);
						wonderTeaSet +=getWonder(houseList[i]->pals[k]);
						currOutPals++;
						currExploringPals++;
						if(currOutPals*1.0/currTotalPals >= flumfAuto)
							break;
					}
				}
				if(currOutPals*1.0/currTotalPals >= flumfAuto)
							break;
			}
	
		}
		
		for(i=0;i<houseNum;i++)
		{
			for(j=0;j<houseList[i]->size;j++)
			{
				if(houseList[i]->pals[j]->status == 1)
				{
					if(houseList[i]->pals[j]->timer == 0 && returnTiming == 0)
					{
						if(pendingItem == NULL)
						{
							pendingItem = getItemGroup(itemList,generateRandomLogVal(ceil(wonderTeaSet/currExploringPals)+1+wonderModifier)-2,chutzpahGravyBoat);
							pendingChutzpah = pendingItem->mass;
							houseList[i]->pals[j]->item = pendingItem;
							avgmoxie += getMoxie(houseList[i]->pals[j],moxieBonus);
						}
						houseList[i]->pals[j]->status = -1;//They're yanking it REAL HAERD
						avgmoxie += getMoxie(houseList[i]->pals[j],moxieBonus);
						pendingChutzpah -= getChutzpah(houseList[i]->pals[j]);
						chutzpahGravyBoat -= getChutzpah(houseList[i]->pals[j]);
						wonderTeaSet -= getWonder(houseList[i]->pals[j]);
						currExploringPals--;
						houseList[i]->pals[j]->chutzpah++;
						palsPullingPals[palpullsize] = houseList[i]->pals[j];
						palpullsize++;
						if(palpullsize  >= palpullcapacity/2)
						{
							palpullcapacity*=2;
							palsPullingPals = realloc(palsPullingPals,sizeof(Adventurer*)*palpullcapacity);
						}
						if(pendingChutzpah <=0)
						{
							pendingChutzpah = 0;
							avgmoxie/=palpullsize+1;
							moveImageTo(pendingItem->image,palsPullingPals[0]->image->x,palsPullingPals[0]->image->y - pendingItem->image->h);
							
							for(l=0;l<palpullsize;l++)
							{
								palsPullingPals[l]->burdenedmoxie = avgmoxie;
								palsPullingPals[l]->status = 2;
								moveAdventurer(palsPullingPals[l],palsPullingPals[0]->image->x - (pendingItem->image->w/(palpullsize))*l - palsPullingPals[l]->image->x,0);		
							}
							
							moveImageTo(pendingItem->image,palsPullingPals[palpullsize-1]->image->x,pendingItem->image->y);
							
							pendingItem = NULL;
							palpullsize = 0;
							returnTiming = 5;
							avgmoxie = 0;
						}
						
						
					}
					else if(houseList[i]->pals[j]->timer > 0 && houseList[i]->pals[j]->delay == 0 && houseList[i]->pals[j]->image->x<scrollmin)
					{
						houseList[i]->pals[j]->timer--;
					}
				}
				
				if(houseList[i]->pals[j]->image->x>pile->image->x && houseList[i]->pals[j]->status == 2)
				{
					putInPile(pile,houseList[i]->pals[j]->item);
					redrawHouses=1;
					houseList[i]->pals[j]->item = NULL;
					houseList[i]->pals[j]->status = 3;
				}
			}

		}
		
		for(i=0;i<houseNum;i++)
		{
			
			if(houseMove == 0 && !inMenu && isClicked(&houseList[i]->image->destPos,game,1) && canRemove(houseList,houseNum,houseList[i]))
			{
				if(game->chain->mouse == 1)
				{
					game->chain->mouse =2;
					houseMove = 2;
					movingHouse = houseList[i];
				}
				clickable = 1;
			}
			for(j=0;j<houseList[i]->size;j++)
			{

				adventurerDo(houseList[i]->pals[j], game, itemList, houseList[i]->image->x + houseList[i]->image->w/2,houseList[i]->image->y,&currOutPals,&redrawHouses,&visEggSize,moxieBonus,scrollmin);
		
				
				if(houseList[i]->pals[j]->status == 5 && houseList[i]->pals[j]->item==NULL )
				{
					for(p=0;p<queue->size ;p++)
					{
						for(q=0;q<queue->queue[p]->itemsize;q++)
						{
							if(queue->queue[p]->itemstatus[q]==1 && houseList[i]->pals[j]->item==NULL)
							{
								queue->queue[p]->itemstatus[q]=2;
								giveAdventurerItem(houseList[i]->pals[j],queue->queue[p]->items[q]);
							}
						}
					}
				}
				else if(houseList[i]->pals[j]->status == 6 && houseList[i]->pals[j]->item!=NULL)
				{
					removeNextItemQueue(queue,houseList[i]->pals[j]->item);
					houseList[i]->pals[j]->item = NULL;
				}
				else if(houseList[i]->pals[j]->status == 8)
				{
					temprect.w = smolEgg->w;
					temprect.h = smolEgg->h;
					temprect.x = houseList[i]->pals[j]->image->x + houseList[i]->pals[j]->image->w/2 - smolEgg->w/2;
					temprect.y = houseList[i]->pals[j]->image->y - smolEgg->h;
					SDL_RenderCopy(game->renderer,smolEgg->image,&smolEgg->srcPos,&temprect);	
				}
				else if(houseList[i]->size + houseList[i]->pending < houseList[i]->capacity && houseList[i]->pals[j]->status == 0 && eggSize > 0)
				{
					currOutPals++;

					houseList[i]->pals[houseList[i]->size+houseList[i]->pending]= queuedEggs[eggSize - 1];
					
					moveImageTo(queuedEggs[eggSize - 1]->image,houseList[i]->image->x + houseList[i]->image->w/2 - queuedEggs[eggSize - 1]->image->x/2, queuedEggs[eggSize - 1]->image->y);
					moveImageTo(queuedEggs[eggSize - 1]->image,queuedEggs[eggSize - 1]->image->x,game->boundingBox.h-30 - queuedEggs[eggSize - 1]->image->h);
					houseList[i]->pals[j]->posTo = i;
					houseList[i]->pending++;
					eggSize--;
					houseList[i]->pals[j]->delay = 3*j;
					houseList[i]->pals[j]->status = 7;
				}
				else if(houseList[i]->pals[j]->status == 9)
				{
					houseList[houseList[i]->pals[j]->posTo]->pending--;
					houseList[houseList[i]->pals[j]->posTo]->size++;
					totalPals++;
					currTotalPals++;
					houseList[i]->pals[j]->status = 0;
					currOutPals--;
				}
			}
			if(houseList[i]->size+houseList[i]->pending< houseList[i]->capacity && eggSize > 0)
			{
				l=0;
				for(k=0;k<i;k++)
				{
					for(l=0;l<houseList[k]->size ;l++)
					{
						if(houseList[k]->pals[l]->status==0)
							break;
					}
					if(l < houseList[k]->size && houseList[k]->pals[l]->status==0)
							break;
				}
				if (k<i)
				{
					currOutPals++;

					houseList[i]->pals[houseList[i]->size+houseList[i]->pending]= queuedEggs[eggSize - 1];

					moveImageTo(queuedEggs[eggSize - 1]->image,houseList[i]->image->x + houseList[i]->image->w/2 - queuedEggs[eggSize - 1]->image->x/2, queuedEggs[eggSize - 1]->image->y);
					moveImageTo(queuedEggs[eggSize - 1]->image,queuedEggs[eggSize - 1]->image->x,game->boundingBox.h-30 - queuedEggs[eggSize - 1]->image->h);
					houseList[k]->pals[l]->posTo = i;
					houseList[i]->pending++;
					eggSize--;
					houseList[k]->pals[l]->delay = 3*l;
					houseList[k]->pals[l]->status = 7;
				}
			}
		}
		
		//Enemy Behaviour Start
		
		for(i =0;i<enemylistsize;i++)
		{
			enemyDo(enemylist[i],game,itemList,0,0,&redrawHouses,&visEggSize);
			if(enemylist[i]->status ==1 && enemylist[i]->image->x>pile->image->x)
			{
				enemylist[i]->status = 2;
				enemylist[i]->item = takeItemFromPile(pile);
				redrawHouses= 1;
				if(enemylist[i]->item != NULL)
					moveImageTo(enemylist[i]->item->image,enemylist[i]->image->x-enemylist[i]->item->image->w/2+enemylist[i]->image->w/2,enemylist[i]->image->y - enemylist[i]->item->image->h);
			}
			
			temprect.x=enemylist[i]->image->x-enemylist[i]->image->w/2;
			temprect.y=enemylist[i]->image->y-enemylist[i]->image->h/2;
			temprect.w=enemylist[i]->image->w*2;
			temprect.h=enemylist[i]->image->h*2;
			SDL_RenderDrawRect(game->renderer,&temprect);
			if (isClicked(&temprect,game,2) && game->chain->mouse == 1)
			{
				game->chain->mouse = 2;
				enemylist[i]->health -=playerDamage;
			}
			
			if((enemylist[i]->status ==2 && enemylist[i]->image->x<0) || enemylist[i]->health<=0)
			{
				
				for(int j=i+1;j<enemylistsize;j++)
				{
					enemylist[j-1]=enemylist[j];
				}
				enemylistsize--;
			}
		}
		if(enemyTimer == 0)
		{
			if(totalPals*10<pile->total)
			{
				enemylistsize++;
				if(enemylistsize>=enemylistcapacity)
				{
					enemylistcapacity*=2;
					enemylist = realloc(enemylist,enemylistcapacity*sizeof(Enemy*));
				}
				enemylist[enemylistsize-1] = newEnemy(1,game);
				enemylist[enemylistsize-1]->status = 1;
				moveImageTo(enemylist[enemylistsize-1]->image,0,game->boundingBox.h - 30- enemylist[enemylistsize-1]->image->h);
				enemyTimer = enemyTimerMax;
			}
		}
		else
		{
			enemyTimer--;
		}
		
		//Enemy Behaviour End
		
		if(animWhistle < 1)
		{
			setToFrame(adventureButton, 0 , 0);
			redrawMenu = 1;
		}
		else
			animWhistle--;
		
		
		if(queue->size > 0)
		{
			j=0;
			for(i=0;i<queue->size;i++)
			{
				for(k=0;k<queue->queue[i]->itemsize;k++)
				{
					if(queue->queue[i]->itemstatus[k]==0)
					{
						for(p=0;p<houseNum;p++)
						{
							for(q=0;q<houseList[p]->size;q++)
							{
								if(houseList[p]->pals[q]->status == 0 && queue->queue[i]->itemstatus[k] == 0)
								{

									queue->queue[i]->itemstatus[k] = 1;
									houseList[p]->pals[q]->status = 4;
									houseList[p]->pals[q]->timer = 150;
									houseList[p]->pals[q]->delay = 20*j;
									j++;
									if(queue->queue[i]->type == 1)
										houseList[p]->pals[q]->posTo = ((House*)(queue->queue[i]->queued))->image->x;
									else if(queue->queue[i]->type == 2)
										houseList[p]->pals[q]->posTo = queue->queue[i]->returnpos;
									
									currOutPals++;
								}
							}
						}
					}
				}
			}
		//
		}
		
		if(!inMenu && isClicked(&flumfButton->destPos,game,3))
		{
			if(game->chain->mouse == 1 )
			{
			inMenu = 4;
			game->chain->mouse = 2;
			}
			clickable = 1;
		}
		else if(!inMenu && isClicked(&statusButton->destPos,game,3))
		{
			if(game->chain->mouse == 1 )
			{
			inMenu = 1;
			game->chain->mouse = 2;
			}
			clickable = 1;
		}
		else if(!inMenu && isClicked(&craftButton->destPos,game,3))
		{
			if(game->chain->mouse == 1 )
			{
			inMenu = 3;
			game->chain->mouse = 2;
			}
			clickable = 1;
		}
		else if(!inMenu && isClicked(&forgeButton->destPos,game,3) && hasForge)
		{
			if(game->chain->mouse == 1 )
			{
			inMenu = 2;
			game->chain->mouse = 2;
			}
			clickable = 1;
		}
		else if(!inMenu && isClicked(&natureButton->destPos,game,3) && specialization == 4)
		{
			if(game->chain->mouse == 1 )
			{
			inMenu = 5;
			game->chain->mouse = 2;
			}
			clickable = 1;
		}
		
		else if(!inMenu && isClicked(&upgradeButton->destPos,game,3) && hasUpgrade)
		{
			if(game->chain->mouse == 1 )
			{
			inMenu = 6;
			game->chain->mouse = 2;
			}
			clickable = 1;
		}
		

		
		if(!inMenu && isClicked(&rightArrow->destPos,game,3))
		{
			if(-game->layerList[0]->scrollDest.x < game->layerList[0]->scrollDest.w-game->boundingBox.w)
			{
				moveLayer(game,-4,0,0);	
				moveLayer(game,-4,0,1);	
				moveLayer(game,-4,0,2);
				redrawMenu = 1;
				clickable = 1;
			}
			
		}
		else if(!inMenu && isClicked(&leftArrow->destPos,game,3))
		{
			if(game->layerList[0]->scrollDest.x < -scrollmin)
			{
				moveLayer(game,4,0,0);	
				moveLayer(game,4,0,1);	
				moveLayer(game,4,0,2);
				redrawMenu = 1;
				clickable = 1;
			}
		}
		else if(!inMenu && isClicked(&upArrow->destPos,game,3))
		{
			if(game->layerList[0]->scrollDest.y < 0)
			{
				moveLayer(game,0,4,0);	
				moveLayer(game,0,4,1);	
				moveLayer(game,0,4,2);
				redrawMenu = 1;
				clickable = 1;
			}
		}
		else if(!inMenu && isClicked(&downArrow->destPos,game,3))
		{
			if(-game->layerList[0]->scrollDest.y < game->layerList[0]->scrollDest.h-game->boundingBox.h)
			{
				moveLayer(game,0,-4,0);	
				moveLayer(game,0,-4,1);	
				moveLayer(game,0,-4,2);
				redrawMenu = 1;
				clickable = 1;
			}
		}
		
	
		if (houseMove>0)
		{
			setLayer(game,1);
			for(i=0;i<queue->size;i++)
			{
				if(queue->queue[i]->type == 1)
				{
					setToFrame(((House*)(queue->queue[i]->queued))->image,1,0);
					drawImage(((House*)(queue->queue[i]->queued))->image,game);
					setToFrame(((House*)(queue->queue[i]->queued))->image,0,0);
				}
			}
			setLayer(game,2);
			housex = game->chain->vx - movingHouse->image->w/6 -game->layerList[1]->scrollDest.x;
			housey = background->h-119 + ((game->chain->vy-background->h+119-43-game->layerList[1]->scrollDest.y)/89)*89;
			if(housex+movingHouse->image->w>=pile->image->x)
			{
				housex=pile->image->x-movingHouse->image->w;
			}
			else if(housex<0)
			{
				housex=0;
			}
			moveImageTo(movingHouse->image,housex,housey);
			setLayer(game,1);
			redrawHouses = 1;
			drawImage(movingHouse->image,game);
			
		
			
			setLayer(game,2);
			
			if(canPlace(houseList,houseNum,movingHouse,background->h))
			{
				
				setToFrame(movingHouse->image,1,0);
				
				if(game->chain->mouse==1)
				{
					setToFrame(movingHouse->image,0,0);
					game->chain->mouse = 2;
					if(houseMove==1)
					{
						addHouseToQueue(queue,movingHouse,recipeList->list[craft-1],pile,itemList);
						movingHouse->posX = housex;
						movingHouse->posY = housey;
						
						for(i=0;i<5;i++)
						{
							levels[i]-=recipeList->list[craft-1]->levels[i];
						}
						if(movingHouse->type == 1)
						{
							unlockTier(2,recipeList);
							unlockTier(3,recipeList);
							recipeList->list[0]->unlocked = 0;
							wonderModifier++;
						}
						if(movingHouse->type== 4)
						{
							specialization = 1;
							lockTier(3,recipeList);
						}
						if(movingHouse->type== 5)
						{
							specialization = 2;
							lockTier(3,recipeList);
						}
						if(movingHouse->type== 6)
						{
							specialization = 4;
							lockTier(3,recipeList);
						}
						if(movingHouse->type== 9)
						{
							specialization = 3;
							lockTier(3,recipeList);
						}
						if(movingHouse->type == 8)
						{
							recipeList->list[7]->unlocked = 0;
							wonderModifier++;
						}
					}
					else if (houseMove==2)
					{
						for(j=0;j<movingHouse->size;j++)
						{
							if(movingHouse->pals[j]->status == 0)
							{
								moveImageTo(houseList[i]->pals[j]->image,movingHouse->posX+movingHouse->image->w/2,houseList[i]->pals[j]->image->y);
							}
						}
					}
					houseMove = 0;
				}
			}
			else
			{
				setToFrame(movingHouse->image,2,0);
				if(game->chain->mouse==1)
				{
					game->chain->mouse = 2;
					if(houseMove==2)
					{
						setToFrame(movingHouse->image,0,0);
						moveImageTo(movingHouse->image,movingHouse->posX,movingHouse->posY);
					}
					houseMove = 0;
				}
				
			}
			
		}	
		else if(inMenu != 0)
		{
			if(inMenu == 1)
			{
				setLayer(game,3);
				clearLayer(game);
				if(isClicked(&close->destPos,game,3))
				{
					if(game->chain->mouse==1)
					{
						inMenu = 0;
						game->chain->mouse = 2;
						redrawMenu = 1;
					}
					clickable = 1;
				}
				
				drawImage(forgeLeftMenu,game);
				drawStatusBar(totalPals,totalCapacity,190,330,20,20,'v',game);
				drawImage(palLevelOutline,game);
				k=forgeLeftMenu->w - 10;
				p=game->height-10;
				q = 0;
				for(i=0;i<houseNum;i++)
				{

					for(j=0;j<houseList[i]->size;j++)
					{
						drawAdventurerAtPoint(houseList[i]->pals[j],game,k,p);
						k+=houseList[i]->pals[j]->image->w*7/10+(q%4)*2-4;
						if(k>game->boundingBox.w)
						{
							k=forgeLeftMenu->w - 10;
							p-=houseList[i]->pals[j]->image->h*7/10;
							q++;
						}
					}
				}
				drawImage(close,game);
				setLayer(game,2);
			}
			if (inMenu ==2)
			{
				setLayer(game,3);
				clearLayer(game);
				if(isClicked(&close->destPos,game,3))
				{
					if(game->chain->mouse==1)
					{
						inMenu = 0;
						game->chain->mouse = 2;
						redrawMenu = 1;
					}
					clickable = 1;
				}
				drawLevels(levels,levelBar,game,forgeSlice,forgeInfo,font);
				drawImage(forgeLeftMenu,game);
				grabbed = drawPileDetail(pile,game,font,&scrolly, &canScroll,hasForge,tinyResources,&maxscroll, &clickable,&selectionInfo ,selectionMenu, ordering);
				if(grabbed!=NULL)
				{
					if((selectionInfo & 3)==1)
					{
						consumption = 1;
					}
					else if((selectionInfo & 3)==2)
					{
						if(grabbed->size<10)
							consumption = grabbed->size;
						else
							consumption = 10;
					}
					else if((selectionInfo & 3)==3)
					{
						consumption = grabbed->size;
					}
					for(i=0;i<consumption;i++)
					{
						levels[0]+=grabbed->item->plastic;
						levels[1]+=grabbed->item->fabric;
						levels[2]+=grabbed->item->metal;
						levels[3]+=grabbed->item->nature;
						levels[4]+=grabbed->item->technology;
						deletePileItem(grabbed);
						pile->total--;
					}
					redrawHouses = 1;
					grabbed = NULL;
				}
				drawImage(close,game);
				setLayer(game,2);
			}
			if (inMenu == 3)
			{
				setLayer(game,3);
				clearLayer(game);
				if(isClicked(&close->destPos,game,3))
				{
					if(game->chain->mouse==1)
					{
						inMenu = 0;
						game->chain->mouse = 2;
						redrawMenu = 1;
					}
					clickable = 1;
				}
				craft = drawRecipes(recipeList,game,font,recipeImg,recipeBlack,itemList,pile,&scrolly, &canScroll,tinyResources,levels,&maxscroll, &clickable);
				drawImage(close,game);
				if(game->chain->mouse==1 && isClicked(&close->destPos,game,3))
				{
					game->chain->mouse = 2;
				}
				else if(craft>0)
				{
					inMenu = 0;
					redrawMenu = 1;
					houseMove = 1;
					movingHouse = newHouseByName(game,0,0,recipeList->list[craft-1]->name);
					game->chain->mouse = 2;
				}
				
				setLayer(game,2);
			}
			else if (inMenu == 4)
			{
				setLayer(game,3);
				clearLayer(game);
				if(isClicked(&close->destPos,game,3))
				{
					if(game->chain->mouse==1)
					{
						inMenu = 0;
						game->chain->mouse = 2;
						redrawMenu = 1;
					}
					clickable = 1;
				}
				if(flumfEatActive)
				{
					drawFeedBackground(game,bkgrnd,flumfAnim, menuImg,flumfEatActive);
					flumfEatActive--;
				}
				else
					drawFeedBackground(game,bkgrnd,flumfAnim,menuImg,0);
				
				grabbed = drawPileDetail(pile,game,font,&scrolly, &canScroll,hasForge,tinyResources, &maxscroll, &clickable,&selectionInfo ,selectionMenu, ordering);
				drawImage(close,game);
				if(grabbed != NULL)
				{
					if((selectionInfo & 3)==1)
					{
						consumption = 1;
					}
					else if((selectionInfo & 3)==2)
					{
						if(grabbed->size<10)
							consumption = grabbed->size;
						else
							consumption = 10;
					}
					else if((selectionInfo & 3)==3)
					{
						consumption = grabbed->size;
					}
					for(i=0;i<consumption;i++)
					{
						flumfEatActive = 30;
						flumfEntropy += grabbed->item->value*desireMods;
						deletePileItem(grabbed);
						pile->total--;
					}
					grabbed = NULL;
					redrawHouses = 1;
					while(flumfEntropy > flumfDesire)
					{
						flumfEntropy -= flumfDesire;
						flumfDesire *= (1.045);
						eggSize++;
						visEggSize++;
						if(queuedEggs == NULL)
							queuedEggs = malloc(sizeof(Adventurer*));
						else
							queuedEggs = realloc(queuedEggs,sizeof(Adventurer*)*eggSize);
						queuedEggs[eggSize - 1]=newAdventurer(1,game);
						moveImageTo(queuedEggs[eggSize - 1]->image,queuedEggs[eggSize - 1]->image->x,game->boundingBox.h-30 - queuedEggs[eggSize - 1]->image->h);
						
					}
					
				}
				setLayer(game,2);
			}
			else if(inMenu == 5)
			{
				setLayer(game,3);
				clearLayer(game);
				if(isClicked(&close->destPos,game,3))
				{
					if(game->chain->mouse==1)
					{
						inMenu = 0;
						game->chain->mouse = 2;
						redrawMenu = 1;
					}
					clickable = 1;
				}
				
				drawBackgroundTile(game,bkgrnd);
				drawImage(menuImg,game);
				if(growItem == NULL)
				{
					grabbed = drawPileDetailSub(pile,game,font,&scrolly, &canScroll,hasForge,tinyResources,0,0,0,1,0,&maxscroll, &clickable,&selectionInfo ,selectionMenu);
					if(grabbed!=NULL)
					{
						grabbed->size--;
						growItem = grabbed->item;
						growTimer = 400;
					}
				}
				else
				{
					if(clickable == 0)
					{
						drawPileDetailSub(pile,game,font,&scrolly, &canScroll,hasForge,tinyResources,0,0,0,1,0,&maxscroll, &clickable,&selectionInfo ,selectionMenu);
						clickable = 0;
					}
					else
					{
						drawPileDetailSub(pile,game,font,&scrolly, &canScroll,hasForge,tinyResources,0,0,0,1,0,&maxscroll, &clickable,&selectionInfo ,selectionMenu);
					}
					
					drawImage(shadowLeftMenu,game);
					if(growTimer == 0)
					{
						moveImageTo(grown,game->boundingBox.w*3/4-grown->w/2,game->boundingBox.h*3/4-grown->h);
						drawImage(grown,game);
						moveImageTo(growItem->image,grown->x-growItem->image->w/2+50,grown->y-growItem->image->w/2+10);
						drawImage(growItem->image,game);
						moveImageTo(growItem->image,grown->x-growItem->image->w/2+5,grown->y-growItem->image->w/2+75);
						drawImage(growItem->image,game);
						moveImageTo(growItem->image,grown->x-growItem->image->w/2+40,grown->y-growItem->image->w/2+100);
						drawImage(growItem->image,game);
						tempRect.x= game->boundingBox.w/2;
						tempRect.w= game->boundingBox.w/2;
						tempRect.y=0;
						tempRect.h=game->boundingBox.h;
						if(isClicked(&tempRect,game,3))
						{
							if(game->chain->mouse==1)
							{
								putInPile(pile,copyItem(growItem));
								putInPile(pile,copyItem(growItem));
								putInPile(pile,growItem);
								growItem = NULL;
							}
							clickable = 1;
						}
					}
					else
					{
						growTimer--;
						moveImageTo(growing,game->boundingBox.w*3/4-growing->w/2,game->boundingBox.h*3/4-growing->h);
						
						drawImage(growing,game);
						moveImageTo(growItem->image,growing->x-growItem->image->w/2,growing->y-growItem->image->h/2);
						drawImage(growItem->image,game);
						
					}
				}
				moveImageTo(dirtPile,game->boundingBox.w*3/4-dirtPile->w/2,game->boundingBox.h*3/4-dirtPile->h/2);
				drawImage(dirtPile,game);
				
				drawImage(close,game);
				setLayer(game,2);
			}
			else if (inMenu == 6)
			{
				setLayer(game,3);
				clearLayer(game);
				if(isClicked(&close->destPos,game,3))
				{
					if(game->chain->mouse==1)
					{
						inMenu = 0;
						game->chain->mouse = 2;
						redrawMenu = 1;
					}
					clickable = 1;
				}
				craft = drawUpgrades(upgradeList,game,font,recipeImg,recipeBlack,itemList,pile,&scrolly, &canScroll,tinyResources,levels, &maxscroll, &clickable);
				drawImage(close,game);
				if(game->chain->mouse==1 && isClicked(&close->destPos,game,3))
				{
					game->chain->mouse = 2;
				}
				else if(craft>0)
				{
					upgradeList->list[craft-1]->unlocked=0;
					if(craft==1)
					{
						moxieBonus += 2;	
					}
					else if(craft==2)
					{
						desireMods += .25;	
					}
					
					activatedUpgrades[craft-1] = 1;
					craft=0;
				}
				if(!upgradesAvailable(upgradeList))
				{
					redrawMenu =1;
					inMenu=0;
				}
				setLayer(game,2);
			}

		}
		if(!inMenu)
		{
			if(redrawMenu == 1)
			{
				setLayer(game,3);
				clearLayer(game);
				drawImage(adventureButton,game);
				drawImage(statusButton,game);
				drawImage(craftButton,game);
				drawImage(flumfButton,game);
				if(hasUpgrade)
					drawImage(upgradeButton,game);
				if(hasForge)
					drawImage(forgeButton,game);
				if(specialization == 4)
					drawImage(natureButton,game);
				if(game->layerList[0]->scrollDest.x < -scrollmin)
					drawImage(leftArrow,game);
				if(-game->layerList[0]->scrollDest.x < game->layerList[0]->scrollDest.w-game->boundingBox.w)
					drawImage(rightArrow,game);
				if(game->layerList[0]->scrollDest.y < 0)
					drawImage(upArrow,game);
				if(-game->layerList[0]->scrollDest.y < game->layerList[0]->scrollDest.h-game->boundingBox.h)
					drawImage(downArrow,game);
				setLayer(game,2);
				redrawMenu = 0;
			}
			scrolly = 0;
			canScroll = 0;
		}
		
		setLayer(game,4);
		clearLayer(game);
		moveImageTo(cursor,game->chain->vx,game->chain->vy);
		if(clickable)
		{
			setToFrame(cursor,1,0);
		}
		else
		{
			setToFrame(cursor,0,0);
		}
		drawImage(cursor,game);
		setLayer(game,2);
		
		if(errorTimer > 0)
		{
			errorTimer--;
			drawImage(error,game);
		}

		
		display(game);
		clearLayer(game);
		//disabled hover-scroll
		/*if( game->chain->y > game->chain->winheight*9/10)
		{
			if((canScroll & 2)  == 2)
				scrolly -=4;
			if(scrolly > 0)
			{
				scrolly = 0;
			}
			if(maxscroll>=0)
			{
				scrolly = 0;
			}
			else if(scrolly < maxscroll)
			{
				scrolly = maxscroll;
			}
		}
		else if(game->chain->y < game->chain->winheight*1/10)
		{
			if((canScroll & 1) == 1)
			{
				scrolly +=4;
			}
			if(scrolly > 0)
			{
				scrolly = 0;
			}
			if(maxscroll>=0)
			{
				scrolly = 0;
			}
			else if(scrolly < maxscroll)
			{
				scrolly = maxscroll;
			}
		}*/
		if(game->chain->scrolly != 0)
		{
			if(game->chain->scrolly>0 && (canScroll&1) == 1)
			{
				scrolly += (game->chain->scrolly)*24;
				
			}
			else if(game->chain->scrolly<0 && (canScroll&2) == 2)
			{
				scrolly += (game->chain->scrolly)*24;
				
			}
			
			if(scrolly > 0)
			{
				scrolly = 0;
			}
			if(maxscroll>=0)
			{
				scrolly = 0;
			}
			else if(scrolly < maxscroll)
			{
				scrolly = maxscroll;
			}
			game->chain->scrolly = 0;
		}
		if(game->chain->window)
		{
			resizeWindow(game);
			game->layerList[3]= newLayer(game,game->boundingBox.w,game->boundingBox.h);
			redrawMenu=1;
			moveImageTo(rightArrow,game->boundingBox.w-rightArrow->w,(game->boundingBox.h-rightArrow->h)/2);
			moveImageTo(upArrow,game->boundingBox.w/2-upArrow->w/2,0);
			moveImageTo(downArrow,game->boundingBox.w/2-downArrow->w/2,(game->boundingBox.h-downArrow->h));
			moveImageTo(adventureButton, game->boundingBox.w - adventureButton->w,game->boundingBox.h - adventureButton->h);
			moveImageTo(close, game->boundingBox.w - close->w,0);
			while(-game->layerList[0]->scrollDest.x > game->layerList[0]->scrollDest.w-game->boundingBox.w)
			{
				moveLayer(game,4,0,0);	
				moveLayer(game,4,0,1);	
				moveLayer(game,4,0,2);
			}
		}		
	}
	
	//Game Save Stuff goes Here
	if(newGame>=0)
	{
		savefile = fopen("save0.sav","w");
		fprintf(savefile,"time %ld\n",time(NULL));
		fprintf(savefile,"eggs %d\n",eggSize);
		fprintf(savefile,"flumf %lld %lf\n",flumfEntropy,flumfDesire);
		fprintf(savefile,"resources %d %d %d %d %d\n",levels[0],levels[1],levels[2],levels[3],levels[4]);

		for(i=0;i<upgradeList->size;i++)
		{
			if(activatedUpgrades[i] == 1)
			{
				fprintf(savefile,"upgrade %d\n",i+1);
			}
		}

		for(i=0;i<houseNum;i++)
		{
			fprintf(savefile,"house %s %d %d %d\n",houseFromType(houseList[i]->type),houseList[i]->size+houseList[i]->pending,houseList[i]->posX,houseList[i]->posY);
			for(j=0;j<houseList[i]->size+houseList[i]->pending;j++)
			{
				fprintf(savefile, "pal %d %d %d\n",houseList[i]->pals[j]->wonder,houseList[i]->pals[j]->chutzpah,houseList[i]->pals[j]->moxie);
				if(houseList[i]->pals[j]->item!=NULL && houseList[i]->pals[j]->status == 2)
					putInPile(pile,houseList[i]->pals[j]->item);
			}
		}
		//Process queue
		itemSave(pile,savefile);
		
		fclose(savefile);
	}
	endGame(game);
	return 0;
}

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
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
	int specializations = 0;
	int endTimer = 0;
	Font * font;
	Image* background, *flumfImg, *menuImg, * recipeImg,* recipeBlack;
	Image* smolEgg, * bkgrnd, * flumfAnim, * error;
	Image *newGameImage, *continueGameImage, *mainMenu, *forgeInfo;
	Image * adventureButton,*craftButton,*leftArrow,*rightArrow,*upArrow,*downArrow, *close, *forgeButton, *natureButton,*clothingButton;
	Image * statusButton, *flumfButton, *upgradeButton, *moveButton, * metalButton, *plasticButton, *techButton;
	Image * levelBar, *forgeSlice, *forgeLeftMenu, *tinyResources, *palLevelOutline, * ordering, *redo;
	Image * poof, *looplaser;
	Image * palImage;
	Image * palImageFight;
	int flumfAuto = 0;
	Image * dirtPile, *growing, *grown, *fuelGauge;
	Image * gumball, * gumballCover, *gumballTurner;
	Image * mineGear1, * mineGear2, * mineBlock;
	Image * gameBox;
	Item * minetemp = NULL;
	Item * enemyObject = NULL;
	SDL_Rect tempRect;
	int growTimer = 0;
	Item * growItem = NULL;
	Image * growBuilding= NULL;
	Item * mineItem = NULL;
	Image * mineBuilding= NULL;
	Item * plasticItem = NULL;
	Image * plasticBuilding= NULL;
	Item * techItem = NULL;
	Image * techBuilding= NULL;
	AmbulationTowerList * ambulationTowerList;
	int flumfEatDelay = 0;
	FlyingItem * flyingItems = NULL;
	int loop = 0;
	double musicVol = 1;
	double sfxVol = 0.75;
	
	int exit = 0;
	
	int exists2 = 0;
	
	int growAutoHarvest = 0;
	int growCanAutoReplant = 0;
	int growAutoReplant = 0;
	int growTimerMax = 400;
	int growTimerMods = 0;
	
	int gumballTimer = 0;
	int gumballDelay = 0;
	
	int levels[5]={0};

	Image * selectionMenu;

	char * activatedUpgrades;
	char * activatedClothing;

	int scrollmin = 2048;
	
	double desireMods = 1;
	
	int enemyTimer = 0;
	int enemyTimerMax = 240;
	
	int playerDamage = 10;

	int maxscroll = 0;

	int moxieBonus = 0;
	int chutzpahBonus = 0;
	int wonderBonus = 0;
	
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

	int volClick = 0;
	miniCookieGame * cookieGame;

	long long int flumfEntropy = 0;
	double flumfDesire = 16;
	Image * cursor,* halfGrey, * settingsBackground, * volSlider, *exitButton;
	
	SDL_Rect temprect;
	
	int scrolly = 0, canScroll = 0, animWhistle = 0,returnTiming = 0;
	
	Adventurer **  queuedEggs = NULL;
	Adventurer **  palsPullingPals = NULL;
	
	int palpullcapacity = 10,palpullsize = 0;
	int eggSize = 0;
	int visEggSize = 0;
	
	RecipeList * recipeList;
	UpgradeList * upgradeList;
	UpgradeList * clothingList;
	
	int specialization = 0;
	int houseMove = 0;
	
	Effect * effects = NULL;

	looseItemList * looseList = initLooseList();
	
	House * movingHouse = NULL;
	
	int clickable = 0;
	int grabable = 0;
	int attackable = 0;
	
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
	
	int fuelLevel = 0;
	int fuelMax = 50000;
	int mineTimer =0;
	int mineTimerMax =350;
	int mineAutoConsume = 0;
	int mineCanAutoConsume = 0;
	
	
	
	Mix_Chunk * backgroundMusic, * eating, * event, *whistle, *minigameMusic;
	Mix_Chunk * attackSound, * buttonSound, *dieSound, *spawnSound, *loopSound;
	
	srand(time(NULL));
	SDL_ShowCursor(SDL_DISABLE);

	recipeList = getRecipeList("lists/recipeList.txt",game);
	upgradeList = getUpgradeList("lists/upgradeList.txt",game);
	clothingList = getUpgradeList("lists/clothingrecipeList.txt",game);
	
	activatedUpgrades = calloc(sizeof(char),upgradeList->size);
	activatedClothing = calloc(sizeof(char),clothingList->size);
	
	

	palsPullingPals = malloc(sizeof(Adventurer*)*palpullcapacity);
	

	
	background = loadImage("images/background.png",1,4,game);
	forgeInfo = loadImage("images/forge-types.png",5,1,game);
	
	looplaser = loadImage("images/looplaser.png",1,1,game);
	
	palLevelOutline = loadImage("images/levels-outline.png",1,1,game);
	moveImageTo(palLevelOutline,0,10);
	
	poof = loadImage("images/poof.png",3,2,game);
	
	levelBar = loadImage("images/levels.png",5,2,game);
	forgeSlice = loadImage("images/forge-slice.png",1,1,game);
	forgeLeftMenu = loadImage("images/forge-left-menu.png",1,1,game);
	tinyResources = loadImage("images/tiny-resources.png",3,5,game);

	halfGrey = loadImage("images/halfgrey.png",1,1,game);
	exitButton = loadImage("images/quitButton.png",1,1,game);
	settingsBackground = loadImage("images/settingsMenu.png",1,1,game);
	volSlider = loadImage("images/volSlider.png",1,1,game);

	bkgrnd = loadImage("images/background-tile.png",4,1,game);
	flumfImg = loadImage("images/flumf.png",4,1,game);
	menuImg = loadImage("images/menu.png",1,1,game);
	recipeImg = loadImage("images/recipeMenu.png",8,6,game);
	recipeBlack = loadImage("images/recipeBlack.png",8,6,game);
	smolEgg = loadImage("images/craftables/egg-smol.png",1,1,game);
	flumfAnim = loadImage("images/flumftestanim.png", 4, 2,game);
	error = loadImage("images/error.png", 1, 1,game);
	palImage = loadImage("images/pals/1.png",8,3,game);
	palImageFight = loadImage("images/pals/2.png",8,3,game);
	adventureButton = loadImage("images/buttons/whistleboy.png",2,2,game);

	close = loadImage("images/buttons/close.png",1,1,game);
	redo = loadImage("images/buttons/redo.png",2,1,game);
	craftButton = loadImage("images/buttons/recipe.png",1,1,game);
	moveButton = loadImage("images/buttons/move.png",1,1,game);
	upgradeButton = loadImage("images/buttons/upgrade.png",1,1,game);
	forgeButton = loadImage("images/buttons/forge.png",1,1,game);
	statusButton = loadImage("images/buttons/party.png",1,1,game);
	natureButton = loadImage("images/buttons/nature.png",1,1,game);
	techButton = loadImage("images/buttons/tech.png",1,1,game);
	clothingButton = loadImage("images/buttons/clothing.png",1,1,game);
	metalButton = loadImage("images/buttons/metal.png",1,1,game);
	plasticButton = loadImage("images/buttons/plastic.png",1,1,game);
	flumfButton = loadImage("images/buttons/flumf.png",1,1,game);
	leftArrow = loadImage("images/buttons/l-arrow.png", 1,1,game);
	rightArrow = loadImage("images/buttons/r-arrow.png", 1,1,game);
	upArrow = loadImage("images/buttons/u-arrow.png", 1,1,game);
	downArrow = loadImage("images/buttons/d-arrow.png", 1,1,game);
	
	cursor = loadImage("images/pointer.png", 4, 1,game);
	mainMenu = loadImage("images/flumf-menu.png", 1, 1,game);
	newGameImage = loadImage("images/newgame.png", 1, 1,game);
	continueGameImage = loadImage("images/continuegame.png", 1, 2,game);
	selectionMenu = loadImage("images/buttons/selection.png", 3, 2,game);
	ordering = loadImage("images/buttons/ordering.png", 2, 2,game);
	
	gumball = loadImage("images/gumball.png",2,1,game);
	gumballCover = loadImage("images/gumball-covering.png",1,1,game);
	gumballTurner = loadImage("images/gumball-handle.png",1,1,game);
	
	dirtPile = loadImage("images/dirt-pile.png", 1,1,game);
	growing = loadImage("images/leaf1.png", 1,1,game);
	grown = loadImage("images/leaf2.png", 1,1,game);
	fuelGauge = loadImage("images/fluid-level.png", 1,8,game);
	mineGear1 = loadImage("images/gear1.png", 1,1,game);
	mineGear2 = loadImage("images/gear2.png", 1,1,game);
	mineBlock = loadImage("images/mine.png", 1,1,game);
	gameBox = loadImage("images/gameFlumf.png",1,1,game);

	font = newFont(loadImage("images/font.png",13,5,game));

	

	backgroundMusic = Mix_LoadWAV("sounds/backgroundMusic.wav");
	minigameMusic = Mix_LoadWAV("sounds/miniGameMusic.wav");
	eating = Mix_LoadWAV("sounds/eating.wav");
	whistle = Mix_LoadWAV("sounds/whistle.wav");
	event = Mix_LoadWAV("sounds/aThing.wav");
	attackSound = Mix_LoadWAV("sounds/attack.wav");
	buttonSound = Mix_LoadWAV("sounds/buttonClick.wav");
	dieSound = Mix_LoadWAV("sounds/die.wav");
	spawnSound = Mix_LoadWAV("sounds/enemySpawn.wav");
	loopSound = Mix_LoadWAV("sounds/gameLoop.wav");
	
	

	moveImageTo(flumfImg, background->w-flumfImg->w,background->h-30-flumfImg->h);
	
	
	moveImageTo(statusButton,statusButton->w+10,0);
	moveImageTo(craftButton,(craftButton->w+10)*2,0);
	moveImageTo(moveButton,(moveButton->w+10)*3,0);
	moveImageTo(forgeButton,(forgeButton->w+10)*0,(upgradeButton->h+10));
	moveImageTo(upgradeButton,(upgradeButton->w+10)*1,(upgradeButton->h+10));
	moveImageTo(natureButton,(natureButton->w+10)*2,(natureButton->h+10));
	moveImageTo(techButton,(natureButton->w+10)*2,(natureButton->h+10));
	moveImageTo(clothingButton,(natureButton->w+10)*2,(natureButton->h+10));
	moveImageTo(metalButton,(natureButton->w+10)*2,(natureButton->h+10));
	moveImageTo(plasticButton,(natureButton->w+10)*2,(natureButton->h+10));
	
	itemList = getList("lists/itemList.txt",game);
	housey = background->h-119;
	
	enemylist=malloc(sizeof(Enemy*)*enemylistcapacity);
	setToFrame(background,0,loop);
	setToFrame(bkgrnd,loop,0);
	setToFrame(flumfImg,loop,0);
	setToLoop(itemList,loop);
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
				Mix_PlayChannel(1,buttonSound, 0);
				newGame=1;
				game->chain->mouse =2;
			}
			clickable = 1;
		}
		else if(savefile && isClicked(&(continueGameImage->destPos),game,0) )
		{
			if(game->chain->mouse ==1)
			{
				Mix_PlayChannel(1,buttonSound, 0);
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
	swapLayers(game,0,6);
	moveLayer(game,0,-256,0);
	moveLayer(game,0,-256,1);
	moveLayer(game,0,-256,2);

	Mix_Volume(0,MIX_MAX_VOLUME*musicVol);
	Mix_Volume(1,MIX_MAX_VOLUME*sfxVol);
	housex = pile->image->x-100;
	if(newGame == 1)
	{
		Mix_PlayChannel(0,backgroundMusic, -1);
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
		Mix_PlayChannel(0,backgroundMusic, -1);
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
			else if(usefulstrcmp(temp,"sound"))
			{
				sscanf(temp,"sound %lf %lf\n",&sfxVol,&musicVol);
				Mix_Volume(0,MIX_MAX_VOLUME*musicVol);
				Mix_Volume(1,MIX_MAX_VOLUME*sfxVol);
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
				
				else if(upgrade==3)
				{
					flumfAuto += 10;	
				}
				else if(upgrade==4)
				{
					chutzpahBonus += 20;	
				}
				else if(upgrade==5)
				{
					wonderBonus += 1;	
				}
				else if(upgrade==6)
				{
					scrollmin -= 1024;	
				}
				else if(upgrade==7)
				{
					growTimerMods += 50;	
				}
				else if(upgrade==8)
				{
					growAutoHarvest = 1;
					upgradeList->list[8]->unlocked =1;
				}
				else if(upgrade==9)
				{
					growAutoReplant = 1;
					growCanAutoReplant = 1;
				}
				activatedUpgrades[upgrade-1] = 1;
			}
			else if(usefulstrcmp(temp,"clothing-upgrade"))
			{
				int upgrade;
				sscanf(temp,"clothing-upgrade %d",&upgrade);
				clothingList->list[upgrade-1]->unlocked=0;
				activatedClothing[upgrade-1] = 1;
				if(upgrade==1)
				{
					if(activatedClothing[1]==1 && activatedClothing[2]==1)
					{
						palImage = loadImage("images/pals/1hgs.png",8,3,game);
					}
					else if(activatedClothing[1]==1)
					{
						palImage = loadImage("images/pals/1hg.png",8,3,game);
					}
					else if(activatedClothing[2]==1)
					{
						palImage = loadImage("images/pals/1hs.png",8,3,game);
					}
					else
					{
						palImage = loadImage("images/pals/1h.png",8,3,game);
					}
					chutzpahBonus+=250;
					
				}
				else if(upgrade==2)
				{
					if(activatedClothing[0]==1 && activatedClothing[2]==1)
					{
						palImage = loadImage("images/pals/1hgs.png",8,3,game);
					}
					else if(activatedClothing[0]==1)
					{
						palImage = loadImage("images/pals/1hg.png",8,3,game);
					}
					else if(activatedClothing[2]==1)
					{
						palImage = loadImage("images/pals/1gs.png",8,3,game);
					}
					else
					{
						palImage = loadImage("images/pals/1g.png",8,3,game);
					}
					wonderBonus += 2;
				}
				else if(upgrade==3)
				{
					if(activatedClothing[1]==1 && activatedClothing[0]==1)
					{
						palImage = loadImage("images/pals/1hgs.png",8,3,game);
					}
					else if(activatedClothing[1]==1)
					{
						palImage = loadImage("images/pals/1gs.png",8,3,game);
					}
					else if(activatedClothing[0]==1)
					{
						palImage = loadImage("images/pals/1hs.png",8,3,game);
					}
					else
					{
						palImage = loadImage("images/pals/1s.png",8,3,game);
					}
					moxieBonus+=5;
				}
			}
			else if(usefulstrcmp(temp,"item"))
			{
				sscanf(temp,"item %s %d",tempItem,&i);
				putMultInPile(pile,getItemByName(tempItem,itemList),i);
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
					specialization += 1;
					specializations++;
					if(specialization&8 != 8)
						moveImageTo(natureButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&16 != 16)
						moveImageTo(techButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&2 != 2)
						moveImageTo(clothingButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&4 != 4)
						moveImageTo(metalButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&1 != 1)
						moveImageTo(plasticButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(loop==0 ||(loop ==1 && specializations>=2) || (loop == 2 && specializations>=3))
						lockTier(3,recipeList);
					unlockTier(4,recipeList);
					plasticBuilding = houseList[houseNum-1]->image;
					unlockUpgradeTier(7,upgradeList);
					wonderModifier++;
				}
				if(houseList[houseNum-1]->type == 5)
				{
					specialization += 2;
					specializations++;
					if(specialization&8 != 8)
						moveImageTo(natureButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&16 != 16)
						moveImageTo(techButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&2 != 2)
						moveImageTo(clothingButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&4 != 4)
						moveImageTo(metalButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&1 != 1)
						moveImageTo(plasticButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(loop==0 ||(loop ==1 && specializations>=2) || (loop == 2 && specializations>=3))
						lockTier(3,recipeList);
					unlockTier(4,recipeList);
					unlockUpgradeTier(6,upgradeList);
					wonderModifier++;
				}
				if(houseList[houseNum-1]->type == 6)
				{
					specialization += 8;
					specializations++;
					if(specialization&8 != 8)
						moveImageTo(natureButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&16 != 16)
						moveImageTo(techButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&2 != 2)
						moveImageTo(clothingButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&4 != 4)
						moveImageTo(metalButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&1 != 1)
						moveImageTo(plasticButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(loop==0 ||(loop ==1 && specializations>=2) || (loop == 2 && specializations>=3))
						lockTier(3,recipeList);
					unlockTier(4,recipeList);
					growBuilding = houseList[houseNum-1]->image;
					unlockUpgradeTier(3,upgradeList);
					upgradeList->list[8]->unlocked =0;
					wonderModifier++;
				}
				if(houseList[houseNum-1]->type == 9)
				{
					specialization += 4;
					specializations++;
					if(specialization&8 != 8)
						moveImageTo(natureButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&16 != 16)
						moveImageTo(techButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&2 != 2)
						moveImageTo(clothingButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&4 != 4)
						moveImageTo(metalButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&1 != 1)
						moveImageTo(plasticButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(loop==0 ||(loop ==1 && specializations>=2) || (loop == 2 && specializations>=3))
						lockTier(3,recipeList);
					unlockTier(4,recipeList);
					mineBuilding = houseList[houseNum-1]->image;
					unlockUpgradeTier(4,upgradeList);
					wonderModifier++;
				}
				if(houseList[houseNum-1]->type == 11)
				{
					specialization += 16;
					specializations++;
					if(specialization&8 != 8)
						moveImageTo(natureButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&16 != 16)
						moveImageTo(techButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&2 != 2)
						moveImageTo(clothingButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&4 != 4)
						moveImageTo(metalButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(specialization&1 != 1)
						moveImageTo(plasticButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
					if(loop==0 ||(loop ==1 && specializations>=2) || (loop == 2 && specializations>=3))
						lockTier(3,recipeList);
					unlockTier(4,recipeList);
					techBuilding = houseList[houseNum-1]->image;
					unlockUpgradeTier(5,upgradeList);
					wonderModifier++;
				}
				if(houseList[houseNum-1]->type == 8)
				{
					unlockHouse(recipeList, houseName,0);
					wonderModifier++;
					hasUpgrade = 1;
				}
				if(houseList[houseNum-1]->type==10)
				{
					houseList[houseNum-1]->capacity=30;
					houseList[houseNum-1]->pals = malloc(sizeof(Adventurer)*houseList[houseNum-1]->capacity);
				}
			
			
				for(i = 0; i < numPalsinHouse;i++)
				{
					fgets(temp,80,savefile);
					if(feof(savefile))
						return(1);
					if(houseList[houseNum-1]->type==10)
						addPaltoHouse(houseList[houseNum-1],newAdventurer(2,game),game);
					else
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
	else
	{
		exit = 1;
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
	moveLayer(game,-scrollmin,0,3);
	cookieGame = initMiniCookieGame(0, 0,game);
	
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
	ambulationTowerList = createAmbulationList(houseList,houseNum);
	drawImagePyramid(smolEgg,game,visEggSize,flumfImg->x,background->h-30,7);
	//Drawing menu
	setLayer(game,3);


	//set to active movement layer
	setLayer(game,2);
	while(!game->chain->close || !exit)
	{
		clickable = 0;
		attackable = 0;
		grabable = 0;
		getEvents(game->chain, game);
		
		if(!game->chain->close)
		{
			
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
						Mix_PlayChannel(1,event, 0);
						houseNum++;
						houseList = realloc(houseList,sizeof(House*)*houseNum);
						houseList[houseNum-1] = queue->queue[i]->queued;
						totalCapacity+=houseList[houseNum-1]->capacity;
						if(houseList[houseNum-1]->type==1)
							hasForge = 1;
						if(houseList[houseNum-1]->type==8)
						{
							unlockHouse(recipeList, "Upgrade",0);
							wonderModifier++;
							hasUpgrade = 1;
						}
						if(houseList[houseNum-1]->type==10)
						{
							houseList[houseNum-1]->capacity=30;
							houseList[houseNum-1]->pals = malloc(sizeof(Adventurer)*houseList[houseNum-1]->capacity);
							for(j=0;j<30;j++)
							{
								addPaltoHouse(houseList[houseNum-1],newAdventurer(2,game),game);
							}
						}
						removeFromQueue(queue,i);
						effects = newEffect(effects,poof,houseList[houseNum-1]->posX,houseList[houseNum-1]->posY-houseList[houseNum-1]->image->h,6);
						removeQueuedLooseItems(looseList,i);
						ambulationTowerList = createAmbulationList(houseList,houseNum);
						redrawHouses = 1;
					} 
				}
									
			}		
			
					
			if(!inMenu && isClicked(&adventureButton->destPos,game,4))
			{
				if(game->chain->mouse == 1)
				{
					
					game->chain->mouse =2;
					if(animWhistle == 0)
					{
						Mix_PlayChannel(1,whistle, 0);
						j=0;
						for(i=0;i<houseNum;i++)
						{
							for(k=0;k<houseList[i]->size;k++)
							{
								if(houseList[i]->pals[k]->status == 0)
								{
									houseList[i]->pals[k]->status = 1;
									houseList[i]->pals[k]->posToY = background->h-30 - houseList[i]->pals[k]->image->h;
									houseList[i]->pals[k]->posToX = 0;
									houseList[i]->pals[k]->timer = 50;
									houseList[i]->pals[k]->delay = 5*j;
									j++;
									chutzpahGravyBoat += getChutzpah(houseList[i]->pals[k])+chutzpahBonus;
									wonderTeaSet +=getWonder(houseList[i]->pals[k])+wonderBonus;
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
			
			if(flumfAuto>0 && currOutPals*100/currTotalPals < flumfAuto)
			{

				for(i=0;i<houseNum;i++)
				{
					for(k=0;k<houseList[i]->size;k++)
					{
						if(houseList[i]->pals[k]->status == 0)
						{
							houseList[i]->pals[k]->status = 1;
							houseList[i]->pals[k]->posToY = background->h-30 - houseList[i]->pals[k]->image->h;
							houseList[i]->pals[k]->posToX = 0;
							houseList[i]->pals[k]->timer = 50;
							houseList[i]->pals[k]->delay = 5*k;
							chutzpahGravyBoat += getChutzpah(houseList[i]->pals[k])+chutzpahBonus;
							wonderTeaSet +=getWonder(houseList[i]->pals[k])+wonderBonus;
							currOutPals++;
							currExploringPals++;
							if(currOutPals*100/currTotalPals >= flumfAuto)
								break;
						}
					}
					if(currOutPals*100/currTotalPals >= flumfAuto)
								break;
				}
		
			}
			
			for(i=0;i<houseNum;i++)
			{
				for(j=0;j<houseList[i]->size;j++)
				{
					if(houseList[i]->pals[j]->status == 1 && houseList[i]->pals[j]->type == 1)
					{
						if(houseList[i]->pals[j]->timer == 0 && returnTiming == 0)
						{
							if(pendingItem == NULL)
							{
								if(specialization&1 == 1)
								{
									if(rand()%100==0)
									{
										pendingItem = getItemByName("flumf-coin",itemList);
									}
									else
									{
										pendingItem = getItemGroup(itemList,generateRandomLogVal(ceil(wonderTeaSet/currExploringPals)+1+wonderModifier)-2,chutzpahGravyBoat);
									}
								}
								else
								{
									pendingItem = getItemGroup(itemList,generateRandomLogVal(ceil(wonderTeaSet/currExploringPals)+1+wonderModifier)-2,chutzpahGravyBoat);
								}
								pendingChutzpah = pendingItem->mass;
								houseList[i]->pals[j]->item = pendingItem;
								avgmoxie += getMoxie(houseList[i]->pals[j],moxieBonus);
							}
							houseList[i]->pals[j]->status = -1;//They're yanking it REAL HAERD
							avgmoxie += getMoxie(houseList[i]->pals[j],moxieBonus);
							pendingChutzpah -= getChutzpah(houseList[i]->pals[j])+chutzpahBonus;
							chutzpahGravyBoat -= getChutzpah(houseList[i]->pals[j]);
							wonderTeaSet -= getWonder(houseList[i]->pals[j])+wonderBonus;
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
									palsPullingPals[l]->posToY = background->h-30 -palsPullingPals[l]->image->h;
									palsPullingPals[l]->posToX = pile->image->x;
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
					
					if(houseList[i]->pals[j]->image->x>pile->image->x-20 && houseList[i]->pals[j]->status == 2 && houseList[i]->pals[j]->type == 1)
					{
						putInPile(pile,houseList[i]->pals[j]->item);
						redrawHouses=1;
						houseList[i]->pals[j]->posToX = houseList[i]->posX;
						houseList[i]->pals[j]->posToY = houseList[i]->posY+ houseList[i]->image->h-houseList[i]->pals[j]->image->h;
						houseList[i]->pals[j]->item = NULL;
						houseList[i]->pals[j]->status = 3;
					}
				}

			}
			
			for(i=0;i<houseNum;i++)
			{
				
				if(houseMove == 0 && inMenu == 7 && isClicked(&houseList[i]->image->destPos,game,1) && canRemove(houseList,houseNum,houseList[i]))
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
					if(houseList[i]->pals[j]->type==1)
						adventurerDo(houseList[i]->pals[j], game, itemList, houseList[i]->image->x + houseList[i]->image->w/2,houseList[i]->image->y,&currOutPals,&redrawHouses,&visEggSize,moxieBonus,scrollmin,ambulationTowerList,palImage);
					else if(houseList[i]->pals[j]->type==2)
						adventurerDo(houseList[i]->pals[j], game, itemList, houseList[i]->image->x + houseList[i]->image->w/2,houseList[i]->image->y,&currOutPals,&redrawHouses,&visEggSize,moxieBonus,scrollmin,ambulationTowerList,palImageFight);
					if(houseList[i]->pals[j]->type == 1)
					{
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
										houseList[i]->pals[j]->posToX = ((House*)(queue->queue[p]->queued))->posX+((House*)(queue->queue[p]->queued))->image->w/2;
										houseList[i]->pals[j]->posToY = ((House*)(queue->queue[p]->queued))->posY+((House*)(queue->queue[p]->queued))->image->h-houseList[i]->pals[j]->image->h;
									}
								}
							}
						}
						else if(houseList[i]->pals[j]->status == 6 && houseList[i]->pals[j]->item!=NULL)
						{
							moveImage(houseList[i]->pals[j]->item->image,0,houseList[i]->pals[j]->image->h);
							addItemToLooseList(looseList,houseList[i]->pals[j]->item,1,removeNextItemQueue(queue,houseList[i]->pals[j]->item));
							houseList[i]->pals[j]->item = NULL;
							houseList[i]->pals[j]->posToX = houseList[i]->posX;
							houseList[i]->pals[j]->posToY = houseList[i]->posY+ houseList[i]->image->h-houseList[i]->pals[j]->image->h;
							houseList[i]->pals[j]->status = 3;
							houseList[i]->pals[j]->advancedMove = 1;
							
							
						}
						else if(houseList[i]->pals[j]->status == 8)
						{
							temprect.w = smolEgg->w;
							temprect.h = smolEgg->h;
							temprect.x = houseList[i]->pals[j]->image->x + houseList[i]->pals[j]->image->w/2 - smolEgg->w/2;
							temprect.y = houseList[i]->pals[j]->image->y - smolEgg->h;
							SDL_RenderCopy(game->renderer,smolEgg->image,&smolEgg->srcPos,&temprect);	
						}
						else if(houseList[i]->pals[j]->status == 0 && (p=looseItemGrabbable(looseList))>=0)
						{
							houseList[i]->pals[j]->posToX = looseList->list[p].item->image->x;
							houseList[i]->pals[j]->posToY = looseList->list[p].item->image->y+looseList->list[p].item->image->h-houseList[i]->pals[j]->image->h;
							houseList[i]->pals[j]->status = 10;
							looseList->list[p].type = 3;
						}
						else if(houseList[i]->pals[j]->status == 11)
						{
							houseList[i]->pals[j]->item = grabItemFromLoose(looseList,houseList[i]->pals[j]->posToX);
							if(houseList[i]->pals[j]->item!=NULL)
								moveImage(houseList[i]->pals[j]->item->image,-houseList[i]->pals[j]->item->image->w/2+houseList[i]->pals[j]->image->w/2,-houseList[i]->pals[j]->image->h);
							houseList[i]->pals[j]->status = 2;
							houseList[i]->pals[j]->posToX = pile->image->x;
							houseList[i]->pals[j]->posToY = background->h-30-houseList[i]->pals[j]->image->h;
						}
						else if(houseList[i]->size + houseList[i]->pending < houseList[i]->capacity && houseList[i]->pals[j]->status == 0 && eggSize > 0)
						{
							currOutPals++;

							houseList[i]->pals[houseList[i]->size+houseList[i]->pending]= queuedEggs[eggSize - 1];
							
							moveImageTo(queuedEggs[eggSize - 1]->image,houseList[i]->image->x + houseList[i]->image->w/2 - queuedEggs[eggSize - 1]->image->x/2, queuedEggs[eggSize - 1]->image->y);
							moveImageTo(queuedEggs[eggSize - 1]->image,queuedEggs[eggSize - 1]->image->x,background->h-30 - queuedEggs[eggSize - 1]->image->h);
							houseList[i]->pals[j]->posTo = i;
							houseList[i]->pals[j]->posToX = pile->image->x+pile->image->w;
							houseList[i]->pals[j]->posToY = background->h-30-houseList[i]->pals[j]->image->h;
							houseList[i]->pending++;
							eggSize--;
							houseList[i]->pals[j]->delay = 3*j;
							houseList[i]->pals[j]->status = 7;
						}
						else if(houseList[i]->pals[j]->status == 18)
						{
							houseList[i]->pals[j]->posToX = houseList[houseList[i]->pals[j]->posTo]->posX;
							houseList[i]->pals[j]->posToY = houseList[houseList[i]->pals[j]->posTo]->posY + houseList[houseList[i]->pals[j]->posTo]->image->h-houseList[i]->pals[j]->image->h;
							houseList[i]->pals[j]->status = 8;
						}
						else if(houseList[i]->pals[j]->status == 9)
						{
							houseList[houseList[i]->pals[j]->posTo]->pending--;
							houseList[houseList[i]->pals[j]->posTo]->size++;
							totalPals++;
							currTotalPals++;
							houseList[i]->pals[j]->status = 3;
							houseList[i]->pals[j]->posToX = houseList[i]->posX;
							houseList[i]->pals[j]->posToY = houseList[i]->posY+ houseList[i]->image->h-houseList[i]->pals[j]->image->h;
							currOutPals--;
						}
					}
					else if(houseList[i]->pals[j]->type == 2)
					{
						if(enemylistsize>0 && enemylist[0]->image->x > scrollmin)
						{
							if(houseList[i]->pals[j]->status == 0)
							{
								houseList[i]->pals[j]->posToX = enemylist[0]->image->x;
								houseList[i]->pals[j]->posToY = enemylist[0]->image->y+enemylist[0]->image->h-houseList[i]->pals[j]->image->h;
								houseList[i]->pals[j]->delay = 3*j;
								houseList[i]->pals[j]->status = 1;
							}
							else if(houseList[i]->pals[j]->status == 1)
							{
								houseList[i]->pals[j]->posToX = enemylist[0]->image->x;
								houseList[i]->pals[j]->posToY = enemylist[0]->image->y+enemylist[0]->image->h-houseList[i]->pals[j]->image->h;
								if(abs(houseList[i]->pals[j]->posToX-houseList[i]->pals[j]->image->x)<20 && abs(houseList[i]->pals[j]->posToY-houseList[i]->pals[j]->image->y)<20)
								{
									houseList[i]->pals[j]->posTo = 1;
									enemylist[0]->health--;
								}
								else
								{
									houseList[i]->pals[j]->posTo = 0;
								}
							}
						}
						else
						{
							if(houseList[i]->pals[j]->status == 1)
							{
								houseList[i]->pals[j]->status = 2;
								houseList[i]->pals[j]->posToX = houseList[i]->posX;
								houseList[i]->pals[j]->posToY = houseList[i]->posY+ houseList[i]->image->h-houseList[i]->pals[j]->image->h;
							}
						}
					}
				}
				if(houseList[i]->size+houseList[i]->pending< houseList[i]->capacity && eggSize > 0)
				{
					l=0;
					for(k=0;k<i;k++)
					{
						for(l=0;l<houseList[k]->size ;l++)
						{
							if(houseList[k]->pals[l]->status==0 && houseList[k]->pals[l]->type == 1)
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
						moveImageTo(queuedEggs[eggSize - 1]->image,queuedEggs[eggSize - 1]->image->x,background->h-30 - queuedEggs[eggSize - 1]->image->h);
						houseList[k]->pals[l]->posTo = i;
						houseList[k]->pals[l]->posToX = pile->image->x+pile->image->w;
						houseList[k]->pals[l]->posToY = background->h-30-houseList[k]->pals[l]->image->h;
						houseList[i]->pending++;
						eggSize--;
						houseList[k]->pals[l]->delay = 3*l;
						houseList[k]->pals[l]->status = 7;
					}
				}
			}
			
			
			for(i = 0; i< looseList->size;i++)
			{
				drawImage(looseList->list[i].item->image,game);
			}
			//Enemy Behaviour Start
			
			for(i =0;i<enemylistsize;i++)
			{
				enemyDo(enemylist[i],game,itemList,0,0,&redrawHouses,&visEggSize,ambulationTowerList);
				if(enemylist[i]->type ==1)
				{
					if(enemylist[i]->status ==1 && enemylist[i]->image->x>pile->image->x)
					{
						enemylist[i]->status = 2;
						enemylist[i]->item = takeItemFromPile(pile);
						redrawHouses= 1;
						if(enemylist[i]->item != NULL)
							moveImageTo(enemylist[i]->item->image,enemylist[i]->image->x-enemylist[i]->item->image->w/2+enemylist[i]->image->w/2,enemylist[i]->image->y - enemylist[i]->item->image->h);
					}
					
					temprect.x=enemylist[i]->image->x-enemylist[i]->image->w*3/2;
					temprect.y=enemylist[i]->image->y-enemylist[i]->image->h*3/2;
					temprect.w=enemylist[i]->image->w*3;
					temprect.h=enemylist[i]->image->h*3;
					SDL_RenderDrawRect(game->renderer,&temprect);
					if (!inMenu && isClicked(&temprect,game,2))
					{
						if(game->chain->mouse ==1)
						{
							game->chain->mouse = 2;
							enemylist[i]->health -=playerDamage;
							Mix_PlayChannel(1,attackSound, 0);
						}
						attackable = 1;
					}
					if((enemylist[i]->status ==2 && enemylist[i]->image->x<0) || enemylist[i]->health<=0)
					{
						
						if(enemylist[i]->health <= 0)
						{
							if(rand()%10==0)
							{
								enemyObject = getItemByName("black_pointy",itemList);
								moveImageTo(enemyObject->image,enemylist[i]->image->x-enemyObject->image->w/2+enemylist[i]->image->w/2,enemylist[i]->image->y);
								addItemToLooseList(looseList,enemyObject,2,0);
							}
							Mix_PlayChannel(1,dieSound, 0);
						}
						if(enemylist[i]->health<=0 && enemylist[i]->item !=NULL)
						{
							moveImage(enemylist[i]->item->image,0,enemylist[i]->image->h);
							addItemToLooseList(looseList,enemylist[i]->item,2,0);
							
					
						}
						for(int j=i+1;j<enemylistsize;j++)
						{
							enemylist[j-1]=enemylist[j];
						}
						enemylistsize--;
					}
				}
				else if(enemylist[i]->type==2)
				{
					if(enemylist[i]->status ==1 && abs(enemylist[i]->image->x-enemylist[i]->posToX)<30)
					{
						enemylist[i]->status = 2;
						enemylist[i]->timer = 148;
						setToFrame(enemylist[i]->image,0,0);
					}
					
					temprect.x=enemylist[i]->image->x-enemylist[i]->image->w*3/2;
					temprect.y=enemylist[i]->image->y-enemylist[i]->image->h*3/2;
					temprect.w=enemylist[i]->image->w*3;
					temprect.h=enemylist[i]->image->h*3;
					SDL_RenderDrawRect(game->renderer,&temprect);
					if (!inMenu && isClicked(&temprect,game,2))
					{
						if(game->chain->mouse ==1)
						{
							game->chain->mouse = 2;
							enemylist[i]->health -=playerDamage;
						}
						attackable = 1;
					}
					if(enemylist[i]->status ==2)
					{
						enemylist[i]->timer--;
						if(enemylist[i]->timer==48)
						{
							setToFrame(enemylist[i]->image,0,2);
							enemylist[i]->image->animTime = 0;
						}
						else if(enemylist[i]->timer<=48)
							animateRangeSpeed(enemylist[i]->image,16,23,6);
						if(enemylist[i]->timer == 0 && houseList[enemylist[i]->class]->size>1)
						{
							houseList[enemylist[i]->class]->size--;
							totalPals--;
							flumfDesire /= (1.075);
							enemylist[i]->timer = 148;
							setToFrame(enemylist[i]->image,0,0);
						}
					}
					if((enemylist[i]->status ==3 && enemylist[i]->image->x<0) || enemylist[i]->health<=0)
					{
						exists2=0;
						if(enemylist[i]->health <= 0)
						{
							if(rand()%10==0)
							{
								enemyObject = getItemByName("black_blocky",itemList);
								moveImageTo(enemyObject->image,enemylist[i]->image->x-enemyObject->image->w/2+enemylist[i]->image->w/2,enemylist[i]->image->y);
								addItemToLooseList(looseList,enemyObject,2,0);
							}
							Mix_PlayChannel(1,dieSound, 0);
						}
						if(enemylist[i]->health<=0 && enemylist[i]->item !=NULL)
						{
							moveImage(enemylist[i]->item->image,0,enemylist[i]->image->h);
							addItemToLooseList(looseList,enemylist[i]->item,2,0);
						}
						for(int j=i+1;j<enemylistsize;j++)
						{
							enemylist[j-1]=enemylist[j];
						}
						enemylistsize--;
					}
				
				}
				
				
			}
			if(enemyTimer == 0)
			{
				if(totalPals*10<pile->total && pile->total > 90)
				{
					enemylistsize++;
					Mix_PlayChannel(1,spawnSound, 0);
					if(enemylistsize>=enemylistcapacity)
					{
						enemylistcapacity*=2;
						enemylist = realloc(enemylist,enemylistcapacity*sizeof(Enemy*));
					}
					enemylist[enemylistsize-1] = newEnemy(1,game);
					enemylist[enemylistsize-1]->status = 1;
					enemylist[enemylistsize-1]->posToX = pile->image->x;
					enemylist[enemylistsize-1]->posToY = background->h-30-enemylist[enemylistsize-1]->image->h;
					moveImageTo(enemylist[enemylistsize-1]->image,-20,background->h - 30- enemylist[enemylistsize-1]->image->h);
				}
				enemyTimer = enemyTimerMax;
			}
			else
			{
				if(enemyTimer==enemyTimerMax/2)
				{
					if(totalPals*2>totalCapacity && !exists2)
					{
						Mix_PlayChannel(1,spawnSound, 0);
						enemylistsize++;
						exists2=1;
						if(enemylistsize>=enemylistcapacity)
						{
							enemylistcapacity*=2;
							enemylist = realloc(enemylist,enemylistcapacity*sizeof(Enemy*));
						}
						
						enemylist[enemylistsize-1] = newEnemy(2,game);
						enemylist[enemylistsize-1]->status = 1;
						for (k =0;k<houseNum;k++)
						{
							if(houseList[k]->size>1 && houseList[k]->type!=10)
							{
								enemylist[enemylistsize-1]->class = k;
								enemylist[enemylistsize-1]->posToX = houseList[k]->posX;
								enemylist[enemylistsize-1]->posToY = houseList[k]->posY+ houseList[k]->image->h-enemylist[enemylistsize-1]->image->h;
								break;
							}
						}
						moveImageTo(enemylist[enemylistsize-1]->image,-20,background->h - 30- enemylist[enemylistsize-1]->image->h);
						if(k==houseNum)
						{
							enemylist[enemylistsize-1]->health = 0;
						}
					}
				}
				enemyTimer--;
			}
			
			//Enemy Behaviour End
			
			//Farm - growing items behaviour
			if((specialization&8)==8 && growItem != NULL)
			{
				if(growTimer == 0)
				{
					if(growAutoHarvest)
					{
						moveImageTo(growItem->image,growBuilding->x,growBuilding->y+growBuilding->h-growItem->image->h);
						addItemToLooseList(looseList,copyItem(growItem),2,0);
						addItemToLooseList(looseList,copyItem(growItem),2,0);
						if(growAutoReplant)
						{
							growTimer = growTimerMax-growTimerMods;
						}
						else
						{
							addItemToLooseList(looseList,growItem,2,0);
							growItem = NULL;

						}
					}
				}
				else
				{
					growTimer--;
				}
			}
			//Farm Behaviour End
			
			//Mine Behaviour
			if((specialization&4)==4)
			{
				if(fuelLevel>0)
				{
					fuelLevel-=4;
					if(fuelLevel <0)
					{
						fuelLevel = 0;
					}
					mineTimer--;
					if(mineTimer <= 0)
					{
						mineTimer = mineTimerMax;
						minetemp = getItemGroup(itemList,9,9999);
						if(minetemp!=NULL)
						{
							moveImageTo(minetemp->image,mineBuilding->x,mineBuilding->y+mineBuilding->h-minetemp->image->h);
							addItemToLooseList(looseList,minetemp,2,0);
						}
						minetemp = NULL;
					}
				}
			}
			//Mine Behaviour End
			//Gumball Behaviour Start
			
			//ProcessAnimations
			effects = processEffects(effects,game);
			
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
									if(houseList[p]->pals[q]->status == 0 && houseList[p]->pals[q]->type == 1 && queue->queue[i]->itemstatus[k] == 0)
									{

										queue->queue[i]->itemstatus[k] = 1;
										houseList[p]->pals[q]->status = 4;
										houseList[p]->pals[q]->timer = 150;
										houseList[p]->pals[q]->delay = 20*j;
										j++;
										if(queue->queue[i]->type == 1)
										{
											
											houseList[p]->pals[q]->posToX = pile->image->x;
											houseList[p]->pals[q]->posToY = background->h-30-houseList[p]->pals[q]->image->h;
										}
										else if(queue->queue[i]->type == 2)
											houseList[p]->pals[q]->posTo = queue->queue[i]->returnpos;
										
										currOutPals++;
									}
								}
							}
						}
					}
				}
			}
			
			if(!inMenu && isClicked(&flumfButton->destPos,game,4))
			{
				if(game->chain->mouse == 1 )
				{
					Mix_PlayChannel(1,buttonSound, 0);
				inMenu = 4;
				game->chain->mouse = 2;
				}
				clickable = 1;
			}
			else if(!inMenu && isClicked(&statusButton->destPos,game,4))
			{
				if(game->chain->mouse == 1 )
				{
					Mix_PlayChannel(1,buttonSound, 0);
				inMenu = 1;
				game->chain->mouse = 2;
				}
				clickable = 1;
			}
			else if(!inMenu && isClicked(&moveButton->destPos,game,4))
			{
				if(game->chain->mouse == 1 )
				{
					Mix_PlayChannel(1,buttonSound, 0);
				inMenu = 7;
				game->chain->mouse = 2;
				}
				clickable = 1;
			}
			else if(!inMenu && isClicked(&craftButton->destPos,game,4))
			{
				if(game->chain->mouse == 1 )
				{
					Mix_PlayChannel(1,buttonSound, 0);
				inMenu = 3;
				game->chain->mouse = 2;
				}
				clickable = 1;
			}
			else if(!inMenu && isClicked(&forgeButton->destPos,game,4) && hasForge)
			{
				if(game->chain->mouse == 1 )
				{
					Mix_PlayChannel(1,buttonSound, 0);
				inMenu = 2;
				game->chain->mouse = 2;
				}
				clickable = 1;
			}
			else if(!inMenu && isClicked(&natureButton->destPos,game,4) && (specialization&8) == 8)
			{
				if(game->chain->mouse == 1 )
				{
					Mix_PlayChannel(1,buttonSound, 0);
				inMenu = 5;
				game->chain->mouse = 2;
				}
				clickable = 1;
			}
			else if(!inMenu && isClicked(&techButton->destPos,game,4) && (specialization&16) == 16)
			{
				if(game->chain->mouse == 1 )
				{
					Mix_PlayChannel(1,buttonSound, 0);
					Mix_HaltChannel(0);
					Mix_PlayChannel(0,minigameMusic, -1);
				inMenu = 12;
				game->chain->mouse = 2;
				}
				clickable = 1;
			}
			else if(!inMenu && isClicked(&clothingButton->destPos,game,4) && (specialization&2) == 2)
			{
				if(game->chain->mouse == 1 )
				{
					Mix_PlayChannel(1,buttonSound, 0);
				inMenu = 10;
				game->chain->mouse = 2;
				}
				clickable = 1;
			}
			else if(!inMenu && isClicked(&metalButton->destPos,game,4) && (specialization&4) == 4)
			{
				if(game->chain->mouse == 1 )
				{
					Mix_PlayChannel(1,buttonSound, 0);
				inMenu = 8;
				game->chain->mouse = 2;
				}
				clickable = 1;
			}
			
			else if(!inMenu && isClicked(&plasticButton->destPos,game,4) && (specialization&1) == 1)
			{
				if(game->chain->mouse == 1 )
				{
					Mix_PlayChannel(1,buttonSound, 0);
				inMenu = 11;
				game->chain->mouse = 2;
				}
				clickable = 1;
			}
			
			else if(!inMenu && isClicked(&upgradeButton->destPos,game,4) && hasUpgrade)
			{
				if(game->chain->mouse == 1 )
				{
					Mix_PlayChannel(1,buttonSound, 0);
				inMenu = 6;
				game->chain->mouse = 2;
				}
				clickable = 1;
			}
			

			
			if((!inMenu || inMenu==7) && isClicked(&rightArrow->destPos,game,4))
			{
				if(-game->layerList[0]->scrollDest.x < game->layerList[0]->scrollDest.w-game->boundingBox.w)
				{
					moveLayer(game,-4,0,0);	
					moveLayer(game,-4,0,1);	
					moveLayer(game,-4,0,2);
					moveLayer(game,-4,0,3);
					redrawMenu = 1;
					clickable = 1;
				}
				
			}
			else if((!inMenu || inMenu==7) && isClicked(&leftArrow->destPos,game,4))
			{
				if(game->layerList[0]->scrollDest.x < -scrollmin)
				{
					moveLayer(game,4,0,0);	
					moveLayer(game,4,0,1);	
					moveLayer(game,4,0,2);
					moveLayer(game,4,0,3);
					redrawMenu = 1;
					clickable = 1;
				}
			}
			else if((!inMenu || inMenu==7)&& isClicked(&upArrow->destPos,game,4))
			{
				if(game->layerList[0]->scrollDest.y < 0)
				{
					moveLayer(game,0,4,0);	
					moveLayer(game,0,4,1);	
					moveLayer(game,0,4,2);
					moveLayer(game,0,4,3);
					redrawMenu = 1;
					clickable = 1;
				}
			}
			else if((!inMenu || inMenu==7)&& isClicked(&downArrow->destPos,game,4))
			{
				if(-game->layerList[0]->scrollDest.y < game->layerList[0]->scrollDest.h-game->boundingBox.h)
				{
					moveLayer(game,0,-4,0);	
					moveLayer(game,0,-4,1);	
					moveLayer(game,0,-4,2);
					moveLayer(game,0,-4,3);
					redrawMenu = 1;
					clickable = 1;
				}
			}
			
			else if(inMenu != 0)
			{
				if(inMenu == 1)
				{
					setLayer(game,4);
					clearLayer(game);
					if(isClicked(&close->destPos,game,4))
					{
						if(game->chain->mouse==1)
						{
							Mix_PlayChannel(1,buttonSound, 0);
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
					setLayer(game,4);
					clearLayer(game);
					
					if(isClicked(&close->destPos,game,4))
					{
						if(game->chain->mouse==1)
						{
							Mix_PlayChannel(1,buttonSound, 0);
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
						Mix_PlayChannel(1,buttonSound, 0);
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
							flyingItems = newFlyingItem(flyingItems,grabbed->item->image,game->chain->vx,game->chain->vy-grabbed->item->image->h/2,game->boundingBox.w/2,game->boundingBox.h,i*4,0);
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
					flyingItems = processFlyingItems(flyingItems,game);
					drawImage(close,game);
					setLayer(game,2);
				}
				else if (inMenu == 3)
				{
					setLayer(game,4);
					clearLayer(game);
					if(isClicked(&close->destPos,game,4))
					{
						if(game->chain->mouse==1)
						{
							Mix_PlayChannel(1,buttonSound, 0);
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
					else if(craft>0 && craft !=13)
					{
						inMenu = 7;
						redrawMenu = 1;
						houseMove = 1;
						movingHouse = newHouseByName(game,0,0,recipeList->list[craft-1]->name);
						game->chain->mouse = 2;
					}
					else if (craft == 13)
					{
						loop++;
						clearToLoop(pile,loop);
						specialization = 0;
						specializations = 0;
						Mix_PlayChannel(1,loopSound, 0);
						palImage = loadImage("images/pals/1.png",8,3,game);
						growItem = NULL;
						growBuilding= NULL;
						mineItem = NULL;
						mineBuilding= NULL;
						plasticItem = NULL;
						plasticBuilding= NULL;
						techItem = NULL;
						techBuilding= NULL;
						flumfEatDelay = 0;
						growAutoHarvest = 0;
						growCanAutoReplant = 0;
						growAutoReplant = 0;
						growTimerMax = 400;
						growTimerMods = 0;
						gumballTimer = 0;
						gumballDelay = 0;
						for(i=0;i<5;i++)
						{
							levels[i]= 0;
						}	
						activatedUpgrades = calloc(sizeof(char),upgradeList->size);
						activatedClothing = calloc(sizeof(char),clothingList->size);

						enemylistsize = 0;

						endTimer = 50;

						totalPals = 0;
						totalCapacity = 0;
						hasForge = 0;
						hasUpgrade = 0;
						queue = initProdQueue();
						pendingItem = NULL;
						flumfEatActive = 0;
						houseList = NULL;
						houseNum = 0;
						flumfEntropy = 0;
						flumfDesire = 16;
						scrolly = 0;
						canScroll = 0;
						animWhistle = 0;
						returnTiming = 0;
						queuedEggs = NULL;
						palsPullingPals = malloc(sizeof(Adventurer*)*palpullcapacity);
						palpullcapacity = 10;
						palpullsize = 0;
						eggSize = 0;
						visEggSize = 0;
						looseList = initLooseList();
						inMenu = 0;
						craft = 0; 
						avgmoxie = 0;
						wonderTeaSet = 0;
						wonderModifier = 0;
						currOutPals = 0;
						currTotalPals = 0;
						currExploringPals = 0;
						chutzpahGravyBoat = 0;
						pendingChutzpah = 0;
						redrawHouses = 1; 
						redrawMenu = 1;
						fuelLevel = 0;
						mineTimer =0;
						mineAutoConsume = 0;
						mineCanAutoConsume = 0;
						unlockUpgradeTier(1,upgradeList);
						lockUpgradeTier(2,upgradeList);
						lockUpgradeTier(3,upgradeList);
						lockUpgradeTier(4,upgradeList);
						lockUpgradeTier(5,upgradeList);
						lockUpgradeTier(6,upgradeList);
						lockUpgradeTier(7,upgradeList);
						lockUpgradeTier(8,upgradeList);
						lockUpgradeTier(9,upgradeList);
						lockUpgradeTier(10,upgradeList);

						unlockTier(1,recipeList);
						lockTier(2,recipeList);
						lockTier(3,recipeList);
						lockTier(4,recipeList);

						moveImageTo(natureButton,(natureButton->w+10)*2,(natureButton->h+10));
						moveImageTo(techButton,(natureButton->w+10)*2,(natureButton->h+10));
						moveImageTo(clothingButton,(natureButton->w+10)*2,(natureButton->h+10));
						moveImageTo(metalButton,(natureButton->w+10)*2,(natureButton->h+10));
						moveImageTo(plasticButton,(natureButton->w+10)*2,(natureButton->h+10));

						setToFrame(background,0,loop);
						setToFrame(bkgrnd,loop,0);
						setToFrame(flumfImg,loop,0);
						setToLoop(itemList,loop);


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

						inMenu = 13;
						
					}
					
					setLayer(game,2);
				}
				else if (inMenu == 4)
				{
					setLayer(game,4);
					clearLayer(game);
					if(isClicked(&close->destPos,game,4))
					{
						if(game->chain->mouse==1)
						{
							Mix_PlayChannel(1,buttonSound, 0);
							inMenu = 0;
							game->chain->mouse = 2;
							redrawMenu = 1;
						}
						clickable = 1;
					}
					if(flumfEatActive)
					{
						drawFeedBackground(game,bkgrnd,flumfAnim, menuImg,flumfEatActive,flumfEatDelay);
						if(flumfEatDelay<=0)
							flumfEatActive--;
						if(flumfEatDelay==1)
						{
							Mix_PlayChannel(1,eating, 0);
						}
						if(flumfEatDelay>0)
							flumfEatDelay--;
					}
					else
						drawFeedBackground(game,bkgrnd,flumfAnim,menuImg,0,1);
					
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
							flyingItems = newFlyingItem(flyingItems,grabbed->item->image,game->chain->vx,game->chain->vy-grabbed->item->image->h/2,game->boundingBox.w-140,game->boundingBox.h-130-grabbed->item->image->h/2,i*4,0);
							if(flumfEatDelay==0)
							{
								flumfEatDelay = (game->boundingBox.w-140-game->chain->vx)/7-10;
								flumfEatActive = 30+4*consumption;
							}
							else
							{
								flumfEatActive=(game->boundingBox.w-140-game->chain->vx)/7-10-flumfEatDelay+30+4*consumption;
							}
						
							
							flumfEntropy += grabbed->item->value*desireMods;
							deletePileItem(grabbed);
							pile->total--;
						}
						grabbed = NULL;
						redrawHouses = 1;
						while(flumfEntropy > flumfDesire)
						{
							flumfEntropy -= flumfDesire;
							flumfDesire *= (1.025);
							eggSize++;
							visEggSize++;
							if(queuedEggs == NULL)
								queuedEggs = malloc(sizeof(Adventurer*));
							else
								queuedEggs = realloc(queuedEggs,sizeof(Adventurer*)*eggSize);
							queuedEggs[eggSize - 1]=newAdventurer(1,game);
							moveImageTo(queuedEggs[eggSize - 1]->image,queuedEggs[eggSize - 1]->image->x,background->h-30 - queuedEggs[eggSize - 1]->image->h);
							
						}
						
					}
						flyingItems = processFlyingItems(flyingItems,game);
						
					setLayer(game,2);
				}
				else if(inMenu == 5)
				{
					setLayer(game,4);
					clearLayer(game);
					if(isClicked(&close->destPos,game,4))
					{
						if(game->chain->mouse==1)
						{
							Mix_PlayChannel(1,buttonSound, 0);
							inMenu = 0;
							game->chain->mouse = 2;
							redrawMenu = 1;
						}
						clickable = 1;
					}
					drawBackgroundTile(game,bkgrnd);
					moveImageTo(redo,game->boundingBox.w-20-redo->w,close->h+20);
					if(growCanAutoReplant)
					{
						if(isClicked(&redo->destPos,game,4))
						{
							if(game->chain->mouse==1)
							{
								growAutoReplant = !growAutoReplant;
								game->chain->mouse = 2;
							}
							clickable = 1;
						}
						setToFrame(redo,!growAutoReplant,0);
						drawImage(redo,game);
					}
					flyingItems = processFlyingItems(flyingItems,game);

					if(growItem == NULL)
					{
						
						moveImageTo(dirtPile,game->boundingBox.w*3/4-dirtPile->w/2,game->boundingBox.h*3/4-dirtPile->h/2);
						drawImage(dirtPile,game);
						drawImage(menuImg,game);
						grabbed = drawPileDetailSub(pile,game,font,&scrolly, &canScroll,hasForge,tinyResources,0,0,0,1,0,&maxscroll, &clickable,&selectionInfo ,selectionMenu, ordering);
						if(grabbed!=NULL)
						{
							Mix_PlayChannel(1,buttonSound, 0);
							flyingItems = newFlyingItem(flyingItems,grabbed->item->image,game->chain->vx,game->chain->vy-grabbed->item->image->h/2,game->boundingBox.w,game->boundingBox.h/2,i*4,0);

							grabbed->size--;
							growItem = grabbed->item;
							growTimer = 400;
						}
					}
					else
					{
						
						if(growTimer == 0)
						{
							moveImageTo(grown,game->boundingBox.w/2-grown->w/2,game->boundingBox.h*3/4-grown->h);
							drawImage(grown,game);
							moveImageTo(growItem->image,grown->x-growItem->image->w/2+50,grown->y-growItem->image->w/2+10);
							drawImage(growItem->image,game);
							moveImageTo(growItem->image,grown->x-growItem->image->w/2+5,grown->y-growItem->image->w/2+75);
							drawImage(growItem->image,game);
							moveImageTo(growItem->image,grown->x-growItem->image->w/2+40,grown->y-growItem->image->w/2+100);
							drawImage(growItem->image,game);
							tempRect.x= game->boundingBox.w/4;
							tempRect.w= game->boundingBox.w/2;
							tempRect.y=0;
							tempRect.h=game->boundingBox.h;
							if(growAutoHarvest)
							{
								
							}
							else if(isClicked(&tempRect,game,5))
							{
								if(game->chain->mouse==1)
								{
									Mix_PlayChannel(1,buttonSound, 0);
									moveImageTo(growItem->image,growBuilding->x,growBuilding->y+growBuilding->h-growItem->image->h);
									addItemToLooseList(looseList,copyItem(growItem),2,0);
									addItemToLooseList(looseList,copyItem(growItem),2,0);
									if(growAutoReplant)
									{
										growTimer = growTimerMax-growTimerMods;
									}
									else
									{
										addItemToLooseList(looseList,growItem,2,0);
										growItem = NULL;
									}
								}
								clickable = 1;
							}
						}
						else
						{
							growTimer--;
							moveImageTo(growing,game->boundingBox.w/2-growing->w/2,game->boundingBox.h*3/4-growing->h);
							
							drawImage(growing,game);
							moveImageTo(growItem->image,growing->x-growItem->image->w/2,growing->y-growItem->image->h/2);
							drawImage(growItem->image,game);
							
						}
						
						moveImageTo(dirtPile,game->boundingBox.w/2-dirtPile->w/2,game->boundingBox.h*3/4-dirtPile->h/2);
						drawImage(dirtPile,game);
					}
					
					
					drawImage(close,game);
					setLayer(game,2);
				}
				else if(inMenu == 8)
				{
					setLayer(game,4);
					clearLayer(game);
					if(isClicked(&close->destPos,game,4))
					{
						if(game->chain->mouse==1)
						{
							Mix_PlayChannel(1,buttonSound, 0);
							inMenu = 0;
							game->chain->mouse = 2;
							redrawMenu = 1;
						}
						clickable = 1;
					}
					drawBackgroundTile(game,bkgrnd);
					
					moveImageTo(redo,game->boundingBox.w-20-redo->w,close->h+20);
					if(mineCanAutoConsume)
					{
						if(isClicked(&redo->destPos,game,4))
						{
							if(game->chain->mouse==1)
							{
								mineAutoConsume = !mineAutoConsume;
								game->chain->mouse = 2;
							}
							clickable = 1;
						}
						setToFrame(redo,!mineAutoConsume,0);
						drawImage(redo,game);
					}
					
					
					moveImageTo(dirtPile,game->boundingBox.w*3/4-dirtPile->w/2,game->boundingBox.h*3/4-dirtPile->h/2);
					drawImage(dirtPile,game);
					moveImageTo(mineBlock,game->boundingBox.w*3/4-mineBlock->w/2,game->boundingBox.h*3/4-dirtPile->h/2-mineBlock->h*3/4);
					drawImage(mineBlock,game);
					moveImageTo(mineGear2,game->boundingBox.w*3/4-mineBlock->w/2+18,game->boundingBox.h*3/4-dirtPile->h/2-mineBlock->h*3/4+17);
					if(fuelLevel>0)
						rotateImage(mineGear2,-1);
					drawImage(mineGear2,game);
					moveImageTo(mineGear1,game->boundingBox.w*3/4-mineBlock->w/2-15,game->boundingBox.h*3/4-dirtPile->h/2-mineBlock->h*3/4+17);
					if(fuelLevel>0)
						rotateImage(mineGear1,1);
					drawImage(mineGear1,game);
					
					moveImageTo(fuelGauge,game->boundingBox.w-fuelGauge->w,game->boundingBox.h/2);
					for(j=10;j>0;j--)
					{
						if(j==1)
						{
							if(fuelLevel*(j+1)>=fuelMax)
							{
								setToFrame(fuelGauge,0,3);
							}
							else
							{
								setToFrame(fuelGauge,0,0);
							}
							drawImage(fuelGauge,game);
							moveImage(fuelGauge,0,-fuelGauge->h);
						}
						else if(j==10)
						{
							if(fuelLevel==0)
							{
								setToFrame(fuelGauge,0,2);
							}
							else if(fuelLevel>=fuelMax/10)
							{
								setToFrame(fuelGauge,0,5);
							}
							else
							{
								setToFrame(fuelGauge,0,7);
							}
							drawImage(fuelGauge,game);
							moveImage(fuelGauge,0,-fuelGauge->h);
						}
						else
						{
							if(fuelLevel*(j)>=fuelMax)
							{
								setToFrame(fuelGauge,0,4);
							}
							else if(fuelLevel*(j+1)>=fuelMax)
							{
								setToFrame(fuelGauge,0,6);
							}
							else
							{
								setToFrame(fuelGauge,0,1);
							}
							drawImage(fuelGauge,game);
							moveImage(fuelGauge,0,-fuelGauge->h);
						}
					}
					drawImage(menuImg,game);
					grabbed = drawPileDetail(pile,game,font,&scrolly, &canScroll,hasForge,tinyResources,&maxscroll, &clickable,&selectionInfo ,selectionMenu, ordering);
					if(grabbed!=NULL)
					{
						Mix_PlayChannel(1,buttonSound, 0);
						flyingItems = newFlyingItem(flyingItems,grabbed->item->image,game->chain->vx,game->chain->vy-grabbed->item->image->h/2,game->boundingBox.w,game->boundingBox.h/2,i*4,0);

						grabbed->size--;
						fuelLevel += grabbed->item->value;
						grabbed=NULL;
					}
					
					flyingItems = processFlyingItems(flyingItems,game);

					drawImage(close,game);
					setLayer(game,2);
				}
				else if (inMenu == 6)
				{
					setLayer(game,4);
					clearLayer(game);
					if(isClicked(&close->destPos,game,4))
					{
						if(game->chain->mouse==1)
						{
							Mix_PlayChannel(1,buttonSound, 0);
							inMenu = 0;
							game->chain->mouse = 2;
							redrawMenu = 1;
						}
						clickable = 1;
					}
					craft = drawUpgrades(upgradeList,game,font,recipeImg,recipeBlack,itemList,pile,&scrolly, &canScroll,tinyResources,levels, &maxscroll, &clickable);
					drawImage(close,game);
					if(game->chain->mouse==1 && isClicked(&close->destPos,game,4))
					{
						game->chain->mouse = 2;
					}
					else if(craft>0)
					{
						Mix_PlayChannel(1,event, 0);
						upgradeList->list[craft-1]->unlocked=0;
						if(craft==1)
						{
							moxieBonus += 2;	
						}
						else if(craft==2)
						{
							desireMods += .25;	
						}
						else if(craft==3)
						{
							flumfAuto += 10;	
						}
						else if(craft==4)
						{
							chutzpahBonus += 20;	
						}
						else if(craft==5)
						{
							wonderBonus += 1;	
						}
						else if(craft==6)
						{
							scrollmin -= 1024;	
						}
						else if(craft==7)
						{
							growTimerMods += 50;	
						}
						else if(craft==8)
						{
							growAutoHarvest = 1;
							upgradeList->list[8]->unlocked =1;
						}
						else if(craft==9)
						{
							growCanAutoReplant = 1;
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
				else if(inMenu == 7)
				{
					redrawHouses = 1;
					if(isClicked(&close->destPos,game,4))
					{
						if(game->chain->mouse==1)
						{
							Mix_PlayChannel(1,buttonSound, 0);
							inMenu = 0;
							game->chain->mouse = 2;
							redrawMenu = 1;
						}
						clickable = 1;
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
									inMenu = 0;
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
										specialization += 1;
										specializations++;
										if((specialization&8) != 8)
											moveImageTo(natureButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&16) != 16)
											moveImageTo(techButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&2) != 2)
											moveImageTo(clothingButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&4) != 4)
											moveImageTo(metalButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&1) != 1)
											moveImageTo(plasticButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if(loop==0 ||(loop ==1 && specializations>=2) || (loop == 2 && specializations>=3))
											lockTier(3,recipeList);
										unlockTier(4,recipeList);
										plasticBuilding = houseList[houseNum-1]->image;
										unlockUpgradeTier(7,upgradeList);
										wonderModifier++;
									}
									if(movingHouse->type== 5)
									{
										specialization += 2;
										specializations++;
										if((specialization&8) != 8)
											moveImageTo(natureButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&16) != 16)
											moveImageTo(techButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&2) != 2)
											moveImageTo(clothingButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&4) != 4)
											moveImageTo(metalButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&1) != 1)
											moveImageTo(plasticButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if(loop==0 ||(loop ==1 && specializations>=2) || (loop == 2 && specializations>=3))
											lockTier(3,recipeList);
										unlockTier(4,recipeList);
										unlockUpgradeTier(6,upgradeList);
										wonderModifier++;
									}
									if(movingHouse->type== 6)
									{
										specialization += 8;
										specializations++;
										if((specialization&8) != 8)
											moveImageTo(natureButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&16) != 16)
											moveImageTo(techButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&2) != 2)
											moveImageTo(clothingButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&4) != 4)
											moveImageTo(metalButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&1) != 1)
											moveImageTo(plasticButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if(loop==0 ||(loop ==1 && specializations>=2) || (loop == 2 && specializations>=3))
											lockTier(3,recipeList);
										
										growBuilding = houseList[houseNum-1]->image;
										unlockUpgradeTier(3,upgradeList);
										upgradeList->list[8]->unlocked =0;
										wonderModifier++;
									}
									if(movingHouse->type== 9)
									{
										specialization += 4;
										specializations++;
										if((specialization&8) != 8)
											moveImageTo(natureButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&16) != 16)
											moveImageTo(techButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&2) != 2)
											moveImageTo(clothingButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&4) != 4)
											moveImageTo(metalButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&1) != 1)
											moveImageTo(plasticButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if(loop==0 ||(loop ==1 && specializations>=2) || (loop == 2 && specializations>=3))
											lockTier(3,recipeList);
										mineBuilding = houseList[houseNum-1]->image;
										unlockUpgradeTier(4,upgradeList);
										wonderModifier++;
									}
									if(movingHouse->type == 11)
									{
										specialization += 16;
										specializations++;
										if((specialization&8) != 8)
											moveImageTo(natureButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&16) != 16)
											moveImageTo(techButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&2) != 2)
											moveImageTo(clothingButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&4) != 4)
											moveImageTo(metalButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if((specialization&1) != 1)
											moveImageTo(plasticButton,(techButton->w+10)*(2+specializations)%3,(techButton->h+10)*(2+specializations)/3);
										if(loop==0 ||(loop ==1 && specializations>=2) || (loop == 2 && specializations>=3))
											lockTier(3,recipeList);
										unlockTier(4,recipeList);
										techBuilding = houseList[houseNum-1]->image;
										unlockUpgradeTier(5,upgradeList);
										wonderModifier++;
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
										if(movingHouse->pals[j]->status == 0 || (movingHouse->pals[j]->status == 1 && movingHouse->pals[j]->delay >0))
										{
											moveImageTo(movingHouse->pals[j]->image,movingHouse->image->x+movingHouse->image->w/2,movingHouse->image->y+movingHouse->image->h-movingHouse->pals[j]->image->h);
										}
									}
								}
								movingHouse->posX = movingHouse->image->x;
								movingHouse->posY = movingHouse->image->y;
								houseMove = 0;
								ambulationTowerList = createAmbulationList(houseList,houseNum);
							}
						}
						else
						{
							setToFrame(movingHouse->image,2,0);
							if(game->chain->mouse==1)
							{
								Mix_PlayChannel(1,buttonSound, 0);
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
					setLayer(game,4);
					clearLayer(game);
					drawImage(close,game);
				}
				else if (inMenu == 10)
				{
					setLayer(game,4);
					clearLayer(game);
					drawBackgroundTile(game,bkgrnd);
					if(isClicked(&close->destPos,game,4))
					{
						if(game->chain->mouse==1)
						{
							Mix_PlayChannel(1,buttonSound, 0);
							inMenu = 0;
							game->chain->mouse = 2;
							redrawMenu = 1;
						}
						clickable = 1;
					}
					craft = drawUpgrades(clothingList,game,font,recipeImg,recipeBlack,itemList,pile,&scrolly, &canScroll,tinyResources,levels, &maxscroll, &clickable);
					drawImage(close,game);
					if(game->chain->mouse==1 && isClicked(&close->destPos,game,4))
					{
						game->chain->mouse = 2;
					}
					else if(craft>0)
					{
						activatedClothing[craft-1] = 1;
						Mix_PlayChannel(1,event, 0);
						clothingList->list[craft-1]->unlocked=0;
						if(craft==1)
						{
							if(activatedClothing[1]==1 && activatedClothing[2]==1)
							{
								palImage = loadImage("images/pals/1hgs.png",8,3,game);
							}
							else if(activatedClothing[1]==1)
							{
								palImage = loadImage("images/pals/1hg.png",8,3,game);
							}
							else if(activatedClothing[2]==1)
							{
								palImage = loadImage("images/pals/1hs.png",8,3,game);
							}
							else
							{
								palImage = loadImage("images/pals/1h.png",8,3,game);
							}
							chutzpahBonus+=250;
							
						}
						else if(craft==2)
						{
							if(activatedClothing[0]==1 && activatedClothing[2]==1)
							{
								palImage = loadImage("images/pals/1hgs.png",8,3,game);
							}
							else if(activatedClothing[0]==1)
							{
								palImage = loadImage("images/pals/1hg.png",8,3,game);
							}
							else if(activatedClothing[2]==1)
							{
								palImage = loadImage("images/pals/1gs.png",8,3,game);
							}
							else
							{
								palImage = loadImage("images/pals/1g.png",8,3,game);
							}
							wonderBonus += 2;
						}
						else if(craft==3)
						{
							if(activatedClothing[1]==1 && activatedClothing[0]==1)
							{
								palImage = loadImage("images/pals/1hgs.png",8,3,game);
							}
							else if(activatedClothing[1]==1)
							{
								palImage = loadImage("images/pals/1gs.png",8,3,game);
							}
							else if(activatedClothing[0]==1)
							{
								palImage = loadImage("images/pals/1hs.png",8,3,game);
							}
							else
							{
								palImage = loadImage("images/pals/1s.png",8,3,game);
							}
							moxieBonus+=5;
						}
						
						craft=0;
					}
					if(!upgradesAvailable(clothingList))
					{
						redrawMenu =1;
						inMenu=0;
					}
					setLayer(game,2);
				}
				else if(inMenu == 11)
				{
					setLayer(game,4);
					clearLayer(game);
					if(isClicked(&close->destPos,game,4))
					{
						if(game->chain->mouse==1)
						{
							Mix_PlayChannel(1,buttonSound, 0);
							inMenu = 0;
							game->chain->mouse = 2;
							redrawMenu = 1;
						}
						clickable = 1;
					}
					drawBackgroundTile(game,bkgrnd);
					moveImageTo(redo,game->boundingBox.w-20-redo->w,close->h+20);
					if(gumballTimer>0)
					{
						gumballTimer--;
					}
					if(gumballTimer == 0 && gumballDelay>0)
					{
						rotateImage(gumballTurner,45);
						animateSpeed(gumball,4);
						gumballDelay--;
					}
					
					drawImage(menuImg,game);
					moveImageTo(gumball,game->boundingBox.w/2+gumball->w/2,game->boundingBox.h/2-gumball->h/4);
					moveImageTo(gumballCover,gumball->x+gumball->w/2-gumballCover->w/2,gumball->y+gumball->h-gumballCover->h);
					moveImageTo(gumballTurner,gumball->x+gumball->w/2-gumballTurner->w/2,gumball->y+gumball->h-gumballCover->h+4);
					drawImage(gumball,game);
					
					flyingItems = processFlyingItems(flyingItems,game);
				
					drawImage(gumballTurner,game);
					grabbed = drawPileDetailSingle(pile,game,font,&scrolly, &canScroll,hasForge,tinyResources,&maxscroll, &clickable,&selectionInfo ,selectionMenu, ordering,"flumf-coin");
					if(grabbed!=NULL)
					{
						Mix_PlayChannel(1,buttonSound, 0);
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
							flyingItems = newFlyingItem(flyingItems,grabbed->item->image,game->chain->vx,game->chain->vy-grabbed->item->image->h/2,gumballCover->x+gumballCover->w/2,gumballCover->y,i*4,0);
							
							grabbed->size--;
							
							plasticItem =getItemGroup(itemList,rand()%6,9999);
							
							moveImageTo(plasticItem->image,plasticBuilding->x,plasticBuilding->y+plasticBuilding->h-plasticItem->image->h);
							flyingItems = newFlyingItem(flyingItems,copyItem(plasticItem)->image,gumballCover->x+gumballCover->w-plasticItem->image->h,gumballCover->y,gumballCover->x+gumballCover->w-plasticItem->image->h,gumballCover->y-115,i*4+(gumballCover->x+gumballCover->w/2-game->chain->vx)/7,1);
							if(gumballTimer == 0)
								gumballTimer = (gumballCover->x+gumballCover->w/2-game->chain->vx)/7;
							gumballDelay+=8;
							addItemToLooseList(looseList,plasticItem,2,0);
						}
						grabbed = NULL;
					}

					
					
					
					drawImage(close,game);
					setLayer(game,2);
				}
				else if(inMenu == 12)
				{
					setLayer(game,4);
					clearLayer(game);
					if(isClicked(&close->destPos,game,4))
					{
						if(game->chain->mouse==1)
						{
							Mix_PlayChannel(1,buttonSound, 0);
							Mix_HaltChannel(0);
							Mix_PlayChannel(0,backgroundMusic, -1);
							inMenu = 0;
							game->chain->mouse = 2;
							redrawMenu = 1;
						}
						clickable = 1;
					}
					drawBackgroundTile(game,bkgrnd);
					moveImageTo(gameBox,game->boundingBox.w/2-gameBox->w/2,game->boundingBox.h-gameBox->h);
					i = cookieGame->score;
					cookieGame->x = gameBox->x + 104;
					cookieGame->y = gameBox->y + 96;
					processCookieGame(cookieGame,game, &clickable, &grabable);
					drawImage(gameBox,game);
					if(i<cookieGame->score)
					{
						fprintf(stderr,"New Item\n");
						techItem =getItemGroup(itemList,rand()%6,9999);
						
						moveImageTo(techItem->image,techBuilding->x,techBuilding->y+techBuilding->h-techItem->image->h);
						flyingItems = newFlyingItem(flyingItems,copyItem(techItem)->image,game->boundingBox.w*5/6,game->boundingBox.h,game->boundingBox.w*5/6,0,0,1);
						
						techItem =getItemGroup(itemList,rand()%6,9999);
						
						moveImageTo(techItem->image,techBuilding->x,techBuilding->y+techBuilding->h-techItem->image->h);
						flyingItems = newFlyingItem(flyingItems,copyItem(techItem)->image,game->boundingBox.w*5/6,game->boundingBox.h,game->boundingBox.w*5/6,0,8,1);
						
						techItem =getItemGroup(itemList,rand()%6,9999);
						
						moveImageTo(techItem->image,techBuilding->x,techBuilding->y+techBuilding->h-techItem->image->h);
						flyingItems = newFlyingItem(flyingItems,copyItem(techItem)->image,game->boundingBox.w*5/6,game->boundingBox.h,game->boundingBox.w*5/6,0,16,1);
					}
					flyingItems = processFlyingItems(flyingItems,game);
					drawImage(close,game);
					setLayer(game,2);
				}
				else if(inMenu == 13)
				{
					setLayer(game,4);
					clearLayer(game);
					moveImageTo(looplaser,flumfAnim->x+36,flumfAnim->y+12);
					rotateImage(looplaser,5);
					drawFeedBackground(game,bkgrnd,flumfAnim, menuImg,1,0);
					drawImage(looplaser,game);
					endTimer--;
					if(endTimer == 0)
					{
						inMenu = 0;
					}
				}

			}
			if(!inMenu)
			{
				flyingItems = NULL;
				if(redrawMenu == 1)
				{
					setLayer(game,4);
					clearLayer(game);
					drawImage(adventureButton,game);
					drawImage(statusButton,game);
					drawImage(moveButton,game);
					drawImage(craftButton,game);
					drawImage(flumfButton,game);
					if(hasUpgrade)
						drawImage(upgradeButton,game);
					if(hasForge)
						drawImage(forgeButton,game);
					if((specialization&1) == 1)
						drawImage(plasticButton,game);
					if((specialization&2) == 2)
						drawImage(clothingButton,game);
					if((specialization&4) == 4)
						drawImage(metalButton,game);
					if((specialization&8) == 8)
						drawImage(natureButton,game);
					if((specialization&16) == 16)
						drawImage(techButton,game);
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
			else if(inMenu == 7)
			{
				setLayer(game,4);
				if(game->layerList[0]->scrollDest.x < -scrollmin)
						drawImage(leftArrow,game);
				if(-game->layerList[0]->scrollDest.x < game->layerList[0]->scrollDest.w-game->boundingBox.w)
					drawImage(rightArrow,game);
				if(game->layerList[0]->scrollDest.y < 0)
					drawImage(upArrow,game);
				if(-game->layerList[0]->scrollDest.y < game->layerList[0]->scrollDest.h-game->boundingBox.h)
					drawImage(downArrow,game);
			}
		}
		setLayer(game,5);
		clearLayer(game);
		if(game->chain->close)
		{
			drawBackgroundTile(game,halfGrey);
			moveImageTo(settingsBackground,game->boundingBox.w/2-settingsBackground->w/2,0);
			drawImage(settingsBackground,game);
			moveImageTo(exitButton,game->boundingBox.w/2-exitButton->w/2,settingsBackground->y+200);
			drawImage(exitButton,game);
			drawImage(close,game);
		
			if(volClick==1)
			{
				moveImageTo(volSlider,game->chain->vx-volSlider->w/2,settingsBackground->y+94);
				
				if(volSlider->x<settingsBackground->x+120)
				{
					moveImageTo(volSlider,settingsBackground->x+120,settingsBackground->y+94);
					sfxVol = 0;
				}
				else if(volSlider->x>settingsBackground->x+120+190)
				{
					moveImageTo(volSlider,settingsBackground->x+120+190,settingsBackground->y+94);
					sfxVol = 1;
				}
				else
				{
					sfxVol =((double)volSlider->x-settingsBackground->x-120)/190 ;
				}
				Mix_Volume(1,MIX_MAX_VOLUME*sfxVol);
				
			}
			else
			{
				moveImageTo(volSlider,settingsBackground->x+120+190*sfxVol,settingsBackground->y+94);
			}
			if(isClicked(&volSlider->destPos,game,5) )
			{
				clickable = 1;
				if(game->chain->mouse == 1)
				{
					game->chain->mouse = 3;
					volClick = 1;
				}
				else if(game->chain->mouse ==2)
				{
					volClick = 0;
				}
			}
			else
			{
				if(volClick == 1)
						volClick = 0;
			}
	
			drawImage(volSlider,game);
			if(volClick==2)
			{
				
				moveImageTo(volSlider,game->chain->vx-volSlider->w/2,settingsBackground->y+140);
				
				if(volSlider->x<settingsBackground->x+120)
				{
					moveImageTo(volSlider,settingsBackground->x+120,settingsBackground->y+140);
					musicVol = 0;
				}
				else if(volSlider->x>settingsBackground->x+120+190)
				{
					moveImageTo(volSlider,settingsBackground->x+120+190,settingsBackground->y+140);
					musicVol = 1;
				}
				else
				{
					musicVol =((double)volSlider->x-settingsBackground->x-120)/190 ;
				}
				Mix_Volume(0,MIX_MAX_VOLUME*musicVol);
			}
			else
			{
				moveImageTo(volSlider,settingsBackground->x+120+190*musicVol,settingsBackground->y+140);
			}
			if(isClicked(&volSlider->destPos,game,5))
			{
				clickable = 1;
				if(game->chain->mouse == 1)
				{
					game->chain->mouse = 3;
					volClick = 2;
				}
				else if(game->chain->mouse ==2)
				{
					volClick = 0;
				}
			}
			else
			{
				if(volClick == 2)
						volClick = 0;
			}
			drawImage(volSlider,game);
			if(isClicked(&exitButton->destPos,game,5))
			{
				if(game->chain->mouse==1)
				{
					exit = 1;
				}
				clickable = 1;
			}
			if(isClicked(&close->destPos,game,5))
			{
				if(game->chain->mouse == 2)
				{
					if(volClick == 2)
						volClick = 0;
				}
				if(game->chain->mouse==1)
				{
					game->chain->mouse = 3;
					game->chain->close= 0;
					game->chain->mouse = 2;
					redrawMenu = 1;
				}
				clickable = 1;
			}
		}
		moveImageTo(cursor,game->chain->vx,game->chain->vy);
		if(attackable)
		{
			moveImage(cursor,-16,-22);
			setToFrame(cursor,2,0);
		}
		else if(grabable)
		{
			setToFrame(cursor,3,0);
		}
		else if(clickable)
		{
			setToFrame(cursor,1,0);
		}
		else
		{
			setToFrame(cursor,0,0);
		}
		drawImage(cursor,game);
		if(attackable)
		{
			moveImage(cursor,16,22);
		}
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
			game->layerList[5]= newLayer(game,game->boundingBox.w,game->boundingBox.h);
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
		fprintf(savefile,"sound %lf %lf\n",sfxVol,musicVol);
		fprintf(savefile,"eggs %d\n",eggSize);
		fprintf(savefile,"flumf %lld %lf\n",flumfEntropy,flumfDesire);
		fprintf(savefile,"resources %d %d %d %d %d\n",levels[0],levels[1],levels[2],levels[3],levels[4]);

	
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
		for(i=0;i<upgradeList->size;i++)
		{
			if(activatedUpgrades[i] == 1)
			{
				fprintf(savefile,"upgrade %d\n",i+1);
			}
		}
		for(i=0;i<clothingList->size;i++)
		{
			if(activatedClothing[i] == 1)
			{
				fprintf(savefile,"clothing-upgrade %d\n",i+1);
			}
		}

		//Process queue
		itemSave(pile,savefile);
		
		fclose(savefile);
	}
	endGame(game);
	return 0;
}

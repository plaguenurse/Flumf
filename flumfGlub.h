#ifndef __FLUMFGLUB_H
#define __FLUMFGLUB_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "flibble.h"

typedef struct Item
{
	Image * image;
	char* name;
	int mass;
	int value;
	int fabric;
	int metal;
	int nature;
	int plastic;
	int technology;
} Item;

typedef struct FlyingItem
{
	Image * image;
	int startX;
	int startY;
	int endX;
	int endY;
	int delay;
	struct FlyingItem * next;
}FlyingItem;

typedef struct AmbulationPlatform
{
	int min, max;
	struct AmbulationPlatform * next;
}AmbulationPlatform;

typedef struct AmbulationTowerList
{
	AmbulationPlatform** tiers;
}AmbulationTowerList;

typedef struct Effect
{
	Image * image;
	int timer;
	int microtimer;
	struct Effect * next;
}Effect;

typedef struct Upgrade
{
	char * name;
	char ** nameList;
	int * numList;
	char * desc;
	Image * image;
	int size;
	int levels[5];
	int unlocked;
	int tier;
	int id;
} Upgrade;

typedef struct UpgradeList
{
	Upgrade ** list;
	int size;
} UpgradeList;

typedef struct looseItem
{
	int type;
	int errata;
	Item * item;
}looseItem;

typedef struct looseItemList
{
	looseItem * list;
	int size;
	int capacity;
} looseItemList;

typedef struct PileItem
{
	Item * item;
	int volume;
	int size;
	struct PileItem * nextMass;
	struct PileItem * nextSize;
	struct PileItem * nextAlpha;	
}PileItem;

typedef struct Pile
{
	PileItem * firstAlpha;
	PileItem * firstSize;
	PileItem * firstMass;
	int size;
	int total;
	Image * image;
} Pile;

typedef struct PQueue
{
	Item ** items;
	int* itemstatus;
	int itemsize;
	int returnnum;
	int returnpos;
	char type;
	void * queued;
}PQueue;

typedef struct ProductionQueue
{
	int size;
	PQueue ** queue;
}ProductionQueue;


typedef struct Recipe
{
	char * name;
	char ** nameList;
	int * numList;
	Image * image;
	int size;
	int levels[5];
	int unlocked;
	int type;
} Recipe;

typedef struct RecipeList
{
	Recipe ** list;
	int size;
} RecipeList;


typedef struct Adventurer
{
	Image * image;
	int type;
	Item * item;
	int status;
	int class;
	int timer;
	int delay;

	short int dir;
	int negpos;
	int burdenedmoxie;
	
	int wonder;
	int chutzpah;
	int moxie;
	
	int posTo;
	int posToX;
	int posToY;
	int posToXnext;
	int posToYnext;
	int advancedMove;
	
} Adventurer;


typedef struct Enemy
{
	Image * image;
	int type;
	Item * item;
	int status;
	int class;
	int timer;
	int delay;
	int speed;
	int posToX;
	int posToY;
	short int dir;
	int negpos;
	int health;
	int posTo;

	
} Enemy;

typedef struct House
{
	Image * image;
	int type;
	int status;
	Adventurer ** pals;
	int capacity;
	int pending;
	int size;
	int posX;
	int posY;
} House;

typedef struct Object
{
	char* name;
	Image * image;
	int rarity;
	int mass;
	int value;
	int fabric;
	int metal;
	int nature;
	int plastic;
	int technology;
} Object;

typedef struct ObjectList
{
	Object*** list;
	int* size;
	int tiersize;
} ObjectList;

typedef struct ItemList
{
	Item** list;
	int size;
} ItemList;


ObjectList* freeList(ObjectList* list);
ObjectList* getList(char * fileLocation,Game * game);
Item * getItemByName(char * item, ObjectList * list);
Item * getItem(Object * object);
Adventurer * newAdventurer(int type, Game * game);
void giveAdventurerNewItem(Adventurer * pal, Object * item);
void giveAdventurerItem(Adventurer * pal, Item * item);
void giveAdventurerNewItemByName(Adventurer * pal, char * itemName, ObjectList * list);
void drawAdventurer(Adventurer * pal, Game * game, Image * palImage);
void drawAdventurerAtPoint(Adventurer * pal, Game * game,int x, int y);
void adventurerDo(Adventurer * pal, Game * game, ObjectList * list, int x, int y,int* currOutPals, char * redraw, int* eggs, int moxieBonus, int scrollmin,AmbulationTowerList * ambulationList, Image * palImage);
House * newHouseByName(Game * game, int x, int y, char * name);
House * newHouse(Game * game, int x, int y, int type);

void swappixel(SDL_Surface *surface, int x1, int y1, int x2, int y2);
int isClicked(SDL_Rect * box, Game * game, int layer);
void freeHouse(House * house);
void freeAdventurer(Adventurer * pal);
void freeItem(Item * item);
void moveAdventurer(Adventurer * pal, int x, int y);
void freeItemList(ItemList * list);
void freeChain(EventChain * chain);
RecipeList* getRecipeList(char * fileLocation, Game * game);
int printNumber(int x, int y, Game * game, Font * font, int num);
void addPaltoHouse(House * house, Adventurer * pal, Game * game);
void drawImagePyramid(Image * image, Game * game , int amount, int midx, int y,int maxwidth);
int getIndexPointforgrabby(ObjectList * list, int tier, int chutzpah);
Item * getItemGroup(ObjectList * list, int tier, int chutzpahPool);
void drawFeedBackground(Game * game, Image * bkgrnd, Image * flumf, Image * menuleft, int anim,int delay);
int dirtyTwoExponent(int power);
int dirtyLogTwo(int value);
int generateRandomLogVal(int maxVal);
int getChutzpah(Adventurer * pal);
int getMoxie(Adventurer * pal, int moxieBonus);
int getWonder(Adventurer * pal);
Image * getImageByName(char * item, ObjectList * list);
ProductionQueue * initProdQueue(void);
void processItemQueue(Item * item, ProductionQueue * queue);
House ** checkQueueHouse(ProductionQueue * queue, House ** houseList, int * houseNum);
void removeFromQueue(ProductionQueue * queue,int i);
int removeNextItemQueue(ProductionQueue * queue,Item * item);
void removeItemFromQueue(PQueue * queue,int i);
char usefulstrcmp(char* input, char * match);
void unlockTier(int tier, RecipeList * list);
void lockTier(int tier, RecipeList * list);
void drawLevels(int* levels, Image * levelBar, Game * game, Image * background, Image* bottomInfo, Font * font);
int drawStatsItem(Item * item,int x, int y,Game * game,Font * font,Image* tinyResources, int maxWidth);
int drawStats(int * levels,int x, int y,Game * game,Font * font,Image* tinyResources, int maxWidth);
int getStatWidth(int * levels,int x, int y,Game * game,Font * font,Image* tinyResources, int maxWidth);
Item * dupeItem(Item * item, ObjectList * list);
void drawStatusBar(int amount, int total, int width, int height, int x, int y,char dir,Game * game);
Enemy * newEnemy(int type, Game * game);
void enemyDo(Enemy * foe, Game * game, ObjectList * list, int x, int y, char * redraw, int* eggs,AmbulationTowerList * ambulationList);
void clearEnemy(Enemy* enemy);
void drawEnemy(Enemy * foe, Game * game);
void moveEnemy(Enemy * foe, int x, int y);
UpgradeList* getUpgradeList(char * fileLocation, Game * game);
void unlockUpgradeTier(int tier, UpgradeList * list);
int upgradesAvailable(UpgradeList * upgradeList);
int writeWordsMulti(char * words, Font * font, int size, Game * game, int x, int y, int wmax);
int canPlace(House ** houseList, int houseNum,House * movingHouse, int mapHeight);
void sortList(ObjectList * objectList);


Pile * initPile(Image * image, int x, int y,ObjectList * items);
PileItem * recurseInsertAlpha(PileItem * object,PileItem * curr);
PileItem * recurseInsertMass(PileItem * object,PileItem * curr);
PileItem * recurseInsertSize(PileItem * object,PileItem * curr);
void insertAllItems(ObjectList * items, Pile * list);
void drawPile(Pile * pile, Game * game);
void deleteItemsRecipe(Recipe * recipe, Pile * pile);
void deletePileItem(PileItem * item);
PileItem * drawPileDetailSub(Pile * pile, Game * game, Font * font, int * offsety, int * canScroll, int forge, Image * tinyResources,char plastic,char fabric,char metal,char nature, char tech,int * scrollMax, int* clickable, int * selection,Image *selectionMenu,Image * ordering);
PileItem * drawPileDetail(Pile * pile, Game * game, Font * font, int * offsety, int * canScroll, int forge, Image* tinyResources, int * scrollMax, int* clickable, int * sortandconsume,Image *selectionMenu,Image * ordering);
void putInPile(Pile * pile,Item * item);
void putMultInPile(Pile * pile,Item * item,int num);
Item * copyItem(Item * item);
Item * takeItemFromPile(Pile * pile);
int canCraft(Recipe * recipe, Pile * pile, ObjectList * list, int*levels);
int canUpgradeCraft(Upgrade * upgrade, Pile * pile, ObjectList * list, int*levels);
void deleteItemsUpgrade(Upgrade * upgrade, Pile * pile, ObjectList * list);
int drawRecipes(RecipeList * recipeList,Game * game,Font * font,Image * recipeBackground,Image * recipeBlack,ObjectList * items, Pile * pile, int * scrolly, int * canScroll, Image * tinyResources, int * levels, int * scrollmax, int* clickable);
int drawUpgrades(UpgradeList * upgradeList,Game * game,Font * font,Image * upgradeBackground,Image * upgradeBlack,ObjectList * items, Pile * pile, int * scrolly, int * canScroll, Image * tinyResources, int * levels, int * scrollmax, int* clickable);
void addHouseToQueue(ProductionQueue * queue, House * house,Recipe * recipe, Pile * pile, ObjectList * list);
void itemSave(Pile * pile, FILE * savefile);
int canRemove(House ** houseList, int houseNum,House * movingHouse);
int getStatHeight(int * levels, Image * tinyResources);
char * houseFromType(int type);
void drawBackgroundTile(Game * game, Image * tile);
void unlockHouse(RecipeList * recipeList,char * name, char unlock);
looseItemList * initLooseList();
void addItemToLooseList(looseItemList * list, Item * item, int type,int errata);
void removeItemFromLooseList(looseItemList * list, Item * item);
void removeQueuedLooseItems(looseItemList * list,int id);
Item * grabItemFromLoose(looseItemList * list, int pos);
int looseItemGrabbable(looseItemList * list);
AmbulationTowerList * createAmbulationList(House ** houseList, int houseNum);
void insertAmbulationPlatform(AmbulationTowerList * list, int level, int min, int max);
AmbulationPlatform * recurseInsertAmbulationPlatform(AmbulationPlatform * curr, int level, int min, int max);
void postAmbulationInsertionRecalibration(AmbulationTowerList * list, int level);
int canAmbulate(AmbulationTowerList * list,int x, int y, int xTo, int yTo);
int canTraverseToLevel(AmbulationTowerList * list, int levelTo, int currLevel, int xTo, int x, int min, int max);
Effect * newEffect(Effect * list,Image * image,int posX, int posY, int length);
Effect * processEffects(Effect * first,Game * game);
Effect * clearTimeredEffects(Effect * effect);
FlyingItem * newFlyingItem(FlyingItem * list,Image * image,int startX, int startY,int endX, int endY, int delay);
FlyingItem * processFlyingItems(FlyingItem * first,Game * game);
FlyingItem * clearFlyingItems(FlyingItem * item);
//needs work
void setToLoop(ObjectList* list,int loop);
//to-do
PileItem * drawPileDetailSingle(Pile * pile, Game * game, Font * font, int * offsety, int * canScroll, int forge, Image* tinyResources, int * scrollMax, int* clickable, int * selection,Image *selectionMenu,Image * ordering, char* name);


#endif

#ifndef __FLUMFGLUB_H
#define __FLUMFGLUB_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

typedef struct Upgrade
{
	char * name;
	char ** nameList;
	int * numList;
	char * desc;
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


typedef struct Clothing
{
	Image * image;
	int type;
	int id;
	int moxie;
	int chutzpah;
	int wonder;
} Clothing;

typedef struct ClothingRack
{
	Clothing ** list;
	int size;
	int type;
} ClothingRack;

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
	
	Clothing * hat;
	Clothing * head;
	Clothing * feet;
	
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
void drawAdventurer(Adventurer * pal, Game * game);
void drawAdventurerAtPoint(Adventurer * pal, Game * game,int x, int y);
void adventurerDo(Adventurer * pal, Game * game, ObjectList * list, int x, int y,int* currOutPals, char * redraw, int* eggs,int moxieBonus, int scrollmin);
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
void drawFeedBackground(Game * game, Image * bkgrnd, Image * flumf, Image * menuleft, int anim);
int dirtyTwoExponent(int power);
int dirtyLogTwo(int value);
int generateRandomLogVal(int maxVal);
Clothing * newClothing(Image * image, int type, int id);
Clothing * givePalNewHat(Clothing * hat, Adventurer * pal);
Clothing * givePalNewHead(Clothing * head, Adventurer * pal);
Clothing * givePalNewFeet(Clothing * feet, Adventurer * pal);
int getChutzpah(Adventurer * pal);
int getMoxie(Adventurer * pal, int moxieBonus);
int getWonder(Adventurer * pal);
Image * getImageByName(char * item, ObjectList * list);
void addClothes(Clothing * clothes, ClothingRack * rack);
Clothing * suckClothes(int n, ClothingRack * rack);
ProductionQueue * initProdQueue(void);
void processItemQueue(Item * item, ProductionQueue * queue);
House ** checkQueueHouse(ProductionQueue * queue, House ** houseList, int * houseNum);
void checkQueueClothing(ProductionQueue * queue, ClothingRack * hatRack, ClothingRack * headRack, ClothingRack * feetRack);
void removeFromQueue(ProductionQueue * queue,int i);
void removeNextItemQueue(ProductionQueue * queue,Item * item);
void removeItemFromQueue(PQueue * queue,int i);
char usefulstrcmp(char* input, char * match);
Clothing * dupeClothing(Clothing * clothes);
void unlockTier(int tier, RecipeList * list);
void drawLevels(int* levels, Image * levelBar, Game * game, Image * background, Image* bottomInfo, Font * font);
int drawStatsItem(Item * item,int x, int y,Game * game,Font * font,Image* tinyResources, int maxWidth);
int drawStats(int * levels,int x, int y,Game * game,Font * font,Image* tinyResources, int maxWidth);
int getStatWidth(int * levels,int x, int y,Game * game,Font * font,Image* tinyResources, int maxWidth);
Item * dupeItem(Item * item);
void drawStatusBar(int amount, int total, int width, int height, int x, int y,char dir,Game * game);
Enemy * newEnemy(int type, Game * game);
void enemyDo(Enemy * foe, Game * game, ObjectList * list, int x, int y, char * redraw, int* eggs);
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
PileItem * drawPileDetailSub(Pile * pile, Game * game, Font * font, int * offsety, int * canScroll, int forge, Image * tinyResources,char plastic,char fabric,char metal,char nature, char tech,int * scrollMax, int* clickable, int * selection,Image *selectionMenu);
PileItem * drawPileDetail(Pile * pile, Game * game, Font * font, int * offsety, int * canScroll, int forge, Image* tinyResources, int * scrollMax, int* clickable, int * sortandconsume,Image *selectionMenu,Image * ordering);
void putInPile(Pile * pile,Item * item);
Item * copyItem(Item * item);
Item * takeItemFromPile(Pile * pile);
int canCraft(Recipe * recipe, Pile * pile, ObjectList * list, int*levels);
int canUpgradeCraft(Upgrade * upgrade, Pile * pile, ObjectList * list, int*levels);
void deleteItemsUpgrade(Upgrade * upgrade, Pile * pile, ObjectList * list);
int drawRecipes(RecipeList * recipeList,Game * game,Font * font,Image * recipeBackground,Image * recipeBlack,ObjectList * items, Pile * pile, int * scrolly, int * canScroll, Image * tinyResources, int * levels, int * scrollmax, int* clickable);
int drawUpgrades(UpgradeList * upgradeList,Game * game,Font * font,Image * upgradeBackground,Image * upgradeBlack,ObjectList * items, Pile * pile, int * scrolly, int * canScroll, Image * tinyResources, int * levels, int * scrollmax, int* clickable);
void addHouseToQueue(ProductionQueue * queue, House * house,Recipe * recipe, Pile * pile, ObjectList * list);
void addClothingToQueue(ProductionQueue * queue, Clothing ** clothes,Recipe * recipe, Pile * pile, ObjectList * list, int clotheNum);
void itemSave(Pile * pile, FILE * savefile);
int canRemove(House ** houseList, int houseNum,House * movingHouse);
int getStatHeight(int * levels, Image * tinyResources);
//needs work

//to-do
void unlockHouse(RecipeList * recipeList,int i, char unlock);

#endif

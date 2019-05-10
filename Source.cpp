//Using SDL, SDL_image, standard IO, strings, and file streams
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <sstream>
#include <exception>
#include <iterator>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

//The dimensions of the level
const int LEVEL_WIDTHs = 800; //1280
const int LEVEL_HEIGHTs = 800; //960
const int LEVEL_WIDTHb = 1600; //1280
const int LEVEL_HEIGHTb = 1600; //960
int const * lw;
int const * lh;
//Tile constants
const int TILE_WIDTH = 80; //10 rows
const int TILE_HEIGHT = 80; // 10 columns
const int TOTAL_TILES = 100; //small
const int TOTAL_TILESb = 400; //big
int const * tt;
const int TOTAL_TILE_SPRITES = 5;

//The different tile sprites
const int TILE_GRASS0 = 0;
const int TILE_GRASS1 = 1;
const int TILE_GRASS2 = 2;
const int TILE_GRASS3 = 3;
const int TILE_WATER0 = 4;

//Building constants
const int BUILDING_WIDTH = 60;
const int BUILDING_HEIGHT = 60;
const int TOTAL_BUILDING_SPRITES = 1;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_DOWN = 1,
	BUTTON_SPRITE_TOTAL = 2
};

struct cameraNotInitializedException : public std::exception {
   const char * what () const throw () {
      return "The camera pointer is null: Action cancelled.";
   }
};
struct textureNotInitializedException : public std::exception {
   const char * what () const throw () {
      return "The texture pointer is null: Action cancelled.";
   }
};

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();
		void setWidth(int w);
		void setHeight(int h);

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};
//The mouse button
//The tile
class Tile
{
    public:
		//Initializes position and type
		Tile( int x, int y, int tileType );

		//Shows the tile
		void render( SDL_Rect& camera );

		//Get the tile type
		int getType();

		int getxy(int index);
		//The attributes of the tile
		SDL_Rect mBox;

		bool const operator==(Tile & tile);
    private:

		//The tile type
		int mType;

		int gX;
		int gY;
};
class Unit
{
public:
	Unit(int x, int y, int unitKind, int team);
	void resetUnitMovement();
	void resetgxy();
	void setKilled(bool killed);
	void setUnitKind(int k);
	void setUnitType(int t);
	void setUnitStr(int s);
	void setUnitMov(int m);
	void setTeam(int team);
	void setRange(int r);
	void setSelected(bool selected);
	void setCanAttack(bool attack);
	int getUnitKind();
	int getUnitType();
	int getUnitStr();
	int getUnitMov();
	int getTeam();
	int getRange();
	int getxy(int index);
	bool getSelected();
	bool canCaptureBuildings();
	bool isSummonable();
	bool killed();
	bool canAttackNow();
	std::string getinfo(int index);
	void render(SDL_Rect &camera);
	void render( SDL_Rect& camera, int index);
	void handlebuttonevents(SDL_Event * d);
	SDL_Rect uBox;
	Unit operator-(const Unit enemy); //attack
private:
	int kind;
	int type;
	int str;
	int maxstr;
	int movement;
	int maxmovement;
	int team;
	int range; //0 range means they cannot attack.
	int gX;
	int gY;
	bool summonable;
	bool isKilled;
	bool isSelected;
	bool canAttack;
	//abilities
	bool canCapture;
	bool canStrikeBack;
	bool canHealOthers;
	bool canHealSelf;
	bool canDestroyBones;
};
class LButton
{
	public:
		//Initializes internal variables
		LButton();

		//Sets top left position
		void setPosition( int x, int y );
		void setSize(int h, int w);
		//Handles mouse event
		bool handleEvent( SDL_Event* e );
		void handleEvent( SDL_Event* e , bool &quit, bool &win);
		void handleEvent(SDL_Event* e, Tile *tiles[]);
		void handleEvent(SDL_Event* e, std::vector<Tile*> tiles, std::vector<Unit> &units);
		void setButtonType(int num);
		int getButtonType();
		SDL_Rect getRect();
		//Shows button sprite
		void render(int index);

	private:
		//Top left position
		SDL_Rect mPosition;
		int buttonType;
		//Currently used global sprite
		LButtonSprite mCurrentSprite;
};
class Text {
public:
	Text(std::string text, int r, int g, int b, int x, int y, int bx, int by, int bw, int bh); //gets the rendered text.
	Text(std::string text, int x, int y, int fonts); //minimized
	Text();
	LTexture getTexture();
	std::string getText();
	void setText(std::string val);
	int getX();
	int getY();
	void drawText();
	SDL_Color color;
private:
	LTexture texture;
	SDL_Rect box;
	std::string text;
	int posx;
	int posy;
	int boxposx;
	int boxposy;
	int boxwidth;
	int boxheight;
	int fontsize;
	double angle;

};
//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		static const int DOT_VEL = 80;

		//Initializes the variables
		Dot();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the dot and check collision against tiles
		void move( Tile *tiles[], Text label);
		void move( Tile *tiles[]);

		void stopmoving();

		void setPosition(int x, int y);

		//Centers the camera over the dot
		void setCamera( SDL_Rect& camera );

		//Shows the dot on the screen
		void render( SDL_Rect& camera );

		SDL_Rect getBox();

    private:
		//Collision box of the dot
		SDL_Rect mBox;

		//The velocity of the dot
		int mVelX, mVelY;
};
class inter //The class interface is mostly used for dialog.
{
public:
		inter();
		void render();
		void render(int index); //the index specifies what type of interface to use.
		//both setposition and setsize are for ibox.
		void setPosition( int x, int y );
		void setSize(int h, int w);
		LTexture *textureptr;
private:
		SDL_Rect iBox;
};
class LTimer
{
    public:
		//Initializes variables
		LTimer();

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

    private:
		//The clock time when the timer started
		Uint32 mStartTicks;

		//The ticks stored when the timer was paused
		Uint32 mPausedTicks;

		//The timer status
		bool mPaused;
		bool mStarted;
};
class Building
{
public:
		Building(int x, int y, int buildingtype);
		int getType();
		void setType(int buildingtype);
		void setControlPoints(int points);
		void setControlled(int playerteam);
		int getControlPoints();
		int getMaxControlPoints();
		int getControlled();
		int getPower();
		int getDefense();
		std::string getinfo(int index);
		void render(SDL_Rect &camera);
		void handlebuttonevents(SDL_Event * d);
		SDL_Rect bBox;
private:
	int controlp; //initially 0.
	int controlmax;
	int iscontrolled; //greater than 0 means that it is controlled by a team. If the number is 0, then no one controls it.
	int bType;
	int defense; //the numerator out of 100.
	int power;
};
class Diety
{
};
class Player //we'll see.
{
public:
	Player(int team);
private:
	int team;
	std::vector<Unit> units;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

int findDifference(Tile *tile, Unit *unit);
int findDifference(Tile *tile1, Tile *tile2);
int findDifference(Unit *unit1, Unit *unit2);

//Frees media and shuts down SDL
void close();
void closetiles(Tile *tiles[]);
void closeBuildings(std::vector<Building> &buildings);

bool endturn(bool &win);

//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b );

//Checks collision box against set of tiles
bool touchesWall( SDL_Rect box, Tile* tiles[] );

//Sets tiles from tile map
bool setTiles( Tile *tiles[]);

bool setBuildings(std::vector<Building> &buildings);

bool setUnits(std::vector<Unit> &totalUnits);

void playerMove(SDL_Event * d, Tile *tiles[], std::vector<Unit> &units);

std::string updateTileInfo(Tile *tiles[], Dot dot, int ID); //Updates tile information where the cursor is located.
std::string updateBuildingInfo(std::vector<Building> buildings, Dot dot);
std::string updateUnitInfo(std::vector<Unit> units, Dot dot);
bool loadMap(int map);

bool loadMenu(SDL_Event e);
bool selectGods(inter &icon);
bool findUpMoves(int &currentpos, std::vector<Tile*> &validUpMoves, Tile *tiles[], Tile *selectedtile, std::vector<Unit> units, int index);
bool findDownRightMoves(int &currentpos, std::vector<Tile*> &validDownRightMoves, Tile *tiles[], Tile *selectedtile, std::vector<Unit> units);
bool findDownLeftMoves(int &currentpos, std::vector<Tile*> &validDownLeftMoves, Tile *tiles[], Tile *selectedtile, std::vector<Unit> units);
void validateDownMoves(std::vector<Tile*> &validMoves, Tile *tiles[], Tile *selectedtile, std::vector<Unit> units);
bool findRightMoves(int &currentpos, std::vector<Tile*> &validRightMoves, Tile *tiles[], Tile *selectedtile, std::vector<Unit> units);
bool findLeftMoves(int &currentpos, std::vector<Tile*> &validLeftMoves, Tile *tiles[], Tile *selectedtile, std::vector<Unit> units);

bool play(Tile *tiles[], std::vector<Building> &buildings, std::vector<Unit> &totalUnits);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

int map;

//Globally used font
TTF_Font *gFont = NULL;

//all text boxes
std::vector<Text> textptr;

std::string mapFile;

std::string bldFile;

std::string uapFile;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;
LTexture gTileTextureGrass;
LTexture gTileTextureWater;
LTexture gTileUnitMove;
LTexture gTileUnitAttack;
LTexture gTileUnitRange;
LTexture gPortalActiveTexture;
LTexture gPortalInactiveTexture;
LTexture gVillageTexture;
LTexture gTowerTexture;
SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];

//Mouse button sprites
SDL_Rect buttonquickplaysprites[ BUTTON_SPRITE_TOTAL ];
SDL_Rect buttonendturnsprites[BUTTON_SPRITE_TOTAL];
SDL_Rect buttonmenugameplaysprites[BUTTON_SPRITE_TOTAL];
SDL_Rect buttonresumesprites[BUTTON_SPRITE_TOTAL];
SDL_Rect buttonquitmenusprites[BUTTON_SPRITE_TOTAL];
SDL_Rect buttonquitdesktopsprites[BUTTON_SPRITE_TOTAL];
LTexture buttonquickplaytexture;
LTexture tileOverlayTexture;
LTexture endTurnTexture;
LTexture menuGamePlayTexture;
LTexture buttonResumeTexture;
LTexture quitToMenuTexture;
LTexture quitToDesktopTexture;

//interface sprites
LTexture bottomGameplayInterface;
LTexture unitTypeSkirmisher;
LTexture unitTypeWarrior;
LTexture unitTypeLeviathan;
LTexture unitTypeRanger;
LTexture unitTypeMagician;
LTexture unitTypeHealer;
LTexture unitTypeFlyer;

//God of goodness
LTexture godTypeAechalg;
LTexture godTypeAechall;
//God of Strength
LTexture godTypeBorgong;
LTexture godTypeBorgonl;
//God of Death
LTexture godTypeDolozg;
LTexture godTypeDolozl;
//Neutral god-alignment
LTexture godTypeNeutrall;
//God of Deserts and Light
LTexture godTypeOlshabug;
LTexture godTypeOlshabul;
//God of Darkness and Destruction
LTexture godTypeUlterachg;
LTexture godTypeUlterachl;
//God of Nature
LTexture godTypeVeinog;
LTexture godTypeVeinol;
//God of Water
LTexture godTypeWaterusg;
LTexture godTypeWaterusl;
//God of evil
LTexture godTypeZaldysg;
LTexture godTypeZaldysl;

//Units
LTexture unitTypeAoth;

LTexture statD;
Dot *cursorptr = nullptr;
SDL_Rect *cameraptr = nullptr;
LTimer *timerptr = nullptr;
Uint32 *cTimeptr = nullptr; //current time pointer
int *turnptr = nullptr;
Unit *selected = nullptr;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}


bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}


void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

void LTexture::setWidth(int w)
{
	mWidth = w;
}
void LTexture::setHeight(int h)
{
	mHeight = h;
}

Tile::Tile( int x, int y, int tileType )
{
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

	gX = x / 80;
	gY = y / 80;

    //Set the collision box
    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

    //Get the tile type
    mType = tileType;
}

void Tile::render( SDL_Rect& camera )
{
    //If the tile is on screen and not out of bounds.
    if( checkCollision( camera, mBox ) )
    {
        if(mType != TILE_WATER0)
		{
        gTileTextureGrass.render( mBox.x - camera.x, mBox.y - camera.y, &gTileClips[ mType ] );
		}
		else
		{
		gTileTextureWater.render( mBox.x - camera.x, mBox.y - camera.y, &gTileClips[ mType ] );
		}

    }
}

int Tile::getType()
{
    return mType;
}

int Tile::getxy(int index)
{
	if(index == 1)
	{
		return gX;
	}
	if(index == 2)
	{
		return gY;
	}
}

bool const Tile::operator==(Tile & tile)
{
	bool equal = false;
	if(this->getxy(1) == tile.getxy(1) && this->getxy(2) == tile.getxy(2)) // same tile
	{
		equal = true;
	}
	return equal;
}

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::setSize(int h, int w)
{
	mPosition.w = w;
	mPosition.h = h;
}

void LButton::setButtonType(int num)
{
	buttonType = num;
}

int LButton::getButtonType()
{
	return buttonType;
}

SDL_Rect LButton::getRect()
{
	return mPosition;
}

bool LButton::handleEvent( SDL_Event* e )
{
	bool done = false;
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + mPosition.w )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + mPosition.h )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
				break;
				case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;
				
				case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
				switch(getButtonType())
				{
				case 0: if(loadMap(0)){done = true;} else {done = false;} break;
				case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14: done = true; break;
				}
			}
		}
	}
	return done;
}

void LButton::handleEvent( SDL_Event* e, Tile *tiles[] )
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );
		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + mPosition.w )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + mPosition.h )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
				break;
				case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;
				
				case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
				switch(getButtonType())
				{
				case 1:	x += cameraptr->x; y += cameraptr->y; for(int j = 0; j < *tt; ++j)  //snapping cursor (no exception handling...implement here!)
						{if(x >= tiles[j]->mBox.x && x <= (tiles[j]->mBox.x + tiles[j]->mBox.w) && (y >= tiles[j]->mBox.y && y <= (tiles[j]->mBox.y + tiles[j]->mBox.h)))
						{cursorptr->setPosition(tiles[j]->mBox.x,tiles[j]->mBox.y); 
						std::cout << "Mouse x:" << x << std::endl;
						std::cout << "Mouse y:" << y << std::endl;
						std::cout << "Tile number:" << j << std::endl;
						std::cout << "Tile Width:" << tiles[j]->mBox.w << std::endl;
						std::cout << "Tile Height:" << tiles[j]->mBox.h << std::endl;
						std::cout << "Tile X:" << tiles[j]->mBox.x << std::endl;
						std::cout << "Tile Y:" << tiles[j]->mBox.y << std::endl;
						std::cout << "camera Y:" << cameraptr->y << std::endl;
						std::cout << "camera X:" << cameraptr->x << std::endl;break;} 
						} break;
				}
				break;
			}
		}
	}
}

void LButton::handleEvent(SDL_Event* e, std::vector<Tile*> tiles, std::vector<Unit> &units)
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );
		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + mPosition.w )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + mPosition.h )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
				break;
				case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;
				
				case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
				switch(getButtonType())
				{
				case 1:x += cameraptr->x; y += cameraptr->y;
							if(selected != NULL)
							{
								for(int t = 0; t < tiles.size(); ++t)
								{
									if(x >= tiles.at(t)->mBox.x && x <= (tiles.at(t)->mBox.x + tiles.at(t)->mBox.w) && (y >= tiles.at(t)->mBox.y && y <= (tiles.at(t)->mBox.y + tiles.at(t)->mBox.h)))
									{
										int tempx = selected->getxy(1);
										std::cout << "tempx i:" << std::to_string(tempx) << std::endl;
										int tempy = selected->getxy(2);
										std::cout << "tempy i:" << std::to_string(tempy) << std::endl;
										selected->uBox.x = (tiles.at(t)->mBox.x + 10);
										selected->uBox.y = (tiles.at(t)->mBox.y + 10);
										selected->resetgxy();
										int temptx = 0;
										if(selected->getxy(1) > tempx)
										{
											temptx = selected->getxy(1) - tempx;
										}
										else
										{
											temptx = tempx - selected->getxy(1);
										}
										int tempty = 0;
										if(selected->getxy(2) > tempy)
										{
											tempty = selected->getxy(2) - tempy;
										}
										else
										{
											tempty = tempy - selected->getxy(2);
										}
										std::cout << "temptx:" << std::to_string(temptx) << std::endl;
										int tempmov = temptx + tempty;
										std::cout << "tempmov:" << std::to_string(tempmov) << std::endl;
										selected->setUnitMov(selected->getUnitMov() - tempmov);
										std::cout << "inside" << std::endl;
										selected = nullptr;
										break;
									}
								}
							}
						}
				break;
			}
		}
	}
}

bool endturn(bool &win)
{
	timerptr->stop();
	++*turnptr;
	selected = nullptr;
	timerptr->start();
	return false;
}

void LButton::handleEvent( SDL_Event* e , bool &quit, bool &win) //I will modify this later.
{
	bool done = false;
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + mPosition.w )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + mPosition.h )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
				break;
				case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;
				
				case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
				switch(getButtonType())
				{
				case 2: if(endturn(win)){done = true;} else {done = true;} break; //if there is a win, the results screen will appear after the "win" design appears for 5 seconds.
				case 3: 
					LButton resume;
					resume.setPosition(250,200);
					resume.setSize(50,250);
					resume.setButtonType(4);
					LButton quitToMenu;
					quitToMenu.setPosition(250,300);
					quitToMenu.setSize(50,250);
					quitToMenu.setButtonType(5);
					LButton quitToDesktop;
					quitToDesktop.setPosition(250,400);
					quitToDesktop.setSize(50,250);
					quitToDesktop.setButtonType(6);

					for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
					{
						buttonresumesprites[ i ].x = 0;
						buttonresumesprites[ i ].y = i * 50;
						buttonresumesprites[ i ].w = resume.getRect().w;
						buttonresumesprites[ i ].h = resume.getRect().h;

						buttonquitmenusprites[ i ].x = 0;
						buttonquitmenusprites[ i ].y = i * 50;
						buttonquitmenusprites[ i ].w = quitToMenu.getRect().w;
						buttonquitmenusprites[ i ].h = quitToMenu.getRect().h;

						buttonquitdesktopsprites[ i ].x = 0;
						buttonquitdesktopsprites[ i ].y = i * 50;
						buttonquitdesktopsprites[ i ].w = quitToDesktop.getRect().w;
						buttonquitdesktopsprites[ i ].h = quitToDesktop.getRect().h;
					}
					cursorptr->stopmoving(); //just in case the cursor was moving.
					timerptr->pause();
					while(!done) //not done
							{
								while( SDL_PollEvent( e ) != 0 )
								{
									if(e->type == SDL_QUIT || quitToDesktop.handleEvent(e))
									{
										quit = true;
										timerptr->stop();
										done = true;
									}
									if(resume.handleEvent(e)) //resume gameplay
									{
										timerptr->unpause();
										done = true;
									}
									if(quitToMenu.handleEvent(e))
									{
										win = false;
										timerptr->stop();
										done = true;
									}
								}
							SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
							SDL_RenderClear( gRenderer );
							resume.render(4);
							quitToMenu.render(5);
							quitToDesktop.render(6);
							SDL_RenderPresent(gRenderer);
							}break;
				}
				break;
			}
		}
	}
}
	
void LButton::render(int index)
{
	//Show current button sprite
	switch(index)
	{
	case 0:buttonquickplaytexture.render( mPosition.x, mPosition.y, &buttonquickplaysprites[ mCurrentSprite ] );
		break;
	case 1:tileOverlayTexture.render(mPosition.x,mPosition.y); break;
	case 2:endTurnTexture.render(mPosition.x,mPosition.y, &buttonendturnsprites[mCurrentSprite]); break;
	case 3:menuGamePlayTexture.render(mPosition.x,mPosition.y, &buttonmenugameplaysprites[mCurrentSprite]); break;
	case 4:buttonResumeTexture.render(mPosition.x,mPosition.y, &buttonresumesprites[mCurrentSprite]); break;
	case 5:quitToMenuTexture.render(mPosition.x,mPosition.y, &buttonquitmenusprites[mCurrentSprite]); break;
	case 6:quitToDesktopTexture.render(mPosition.x,mPosition.y, &buttonquitdesktopsprites[mCurrentSprite]); break;
	case 7:godTypeAechall.render(mPosition.x,mPosition.y); break;
	case 8:godTypeZaldysl.render(mPosition.x,mPosition.y); break;
	case 9:godTypeVeinol.render(mPosition.x,mPosition.y); break;
	case 10:godTypeWaterusl.render(mPosition.x,mPosition.y); break;
	case 11:godTypeOlshabul.render(mPosition.x,mPosition.y); break;
	case 12:godTypeUlterachl.render(mPosition.x,mPosition.y); break;
	case 13:godTypeDolozl.render(mPosition.x,mPosition.y); break;
	case 14:godTypeBorgonl.render(mPosition.x,mPosition.y); break;
	}
}

Text::Text(std::string text, int r, int g, int b, int x, int y, int bx, int by, int bw, int bh)
{
	color.r = r;
	color.b = b;
	color.g = g;
	posx = x;
	posy = y;
	box.x = bx;
	box.y = by;
	box.w = bw;
	box.h = bh;
	texture.loadFromRenderedText(text,color);
}
Text::Text(std::string text, int x, int y, int fonts)
{
	color.r = 0;
	color.b = 0;
	color.g = 0;
	posx = x;
	posy = y;
	Text::text = text;
	texture.loadFromRenderedText(text,color);
	texture.setHeight(texture.getHeight() - (10 - fonts));
	int loop = 400;
	for(int x = 0x2; x <= 10; ++x)
	{
		if(texture.getWidth() > loop && texture.getWidth() < (loop + 400))
		{
			texture.setWidth(texture.getWidth() / x);
			break;
		}
		loop += 400;
	}
}

LTexture Text::getTexture()
{
	return texture;
}
std::string Text::getText()
{
	return text;
}
int Text::getX()
{
	return posx;
}
int Text::getY()
{
	return posy;
}

void Text::setText(std::string val)
{
	text = val;
	texture.loadFromRenderedText(text,color);
}
void Text::drawText()
{
	texture.render(posx,posy);
/*  box.x = posx;
	box.y = posy - 2;
	box.w = texture.getWidth();
	box.h = texture.getHeight();
	*/
}
Dot::Dot()
{
    //Initialize the collision box
    mBox.x = 0;
    mBox.y = 0;
	mBox.w = DOT_WIDTH;
	mBox.h = DOT_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

void Dot::move( Tile *tiles[], Text label)
{
    //Move the dot left or right
	SDL_Delay(100);
    mBox.x += mVelX;

    //If the dot went too far to the left or right or touched a wall
    if( ( mBox.x < 0 ) || ( mBox.x + DOT_WIDTH > *lw ))
    {
        //move back
        mBox.x -= mVelX;
    }

    //Move the dot up or down
    mBox.y += mVelY;

    //If the dot went too far up or down or touched a wall
    if( ( mBox.y < 0 ) || ( mBox.y + DOT_HEIGHT > *lh ))
    {
        //move back
        mBox.y -= mVelY;
    }
}

void Dot::stopmoving()
{
	if(mVelX != 0)
	{
		mVelX = 0;
	}
	if(mVelY != 0)
	{
		mVelY = 0;
	}
}

void Dot::setCamera( SDL_Rect& camera )
{
	//Center the camera over the dot
	camera.x = ( mBox.x + DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
	camera.y = ( mBox.y + DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if( camera.x < 0 )
	{ 
		camera.x = 0;
	}
	if( camera.y < 0 )
	{
		camera.y = 0;
	}
	if( camera.x > *lw - camera.w )
	{
		camera.x = *lw - camera.w;
	}
	if( camera.y > *lh - camera.h )
	{
		camera.y = *lh - camera.h;
	}
}

void Dot::move( Tile *tiles[])
{
	SDL_Delay(100);
    mBox.x += mVelX;

    //If the dot went too far to the left or right or touched a wall
    if( ( mBox.x < 0 ) || ( mBox.x + DOT_WIDTH > *lw ))
    {
        //move back
        mBox.x -= mVelX;
    }

    //Move the dot up or down
    mBox.y += mVelY;

    //If the dot went too far up or down or touched a wall
    if( ( mBox.y < 0 ) || ( mBox.y + DOT_HEIGHT > *lh ))
    {
        //move back
        mBox.y -= mVelY;
    }
}

void Dot::setPosition(int x, int y)
{
	mBox.x = x;
	mBox.y = y;
}

SDL_Rect Dot::getBox()
{
	return mBox;
}

void Dot::render( SDL_Rect& camera )
{
    //Show the dot
	gDotTexture.render( mBox.x - camera.x, mBox.y - camera.y );
}
inter::inter()
{
	iBox.x = 0;
	iBox.w = 0;
	textureptr = NULL;
}

void inter::setPosition( int x, int y )
{
	iBox.x = x;
	iBox.y = y;
}

void inter::setSize(int h, int w)
{
	iBox.w = w;
	iBox.h = h;
}
void inter::render()
{
	try
	{
	if(textureptr == NULL)
	{
		throw textureNotInitializedException();
	}
	else
	{
		textureptr->render(iBox.x,iBox.y);
	}
	}
	catch(textureNotInitializedException& t)
	{
		std::cout << t.what() << std::endl;
	}
}

void inter::render(int index)
{
	switch(index)
	{
	case 1:bottomGameplayInterface.render(iBox.x, iBox.y); break;
	case 4:statD.render(iBox.x,iBox.y);
	}
}

std::string updateTileInfo(Tile *tiles[], Dot dot, int id)
{
std::string text = "N/A";
	//checks type
	for(int x = 0; x < *tt; ++x)
	{
		if(tiles[x]->mBox.x == dot.getBox().x && tiles[x]->mBox.y == dot.getBox().y)
		{
			if(tiles[x]->getType() <= 3)
			{
				switch(id)
				{
				case 1: text = "Grassland,Open Road"; break;
				case 2: text = "(D:10%,M:1)"; break;
				case 3: text = "X:" + std::to_string(tiles[x]->getxy(1)); break;
				case 4: text = "Y:" + std::to_string(tiles[x]->getxy(2)); break;
				}
				break;
			}
			else
			{
				switch(id)
				{
				case 1: text = "Water"; break;
				case 2: text = "(D:10%,M:0)"; break;
				case 3: text = "X:" + std::to_string(tiles[x]->getxy(1)); break;
				case 4: text = "Y:" + std::to_string(tiles[x]->getxy(2)); break;
				}
				break;
			}
		}
		else
		{
			text = "N/A";
		}
	}
	return text;
}
std::string updateBuildingInfo(std::vector<Building> buildings, Dot dot)
{
	std::string text = "Building:None";
	for(auto item : buildings)
	{
		if((item.bBox.x - 10) == dot.getBox().x && (item.bBox.y - 10) == dot.getBox().y)
		{
			switch(item.getType())
			{
			case 1: text = item.getinfo(1) + "(D:" + std::to_string(item.getDefense()) + "%P:" + std::to_string(item.getPower()) + "C:" + std::to_string(item.getControlPoints()) + "/" + std::to_string(item.getMaxControlPoints()) + ")" + item.getinfo(4); break;
			case 2: text = item.getinfo(1) + "(D:" + std::to_string(item.getDefense()) + "%P:" + std::to_string(item.getPower()) + "C:" + std::to_string(item.getControlPoints()) + "/" + std::to_string(item.getMaxControlPoints()) + ")" + item.getinfo(4); break;
			case 3: text = item.getinfo(1) + "(D:" + std::to_string(item.getDefense()) + "%P:" + std::to_string(item.getPower()) + "C:" + std::to_string(item.getControlPoints()) + "/" + std::to_string(item.getMaxControlPoints()) + ")" + item.getinfo(4); break;
			case 4: text = item.getinfo(1) + "(D:" + std::to_string(item.getDefense()) + "%P:" + std::to_string(item.getPower()) + "C:" + std::to_string(item.getControlPoints()) + "/" + std::to_string(item.getMaxControlPoints()) + ")" + item.getinfo(4); break;
			}
		}
	}
	return text;
}
std::string updateUnitInfo(std::vector<Unit> units, Dot dot)
{
	std::string text = "Building:None";
	for(auto item : units)
	{
		if((item.uBox.x - 10) == dot.getBox().x && (item.uBox.y - 10) == dot.getBox().y)
		{
			switch(item.getUnitKind())
			{
			//case 1: text = item.getinfo(1) + "(D:" + std::to_string(item.getDefense()) + "%P:" + std::to_string(item.getPower()) + "C:" + std::to_string(item.getControlPoints()) + "/" + std::to_string(item.getMaxControlPoints()) + ")" + item.getinfo(4); break;
			//case 2: text = item.getinfo(1) + "(D:" + std::to_string(item.getDefense()) + "%P:" + std::to_string(item.getPower()) + "C:" + std::to_string(item.getControlPoints()) + "/" + std::to_string(item.getMaxControlPoints()) + ")" + item.getinfo(4); break;
			//case 3: text = item.getinfo(1) + "(D:" + std::to_string(item.getDefense()) + "%P:" + std::to_string(item.getPower()) + "C:" + std::to_string(item.getControlPoints()) + "/" + std::to_string(item.getMaxControlPoints()) + ")" + item.getinfo(4); break;
			//case 4: text = item.getinfo(1) + "(D:" + std::to_string(item.getDefense()) + "%P:" + std::to_string(item.getPower()) + "C:" + std::to_string(item.getControlPoints()) + "/" + std::to_string(item.getMaxControlPoints()) + ")" + item.getinfo(4); break;
			}
		}
	}
	return text;
}
LTimer::LTimer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void LTimer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
	mPausedTicks = 0;
}

void LTimer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

	//Clear tick variables
	mStartTicks = 0;
	mPausedTicks = 0;
}

void LTimer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
		mStartTicks = 0;
    }
}

void LTimer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

Uint32 LTimer::getTicks()
{
	//The actual timer time
	Uint32 time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool LTimer::isStarted()
{
	//Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused()
{
	//Timer is running and paused
    return mPaused && mStarted;
}

Building::Building(int x, int y, int buildingtype)
{
	bBox.x = x;
	bBox.y = y;
	bBox.w = BUILDING_WIDTH;
	bBox.h = BUILDING_HEIGHT;
	bType = buildingtype;
	iscontrolled = 0;
	controlp = 0;
	switch(buildingtype)
	{
	case 1: defense = 20; power = 50; controlmax = 10; break; //active portal
	case 2: defense = 10; power = 0; controlmax = 10; break; //inactive portal
	case 3: defense = 25; power = 25; controlmax = 10; break; //village
	case 4: defense = 50; power = 75; controlmax = 30; break; //tower
	default: defense = 0; power = 0; controlmax = 1; std::cout << "The building type was not produced correctly." << std::endl; //shouldn't get here.
	}
}

int Building::getType()
{
	return bType;
}

void Building::setType(int buildingtype)
{
	bType = buildingtype;
}

void Building::setControlPoints(int points)
{
	controlp = points;
}

void Building::setControlled(int playerteam)
{
	iscontrolled = playerteam;
}

int Building::getControlPoints()
{
	return controlp;
}
int Building::getMaxControlPoints()
{
	return controlmax;
}
int Building::getControlled()
{
	return iscontrolled;
}

int Building::getPower()
{
	return power;
}
int Building::getDefense()
{
	return defense;
}
std::string Building::getinfo(int index)
{
	std::string text = "";
	switch(index)
	{
	case 1: switch(bType)
			{
	case 1: text = "Portal(A)"; break;
	case 2: text = "Portal(I)"; break;
	case 3: text = "Village"; break;
	case 4: text = "Tower"; break;
			}break;
	case 2: text = "Defense:" + std::to_string(defense) + "%"; break;
	case 3: text = "Power:" + std::to_string(power); break;
	case 4: text = "Team:" + std::to_string(iscontrolled); break;
	case 5: text = "Controlled:" + std::to_string(controlp) + "/" + std::to_string(controlmax); break;
	default: text = "What happened?"; //shouldn't get here
	}
	return text;
}

void Building::render(SDL_Rect &camera)
{
	if(checkCollision(camera,bBox))
	{
		switch(bType)
		{
		case 1: gPortalActiveTexture.render(bBox.x - camera.x,bBox.y - camera.y); break;
		case 2: gPortalInactiveTexture.render(bBox.x - camera.x,bBox.y - camera.y); break;
		case 3: gVillageTexture.render(bBox.x - camera.x,bBox.y - camera.y); break;
		case 4: gTowerTexture.render(bBox.x - camera.x,bBox.y - camera.y); break;
		}
	}
}

void Building::handlebuttonevents(SDL_Event * d)
{
		bool done = false;
		int x, y;
		SDL_GetMouseState(&x,&y);
		try
		{
		if(cameraptr == NULL)
		{
			throw cameraNotInitializedException();
		}
		if(x >= (bBox.x - cameraptr->x) && x <= (bBox.x + bBox.w - cameraptr->x) && y >= (bBox.y - cameraptr->y) && y <= (bBox.y + bBox.h - cameraptr->y))
		{
			switch( d->type )
			{
				case SDL_MOUSEMOTION:
				break;
				case SDL_MOUSEBUTTONDOWN: std::cout << "Building clicked" << std::endl; 
				break;
				case SDL_MOUSEBUTTONUP: std::cout << "Building released" << std::endl;
				break;
			}
		}
		}
		catch(cameraNotInitializedException& c)
		{
			std::cout << c.what() << std::endl;
		}
}

Unit::Unit(int x, int y, int unitKind, int team)
{
	uBox.x = x;
	uBox.y = y;
	uBox.h = 60;
	uBox.w = 60;
	gX = (x + 20) / 80;
	gY = (y + 20) / 80;
	kind = unitKind;
	isSelected = false;
	Unit::team = team;
	switch(unitKind)
	{
	case 1: type = 1; maxstr = 5; str = 5; maxmovement = 6; movement = 6; range = 1; canAttack = true; canCapture = true; summonable = true; isKilled = false; canStrikeBack = true; break; //Aoth
	default: type = 1; maxstr = 1; str = 1; maxmovement = 1; movement = 1; range = 1; canAttack = true; canCapture = false; summonable = false; isKilled = true; canStrikeBack = true; //Shouldn't get here.
	}
}
void Unit::resetUnitMovement()
{
	movement = maxmovement;
}

void Unit::resetgxy()
{
	gX = (uBox.x + 20) / 80;
	gY = (uBox.y + 20) / 80;
}

void Unit::setKilled(bool killed)
{
	isKilled = killed;
}
void Unit::setUnitKind(int k)
{
	kind = k;
}
void Unit::setUnitType(int t)
{
	type = t;
}
void Unit::setUnitStr(int s)
{
	str = s;
}
void Unit::setUnitMov(int m)
{
	movement = m;
}
void Unit::setTeam(int team)
{
	Unit::team = team;
}
void Unit::setRange(int r)
{
	range = r;
}

void Unit::setSelected(bool selected)
{
	isSelected = selected;
}

void Unit::setCanAttack(bool attack)
{
	canAttack = attack;
}

int Unit::getUnitKind()
{
	return kind;
}
int Unit::getUnitType()
{
	return type;
}
int Unit::getUnitStr()
{
	return str;
}
int Unit::getUnitMov()
{
	return movement;
}
int Unit::getTeam()
{
	return team;
}
int Unit::getRange()
{
	return range;
}

int Unit::getxy(int index)
{
	if(index == 1)
	{
		return gX;
	}
	if(index == 2)
	{
		return gY;
	}
}

bool Unit::getSelected()
{
	return isSelected;
}

bool Unit::canCaptureBuildings()
{
	return canCapture;
}
bool Unit::isSummonable()
{
	return summonable;
}

bool Unit::killed()
{
	if(str <= 0)
	{
		isKilled = true;
		return isKilled;
	}
	else
	{
		isKilled = false;
		return isKilled;
	}
}

bool Unit::canAttackNow()
{
	return canAttack;
}

std::string Unit::getinfo(int index)
{
	std::string text = "";
	switch(index)
	{
	case 1: switch(kind)
			{
	case 1: text = "Aoth"; break;
			}break;
	case 2: text = "Strength:" + std::to_string(str) + "/" + std::to_string(maxstr); break;
	case 3: text = "Moves:" + std::to_string(movement) + "/" + std::to_string(maxmovement); break;
	case 4: text = "Team:" + std::to_string(team); break;
	default: text = "What happened?"; //shouldn't get here
	}
	return text;
}

void Unit::render(SDL_Rect &camera)
{
	if(checkCollision(camera,uBox))
	{
		switch(kind)
		{
		case 1: unitTypeAoth.render(uBox.x - camera.x, uBox.y - camera.y);
		}
	}
}

void Unit::render(SDL_Rect &camera, int index)
{
	if(checkCollision(camera,uBox))
	{
	}
}

void Unit::handlebuttonevents(SDL_Event * d)
{
		int x, y;
		SDL_GetMouseState(&x,&y);
		try
		{
		if(cameraptr == NULL)
		{
			throw cameraNotInitializedException();
		}
		if(x >= (uBox.x - cameraptr->x) && x <= (uBox.x + uBox.w - cameraptr->x) && y >= (uBox.y - cameraptr->y) && y <= (uBox.y + uBox.h - cameraptr->y))
		{
			switch( d->type )
			{
				case SDL_MOUSEMOTION:
				break;
				case SDL_MOUSEBUTTONDOWN: std::cout << "Unit clicked" << std::endl; 
				break;
				case SDL_MOUSEBUTTONUP: std::cout << "Unit released" << std::endl;
				selected = this;
				break;
			}
		}
		}
		catch(cameraNotInitializedException& c)
		{
			std::cout << c.what() << std::endl;
		}
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "God Wars", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	if( !gDotTexture.loadFromFile( "N://GodWarsimages//butile.png" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	//Load tile texture
	if( !gTileTextureGrass.loadFromFile( "N://GodWarsImages//grasstiles.png" ) )
	{
		printf( "Failed to load tile set texture: grass tiles!\n" );
		success = false;
	}
	if( !gTileTextureWater.loadFromFile( "N://GodWarsImages//watertile.png" ) )
	{
		printf( "Failed to load tile set texture: water tiles!\n" );
		success = false;
	}
	if( !gPortalActiveTexture.loadFromFile( "N://GodWarsImages//portalactive.png" ) )
	{
		printf( "Failed to load building texture:: portal active!\n" );
		success = false;
	}
	if( !gPortalInactiveTexture.loadFromFile( "N://GodWarsImages//portalinactive.png" ) )
	{
		printf( "Failed to load building texture: portal inactive!\n" );
		success = false;
	}
	if( !gVillageTexture.loadFromFile( "N://GodWarsImages//village.png" ) )
	{
		printf( "Failed to load building texture: village!\n" );
		success = false;
	}
	if( !gTowerTexture.loadFromFile( "N://GodWarsImages//tower.png" ) )
	{
		printf( "Failed to load building texture: tower!\n" );
		success = false;
	}
	if( !gTileUnitMove.loadFromFile( "N://GodWarsImages//unitmove.png" ) )
	{
		printf( "Failed to load tile set texture: unit move!\n" );
		success = false;
	}
	if( !gTileUnitAttack.loadFromFile( "N://GodWarsImages//unitattack.png" ) )
	{
		printf( "Failed to load tile set texture: unit attack!\n" );
		success = false;
	}
	if( !gTileUnitRange.loadFromFile( "N://GodWarsImages//unitrange.png" ) )
	{
		printf( "Failed to load tile set texture: unit range!\n" );
		success = false;
	}
		//Load sprites
	if( !buttonquickplaytexture.loadFromFile( "N://GodWarsImages//buttonquickplay.png" ) )
	{
		printf( "Failed to load button sprite texture: quickplay!\n" );
		success = true;
	}
	if( !endTurnTexture.loadFromFile( "N://GodWarsImages//endturn.png" ) )
	{
		printf( "Failed to load button sprite texture: endturn!\n" );
		success = true;
	}
	if( !menuGamePlayTexture.loadFromFile( "N://GodWarsImages//menugameplay.png" ) )
	{
		printf( "Failed to load button sprite texture: Menu Game Play!\n" );
		success = true;
	}
	if( !tileOverlayTexture.loadFromFile( "N://GodWarsImages//tileoverlaytest.png" ) )
	{
		printf( "Failed to load sprite texture: tileoverlay!\n" );
		success = true;
	}
	if( !bottomGameplayInterface.loadFromFile( "N://GodWarsImages//bgi.png" ) )
	{
		printf( "Failed to load sprite texture: bottom gameplay interface!\n" );
		success = true;
	}
	if( !buttonResumeTexture.loadFromFile( "N://GodWarsImages//buttonresume.png" ) )
	{
		printf( "Failed to load sprite texture: buttonresume.png!\n" );
		success = true;
	}
	if( !quitToMenuTexture.loadFromFile( "N://GodWarsImages//quittomenu.png" ) )
	{
		printf( "Failed to load sprite texture: quittomenu.png!\n" );
		success = true;
	}
	if( !quitToDesktopTexture.loadFromFile( "N://GodWarsImages//quittodesktop.png" ) )
	{
		printf( "Failed to load sprite texture: quittodesktop.png!\n" );
		success = true;
	}
	if( !unitTypeSkirmisher.loadFromFile( "N://GodWarsImages//iskirm.png" ) )
	{
		printf( "Failed to load sprite texture: unit type skirmisher!\n" );
		success = true;
	}
	if( !unitTypeWarrior.loadFromFile( "N://GodWarsImages//iwarr.png" ) )
	{
		printf( "Failed to load sprite texture: unit type warrior!\n" );
		success = true;
	}
	if( !unitTypeLeviathan.loadFromFile( "N://GodWarsImages//ilevi.png" ) )
	{
		printf( "Failed to load sprite texture: unit type leviathan!\n" );
		success = true;
	}
	if( !unitTypeRanger.loadFromFile( "N://GodWarsImages//irang.png" ) )
	{
		printf( "Failed to load sprite texture: unit type ranger!\n" );
		success = true;
	}
	if( !unitTypeMagician.loadFromFile( "N://GodWarsImages//imagi.png" ) )
	{
		printf( "Failed to load sprite texture: unit type magician!\n" );
		success = true;
	}
	if( !unitTypeHealer.loadFromFile( "N://GodWarsImages//iheal.png" ) )
	{
		printf( "Failed to load sprite texture: unit type healer!\n" );
		success = true;
	}
	if( !unitTypeFlyer.loadFromFile( "N://GodWarsImages//ifly.png" ) )
	{
		printf( "Failed to load sprite texture: unit type flyer!\n" );
		success = true;
	}
	if( !statD.loadFromFile( "N://GodWarsImages//statdialog.png" ) ) //unused
	{
		printf( "Failed to load sprite texture: unit type skirmisher!\n" );
		success = true;
	}
	if( !godTypeAechalg.loadFromFile( "N://GodWarsImages//aechalingame.png" ) )
	{
		printf( "Failed to load sprite texture: god type aechal (g)!\n" );
		success = true;
	}
	if( !godTypeAechall.loadFromFile( "N://GodWarsImages//aechal.png" ) )
	{
		printf( "Failed to load sprite texture: god type aechal (l)!\n" );
		success = true;
	}
	if( !godTypeBorgong.loadFromFile( "N://GodWarsImages//borgoningame.png" ) )
	{
		printf( "Failed to load sprite texture: god type borgon (g)!\n" );
		success = true;
	}
	if( !godTypeBorgonl.loadFromFile( "N://GodWarsImages//borgon.png" ) )
	{
		printf( "Failed to load sprite texture: god type borgon (l)!\n" );
		success = true;
	}
	if( !godTypeDolozg.loadFromFile( "N://GodWarsImages//dolozingame.png" ) )
	{
		printf( "Failed to load sprite texture: god type doloz (g)!\n" );
		success = true;
	}
	if( !godTypeDolozl.loadFromFile( "N://GodWarsImages//doloz.png" ) )
	{
		printf( "Failed to load sprite texture: god type doloz (l)!\n" );
		success = true;
	}
	if( !godTypeOlshabug.loadFromFile( "N://GodWarsImages//olshabuingame.png" ) )
	{
		printf( "Failed to load sprite texture: god type olshabu (g)!\n" );
		success = true;
	}
	if( !godTypeOlshabul.loadFromFile( "N://GodWarsImages//olshabu.png" ) )
	{
		printf( "Failed to load sprite texture: god type olshabu (l)!\n" );
		success = true;
	}
	if( !godTypeUlterachg.loadFromFile( "N://GodWarsImages//ulterachingame.png" ) )
	{
		printf( "Failed to load sprite texture: god type ulterach (g)!\n" );
		success = true;
	}
	if( !godTypeUlterachl.loadFromFile( "N://GodWarsImages//ulterach.png" ) )
	{
		printf( "Failed to load sprite texture: god type ulterach (l)!\n" );
		success = true;
	}
	if( !godTypeVeinog.loadFromFile( "N://GodWarsImages//veinoingame.png" ) )
	{
		printf( "Failed to load sprite texture: god type veino (g)!\n" );
		success = true;
	}
	if( !godTypeVeinol.loadFromFile( "N://GodWarsImages//veino.png" ) )
	{
		printf( "Failed to load sprite texture: god type veino (l)!\n" );
		success = true;
	}
	if( !godTypeWaterusg.loadFromFile( "N://GodWarsImages//waterusingame.png" ) )
	{
		printf( "Failed to load sprite texture: god type waterus (g)!\n" );
		success = true;
	}
	if( !godTypeWaterusl.loadFromFile( "N://GodWarsImages//waterus.png" ) )
	{
		printf( "Failed to load sprite texture: god type waterus (l)!\n" );
		success = true;
	}
	if( !godTypeZaldysg.loadFromFile( "N://GodWarsImages//zaldysingame.png" ) )
	{
		printf( "Failed to load sprite texture: god type zaldys (g)!\n" );
		success = true;
	}
	if( !godTypeZaldysl.loadFromFile( "N://GodWarsImages//zaldys.png" ) )
	{
		printf( "Failed to load sprite texture: god type zaldys (l)!\n" );
		success = true;
	}
	if( !godTypeNeutrall.loadFromFile( "N://GodWarsImages//neutral.png" ) )
	{
		printf( "Failed to load sprite texture: god type neutral (g)!\n" );
		success = true;
	}
	if( !unitTypeAoth.loadFromFile( "N://GodWarsImages//Aoth2.png" ) )
	{
		printf( "Failed to load sprite texture: unit type Aoth!\n" );
		success = true;
	}
	//Open the font
	gFont = TTF_OpenFont( "N://GodWarsImages//arial.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = true;
	}
	return success;
}

int findDifference(Tile *tile, Unit *unit)
{
	int difference = 0;
	int x = tile->getxy(1);
	int y = tile->getxy(2);
	int xy = x + y;
	int sx = unit->getxy(1);
	int sy = unit->getxy(2);
	int sxy = sx + sy;
	if(xy > sxy)
	{
		difference = xy - sxy;
	}
	else if(sxy > xy)
	{
		difference = sxy - xy;
	}
	else
	{
		difference = sxy - xy;
	}
	return difference;
}
int findDifference(Tile *tile1, Tile *tile2)
{
	int difference = 0;
	int x = tile1->getxy(1);
	int y = tile1->getxy(2);
	int xy = x + y;
	int sx = tile2->getxy(1);
	int sy = tile2->getxy(2);
	int sxy = sx + sy;
	if(xy > sxy)
	{
		difference = xy - sxy;
	}
	else if(sxy > xy)
	{
		difference = sxy - xy;
	}
	else
	{
		difference = sxy - xy;
	}
	return difference;
}
int findDifference(Unit *unit1, Unit *unit2)
{
	int difference = 0;
	int x = unit1->getxy(1);
	int y = unit1->getxy(2);
	int xy = x + y;
	int sx = unit2->getxy(1);
	int sy = unit2->getxy(2);
	int sxy = sx + sy;
	if(xy > sxy)
	{
		difference = xy - sxy;
	}
	else if(sxy > xy)
	{
		difference = sxy - xy;
	}
	else
	{
		difference = sxy - xy;
	}
	return difference;
}

void close()
{
	//Free loaded images
	gDotTexture.free();
	gTileTextureGrass.free();
	gTileTextureWater.free();
	gPortalActiveTexture.free();
	gPortalInactiveTexture.free();
	gVillageTexture.free();
	gTowerTexture.free();
	gTileUnitMove.free();
	gTileUnitAttack.free();
	gTileUnitRange.free();
	buttonquickplaytexture.free();
	tileOverlayTexture.free();
	bottomGameplayInterface.free();
	endTurnTexture.free();
	menuGamePlayTexture.free();
	buttonResumeTexture.free();
	quitToMenuTexture.free();
	quitToDesktopTexture.free();
	unitTypeSkirmisher.free();
	unitTypeWarrior.free();
	unitTypeLeviathan.free();
	unitTypeRanger.free();
	unitTypeMagician.free();
	unitTypeHealer.free();
	unitTypeFlyer.free();
	godTypeAechalg.free();
	godTypeAechall.free();
	godTypeBorgong.free();
	godTypeBorgonl.free();
	godTypeDolozg.free();
	godTypeDolozl.free();
	godTypeOlshabug.free();
	godTypeOlshabul.free();
	godTypeUlterachg.free();
	godTypeUlterachl.free();
	godTypeVeinog.free();
	godTypeVeinol.free();
	godTypeWaterusg.free();
	godTypeWaterusl.free();
	godTypeZaldysg.free();
	godTypeZaldysl.free();
	godTypeNeutrall.free();
	unitTypeAoth.free();
	statD.free();
	if(!textptr.empty())
	{
	for(auto &item : textptr)
	{
		item.getTexture().free();
	}
	}

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void closetiles(Tile *tiles[])
{
	//Deallocate tiles
	for( int i = 0; i < *tt; ++i )
	{
		 if( tiles[ i ] == NULL )
		 {
			delete tiles[ i ];
			tiles[ i ] = NULL;
		 }
	}
}

void closeBuildings(std::vector<Building> &buildings)
{
	if(!buildings.empty())
	{
	buildings.clear();
	buildings.shrink_to_fit();
	}
}

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

bool setTiles( Tile* tiles[])
{
	//Success flag
	bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;
    //Open the map
    std::ifstream map(mapFile);
    //If the map couldn't be loaded
    if( map == NULL )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
		//Initialize the tiles
		int i = 0;
		while(!map.eof())
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if( map.fail() )
			{
				//Stop loading map
				printf( "Error loading map: Unexpected end of file!\n" );
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
			{
				tiles[ i ] = new Tile( x, y, tileType );
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf( "Error loading map: Invalid tile type at %d!\n", i );
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if( x >= *lw )
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
			++i;
		}
		
		//Clip the sprite sheet
		if( tilesLoaded )
		{
			gTileClips[ TILE_GRASS0 ].x = 0;
			gTileClips[ TILE_GRASS0 ].y = 0;
			gTileClips[ TILE_GRASS0 ].w = TILE_WIDTH;
			gTileClips[ TILE_GRASS0 ].h = TILE_HEIGHT;

			gTileClips[ TILE_GRASS1 ].x = 0;
			gTileClips[ TILE_GRASS1 ].y = 80;
			gTileClips[ TILE_GRASS1 ].w = TILE_WIDTH;
			gTileClips[ TILE_GRASS1 ].h = TILE_HEIGHT;

			gTileClips[ TILE_GRASS2 ].x = 80;
			gTileClips[ TILE_GRASS2 ].y = 0;
			gTileClips[ TILE_GRASS2 ].w = TILE_WIDTH;
			gTileClips[ TILE_GRASS2 ].h = TILE_HEIGHT;

			gTileClips[ TILE_GRASS3 ].x = 80;
			gTileClips[ TILE_GRASS3 ].y = 80;
			gTileClips[ TILE_GRASS3 ].w = TILE_WIDTH;
			gTileClips[ TILE_GRASS3 ].h = TILE_HEIGHT;

			gTileClips[ TILE_WATER0 ].x = 0;
			gTileClips[ TILE_WATER0 ].y = 0;
			gTileClips[ TILE_WATER0 ].w = TILE_WIDTH;
			gTileClips[ TILE_WATER0 ].h = TILE_HEIGHT;
		}
	}

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}

bool setBuildings(std::vector<Building> &buildings) //predefined building placement using the .bld file.
{
	bool buildingsset = true;
    int x = 0, y = 0;
    //Open the map
    std::ifstream map(bldFile);
	 //If the map couldn't be loaded
    if( map == NULL )
    {
		printf( "Unable to load map file!\n" );
		buildingsset = false;
    }
	else
	{
		int i = 0;
		while(!map.eof())
		{
		int buildingtype = -1;
		map >> buildingtype;
		//If the was a problem in reading the map
		if( map.fail() )
			{
				//Stop loading map
				printf( "Error loading map: Unexpected end of file!\n" );
				buildingsset = false;
				break;
			}
		if(buildingtype < 5) //maximum amount of types.
		{
			if(buildingtype > 0) //0 means no building is present on this tile.
			{
				buildings.push_back(Building(x + 10,y + 10,buildingtype));
			}
		}
		else
		{
				//Stop loading map
				printf( "Error loading map: Invalid tile type at %d!\n", i );
				buildingsset = false;
				break;
		}
			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if( x >= *lw )
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
			++i;
		}
	}
	return buildingsset;
}

bool setUnits(std::vector<Unit> &totalUnits)
{
	bool unitsSet = true;
    int x = 0, y = 0;
    //Open the map
    std::ifstream map(uapFile);
	 //If the map couldn't be loaded
    if( map == NULL )
    {
		printf( "Unable to load uap file!\n" );
		unitsSet = false;
    }
	else
	{
		int i = 0;
		const int teammax = 4;
		int teamcount = 0;
		while(!map.eof())
		{
		int teamm = -1;
		map >> teamm;
		//If the was a problem in reading the map
		if( map.fail() )
			{
				//Stop loading map
				printf( "Error loading map: Unexpected end of file!\n" );
				unitsSet = false;
				break;
			}
		if(teamm <= teammax && teamcount <= teammax) //maximum amount of teams
		{
			if(teamm > teamcount) //0 means no unit is on the tile AND is not associated with a team.
			{
				totalUnits.push_back(Unit(x + 10,y + 10,1,teamm));
				++teamcount;
			}
		}
		else
		{
				//Stop loading map
				printf( "Error loading map: Invalid tile type at %d!\n", i );
				unitsSet = false;
				break;
		}
			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if( x >= *lw )
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
			++i;
		}
	}
	return unitsSet;
}

bool touchesWall( SDL_Rect box, Tile* tiles[] ) //change to cursor
{
    ////Go through the tiles
    //for( int i = 0; i < *tt; ++i )
    //{
    //    //If the tile is a wall type tile (TILE_CENTER = 3 AND TILE_TOPLEFT = 11)
    //    if( ( tiles[ i ]->getType() >= TILE_CENTER ) && ( tiles[ i ]->getType() <= TILE_TOPLEFT ) )
    //    {
    //        //If the collision box touches the wall tile
    //        if( checkCollision( box, tiles[ i ]->getBox() ) )
    //        {
    //            return true;
    //        }
    //    }
    //}

    ////If no wall tiles were touched
    return false;
}

bool loadMap(int map)
{
	bool success = false;
	std::vector<Building> build;
	std::vector<Unit> units;
	switch(map)
	{
	case 0: mapFile = "N://39_tiling//debug.map"; bldFile = "N://39_tiling//debug.bld"; uapFile = "N://39_tiling//debug.uap"; Tile* tiles[TOTAL_TILES]; tt = &TOTAL_TILES; lw = &LEVEL_WIDTHs; lh = &LEVEL_HEIGHTs; 	if(!setTiles(tiles))
	{
		printf( "Failed to load tiles!");
	}	
	if(!setBuildings(build))
		{
			printf( "Failed to load buildings!");
		}
	if(!setUnits(units))
	{
		printf("Failed to load Units!");
	}
	if(play(tiles,build,units))
		{
			success = true;
		}
	else
		{
		success = false;
		closeBuildings(build);
		closetiles(tiles);
		}
		break;
	case 1: mapFile = "N://39_tiling//Firstlevel.map"; bldFile = "N://39_tiling//Firstlevel.bld"; uapFile = "N://39_tiling//Firstlevel.uap"; Tile* tile[TOTAL_TILESb]; tt = &TOTAL_TILESb; lw = &LEVEL_WIDTHb; lh = &LEVEL_HEIGHTb; 	if(!setTiles(tile))
	{
		printf( "Failed to load tiles!");
	}	
		if(!setBuildings(build))
		{
			printf( "Failed to load buildings!");
		}
	if(!setUnits(units))
	{
		printf("Failed to load Units!");
	}
	if(play(tile,build,units))
		{
			success = true;
		}
	else
		{
		success = false;
		closeBuildings(build);
		closetiles(tile);
		};
		break;
	default: mapFile = "N://39_tiling//debug.map"; bldFile = "N://39_tiling//debug.bld"; uapFile = "N://39_tiling//debug.uap"; Tile* til[TOTAL_TILES]; tt = &TOTAL_TILES; lw = &LEVEL_WIDTHs; lh = &LEVEL_HEIGHTs; 	if(!setTiles(til))
	{
		printf( "Failed to load tiles!");
	}	if(!setBuildings(build))
		{
			printf( "Failed to load buildings!");
		}
		if(!setUnits(units))
	{
		printf("Failed to load Units!");
	}
	if(play(til,build,units))
		{
			success = true;
		}
	else
		{
		success = false;
		closeBuildings(build);
		closetiles(til);
		}
	}
	return success;
}

bool loadMenu(SDL_Event e) //1
{
	bool success = false;
	LButton quickplay;
	quickplay.setPosition( 210,200);
	quickplay.setSize(150,300);
	quickplay.setButtonType(0);
	//loadMap(tileset,1);
	
	while(!success)
	{
	if( e.type == SDL_QUIT )
	{
		close();
		success = true;
	}
	 while(SDL_PollEvent( &e ) != 0)
	 {
		if(quickplay.handleEvent(&e))
		{
			success = true;
		}//2 whichever button is pressed
	 }
	 //Set sprites
		for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
		{
			buttonquickplaysprites[ i ].x = 0;
			buttonquickplaysprites[ i ].y = i * 150;
			buttonquickplaysprites[ i ].w = quickplay.getRect().w;
			buttonquickplaysprites[ i ].h = quickplay.getRect().h;
		}
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
	SDL_RenderClear( gRenderer );
	quickplay.render(0);

	//Update screen
	SDL_RenderPresent( gRenderer );
}
	return success;
}

bool selectGods(inter &icon)
{
	SDL_Event locale;
	bool done = true;
	bool quit = false;
	Text godLabel("Select a God",325,30,20);
	LButton aechal;
	aechal.setPosition(350,100);
	aechal.setSize(100,100);
	aechal.setButtonType(7);
	LButton zaldys;
	zaldys.setPosition(350,600);
	zaldys.setSize(100,100);
	zaldys.setButtonType(8);
	LButton veino;
	veino.setPosition(100,200);
	veino.setSize(100,100);
	veino.setButtonType(9);
	LButton waterus;
	waterus.setPosition(600,200);
	waterus.setSize(100,100);
	waterus.setButtonType(10);
	LButton olshabu;
	olshabu.setPosition(50,350);
	olshabu.setSize(100,100);
	olshabu.setButtonType(11);
	LButton ulterach;
	ulterach.setPosition(650,350);
	ulterach.setSize(100,100);
	ulterach.setButtonType(12);
	LButton doloz;
	doloz.setPosition(100,500);
	doloz.setSize(100,100);
	doloz.setButtonType(13);
	LButton borgon;
	borgon.setPosition(600,500);
	borgon.setSize(100,100);
	borgon.setButtonType(14);
	while(done)
	{
		while( SDL_PollEvent( &locale ) != 0 )
		{
			if(locale.type == SDL_QUIT)
			{
				quit = true;
				done = false;
			}
			if(aechal.handleEvent(&locale))
			{
				icon.textureptr = &godTypeAechalg;
				done = false;
			}
			if(zaldys.handleEvent(&locale))
			{
				icon.textureptr = &godTypeZaldysg;
				done = false;
			}
			if(veino.handleEvent(&locale))
			{
				icon.textureptr = &godTypeVeinog;
				done = false;
			}
			if(waterus.handleEvent(&locale))
			{
				icon.textureptr = &godTypeWaterusg;
				done = false;
			}
			if(olshabu.handleEvent(&locale))
			{
				icon.textureptr = &godTypeOlshabug;
				done = false;
			}
			if(ulterach.handleEvent(&locale))
			{
				icon.textureptr = &godTypeUlterachg;
				done = false;
			}
			if(doloz.handleEvent(&locale))
			{
				icon.textureptr = &godTypeDolozg;
				done = false;
			}
			if(borgon.handleEvent(&locale))
			{
				icon.textureptr = &godTypeBorgong;
				done = false;
			}
		}
		SDL_SetRenderDrawColor( gRenderer, 0x55, 0x55, 0x55, 0xFF );
		SDL_RenderClear( gRenderer );
		godLabel.drawText();
		aechal.render(7);
		zaldys.render(8);
		veino.render(9);
		waterus.render(10);
		olshabu.render(11);
		ulterach.render(12);
		doloz.render(13);
		borgon.render(14);
		SDL_RenderPresent(gRenderer);
	}
	return done || quit;
}

bool findUpMoves(int &currentpos, std::vector<Tile*> &validUpMoves, Tile *tiles[], Tile *selectedtile, std::vector<Unit> units, int index)
{
	bool found = false;
	//false is empty and not in the validmoves vector
	bool right = false;
	bool left = false;
	bool up = false;
	//remove duplicates
	std::set<Tile*> s;
	unsigned size = validUpMoves.size();
	for( unsigned i = 0; i < size; ++i )
	{
		s.insert(validUpMoves[i]);
	}
	validUpMoves.assign( s.begin(), s.end() );
	if(index == 1)
	{
		if(currentpos <= selected->getUnitMov()) //less than the current position
		{
			//First, obtain the current valid move selected by the Tile pointer.
			for(auto & ite : validUpMoves)
			{
				if(ite == selectedtile) //if the item in the valid moves matches the selected tile. This is to ensure that the selected pointer is a valid move.
				{
					for(auto &item2 : validUpMoves) //iterates through to ensure that the right, left, top or bottom sides are NOT taken by the validMoves vector.
					{
						if((selectedtile->getxy(1) + 1) >= (*lw / 80))
						{
							right = true;
							//std::cout << "The tile on the right doesn't exist" << std::endl;
						}
						else
						{
							if((selectedtile->getxy(1) + 1) == item2->getxy(1) && selectedtile->getxy(2) == item2->getxy(2)) //right
							{
								//std::cout << "The tile on the right is taken" << std::endl;
								right = true; //the right tile is locked.
							}
						}
						if((selectedtile->getxy(1) - 1) < 0)
						{
							left = true;
							//std::cout << "The tile on the left doesn't exist" << std::endl;
						}
						else
						{
							if((selectedtile->getxy(1) - 1) == item2->getxy(1) && selectedtile->getxy(2) == item2->getxy(2)) //left
							{
								//std::cout << "The tile on the left is taken" << std::endl;
								left = true; //the left tile is locked.
							}
						}
						//if((selectedtile->getxy(2) + 1) >= (*lh / 80))
						//{
						//	down = true;
						//	//std::cout << "The tile on the bottom doesn't exist" << std::endl;
						//}
						//else
						//{
						//	if((selectedtile->getxy(2) + 1) == item2->getxy(2) && selectedtile->getxy(1) == item2->getxy(1)) //bottom
						//	{
						//		//std::cout << "The tile on the bottom is taken" << std::endl;
						//		down = true; //the bottom tile is locked.
						//	}
						//}
						if((selectedtile->getxy(2) - 1) < 0)
						{
							up = true;
							//std::cout << "The tile on the top doesn't exist" << std::endl;
						}
						else
						{
							if((selectedtile->getxy(2) - 1) == item2->getxy(2) && selectedtile->getxy(1) == item2->getxy(1)) //top
							{
								//std::cout << "The tile on the top is taken" << std::endl;
								up = true; //the top tile is locked.
							}
						}

					}
					for(auto &unit : units)
					{
						if((selectedtile->getxy(1) + 1) == unit.getxy(1) && selectedtile->getxy(2) == unit.getxy(2)) //unit is not on the tile to the right.
						{
							right = true; //right is locked.
							//std::cout << "There is a unit on the right of the selected tile" << std::endl;
						}
						if((selectedtile->getxy(1) - 1) == unit.getxy(1) && selectedtile->getxy(2) == unit.getxy(2)) //unit is not on the tile to the right.
						{
							left = true; //right is locked.
							//std::cout << "There is a unit on the left of the selected tile" << std::endl;
						}
						if((selectedtile->getxy(2) - 1) == unit.getxy(2) && selectedtile->getxy(1) == unit.getxy(1)) //unit is not on the tile to the right.
						{
							up = true; //right is locked.
							//std::cout << "There is a unit on the up of the selected tile" << std::endl;
						}
					}
					break;
				}
			}
			for(int t = 0; t < *tt; ++t)
			{
				if(!(*selectedtile == *tiles[t])) //not the same tile
				{
					if(!right) //right is not taken
					{
						if((selectedtile->getxy(1) + 1) == tiles[t]->getxy(1) && selectedtile->getxy(2) == tiles[t]->getxy(2))
						{
							validUpMoves.push_back(tiles[t]);
							Tile *rightTile = tiles[t];
							++currentpos;
							if(currentpos <= selected->getUnitMov())
							{
								if(findUpMoves(currentpos, validUpMoves, tiles, rightTile, units, 1))
								{
									--currentpos;
									right = true;
								}
							}
							else
							{
								--currentpos;
								right = true;
							}
						}
					}
					if(!left) //left is not taken
					{
						if((selectedtile->getxy(1) - 1) == tiles[t]->getxy(1) && selectedtile->getxy(2) == tiles[t]->getxy(2))
						{
							validUpMoves.push_back(tiles[t]);
							Tile *leftTile = tiles[t];
							++currentpos;
							if(currentpos <= selected->getUnitMov())
							{
								if(findUpMoves(currentpos, validUpMoves, tiles, leftTile, units, 1))
								{
									--currentpos;
									left = true;
								}
							}
							else
							{
								--currentpos;
								left = true;
							}
						}
					}
					if(!up) //top is not taken
					{
						if((selectedtile->getxy(2) - 1) == tiles[t]->getxy(2) && selectedtile->getxy(1) == tiles[t]->getxy(1))
						{
							validUpMoves.push_back(tiles[t]);
							Tile *upTile = tiles[t];
							++currentpos;
							if(currentpos <= selected->getUnitMov())
							{
								if(findUpMoves(currentpos, validUpMoves, tiles, upTile, units, 1))
								{
									--currentpos;
									up = true;
								}
							}
							else
							{
								--currentpos;
								up = true;
							}
						}
					}
					//if(!down) //bottom is not taken
					//{
					//	if((selectedtile->getxy(2) + 1) == tiles[t]->getxy(2) && selectedtile->getxy(1) == tiles[t]->getxy(1))
					//	{
					//	validUpMoves.push_back(tiles[t]);
					//	Tile *downTile = tiles[t];
					//	++currentpos;
					//	if(currentpos <= selected->getUnitMov())
					//		{
					//			if(findMoves(currentpos, validUpMoves, tiles, downTile))
					//			{
					//				--currentpos;
					//				down = true;
					//			}
					//		}
					//		else
					//		{
					//			--currentpos;
					//			down = true;
					//		}
					//	}
					/*}*/
					if(up && left && right)
					{
						found = true;
						break;
					}
				}
				else
				{
					continue;
				}
			}
		}
		else //done
		{
			found = true;
		}
	}
	if(index == 2)
	{
		if(currentpos <= (selected->getUnitMov() + selected->getRange())) //less than the current position
		{
			//First, obtain the current valid move selected by the Tile pointer.
			for(auto & ite : validUpMoves)
			{
				if(ite == selectedtile) //if the item in the valid moves matches the selected tile. This is to ensure that the selected pointer is a valid move.
				{
					for(auto &item2 : validUpMoves) //iterates through to ensure that the right, left, top or bottom sides are NOT taken by the validMoves vector.
					{
						if((selectedtile->getxy(1) + 1) >= (*lw / 80))
						{
							right = true;
							//std::cout << "The tile on the right doesn't exist" << std::endl;
						}
						else
						{
							if((selectedtile->getxy(1) + 1) == item2->getxy(1) && selectedtile->getxy(2) == item2->getxy(2)) //right
							{
								//std::cout << "The tile on the right is taken" << std::endl;
								right = true; //the right tile is locked.
							}
						}
						if((selectedtile->getxy(1) - 1) < 0)
						{
							left = true;
							//std::cout << "The tile on the left doesn't exist" << std::endl;
						}
						else
						{
							if((selectedtile->getxy(1) - 1) == item2->getxy(1) && selectedtile->getxy(2) == item2->getxy(2)) //left
							{
								//std::cout << "The tile on the left is taken" << std::endl;
								left = true; //the left tile is locked.
							}
						}
						//if((selectedtile->getxy(2) + 1) >= (*lh / 80))
						//{
						//	down = true;
						//	//std::cout << "The tile on the bottom doesn't exist" << std::endl;
						//}
						//else
						//{
						//	if((selectedtile->getxy(2) + 1) == item2->getxy(2) && selectedtile->getxy(1) == item2->getxy(1)) //bottom
						//	{
						//		//std::cout << "The tile on the bottom is taken" << std::endl;
						//		down = true; //the bottom tile is locked.
						//	}
						//}
						if((selectedtile->getxy(2) - 1) < 0)
						{
							up = true;
							//std::cout << "The tile on the top doesn't exist" << std::endl;
						}
						else
						{
							if((selectedtile->getxy(2) - 1) == item2->getxy(2) && selectedtile->getxy(1) == item2->getxy(1)) //top
							{
								//std::cout << "The tile on the top is taken" << std::endl;
								up = true; //the top tile is locked.
							}
						}

					}
					for(auto &unit : units)
					{
						if((selectedtile->getxy(1) + 1) == unit.getxy(1) && selectedtile->getxy(2) == unit.getxy(2)) //unit is not on the tile to the right.
						{
							right = true; //right is locked.
							//std::cout << "There is a unit on the right of the selected tile" << std::endl;
						}
						if((selectedtile->getxy(1) - 1) == unit.getxy(1) && selectedtile->getxy(2) == unit.getxy(2)) //unit is not on the tile to the right.
						{
							left = true; //right is locked.
							//std::cout << "There is a unit on the left of the selected tile" << std::endl;
						}
						if((selectedtile->getxy(2) - 1) == unit.getxy(2) && selectedtile->getxy(1) == unit.getxy(1)) //unit is not on the tile to the right.
						{
							up = true; //right is locked.
							//std::cout << "There is a unit on the up of the selected tile" << std::endl;
						}
					}
					break;
				}
			}
			for(int t = 0; t < *tt; ++t)
			{
				if(!(*selectedtile == *tiles[t])) //not the same tile
				{
					if(!right) //right is not taken
					{
						if((selectedtile->getxy(1) + 1) == tiles[t]->getxy(1) && selectedtile->getxy(2) == tiles[t]->getxy(2))
						{
							validUpMoves.push_back(tiles[t]);
							Tile *rightTile = tiles[t];
							++currentpos;
							if(currentpos <= (selected->getUnitMov() + selected->getRange()))
							{
								if(findUpMoves(currentpos, validUpMoves, tiles, rightTile, units, 2))
								{
									--currentpos;
									right = true;
								}
							}
							else
							{
								--currentpos;
								right = true;
							}
						}
					}
					if(!left) //left is not taken
					{
						if((selectedtile->getxy(1) - 1) == tiles[t]->getxy(1) && selectedtile->getxy(2) == tiles[t]->getxy(2))
						{
							validUpMoves.push_back(tiles[t]);
							Tile *leftTile = tiles[t];
							++currentpos;
							if(currentpos <= (selected->getUnitMov() + selected->getRange()))
							{
								if(findUpMoves(currentpos, validUpMoves, tiles, leftTile, units, 2))
								{
									--currentpos;
									left = true;
								}
							}
							else
							{
								--currentpos;
								left = true;
							}
						}
					}
					if(!up) //top is not taken
					{
						if((selectedtile->getxy(2) - 1) == tiles[t]->getxy(2) && selectedtile->getxy(1) == tiles[t]->getxy(1))
						{
							validUpMoves.push_back(tiles[t]);
							Tile *upTile = tiles[t];
							++currentpos;
							if(currentpos <= (selected->getUnitMov() + selected->getRange()))
							{
								if(findUpMoves(currentpos, validUpMoves, tiles, upTile, units, 2))
								{
									--currentpos;
									up = true;
								}
							}
							else
							{
								--currentpos;
								up = true;
							}
						}
					}
					//if(!down) //bottom is not taken
					//{
					//	if((selectedtile->getxy(2) + 1) == tiles[t]->getxy(2) && selectedtile->getxy(1) == tiles[t]->getxy(1))
					//	{
					//	validUpMoves.push_back(tiles[t]);
					//	Tile *downTile = tiles[t];
					//	++currentpos;
					//	if(currentpos <= selected->getUnitMov())
					//		{
					//			if(findMoves(currentpos, validUpMoves, tiles, downTile))
					//			{
					//				--currentpos;
					//				down = true;
					//			}
					//		}
					//		else
					//		{
					//			--currentpos;
					//			down = true;
					//		}
					//	}
					/*}*/
					if(up && left && right)
					{
						found = true;
						break;
					}
				}
				else
				{
					continue;
				}
			}
		}
		else //done
		{
			found = true;
		}
		for(auto &unit : units) //check to see if the unit can be attacked if inside or one space outside the collated valid moves.
		{
			for(auto &validMove : validUpMoves)
			{
				if(selected->getTeam() == unit.getTeam()) //this includes itself and its teammates.
				{
					continue;
				}
				else
				{
					if(validMove->getxy(1) == unit.getxy(1) && validMove->getxy(2) == unit.getxy(2)) //already a valid attack move.
					{
						continue;
					}
					else
					{
						if((validMove->getxy(1) + 1) == unit.getxy(1) && validMove->getxy(2) == unit.getxy(2)) //if the is a unit on the right of a valid move
						{
							for(int r = 0; r < *tt; ++r) //get tile.
							{
								if(tiles[r]->getxy(1) == unit.getxy(1) && tiles[r]->getxy(2) == unit.getxy(2)) //if that tile equals the unit the validmove is referring to
								{
									validUpMoves.push_back(tiles[r]);
									break;
								}
							}
							break;
						}
						if((validMove->getxy(1) - 1) == unit.getxy(1) && validMove->getxy(2) == unit.getxy(2)) //if the is a unit on the left of a valid move
						{
							for(int r = 0; r < *tt; ++r) //get tile.
							{
								if(tiles[r]->getxy(1) == unit.getxy(1) && tiles[r]->getxy(2) == unit.getxy(2)) //if that tile equals the unit the validmove is referring to
								{
									validUpMoves.push_back(tiles[r]);
									break;
								}
							}
							break;
						}
						if(validMove->getxy(1) == unit.getxy(1) && (validMove->getxy(2) + 1) == unit.getxy(2)) //if the is a unit on the bottom of a valid move
						{
							for(int r = 0; r < *tt; ++r) //get tile.
							{
								if(tiles[r]->getxy(1) == unit.getxy(1) && tiles[r]->getxy(2) == unit.getxy(2)) //if that tile equals the unit the validmove is referring to
								{
									validUpMoves.push_back(tiles[r]);
									break;
								}
							}
							break;
						}
						if(validMove->getxy(1) == unit.getxy(1) && (validMove->getxy(2) - 1) == unit.getxy(2)) //if the is a unit on the top of a valid move
						{
							for(int r = 0; r < *tt; ++r) //get tile.
							{
								if(tiles[r]->getxy(1) == unit.getxy(1) && tiles[r]->getxy(2) == unit.getxy(2)) //if that tile equals the unit the validmove is referring to
								{
									validUpMoves.push_back(tiles[r]);
									break;
								}
							}
							break;
						}
					}
				}
			}
		}
	}
	//std::cout << validMoves.size() << std::endl;
	//std::cout << right << std::endl;
	//std::cout << currentpos << std::endl;
	return found;
}

bool findDownRightMoves(int &currentpos, std::vector<Tile*> &validDownRightMoves, Tile *tiles[], Tile *selectedtile, std::vector<Unit> units)
{
	bool found = false;
	//false is empty and not in the validmoves vector
	bool right = false;
	/*bool up = false;*/
	bool down = false;
	//remove duplicates
	std::set<Tile*> s;
	unsigned size = validDownRightMoves.size();
	for( unsigned i = 0; i < size; ++i )
	{
		s.insert(validDownRightMoves[i]);
	}
	validDownRightMoves.assign( s.begin(), s.end() );
	if(currentpos <= selected->getUnitMov()) //less than the current position
	{
		//First, obtain the current valid move selected by the Tile pointer.
		for(auto & ite : validDownRightMoves)
		{
			if(ite == selectedtile) //if the item in the valid moves matches the selected tile. This is to ensure that the selected pointer is a valid move.
			{
				for(auto &item2 : validDownRightMoves) //iterates through to ensure that the right, left, top or bottom sides are NOT taken by the validMoves vector.
				{
					if((selectedtile->getxy(1) + 1) >= (*lw / 80))
					{
						right = true;
						//std::cout << "The tile on the right doesn't exist" << std::endl;
					}
					else
					{
						if((selectedtile->getxy(1) + 1) == item2->getxy(1) && selectedtile->getxy(2) == item2->getxy(2)) //right
						{
							//std::cout << "The tile on the right is taken" << std::endl;
							right = true; //the right tile is locked.
						}
					}
					if((selectedtile->getxy(2) + 1) >= (*lh / 80))
					{
						down = true;
						//std::cout << "The tile on the bottom doesn't exist" << std::endl;
					}
					else
					{
						if((selectedtile->getxy(2) + 1) == item2->getxy(2) && selectedtile->getxy(1) == item2->getxy(1)) //bottom
						{
							//std::cout << "The tile on the bottom is taken" << std::endl;
							down = true; //the bottom tile is locked.
						}
					}
				}
				for(auto &unit : units)
				{
					if((selectedtile->getxy(1) + 1) == unit.getxy(1) && selectedtile->getxy(2) == unit.getxy(2)) //unit is not on the tile to the right.
					{
						right = true; //right is locked.
						//std::cout << "There is a unit on the right of the selected tile" << std::endl;
					}
					if((selectedtile->getxy(2) + 1) == unit.getxy(2) && selectedtile->getxy(1) == unit.getxy(1)) //unit is not on the tile to the bottom.
					{
						down = true; //down is locked.
						//std::cout << "There is a unit on the bottom of the selected tile" << std::endl;
					}
				}
				break;
			}
		}
		for(int t = 0; t < *tt; ++t)
		{
			if(!(*selectedtile == *tiles[t])) //not the same tile
			{
				if(!right) //right is not taken
				{
					if((selectedtile->getxy(1) + 1) == tiles[t]->getxy(1) && selectedtile->getxy(2) == tiles[t]->getxy(2))
					{
						validDownRightMoves.push_back(tiles[t]);
						Tile *rightTile = tiles[t];
						++currentpos;
						if(currentpos <= selected->getUnitMov())
						{
							if(findDownRightMoves(currentpos, validDownRightMoves, tiles, rightTile, units))
							{
								--currentpos;
								right = true;
							}
						}
						else
						{
							--currentpos;
							right = true;
						}
					}
				}
				if(!down) //bottom is not taken
				{
					if((selectedtile->getxy(2) + 1) == tiles[t]->getxy(2) && selectedtile->getxy(1) == tiles[t]->getxy(1))
					{
					validDownRightMoves.push_back(tiles[t]);
					Tile *downTile = tiles[t];
					++currentpos;
					if(currentpos <= selected->getUnitMov())
						{
							if(findDownRightMoves(currentpos, validDownRightMoves, tiles, downTile, units))
							{
								--currentpos;
								down = true;
							}
						}
						else
						{
							--currentpos;
							down = true;
						}
					}
				}
				if(down && right)
				{
					found = true;
					break;
				}
			}
			else
			{
				continue;
			}
		}
	}
	else //done
	{
		found = true;
	}
	//std::cout << validMoves.size() << std::endl;
	//std::cout << right << std::endl;
	//std::cout << currentpos << std::endl;
	return found;
}

bool findDownLeftMoves(int &currentpos, std::vector<Tile*> &validDownLeftMoves, Tile *tiles[], Tile *selectedtile, std::vector<Unit> units)
{
	bool found = false;
	//false is empty and not in the validDownLeftMoves vector
	bool right = false;
	bool left = false;
	/*bool up = false;*/
	bool down = false;
	//remove duplicates
	std::set<Tile*> s;
	unsigned size = validDownLeftMoves.size();
	for( unsigned i = 0; i < size; ++i )
	{
		s.insert(validDownLeftMoves[i]);
	}
	validDownLeftMoves.assign( s.begin(), s.end() );
	if(currentpos <= selected->getUnitMov()) //less than the current position
	{
		//First, obtain the current valid move selected by the Tile pointer.
		for(auto & ite : validDownLeftMoves)
		{
			if(ite == selectedtile) //if the item in the valid moves matches the selected tile. This is to ensure that the selected pointer is a valid move.
			{
				for(auto &item2 : validDownLeftMoves) //iterates through to ensure that the right, left, top or bottom sides are NOT taken by the validDownLeftMoves vector.
				{
					if((selectedtile->getxy(1) - 1) < 0)
					{
						left = true;
						//std::cout << "The tile on the left doesn't exist" << std::endl;
					}
					else
					{
						if((selectedtile->getxy(1) - 1) == item2->getxy(1) && selectedtile->getxy(2) == item2->getxy(2)) //left
						{
							//std::cout << "The tile on the left is taken" << std::endl;
							left = true; //the left tile is locked.
						}
					}
					if((selectedtile->getxy(2) + 1) >= (*lh / 80))
					{
						down = true;
						//std::cout << "The tile on the bottom doesn't exist" << std::endl;
					}
					else
					{
						if((selectedtile->getxy(2) + 1) == item2->getxy(2) && selectedtile->getxy(1) == item2->getxy(1)) //bottom
						{
							//std::cout << "The tile on the bottom is taken" << std::endl;
							down = true; //the bottom tile is locked.
						}
					}
				}
				for(auto &unit : units)
				{
					if((selectedtile->getxy(1) - 1) == unit.getxy(1) && selectedtile->getxy(2) == unit.getxy(2)) //unit is not on the tile to the left.
					{
						left = true; //left is locked.
						//std::cout << "There is a unit on the left of the selected tile" << std::endl;
					}
					if((selectedtile->getxy(2) + 1) == unit.getxy(2) && selectedtile->getxy(1) == unit.getxy(1)) //unit is not on the tile to the bottom.
					{
						down = true; //bottom is locked.
						//std::cout << "There is a unit on the bottom of the selected tile" << std::endl;
					}
				}
				break;
			}
		}
		for(int t = 0; t < *tt; ++t)
		{
			if(!(*selectedtile == *tiles[t])) //not the same tile
			{
				if(!left) //left is not taken
				{
					if((selectedtile->getxy(1) - 1) == tiles[t]->getxy(1) && selectedtile->getxy(2) == tiles[t]->getxy(2))
					{
						validDownLeftMoves.push_back(tiles[t]);
						Tile *leftTile = tiles[t];
						++currentpos;
						if(currentpos <= selected->getUnitMov())
						{
							if(findDownLeftMoves(currentpos, validDownLeftMoves, tiles, leftTile, units))
							{
								--currentpos;
								left = true;
							}
						}
						else
						{
							--currentpos;
							left = true;
						}
					}
				}
				if(!down) //bottom is not taken
				{
					if((selectedtile->getxy(2) + 1) == tiles[t]->getxy(2) && selectedtile->getxy(1) == tiles[t]->getxy(1))
					{
					validDownLeftMoves.push_back(tiles[t]);
					Tile *downTile = tiles[t];
					++currentpos;
					if(currentpos <= selected->getUnitMov())
						{
							if(findDownLeftMoves(currentpos, validDownLeftMoves, tiles, downTile, units))
							{
								--currentpos;
								down = true;
							}
						}
						else
						{
							--currentpos;
							down = true;
						}
					}
				}
				if(down && left)
				{
					found = true;
					break;
				}
			}
			else
			{
				continue;
			}
		}
	}
	else //done
	{
		found = true;
	}
	//std::cout << validDownLeftMoves.size() << std::endl;
	//std::cout << right << std::endl;
	//std::cout << currentpos << std::endl;
	return found;
}

void validateDownMoves(std::vector<Tile*> &validDownMoves, Tile *tiles[], Tile *selectedtile, std::vector<Unit> units)
{
	bool outofrange = false;
	int difference;
	int unitcount = 2;
	bool unitontile = false;
	Tile *downtile = nullptr;
	if(selected != nullptr && !(validDownMoves.empty()) && downtile == nullptr)
	{
			for(auto &unit : units) //locate the unit
			{
				if(selected->getxy(2) >= unit.getxy(2) && selected->getxy(1) == unit.getxy(1)) //is selected below the unit?
				{
					continue;
				}
				if(selected->getxy(1) == unit.getxy(1) && selected->getxy(2) == unit.getxy(2)) //selected is the same unit?
				{
					continue;
				}
				else //not the same
				{
					for(int r = 0; r < *tt; ++r) //first is to locate the tile beneath the unit.
					{
						if((unit.getxy(2) + 1) >= (*lh / 80)) //check to see if the location is out of scope.
						{
							outofrange = true;
							break;
						}
						if((unit.getxy(2) + 1) == tiles[r]->getxy(2) && unit.getxy(1) == tiles[r]->getxy(1)) //if there is a tile beneath the unit.
						{
							for(auto unit2 : units) //small check to see if there are any units below this.
							{
								if(unit.getxy(1) == unit2.getxy(1) && unit.getxy(1) == unit2.getxy(2)) //same unit?
								{
									continue;
								}
								if(selected->getxy(1) == unit2.getxy(1) && selected->getxy(2) == unit2.getxy(2)) //selected unit the same unit?
								{
									continue;
								}
							}
							if(!unitontile)
							{
								if((selected->getxy(2) + 1) <= unit.getxy(2) && selected->getxy(1) == unit.getxy(1))
								{
									for(auto validmove : validDownMoves)
									{
										if(((validmove->getxy(1) + 1) == tiles[r]->getxy(1) && validmove->getxy(2) == tiles[r]->getxy(2)) || ((validmove->getxy(1) - 1) == tiles[r]->getxy(1) && validmove->getxy(2) == tiles[r]->getxy(2)))
										{ //if the validmove on the right or left side of the tile can be reached.
											//std::cout << "tile coordinates:" << tiles[r]->getxy(1) << "," << tiles[r]->getxy(2) << std::endl;
											difference = findDifference(tiles[r],selected);
											if((selected->getUnitMov() - unitcount) >= difference)
											{
												validDownMoves.push_back(tiles[r]);
												downtile = tiles[r];
												break;
											}
										}
									}
								}
							}
						}
					}
				}
				if(outofrange)
				{
					std::cout << "The bottom tile is out of range." << std::endl;
					outofrange = false;
					break;
				}
			}
	}
	if(downtile != nullptr) //process reiterates
	{
		for(unsigned int e = 0; e < *tt; ++e)
		{						
			if((downtile->getxy(2) + 1) >= (*lh / 80)) //check to see if the location is out of scope.
			{
				break;
			}
			if((downtile->getxy(2) + 1) == tiles[e]->getxy(2) && downtile->getxy(1) == tiles[e]->getxy(1)) //if there is a tile beneath the selected tile.
			{
				for(auto &unit : units) //determine is there is a unit below the selected tile.
				{
					if(unit.getxy(2) <= downtile->getxy(2) ) //ignores unit location above or at the downtile.
					{
						continue;
					}
					if((downtile->getxy(2) + 1) == unit.getxy(2) && downtile->getxy(1) == unit.getxy(1))
					{
						unitontile = true;
						unitcount += 2;
					}
				}
				if(!unitontile)
				{
					for(auto validmove : validDownMoves)
					{
						if(((validmove->getxy(1) + 1) == tiles[e]->getxy(1) && validmove->getxy(2) == tiles[e]->getxy(2)) || ((validmove->getxy(1) - 1) == tiles[e]->getxy(1) && validmove->getxy(2) == tiles[e]->getxy(2)))
						{
							difference = findDifference(tiles[e],selected);
							if((selected->getUnitMov() - unitcount) >= difference)
							{
								validDownMoves.push_back(tiles[e]);
								//std::cout << "coordinates:" << tiles[e]->getxy(1) << "," << tiles[e]->getxy(2) << std::endl;
								//std::cout << "difference:" << difference << std::endl;
								//std::cout << "unit count:" << unitcount << std::endl;
								downtile = tiles[e];
								break;
							}
						}
					}
				}
				else
				{
					continue;
				}
			}
		}
	}
	for(auto &unit : units)
	{
		for(int v = 0; v < validDownMoves.size(); ++v)
		{
			if(selected->getxy(1) == unit.getxy(1) && selected->getxy(2) == unit.getxy(2))
			{
				continue;
			}
			if(unit.getxy(1) == validDownMoves.at(v)->getxy(1) && unit.getxy(2) == validDownMoves.at(v)->getxy(2))
			{
				validDownMoves.erase(validDownMoves.begin() + v);
			}
		}
	}
    //remove duplicates
	std::set<Tile*> s;
	unsigned size = validDownMoves.size();
	for( unsigned i = 0; i < size; ++i )
	{
		s.insert(validDownMoves[i]);
	}
	validDownMoves.assign( s.begin(), s.end() );
}

bool findRightMoves(int &currentpos, std::vector<Tile*> &validRightMoves, Tile *tiles[], Tile *selectedtile, std::vector<Unit> units)
{
	bool found = false;
	//false is empty and not in the validRightMoves vector
	bool right = false;
	bool up = false;
	/*bool up = false;*/
	bool down = false;
	//remove duplicates
	std::set<Tile*> s;
	unsigned size = validRightMoves.size();
	for( unsigned i = 0; i < size; ++i )
	{
		s.insert(validRightMoves[i]);
	}
	validRightMoves.assign( s.begin(), s.end() );
	if(currentpos <= selected->getUnitMov()) //less than the current position
	{
		//First, obtain the current valid move selected by the Tile pointer.
		for(auto & ite : validRightMoves)
		{
			if(ite == selectedtile) //if the item in the valid moves matches the selected tile. This is to ensure that the selected pointer is a valid move.
			{
				for(auto &item2 : validRightMoves) //iterates through to ensure that the right, left, top or bottom sides are NOT taken by the validRightMoves vector.
				{
					if((selectedtile->getxy(1) + 1) >= (*lw / 80))
					{
						right = true;
						//std::cout << "The tile on the right doesn't exist" << std::endl;
					}
					else
					{
						if((selectedtile->getxy(1) + 1) == item2->getxy(1) && selectedtile->getxy(2) == item2->getxy(2)) //right
						{
							//std::cout << "The tile on the right is taken" << std::endl;
							right = true; //the right tile is locked.
						}
					}
					if((selectedtile->getxy(2) - 1) < 0)
					{
						up = true;
						//std::cout << "The tile on the top doesn't exist" << std::endl;
					}
					else
					{
						if((selectedtile->getxy(2) - 1) == item2->getxy(2) && selectedtile->getxy(1) == item2->getxy(1)) //top
						{
							//std::cout << "The tile on the top is taken" << std::endl;
							up = true; //the top tile is locked.
						}
					}
					if((selectedtile->getxy(2) + 1) >= (*lh / 80))
					{
						down = true;
						//std::cout << "The tile on the bottom doesn't exist" << std::endl;
					}
					else
					{
						if((selectedtile->getxy(2) + 1) == item2->getxy(2) && selectedtile->getxy(1) == item2->getxy(1)) //bottom
						{
							//std::cout << "The tile on the bottom is taken" << std::endl;
							down = true; //the bottom tile is locked.
						}
					}
				}
				for(auto &unit : units)
				{
					if((selectedtile->getxy(1) + 1) == unit.getxy(1) && selectedtile->getxy(2) == unit.getxy(2)) //unit is not on the tile to the right.
					{
						right = true; //right is locked.
						//std::cout << "There is a unit on the right of the selected tile" << std::endl;
					}
					if((selectedtile->getxy(2) - 1) == unit.getxy(2) && selectedtile->getxy(1) == unit.getxy(1)) //unit is not on the tile to the right.
					{
						up = true; //right is locked.
						//std::cout << "There is a unit on the up of the selected tile" << std::endl;
					}
					if((selectedtile->getxy(2) + 1) == unit.getxy(2) && selectedtile->getxy(1) == unit.getxy(1)) //unit is not on the tile to the bottom.
					{
						down = true; //bottom is locked.
						//std::cout << "There is a unit on the bottom of the selected tile" << std::endl;
					}
				}
				break;
			}
		}
		for(int t = 0; t < *tt; ++t)
		{
			if(!(*selectedtile == *tiles[t])) //not the same tile
			{
				if(!right) //right is not taken
				{
					if((selectedtile->getxy(1) + 1) == tiles[t]->getxy(1) && selectedtile->getxy(2) == tiles[t]->getxy(2))
					{
						validRightMoves.push_back(tiles[t]);
						Tile *rightTile = tiles[t];
						++currentpos;
						if(currentpos <= selected->getUnitMov())
						{
							if(findRightMoves(currentpos, validRightMoves, tiles, rightTile, units))
							{
								--currentpos;
								right = true;
							}
						}
						else
						{
							--currentpos;
							right = true;
						}
					}
				}
				if(!up) //top is not taken
				{
					if((selectedtile->getxy(2) - 1) == tiles[t]->getxy(2) && selectedtile->getxy(1) == tiles[t]->getxy(1))
					{
						validRightMoves.push_back(tiles[t]);
						Tile *upTile = tiles[t];
						++currentpos;
						if(currentpos <= selected->getUnitMov())
						{
							if(findRightMoves(currentpos, validRightMoves, tiles, upTile, units))
							{
								--currentpos;
								up = true;
							}
						}
						else
						{
							--currentpos;
							up = true;
						}
					}
				}
				if(!down) //bottom is not taken
				{
					if((selectedtile->getxy(2) + 1) == tiles[t]->getxy(2) && selectedtile->getxy(1) == tiles[t]->getxy(1))
					{
					validRightMoves.push_back(tiles[t]);
					Tile *downTile = tiles[t];
					++currentpos;
					if(currentpos <= selected->getUnitMov())
						{
							if(findRightMoves(currentpos, validRightMoves, tiles, downTile, units))
							{
								--currentpos;
								down = true;
							}
						}
						else
						{
							--currentpos;
							down = true;
						}
					}
				}
				if(down && up && right)
				{
					found = true;
					break;
				}
			}
			else
			{
				continue;
			}
		}
	}
	else //done
	{
		found = true;
	}
	//std::cout << validRightMoves.size() << std::endl;
	//std::cout << right << std::endl;
	//std::cout << currentpos << std::endl;
	return found;
}

bool findLeftMoves(int &currentpos, std::vector<Tile*> &validLeftMoves, Tile *tiles[], Tile *selectedtile, std::vector<Unit> units)
{
	bool found = false;
	//false is empty and not in the validLeftMoves vector
	bool up = false;
	bool left = false;
	/*bool up = false;*/
	bool down = false;
	//remove duplicates
	std::set<Tile*> s;
	unsigned size = validLeftMoves.size();
	for( unsigned i = 0; i < size; ++i )
	{
		s.insert(validLeftMoves[i]);
	}
	validLeftMoves.assign( s.begin(), s.end() );
	if(currentpos <= selected->getUnitMov()) //less than the current position
	{
		//First, obtain the current valid move selected by the Tile pointer.
		for(auto & ite : validLeftMoves)
		{
			if(ite == selectedtile) //if the item in the valid moves matches the selected tile. This is to ensure that the selected pointer is a valid move.
			{
				for(auto &item2 : validLeftMoves) //iterates through to ensure that the right, left, top or bottom sides are NOT taken by the validLeftMoves vector.
				{
					if((selectedtile->getxy(1) - 1) < 0)
					{
						left = true;
						//std::cout << "The tile on the left doesn't exist" << std::endl;
					}
					else
					{
						if((selectedtile->getxy(1) - 1) == item2->getxy(1) && selectedtile->getxy(2) == item2->getxy(2)) //left
						{
							//std::cout << "The tile on the left is taken" << std::endl;
							left = true; //the left tile is locked.
						}
					}
					if((selectedtile->getxy(2) + 1) >= (*lh / 80))
					{
						down = true;
						//std::cout << "The tile on the bottom doesn't exist" << std::endl;
					}
					else
					{
						if((selectedtile->getxy(2) + 1) == item2->getxy(2) && selectedtile->getxy(1) == item2->getxy(1)) //bottom
						{
							//std::cout << "The tile on the bottom is taken" << std::endl;
							down = true; //the bottom tile is locked.
						}
					}
					if((selectedtile->getxy(2) - 1) < 0)
					{
						up = true;
						//std::cout << "The tile on the top doesn't exist" << std::endl;
					}
					else
					{
						if((selectedtile->getxy(2) - 1) == item2->getxy(2) && selectedtile->getxy(1) == item2->getxy(1)) //top
						{
							//std::cout << "The tile on the top is taken" << std::endl;
							up = true; //the top tile is locked.
						}
					}
				}
				for(auto &unit : units)
				{
					if((selectedtile->getxy(1) - 1) == unit.getxy(1) && selectedtile->getxy(2) == unit.getxy(2)) //unit is not on the tile to the left.
					{
						left = true; //left is locked.
						//std::cout << "There is a unit on the left of the selected tile" << std::endl;
					}
					if((selectedtile->getxy(2) + 1) == unit.getxy(2) && selectedtile->getxy(1) == unit.getxy(1)) //unit is not on the tile to the bottom.
					{
						down = true; //bottom is locked.
						//std::cout << "There is a unit on the bottom of the selected tile" << std::endl;
					}
					if((selectedtile->getxy(2) - 1) == unit.getxy(2) && selectedtile->getxy(1) == unit.getxy(1)) //unit is not on the tile to the right.
					{
						up = true; //right is locked.
						//std::cout << "There is a unit on the up of the selected tile" << std::endl;
					}
				}
				break;
			}
		}
		for(int t = 0; t < *tt; ++t)
		{
			if(!(*selectedtile == *tiles[t])) //not the same tile
			{
				if(!left) //left is not taken
				{
					if((selectedtile->getxy(1) - 1) == tiles[t]->getxy(1) && selectedtile->getxy(2) == tiles[t]->getxy(2))
					{
						validLeftMoves.push_back(tiles[t]);
						Tile *leftTile = tiles[t];
						++currentpos;
						if(currentpos <= selected->getUnitMov())
						{
							if(findLeftMoves(currentpos, validLeftMoves, tiles, leftTile, units))
							{
								--currentpos;
								left = true;
							}
						}
						else
						{
							--currentpos;
							left = true;
						}
					}
				}
				if(!down) //bottom is not taken
				{
					if((selectedtile->getxy(2) + 1) == tiles[t]->getxy(2) && selectedtile->getxy(1) == tiles[t]->getxy(1))
					{
					validLeftMoves.push_back(tiles[t]);
					Tile *downTile = tiles[t];
					++currentpos;
					if(currentpos <= selected->getUnitMov())
						{
							if(findLeftMoves(currentpos, validLeftMoves, tiles, downTile, units))
							{
								--currentpos;
								down = true;
							}
						}
						else
						{
							--currentpos;
							down = true;
						}
					}
				}
				if(!up) //top is not taken
				{
					if((selectedtile->getxy(2) - 1) == tiles[t]->getxy(2) && selectedtile->getxy(1) == tiles[t]->getxy(1))
					{
						validLeftMoves.push_back(tiles[t]);
						Tile *upTile = tiles[t];
						++currentpos;
						if(currentpos <= selected->getUnitMov())
						{
							if(findLeftMoves(currentpos, validLeftMoves, tiles, upTile, units))
							{
								--currentpos;
								up = true;
							}
						}
						else
						{
							--currentpos;
							up = true;
						}
					}
				}
				if(down && left && up)
				{
					found = true;
					break;
				}
			}
			else
			{
				continue;
			}
		}
	}
	else //done
	{
		found = true;
	}
	//std::cout << validLeftMoves.size() << std::endl;
	//std::cout << right << std::endl;
	//std::cout << currentpos << std::endl;
	return found;
}

bool play(Tile *tiles[],std::vector<Building> &buildings, std::vector<Unit> &totalUnits)
{
	SDL_Event d;
	bool quit = false;
	bool notwin = true;
	bool grid = false;
	int x,y;
	Dot dot;
	inter bottom;
	inter unitType;
	inter godicon;
	inter unitType2;
	inter unitType3;
	quit = selectGods(godicon);
	Text tileLabel("Tile:",10,700,10);
	Text tileName("N/A",60,700,10);
	Text tilestats("N/A",350,700,10);
	Text tileX("X:0",10,746,3);
	Text tileY("Y:0",10,769,3);
	Text unitLabel("Unit:Crystalline Shape-Shifter",10,722,10);
	Text unitstats("Stats: S:10/20; M:8/20; Type:",82,744,10);
	Text buildingLabel("Building:Village(D:40%;P:25)",82,766,10);
	Text power("Power:",550,700,10);
	Text timer("300",740,766,10);
	Text turns("Turn:",670,724,10);
	Text uname("Crystalline Shape-Shifter",0,0,10);
	Text uStr("Stength:10/99",0,22,10);
	Text uMov("Moves:12/20",190,22,10);
	Text uTeam("Team: 2",690,0,10);
	Text uType("Type: ",500,0,10);
	Text comp("=",600,0,10);
	Text bName("Village",0,667,10);
	Text bDef("Defense: 20%",120,667,10);
	Text bPow("Power: 25",300,667,10);
	Text bTeam("Team: 2",690,667,10);
	Text bCon("Controlled:10/10",450,667,10);
	bottom.setPosition(0,700);
	bottom.setSize(100,800);
	unitType.setPosition(450,752);
	unitType.setSize(20,20);
	unitType2.setPosition(570,5);
	unitType2.setSize(20,20);
	unitType3.setPosition(620,5);
	unitType3.setSize(20,20);
	godicon.setPosition(550,744);
	godicon.setSize(50,50);
	LButton tileOverlay;
	tileOverlay.setPosition(0,0);
	tileOverlay.setSize(700,800);
	tileOverlay.setButtonType(1);
	LButton endturnb;
	endturnb.setPosition(600,774);
	endturnb.setSize(20,120);
	endturnb.setButtonType(2);
	LButton menu;
	menu.setPosition(600,754);
	menu.setSize(20,120);
	menu.setButtonType(3);
	//The application timer
	LTimer timerturn;

	//In memory text stream
	std::stringstream timeText;

	std::vector<Tile*> validMoves;
	std::vector<Tile*> validUpMoves;
	std::vector<Tile*> validDownRightMoves;
	std::vector<Tile*> validDownLeftMoves;
	std::vector<Tile*> validLeftMoves;
	std::vector<Tile*> validRightMoves;

	std::vector<Tile*> validAttackMoves;
	std::vector<Tile*> validUpAttackMoves;

	Uint32 val = 0;
	int turn = 1;
	timerptr = &timerturn;
	timerturn.start();


	for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
	{
			buttonendturnsprites[ i ].x = 0;
			buttonendturnsprites[ i ].y = i * 20;
			buttonendturnsprites[ i ].w = endturnb.getRect().w;
			buttonendturnsprites[ i ].h = endturnb.getRect().h;

			buttonmenugameplaysprites[ i ].x = 0;
			buttonmenugameplaysprites[ i ].y = i * 20;
			buttonmenugameplaysprites[ i ].w = menu.getRect().w;
			buttonmenugameplaysprites[ i ].h = menu.getRect().h;
	}
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - 100};
	while(!quit && notwin)
	{
		//Handle events on queue
		while( SDL_PollEvent( &d ) != 0 )
		{
			//User requests quit
			if( d.type == SDL_QUIT )
			{
				//collects text to release textures (possibly implement exception)
				textptr.push_back(tileName);
				textptr.push_back(tileLabel);
				textptr.push_back(tilestats);
				textptr.push_back(tileX);
				textptr.push_back(tileY);
				textptr.push_back(unitLabel);
				textptr.push_back(unitstats);
				textptr.push_back(buildingLabel);
				textptr.push_back(power);
				textptr.push_back(timer);
				textptr.push_back(turns);
				textptr.push_back(uname);
				textptr.push_back(uStr);
				textptr.push_back(uMov);
				textptr.push_back(uTeam);
				textptr.push_back(uType);
				textptr.push_back(comp);
				textptr.push_back(bName);
				textptr.push_back(bDef);
				textptr.push_back(bPow);
				textptr.push_back(bTeam);
				textptr.push_back(bCon);
				timerturn.stop();
				quit = true;
			}
			if( d.type == SDL_KEYDOWN )
            {
                        //Select surfaces based on key press
                        switch( d.key.keysym.sym )
                        {
						case SDLK_BACKSPACE: selected = NULL;
							break;
						case SDLK_g: if(grid == false){grid = true;} else {grid = false;}
                        }
            }
			//Handle input for the dot
			dot.handleEvent( d );
			tileOverlay.handleEvent(&d, tiles);
			tileOverlay.handleEvent(&d, validMoves,totalUnits);
			endturnb.handleEvent(&d,quit,notwin);
			menu.handleEvent(&d,quit,notwin);
		for(int z = 0; z < buildings.size(); ++z)
		{
			buildings.at(z).handlebuttonevents(&d);
		}
		for(int w = 0; w < totalUnits.size(); ++w)
		{
			totalUnits.at(w).handlebuttonevents(&d);
		}
		}
		SDL_GetMouseState(&x,&y);
		turnptr = &turn;
		dot.move(tiles);
		dot.setCamera( camera );
		cursorptr = &dot;
		tileName.setText(updateTileInfo(tiles,dot,1));
		tilestats.setText(updateTileInfo(tiles,dot,2));
		tileX.setText(updateTileInfo(tiles,dot,3));
		tileY.setText(updateTileInfo(tiles,dot,4));
		buildingLabel.setText(updateBuildingInfo(buildings,dot));
		cameraptr = &camera;
		SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
		SDL_RenderClear( gRenderer );
		for( int i = 0; i < *tt; ++i )
		{
			tiles[ i ]->render( camera );
		}
		if(grid)
		{
			for(int u = 0; u < totalUnits.size(); ++u)
			{
				for(int i = 0; i < *tt; ++i)
				{
					if(totalUnits.at(u).getxy(1) >= tiles[i]->getxy(1) - (totalUnits.at(u).getUnitMov() + totalUnits.at(u).getRange()) && totalUnits.at(u).getxy(1) <= tiles[i]->getxy(1) + (totalUnits.at(u).getUnitMov() + totalUnits.at(u).getRange()) && totalUnits.at(u).getxy(2) >= tiles[i]->getxy(2) - (totalUnits.at(u).getUnitMov() + totalUnits.at(u).getRange()))
					{
						if((totalUnits.at(u).getxy(1) + totalUnits.at(u).getxy(2)) + (totalUnits.at(u).getUnitMov() + totalUnits.at(u).getRange()) >= (tiles[i]->getxy(1) + tiles[i]->getxy(2)) && (totalUnits.at(u).getxy(1) - totalUnits.at(u).getxy(2)) - (totalUnits.at(u).getUnitMov() + totalUnits.at(u).getRange()) <= (tiles[i]->getxy(1) - tiles[i]->getxy(2)))
						{
							if((totalUnits.at(u).getxy(1) + totalUnits.at(u).getxy(2)) - (totalUnits.at(u).getUnitMov() + totalUnits.at(u).getRange()) <= (tiles[i]->getxy(1) + tiles[i]->getxy(2)) && (totalUnits.at(u).getxy(1) - totalUnits.at(u).getxy(2)) + (totalUnits.at(u).getUnitMov() + totalUnits.at(u).getRange()) >= (tiles[i]->getxy(1) - tiles[i]->getxy(2)))
							{
								if(totalUnits.at(u).getTeam() != 1)
								{
								gTileUnitRange.render(tiles[i]->mBox.x - camera.x, tiles[i]->mBox.y - camera.y);
								}
							}
						}
					}
				}
			}
		}
		if(!validAttackMoves.empty())
		{
			validAttackMoves.clear();
			validAttackMoves.shrink_to_fit();
		}
		if(!validUpAttackMoves.empty())
		{
			validUpAttackMoves.clear();
			validUpAttackMoves.shrink_to_fit();
		}
		if(selected != nullptr)
		{
		int totall = 0;
		Tile *selecteda = nullptr;
		while(totall <= (selected->getUnitMov() + selected->getRange()))
		{
			if(totall == 0) //unit starting position
			{
				for(int i = 0; i < *tt; ++i)
				{
					if(tiles[i]->getxy(1) == selected->getxy(1) && tiles[i]->getxy(2) == selected->getxy(2)) //locate unit position
					{
						gTileUnitAttack.render(tiles[i]->mBox.x - camera.x, tiles[i]->mBox.y - camera.y);
						validAttackMoves.push_back(tiles[i]);
						validUpAttackMoves.push_back(tiles[i]);
						//validDownRightMoves.push_back(tiles[i]);
						//validDownLeftMoves.push_back(tiles[i]);
						//validRightMoves.push_back(tiles[i]);
						//validLeftMoves.push_back(tiles[i]);
						selecteda = tiles[i];
						++totall;
						break;
					}
				}
			}
			else
			{
				if(findUpMoves(totall, validUpAttackMoves, tiles, selecteda, totalUnits, 2))
				{
					std::cout << "Total Up tiles found:" << validUpAttackMoves.size() << std::endl;
				}
				else
				{
					std::cout << "No Up moves were found!" << std::endl;
				}
				//if(findDownRightMoves(total, validDownRightMoves, tiles, selectedd, totalUnits))
				//{
				//	std::cout << "Total Down Right tiles found:" << validDownRightMoves.size() << std::endl;
				//}
				//else
				//{
				//	std::cout << "No Down Right moves were found!" << std::endl;
				//}
				//if(findDownLeftMoves(total, validDownLeftMoves, tiles, selectedd, totalUnits))
				//{
				//	std::cout << "Total Down Left tiles found:" << validMoves.size() << std::endl;
				//}
				//else
				//{
				//	std::cout << "No Down Right moves were found!" << std::endl;
				//}
				//if(findRightMoves(total, validRightMoves, tiles, selectedd, totalUnits))
				//{
				//	std::cout << "Total Right tiles found:" << validRightMoves.size() << std::endl;
				//}
				//else
				//{
				//	std::cout << "No Right moves were found!" << std::endl;
				//}
				//if(findLeftMoves(total, validLeftMoves, tiles, selectedd, totalUnits))
				//{
				//	std::cout << "Total Left tiles found:" << validMoves.size() << std::endl;
				//}
				//else
				//{
				//	std::cout << "No Left moves were found!" << std::endl;
				//}
				validAttackMoves.insert(validAttackMoves.end(), validUpAttackMoves.begin(), validUpAttackMoves.end());
				//validMoves.insert(validMoves.end(), validDownRightMoves.begin(), validDownRightMoves.end());
				//validMoves.insert(validMoves.end(), validDownLeftMoves.begin(), validDownLeftMoves.end());
				//validMoves.insert(validMoves.end(), validRightMoves.begin(), validRightMoves.end());
				//validMoves.insert(validMoves.end(), validLeftMoves.begin(), validLeftMoves.end());
				//remove duplicates
				std::set<Tile*> t;
				unsigned size = validAttackMoves.size();
				for( unsigned i = 0; i < size; ++i )
				{
					t.insert(validAttackMoves[i]);
				}
				validAttackMoves.assign( t.begin(), t.end() );
				/*validateDownMoves(validMoves,tiles,selecteda,totalUnits);*/
				for(auto &item : validAttackMoves)
				{
					gTileUnitAttack.render(item->mBox.x - camera.x, item->mBox.y - camera.y);
				}
				break;
			}
		}
		}
		//attack moves
		//if(selected != NULL)
		//{
		//for(int i = 0; i < *tt; ++i)
		//{
		//	if(selected->getxy(1) >= tiles[i]->getxy(1) - (selected->getUnitMov() + selected->getRange()) && selected->getxy(1) <= tiles[i]->getxy(1) + (selected->getUnitMov() + selected->getRange()) && selected->getxy(2) >= tiles[i]->getxy(2) - (selected->getUnitMov() + selected->getRange()))
		//	{
		//		if((selected->getxy(1) + selected->getxy(2)) + (selected->getUnitMov() + selected->getRange()) >= (tiles[i]->getxy(1) + tiles[i]->getxy(2)) && (selected->getxy(1) - selected->getxy(2)) - (selected->getUnitMov() + selected->getRange()) <= (tiles[i]->getxy(1) - tiles[i]->getxy(2)))
		//		{
		//			if((selected->getxy(1) + selected->getxy(2)) - (selected->getUnitMov() + selected->getRange()) <= (tiles[i]->getxy(1) + tiles[i]->getxy(2)) && (selected->getxy(1) - selected->getxy(2)) + (selected->getUnitMov() + selected->getRange()) >= (tiles[i]->getxy(1) - tiles[i]->getxy(2)))
		//			{
		//						gTileUnitAttack.render(tiles[i]->mBox.x - camera.x, tiles[i]->mBox.y - camera.y);
		//						validAttackMoves.push_back(tiles[i]);
		//				}
		//			}
		//	}
		//	}
		//}
		//valid moves
		if(!validMoves.empty())
		{
			validMoves.clear();
		}
		if(!validUpMoves.empty())
		{
			validUpMoves.clear();
		}
		if(!validDownRightMoves.empty())
		{
			validDownRightMoves.clear();
		}
		if(!validDownLeftMoves.empty())
		{
			validDownLeftMoves.clear();
		}
		if(!validRightMoves.empty())
		{
			validRightMoves.clear();
		}
		if(!validLeftMoves.empty())
		{
			validLeftMoves.clear();
		}
		if(selected != nullptr)
		{
		//first approach: iterates through the entire tiles
		//for(int i = 0; i < *tt; ++i)
		//{
		//	//if(selected->getxy(1) >= tiles[i]->getxy(1) - selected->getUnitMov() && selected->getxy(1) <= tiles[i]->getxy(1) + selected->getUnitMov() && selected->getxy(2) >= tiles[i]->getxy(2) - selected->getUnitMov()) //up, down, left and right
		//	//{
		//		if((selected->getxy(1) + selected->getxy(2)) + selected->getUnitMov() >= (tiles[i]->getxy(1) + tiles[i]->getxy(2)) && (selected->getxy(1) - selected->getxy(2)) - selected->getUnitMov() <= (tiles[i]->getxy(1) - tiles[i]->getxy(2))) //left bottom, right top
		//		{
		//			if((selected->getxy(1) + selected->getxy(2)) - selected->getUnitMov() <= (tiles[i]->getxy(1) + tiles[i]->getxy(2)) && (selected->getxy(1) - selected->getxy(2)) + selected->getUnitMov() >= (tiles[i]->getxy(1) - tiles[i]->getxy(2))) //left top, right bottom
		//			{
		//				bool unitOnTile = false;
		//				for(int a = 0; a < totalUnits.size(); ++a)
		//				{
		//					if(totalUnits.at(a).getxy(1) == tiles[i]->getxy(1) && totalUnits.at(a).getxy(2) == tiles[i]->getxy(2))
		//					{
		//						unitOnTile = true;
		//						break;
		//					}
		//				}
		//				if(!unitOnTile)
		//				{
		//						gTileUnitMove.render(tiles[i]->mBox.x - camera.x, tiles[i]->mBox.y - camera.y);
		//						validMoves.push_back(tiles[i]);
		//				}
		//			}
		//		}
		//	//}
		//}
		//}
		//second approach: takes the unit's position and scales until there are no tiles left to move on.
		int total = 0;
		Tile *selectedd = nullptr;
		while(total <= selected->getUnitMov())
		{
			if(total == 0) //unit starting position
			{
				for(int i = 0; i < *tt; ++i)
				{
					if(tiles[i]->getxy(1) == selected->getxy(1) && tiles[i]->getxy(2) == selected->getxy(2)) //locate unit position
					{
						gTileUnitMove.render(tiles[i]->mBox.x - camera.x, tiles[i]->mBox.y - camera.y);
						validMoves.push_back(tiles[i]);
						validUpMoves.push_back(tiles[i]);
						validDownRightMoves.push_back(tiles[i]);
						validDownLeftMoves.push_back(tiles[i]);
						validRightMoves.push_back(tiles[i]);
						validLeftMoves.push_back(tiles[i]);
						selectedd = tiles[i];
						++total;
						break;
					}
				}
			}
			else
			{
				if(findUpMoves(total, validUpMoves, tiles, selectedd, totalUnits, 1))
				{
					std::cout << "Total Up tiles found:" << validMoves.size() << std::endl;
				}
				else
				{
					std::cout << "No Up moves were found!" << std::endl;
				}
				if(findDownRightMoves(total, validDownRightMoves, tiles, selectedd, totalUnits))
				{
					std::cout << "Total Down Right tiles found:" << validDownRightMoves.size() << std::endl;
				}
				else
				{
					std::cout << "No Down Right moves were found!" << std::endl;
				}
				if(findDownLeftMoves(total, validDownLeftMoves, tiles, selectedd, totalUnits))
				{
					std::cout << "Total Down Left tiles found:" << validMoves.size() << std::endl;
				}
				else
				{
					std::cout << "No Down Right moves were found!" << std::endl;
				}
				if(findRightMoves(total, validRightMoves, tiles, selectedd, totalUnits))
				{
					std::cout << "Total Right tiles found:" << validRightMoves.size() << std::endl;
				}
				else
				{
					std::cout << "No Right moves were found!" << std::endl;
				}
				if(findLeftMoves(total, validLeftMoves, tiles, selectedd, totalUnits))
				{
					std::cout << "Total Left tiles found:" << validMoves.size() << std::endl;
				}
				else
				{
					std::cout << "No Left moves were found!" << std::endl;
				}
				validMoves.insert(validMoves.end(), validUpMoves.begin(), validUpMoves.end());
				validMoves.insert(validMoves.end(), validDownRightMoves.begin(), validDownRightMoves.end());
				validMoves.insert(validMoves.end(), validDownLeftMoves.begin(), validDownLeftMoves.end());
				validMoves.insert(validMoves.end(), validRightMoves.begin(), validRightMoves.end());
				validMoves.insert(validMoves.end(), validLeftMoves.begin(), validLeftMoves.end());
				//remove duplicates
				std::set<Tile*> s;
				unsigned size = validMoves.size();
				for( unsigned i = 0; i < size; ++i )
				{
					s.insert(validMoves[i]);
				}
				validMoves.assign( s.begin(), s.end() );
				validateDownMoves(validMoves,tiles,selectedd,totalUnits);
				for(auto &item : validMoves)
				{
					gTileUnitMove.render(item->mBox.x - camera.x, item->mBox.y - camera.y);
				}
				break;
			}
		}
		}
		dot.render( camera );
		for(int b = 0; b < buildings.size(); ++b)
		{
			buildings.at(b).render(camera);
		}
		for(int z = 0; z < buildings.size(); ++z)
		{
		if(x >= (buildings.at(z).bBox.x - cameraptr->x) && x <= (buildings.at(z).bBox.x + buildings.at(z).bBox.w - cameraptr->x) && y >= (buildings.at(z).bBox.y - cameraptr->y) && y <= (buildings.at(z).bBox.y + buildings.at(z).bBox.h - cameraptr->y))
		{
			//unitType2.render(2);
			//comp.drawText();
			//unitType3.render(2);
			bName.setText(buildings.at(z).getinfo(1));
			bName.drawText();
			bDef.setText(buildings.at(z).getinfo(2));
			bDef.drawText();
			bPow.setText(buildings.at(z).getinfo(3));
			bPow.drawText();
			bTeam.setText(buildings.at(z).getinfo(4));
			bTeam.drawText();
			bCon.setText(buildings.at(z).getinfo(5));
			bCon.drawText();
		}
		}
		for(int q = 0; q < totalUnits.size(); ++q)
		{
			totalUnits.at(q).render(camera);
		}
		for(int u = 0; u < totalUnits.size(); ++u)
		{
			if(x >= (totalUnits.at(u).uBox.x - cameraptr->x) && x <= (totalUnits.at(u).uBox.x + totalUnits.at(u).uBox.w - cameraptr->x) && y >= (totalUnits.at(u).uBox.y - cameraptr->y) && y <= (totalUnits.at(u).uBox.y + totalUnits.at(u).uBox.h - cameraptr->y))
			{
			uname.setText(totalUnits.at(u).getinfo(1));
			uname.drawText();
			uStr.setText(totalUnits.at(u).getinfo(2));
			uStr.drawText();
			uMov.setText(totalUnits.at(u).getinfo(3));
			uMov.drawText();
			uTeam.setText(totalUnits.at(u).getinfo(4));
			uTeam.drawText();
			uType.drawText();
			switch(totalUnits.at(u).getUnitType())
			{
			case 1: unitTypeSkirmisher.render((uType.getX() + 70), (uType.getY() + 10)); break;
			}
			}
		}
		bottom.render(1);
		unitType.render(2);
		godicon.render();
		tileOverlay.render(1);
		endturnb.render(2);
		menu.render(3);
		tileLabel.drawText();
		tileName.drawText();
		tilestats.drawText();
		tileX.drawText();
		tileY.drawText();
		unitLabel.drawText();
		unitstats.drawText();
		buildingLabel.drawText();
		power.drawText();
		turns.setText("Turns:" + std::to_string(*turnptr));
		turns.drawText();
		timeText.str("");
		val = 20 - ( timerturn.getTicks() / 1000.f );
		if(val == 0)
		{
			endturn(notwin);
			for(auto &item : totalUnits)
			{
				item.resetUnitMovement();
			}
		}
		else
		{
			timeText << val;
			cTimeptr = &val;
			timer.setText(timeText.str());
			timer.drawText();
		}
		SDL_RenderPresent( gRenderer );
	}
	return (notwin || quit);
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//The level tiles
		//Tile* tileSet[ *tt ];
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//if(!loadMap(tileSet,3))
			//{
			//	close(tileSet);
			//	return 0;
			//}
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen
			//Dot dot;

			//Level camera
			//SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
						//close( tileSet );
						return 0;
					}

					//Handle input for the dot
					//dot.handleEvent( e );
				}

				////Move the dot
				//dot.move( tileSet );
				//dot.setCamera( camera );
				////Render level
				//for( int i = 0; i < *tt; ++i )
				//{
				//	tileSet[ i ]->render( camera );
				//}
				if(loadMenu(e))
				{
					close();
					return 0;
				}
				////Render dot
				//dot.render( camera );
			}
		}
	}
}
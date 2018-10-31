#include "SDL2_header.h"
#include <cstdio>
#include <map>



const std::string Game::TitleName = "Not A Simple Game Demo";
const int Game::SCREEN_WIDTH	= 640;
const int Game::SCREEN_HEIGHT	= 480;
using namespace Game;

std::map<int, bool> keyboard;



PointD posPlayer, velocityPlayer;
std::vector <PointD> posEnemy(10);
std::vector <PointD> posBullet(10);
PointD velocityBullet;

double speedPlayer;

Image *imagePlayer, *imageBullet, *imageEnemy, *images[100];

void loadPictures()
{
	imagePlayer = loadImage( "player.png" );
	imageBullet = loadImage( "bullet.png" );
	imageEnemy	= loadImage( "enemy.png"  );
}

void initialize()
{
	FPS_DISPLAY = true;
	posPlayer = PointD( SCREEN_WIDTH/2, SCREEN_HEIGHT/2 );
	posEnemy[0] = posPlayer;
	velocityBullet= PointD(0,-5);
	speedPlayer=5;
	canvasColor = {0, 0, 0, 255};
	loadPictures();
}

void drawPlayer()
{
	int w,h;
	getImageSize( imagePlayer, w, h );
	setImageAlpha( imagePlayer, 150);
	drawImage( imagePlayer, posPlayer.x-w/2, posPlayer.y-h/2, 0.5, 0.5 );
}

void drawBackground()
{
	Rect rect = {70, 50, 80, 90};
	setPenColor((Color){255, 255, 0, 255});
	//	Pay attention: (Color){255,255,0} means (Color){255,255,0,0}
	//	and means you will draw nothing
	drawRect( rect, true );
}

void drawForeground()
{
	Rect rect = {400, 0, 85, 800};//画一块颜色,左边界，上边界，右边界 ，下边界
	setPenColor((Color){0, 255, 0, 200});
	drawRect( rect, true );
}

void drawHint()
{
	Image *text = textToImage( "< This is not a simple game demo. >");
	int w,h;
	getImageSize( text, w, h );
	drawImage( text, SCREEN_WIDTH-h/2-w/2, SCREEN_HEIGHT/2-h/2, 1, 1, 90+180 );
	cleanup(text);
}

void drawBullet()
{
   for(std::vector<PointD>::iterator iter=posBullet.begin();iter!=posBullet.end();++iter)
	  drawImage(imageBullet, (*iter).x, (*iter).y, 0.5, 0.5 );
}
int lastAnime = 0;
void drawEnemy()
{
	int w,h;
	getImageSize( imageEnemy, w, h );
	lastAnime = (lastAnime+15)%(4*60);
	Rect clip = {w/16*(lastAnime/60), 0, w/16,h/16};
	setImageAlpha( imageEnemy, 255 );
	drawImage( imageEnemy, posEnemy[0].x, posEnemy[0].y, 2, 2, 0, NULL, FLIP_NONE, &clip );
}

void draw()
{
	//drawBackground();
	drawPlayer();
	drawBullet();
	drawEnemy();
	//drawForeground();
	drawHint();
}
void deal()
{
    bool shoot=false;
	bool move = false;
	if(keyboard['k'])
    {
        shoot=true;
    }
	if( keyboard[KEY_UP]	|| keyboard['w'] )
	{
		velocityPlayer = velocityPlayer + PointD(0,-1)*speedPlayer;
		move = true;
	}
	if( keyboard[KEY_DOWN]	|| keyboard['s'] )
	{
		velocityPlayer = velocityPlayer + PointD(0,+1)*speedPlayer;
		move = true;
	}
	if( keyboard[KEY_LEFT]	|| keyboard['a'] )
	{
		velocityPlayer = velocityPlayer + PointD(-1,0)*speedPlayer;
		move = true;
	}
	if( keyboard[KEY_RIGHT] || keyboard['d'] )
	{
		velocityPlayer = velocityPlayer + PointD(+1,0)*speedPlayer;
		move = true;
	}
	double len = velocityPlayer.length();
	if( len > speedPlayer )
	{
		velocityPlayer = velocityPlayer/len*speedPlayer;
	}

   for(std::vector<PointD>::iterator iter=posBullet.begin();iter!=posBullet.end();++iter)
	{
	     *iter=*iter+ velocityBullet;
        if(-(*iter).y>SCREEN_WIDTH){
            posBullet.erase(iter);
            std::cout<<"Bulletout!"<<std::endl;
        }
	}

	if(shoot){
       posBullet.push_back(posPlayer);
	}


    if(posPlayer.x+ velocityPlayer.x<0||posPlayer.y+velocityPlayer.y<0||posPlayer.y+velocityPlayer.y>SCREEN_HEIGHT||
       posPlayer.x+ velocityPlayer.x>SCREEN_WIDTH){
        velocityPlayer=velocityPlayer*0;
    }
	else{
    posPlayer = posPlayer + velocityPlayer;
	}

	if(!move)
	{
		velocityPlayer = velocityPlayer * 0.8;
		if( velocityPlayer.length() < 0.1 )
			velocityPlayer = PointD();
	}
	posEnemy[0] = PointD( mouseX, mouseY );
}

int work( bool &quit )
{
	deal();
	draw();
	if( keyboard[KEY_ESC] )
		quit = true;
	return 0;
}

void mousePress()
{
}

void mouseMove()
{

}

void mouseRelease()
{

}

void keyDown()
{
	keyboard[keyValue] = true;
}

void keyUp()
{
	keyboard[keyValue] = false;
}

void finale()
{
	cleanup( imagePlayer, imageBullet, imageEnemy);
	for( int i = 0; i < 100; ++i )
		cleanup( images[i] );
}



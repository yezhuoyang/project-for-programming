#include "SDL2_header.h"
#include <cstdio>
#include <map>
#include<ctime>

const std::string Game::TitleName = "Not A Simple Game Demo";
const int Game::SCREEN_WIDTH	= 640;//宽度，x坐标
const int Game::SCREEN_HEIGHT	= 480;//长度,y坐标
using namespace Game;
std::map<int, bool> keyboard;
PointD posPlayer, velocityPlayer;
std::vector <PointD> posEnemy(10);
std::vector <PointD> posBullet(10);
std::vector <PointD> EnemyBullet(100);//用于存放敌人子弹


PointD velocityBullet;
PointD velocityEnemyBullet;

double speedPlayer;
double radiusPlayer=5,radiusEnemy=5;//玩家飞机与敌机的半径
double lifeofPlayer;//玩家的生命值
double lifeofEnemy;//玩家的生命值
double powerofPlayer;//玩家子弹的攻击力
double powerofEnemy;//敌人子弹的攻击力


Image *imagePlayer, *imageBullet, *imageEnemy, *images[100],*imageEnemyBullet;

void loadPictures()
{
	imagePlayer = loadImage( "player.png" );
	imageBullet = loadImage( "bullet.png" );
	imageEnemyBullet=loadImage("bullet.png");//敌人子弹图像
	imageEnemy=loadImage( "enemy.png" );
}

void initialize()
{
	FPS_DISPLAY = true;
	posPlayer = PointD( SCREEN_WIDTH/2, SCREEN_HEIGHT/2 );
	//posEnemy[0] = posPlayer;
	velocityBullet= PointD(0,-5);
	lifeofEnemy=10;//敌人初始生命值
	lifeofPlayer=10;//玩家初始生命值
	powerofPlayer=10;//玩家子弹的攻击力
	powerofEnemy=1;//玩家子弹的攻击力
	speedPlayer=5;
	canvasColor = {0, 0, 0, 255};
	loadPictures();
    //设置随机数种子
    srand((unsigned)time(NULL));
}

void drawPlayer()
{
	int w,h;
	getImageSize( imagePlayer, w, h );
	setImageAlpha( imagePlayer, 150);
	drawImage( imagePlayer, posPlayer.x-w/2, posPlayer.y-h/2, 0.5, 0.5 );
}

//判断是否发生碰撞

bool collide(PointD A,double RA,PointD B,double RB){
    return false;//(B-A).length()<=(RA+RB);
}



//以下两个函数用于画游戏开始与结束
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


//以下函数用于弹出提示字符
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

   for(std::vector<PointD>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();++iter)
	  drawImage(imageEnemyBullet, (*iter).x, (*iter).y, 0.5, 0.5 );

}

int lastAnime = 0;

void drawEnemy()
{
   for(std::vector<PointD>::iterator iter=posEnemy.begin();iter!=posEnemy.end();++iter)
	  drawImage(imageEnemy, (*iter).x, (*iter).y, 0.5, 0.5 );

/*
	int w,h;
	getImageSize( imageEnemy, w, h );
	lastAnime = (lastAnime+15)%(4*60);
	Rect clip = {w/16*(lastAnime/60), 0, w/16,h/16};
	setImageAlpha( imageEnemy, 255 );
	drawImage( imageEnemy, posEnemy[0].x, posEnemy[0].y, 2, 2, 0, NULL, FLIP_NONE, &clip );
*/
}

//此函数用于生成敌人
void  newEnemy(){
    PointD newposition;
    newposition.x=(rand()%640);
    newposition.y=0;
    newposition.life=lifeofEnemy;
    posEnemy.push_back(newposition);
}

void draw()
{
	//drawBackground();
	drawPlayer();
	drawBullet();
	drawEnemy();
	//drawForeground();
	//drawHint();
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
        //如果超出屏幕，就把子弹的内存释放掉
        if(-(*iter).y>SCREEN_WIDTH){
            posBullet.erase(iter);
            std::cout<<"Bulletout!"<<std::endl;
        }
        //判断每颗玩家的子弹否与某个敌人相碰撞
        for(std::vector<PointD>::iterator iter2=posEnemy.begin();iter2!=posEnemy.end();++iter2)
        {
            if(collide(*iter,0,*iter2,radiusEnemy))
            {
                //如果碰撞，则将子弹去除，待加碰撞效果
                posBullet.erase(iter);
                (*iter2).life-=powerofPlayer;
                std::cout<<"Shoot!"<<std::endl;
            }
        }
	}

	//除去被击毁的地机
    for(std::vector<PointD>::iterator iter=posEnemy.begin();iter!=posEnemy.end();++iter)
    {
        if((*iter).life<0){
            posEnemy.erase(iter);
        }
    }

    //如果按了射击键，则生成一个新的子弹
	if(shoot){
       posBullet.push_back(posPlayer);
	}

    //判断玩家是否触游戏边界
    if(posPlayer.x+ velocityPlayer.x<0||posPlayer.y+velocityPlayer.y<0||posPlayer.y+velocityPlayer.y>SCREEN_HEIGHT||
       posPlayer.x+ velocityPlayer.x>SCREEN_WIDTH){
        velocityPlayer=velocityPlayer*0;
    }
	else{
    posPlayer = posPlayer + velocityPlayer;
	}


    //玩家移动
	if(!move)
	{
		velocityPlayer = velocityPlayer * 0.8;
		if( velocityPlayer.length() < 0.1 )
			velocityPlayer = PointD();
	}


	//posEnemy[0] = PointD(mouseX,mouseY);
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



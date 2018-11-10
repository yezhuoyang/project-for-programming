#include "SDL2_header.h"
#include <cstdio>
#include <map>
#include<ctime>
#include <string>
const std::string Game::TitleName = "Not A Simple Game Demo";
const int Game::SCREEN_WIDTH	= 640;//宽度，x坐标
const int Game::SCREEN_HEIGHT	= 480;//长度,y坐标
using namespace Game;
std::map<int, bool> keyboard;
object Player;
PointD velocityPlayer;
std::vector <object> Enemy={};
std::vector <object> PlayerBullet={};
std::vector <object> EnemyBullet={};//用于存放敌人子弹


double velocityBullet;
double velocityEnemyBullet;
double speedPlayer;
double radiusPlayer=50,radiusEnemy=50;//玩家飞机与敌机的半径
double lifeofPlayer;//玩家的生命值
double lifeofEnemy;//玩家的生命值
double powerofPlayer;//玩家子弹的攻击力
double powerofEnemy;//敌人子弹的攻击力
double powerofCollide;//任意两个物体相撞的伤害值


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
    lifeofPlayer=10;//玩家初始生命值
	Player = object( SCREEN_WIDTH/2, SCREEN_HEIGHT/2,radiusPlayer,lifeofPlayer,0,0,0);
	//Enemy[0] = Player
	FPS_DISPLAY = true;;
	velocityBullet=-5;
	velocityEnemyBullet=-10;
	lifeofEnemy=100;//敌人初始生命值
    Player.life=lifeofPlayer;
	powerofPlayer=50;//玩家子弹的攻击力
	powerofCollide=100;
	powerofEnemy=1;//玩家子弹的攻击力
	speedPlayer=10;
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
	drawImage( imagePlayer, Player.pos.x-w/2, Player.pos.y-h/2, 0.5, 0.5 );
}

//判断是否发生碰撞

bool collide(object A,object B){
    return (B.pos-A.pos).length()<=(A.radius+B.radius);
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

//显示玩家当前状态

void drawStatus(){
       std::stringstream ss;
       std::string status;
       ss<<Player.life;
       ss>>status;
       Image *text=textToImage(status);
       int w,h;
       getImageSize(text,w,h);
	   drawImage(text,0,SCREEN_HEIGHT/2,1,1,0);
	   cleanup(text);
}

void drawBullet()
{
   for(std::vector<object>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();++iter)
	  drawImage(imageBullet, (*iter).pos.x, (*iter).pos.y, 0.5, 0.5 );
   for(std::vector<object>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();++iter)
	  drawImage(imageEnemyBullet, (*iter).pos.x, (*iter).pos.y, 0.5, 0.5 );
}

int lastAnime = 0;
void drawEnemy()
{
   int w=10,h=10;
   for(std::vector<object>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter){
          getImageSize(imageEnemy,w,h );
	      lastAnime = (lastAnime+15)%(4*60);
          Rect clip = {w/16*(lastAnime/60), 0, w/16,h/16};
	      setImageAlpha( imageEnemy, 255 );
          drawImage( imageEnemy, (*iter).pos.x, (*iter).pos.y, 2, 2, 0, NULL, FLIP_NONE, &clip );
	      //drawImage(imageEnemy, (*iter).pos.x,(*iter).pos.y, 0.5,0.5);
}

/*
	int w,h;
	getImageSize( imageEnemy, w, h );
	lastAnime = (lastAnime+15)%(4*60);
	Rect clip = {w/16*(lastAnime/60), 0, w/16,h/16};
	setImageAlpha(imageEnemy,255);
	drawImage( imageEnemy, Enemy[0].x, Enemy[0].y, 2, 2, 0, NULL, FLIP_NONE, &clip );
*/
}
//此函数用于生成敌人


void  newEnemy(){
    if(Enemy.size()<4){
        object tmp((rand()%640),10,lifeofEnemy,radiusEnemy,0,0,1);
        Enemy.push_back(tmp);
    }
}


//速度更新,随机移动
void moveEnemy(){
/*
for(std::vector<object>::iterator iter1=Enemy.begin();iter1!=Enemy.end();++iter1)
{
    for(std::vector<object>::iterator iter2=iter1;iter2!=Enemy.end();++iter2)
    {
        if(collide(*iter1,*iter2)){
                 (*iter1).speed.x=0;
                 (*iter1).speed.y=0;
         }
    }
}
*/
for(std::vector<object>::iterator iter=Enemy.begin();iter!=Enemy.end();)
{
     if((*iter).pos.x+(*iter).speed.x<0||((*iter).pos.x+(*iter).speed.x>SCREEN_WIDTH-30)){
          (*iter).speed.x=0;
     }
     (*iter).pos=(*iter).pos+(*iter).speed;
     if((*iter).pos.y>SCREEN_HEIGHT){
        Enemy.erase(iter);
     }
     else
        iter++;
}

int judge;
 for(std::vector<object>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
{
    if((*iter).period%30==0){
            judge=rand()%3;
            switch(judge){
             case 0:
                (*iter).speed.x=-5;break;
             case 1:
                (*iter).speed.x=5;break;
             case 2:
                (*iter).speed.x=0;
            }
    }
}



}


void draw()
{
	//drawBackground();
	drawPlayer();
	drawBullet();
	drawEnemy();
	//drawForeground();
	drawHint();
	drawStatus();//显示用户生命值，其他状态的函数
}

void deal()
{
    Player.period++;//用period表示对象存活的时间


    for(std::vector<object>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
	{
         (*iter).period++;
         std::cout<<(*iter).period<<" ";
	}
    std::cout<<std::endl;
    bool shoot=false;
	bool move = false;
	object tmp;//用来给新生成的节点结构体赋初值

	if(keyboard['k']&&Player.period%5==1)
    {
        shoot=true;
    }
	if( keyboard[KEY_UP]	|| keyboard['w'] )
	{
		Player.speed=Player.speed+ PointD(0,-1)*speedPlayer;
		move = true;
	}
	if( keyboard[KEY_DOWN]	|| keyboard['s'] )
	{
		Player.speed=Player.speed+ PointD(0,+1)*speedPlayer;
		move = true;
	}
	if( keyboard[KEY_LEFT]	|| keyboard['a'] )
	{
		Player.speed=Player.speed+PointD(-1,0)*speedPlayer;
		move = true;
	}

	if( keyboard[KEY_RIGHT] || keyboard['d'] )
	{
		Player.speed=Player.speed+PointD(+1,0)*speedPlayer;
		move = true;
	}
	double len = Player.speed.length();
	if( len > speedPlayer )
	{
		Player.speed = Player.speed/len*speedPlayer;
	}



    moveEnemy();

//以下用于更新玩家的子弹
for(std::vector<object>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();)
{
        (*iter).pos=(*iter).pos+(*iter).speed;
        for(std::vector<object>::iterator iter2=Enemy.begin();iter2!=Enemy.end();++iter2)
        {
            if(collide((*iter),(*iter2)))
            {
                //如果碰撞，则将子弹去除，待加碰撞效果
                (*iter).life-=powerofCollide;
                (*iter2).life-=powerofPlayer;
                //std::cout<<"Shoot!"<<std::endl;
            }
        }
        //判断每颗玩家的子弹否与某个敌人相碰撞
         //判断子弹是否出界
         if(-(*iter).pos.y>SCREEN_WIDTH||(*iter).life<0){
                    PlayerBullet.erase(iter);
                }
                else{
                    iter++;
                }
}


for(std::vector<object>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();)
	{
           (*iter).pos=(*iter).pos-(*iter).speed;
            if(collide((*iter),Player))
            {
                //如果碰撞，则将子弹去除，待加碰撞效果
                (*iter).life-=powerofCollide;
                 Player.life-=powerofEnemy;
                //std::cout<<"You are Shot!"<<std::endl;
            }

            if((*iter).pos.y>480||(*iter).life<0){
                    EnemyBullet.erase(iter);
                    //std::cout<<"YEnemybullet out!"<<std::endl;
                }
                else{
                    iter++;
                }
}
	//除去被击毁的敌机，判断敌机是否与玩家碰撞
    for(std::vector<object>::iterator iter=Enemy.begin();iter!=Enemy.end();)
    {
        if(collide((*iter),Player))
            {
                //如果碰撞，则将子弹去除，待加碰撞效果
                (*iter).life-=powerofCollide;
                 Player.life-=powerofCollide;
                //std::cout<<"You are Shot!"<<std::endl;
            }

        if((*iter).life<0){
            Enemy.erase(iter);
        }
        else{
             iter++;
        }
    }

    //如果按了射击键，则生成一个新的子弹
	if(shoot){
       tmp=object(Player.pos.x-25,Player.pos.y-50,0,1,0,0,velocityBullet);
       PlayerBullet.push_back(tmp);
	}

	//敌机发射子弹
    for(std::vector<object>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
	{
       if((*iter).period%81==1){
               tmp=object((*iter).pos.x,(*iter).pos.y,0,1,0,0,velocityEnemyBullet);
               EnemyBullet.push_back(tmp);
       }
	}

    //判断玩家是否触游戏边界
    if(Player.pos.x+ Player.speed.x<0||Player.pos.y+Player.speed.y<0||Player.pos.y+Player.speed.y>SCREEN_HEIGHT||
       Player.pos.x+ Player.speed.x>SCREEN_WIDTH){
        Player.speed=Player.speed*0;
    }
	else{
    Player.pos= Player.pos+  Player.speed;
	}
    //玩家移动
	if(!move)
	{
		Player.speed=Player.speed* 0.8;
		if( Player.speed.length() < 0.1 )
			Player.speed= PointD();
	}
	//Enemy[0] = PointD(mouseX,mouseY);
}

int work( bool &quit )
{
	deal();
	draw();
	if( keyboard[KEY_ESC])
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



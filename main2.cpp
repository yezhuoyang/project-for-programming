#include "SDL2_header.h"
#include <cstdio>
#include <map>
#include<ctime>
#include <string>
const std::string Game::TitleName = "Not A Simple Game Demo";
const int Game::SCREEN_WIDTH	= 640;//��ȣ�x����
const int Game::SCREEN_HEIGHT	= 480;//����,y����
using namespace Game;
std::map<int, bool> keyboard;
object Player;
PointD velocityPlayer;
std::vector <object> Enemy={};
std::vector <object> PlayerBullet={};
std::vector <object> EnemyBullet={};//���ڴ�ŵ����ӵ�


double velocityBullet;
double velocityEnemyBullet;
double speedPlayer;
double radiusPlayer=50,radiusEnemy=50;//��ҷɻ���л��İ뾶
double lifeofPlayer;//��ҵ�����ֵ
double lifeofEnemy;//��ҵ�����ֵ
double powerofPlayer;//����ӵ��Ĺ�����
double powerofEnemy;//�����ӵ��Ĺ�����
double powerofCollide;//��������������ײ���˺�ֵ


Image *imagePlayer, *imageBullet, *imageEnemy, *images[100],*imageEnemyBullet;

void loadPictures()
{
	imagePlayer = loadImage( "player.png" );
	imageBullet = loadImage( "bullet.png" );
	imageEnemyBullet=loadImage("bullet.png");//�����ӵ�ͼ��
	imageEnemy=loadImage( "enemy.png" );
}

void initialize()
{
    lifeofPlayer=10;//��ҳ�ʼ����ֵ
	Player = object( SCREEN_WIDTH/2, SCREEN_HEIGHT/2,radiusPlayer,lifeofPlayer,0,0,0);
	//Enemy[0] = Player
	FPS_DISPLAY = true;;
	velocityBullet=-5;
	velocityEnemyBullet=-10;
	lifeofEnemy=100;//���˳�ʼ����ֵ
    Player.life=lifeofPlayer;
	powerofPlayer=50;//����ӵ��Ĺ�����
	powerofCollide=100;
	powerofEnemy=1;//����ӵ��Ĺ�����
	speedPlayer=10;
	canvasColor = {0, 0, 0, 255};
	loadPictures();
    //�������������
    srand((unsigned)time(NULL));
}

void drawPlayer()
{
	int w,h;
	getImageSize( imagePlayer, w, h );
	setImageAlpha( imagePlayer, 150);
	drawImage( imagePlayer, Player.pos.x-w/2, Player.pos.y-h/2, 0.5, 0.5 );
}

//�ж��Ƿ�����ײ

bool collide(object A,object B){
    return (B.pos-A.pos).length()<=(A.radius+B.radius);
}

//���������������ڻ���Ϸ��ʼ�����
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
	Rect rect = {400, 0, 85, 800};//��һ����ɫ,��߽磬�ϱ߽磬�ұ߽� ���±߽�
	setPenColor((Color){0, 255, 0, 200});
	drawRect( rect, true );
}

//���º������ڵ�����ʾ�ַ�
void drawHint()
{
	Image *text = textToImage( "< This is not a simple game demo. >");
	int w,h;
	getImageSize( text, w, h );
	drawImage( text, SCREEN_WIDTH-h/2-w/2, SCREEN_HEIGHT/2-h/2, 1, 1, 90+180 );
	cleanup(text);
}

//��ʾ��ҵ�ǰ״̬

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
//�˺����������ɵ���


void  newEnemy(){
    if(Enemy.size()<4){
        object tmp((rand()%640),10,lifeofEnemy,radiusEnemy,0,0,1);
        Enemy.push_back(tmp);
    }
}


//�ٶȸ���,����ƶ�
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
	drawStatus();//��ʾ�û�����ֵ������״̬�ĺ���
}

void deal()
{
    Player.period++;//��period��ʾ�������ʱ��


    for(std::vector<object>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
	{
         (*iter).period++;
         std::cout<<(*iter).period<<" ";
	}
    std::cout<<std::endl;
    bool shoot=false;
	bool move = false;
	object tmp;//�����������ɵĽڵ�ṹ�帳��ֵ

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

//�������ڸ�����ҵ��ӵ�
for(std::vector<object>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();)
{
        (*iter).pos=(*iter).pos+(*iter).speed;
        for(std::vector<object>::iterator iter2=Enemy.begin();iter2!=Enemy.end();++iter2)
        {
            if(collide((*iter),(*iter2)))
            {
                //�����ײ�����ӵ�ȥ����������ײЧ��
                (*iter).life-=powerofCollide;
                (*iter2).life-=powerofPlayer;
                //std::cout<<"Shoot!"<<std::endl;
            }
        }
        //�ж�ÿ����ҵ��ӵ�����ĳ����������ײ
         //�ж��ӵ��Ƿ����
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
                //�����ײ�����ӵ�ȥ����������ײЧ��
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
	//��ȥ�����ٵĵл����жϵл��Ƿ��������ײ
    for(std::vector<object>::iterator iter=Enemy.begin();iter!=Enemy.end();)
    {
        if(collide((*iter),Player))
            {
                //�����ײ�����ӵ�ȥ����������ײЧ��
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

    //��������������������һ���µ��ӵ�
	if(shoot){
       tmp=object(Player.pos.x-25,Player.pos.y-50,0,1,0,0,velocityBullet);
       PlayerBullet.push_back(tmp);
	}

	//�л������ӵ�
    for(std::vector<object>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
	{
       if((*iter).period%81==1){
               tmp=object((*iter).pos.x,(*iter).pos.y,0,1,0,0,velocityEnemyBullet);
               EnemyBullet.push_back(tmp);
       }
	}

    //�ж�����Ƿ���Ϸ�߽�
    if(Player.pos.x+ Player.speed.x<0||Player.pos.y+Player.speed.y<0||Player.pos.y+Player.speed.y>SCREEN_HEIGHT||
       Player.pos.x+ Player.speed.x>SCREEN_WIDTH){
        Player.speed=Player.speed*0;
    }
	else{
    Player.pos= Player.pos+  Player.speed;
	}
    //����ƶ�
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



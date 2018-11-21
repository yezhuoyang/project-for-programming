#include "SDL2_header.h"
#include <cstdio>
#include <map>
#include<ctime>
#include <string>
const std::string Game::TitleName = "Not A Simple Game Demo";
const int Game::SCREEN_WIDTH	= 1000;//��ȣ�x����
const int Game::SCREEN_HEIGHT	= 700;//����,y����
using namespace Game;
std::map<int, bool> keyboard;
PointD velocityPlayer;
double velocityBullet;
double velocityEnemyBullet;
double speedPlayer;
double radiusPlayer=50,radiusEnemy=50;//��ҷɻ���л��İ뾶
double lifeofPlayer;//��ҵ�����ֵ
double lifeofEnemy;//��ҵ�����ֵ
double powerofPlayer;//����ӵ��Ĺ�����
double powerofEnemy;//�����ӵ��Ĺ�����
double powerofCollide;//��������������ײ���˺�ֵ
int SCORE;


Image *imageplane[10],*imagebullet[10],*imageflower[10];
plane Player;
std::vector <plane> Enemy={};
std::vector <bullet> PlayerBullet={};
std::vector <bullet> EnemyBullet={};//���ڴ�ŵ����ӵ�
std::vector <flower> flowerlist={};//ʵ����ײ�Ļ�Ч��




void loadPictures()
{
    imageplane[0]=loadImage( "player.png" );
    imagebullet[0]=loadImage( "bullet.png" );
    imageflower[0]=loadImage("red_strip24.png");

 /*
	imagePlayer = loadImage( "player.png" );
	imageBullet = loadImage( "bullet.png" );
	imageEnemyBullet=loadImage("bullet.png");//�����ӵ�ͼ��
	imageEnemy=loadImage( "player.png" );
	imagedie=loadImage( "player_u.png" );
	//imageEnemy=loadImage( "enemy.png" );
*/
}


void plotbullet(const bullet &A, int flip){
     switch(flip){
          case 0:
               drawImage(imagebullet[A.type],A.pos.x, A.pos.y,1,1,A.speed.x*4,NULL,FLIP_NONE);
          break;
          break;
          case 1:
                 drawImage(imagebullet[A.type],A.pos.x, A.pos.y,1,1,-A.speed.x*4,NULL,FLIP_VERTICAL);
     }
}


void plotplane(const plane &A, int flip){
     switch(flip){
          case 0:
          drawImage(imageplane[A.type],A.pos.x, A.pos.y,1,1,A.speed.x*4,NULL,FLIP_NONE);
          break;
          break;
          case 1:
                 drawImage(imageplane[A.type],A.pos.x, A.pos.y,1,1,-A.speed.x*4,NULL,FLIP_VERTICAL);
     }
}


void initialize()
{
    lifeofPlayer=10;//��ҳ�ʼ����ֵ
	Player.pos.x=SCREEN_WIDTH/2;Player.pos.y=SCREEN_HEIGHT/2;Player.radius=radiusPlayer;Player.life=lifeofPlayer;
	Player.type=0;Player.bulletnum=3;
	FPS_DISPLAY = true;
	velocityBullet=-5;
	velocityEnemyBullet=-10;
	lifeofEnemy=100;//���˳�ʼ����ֵ
    Player.life=lifeofPlayer*10000;
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
   plotplane(Player,0);
}


void Enemyshoot(){
    for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
	{
       if((*iter).period%81==1){
            int k=-((*iter).bulletnum-1)/2;
            for(int i=0;i<(*iter).bulletnum;++i){
               bullet tmp(0,(*iter).pos.x,(*iter).pos.y,0,1,0,3*k,velocityEnemyBullet);
               EnemyBullet.push_back(tmp);
               k++;
            }
       }
	}
}



void Playershoot(){
            int k=-(Player.bulletnum-1)/2;
            for(int i=0;i<Player.bulletnum;++i){
               bullet tmp(0,Player.pos.x-25,Player.pos.y-50,0,1,0,3*k,velocityBullet);
               PlayerBullet.push_back(tmp);
               k++;
            }
}


//�ж��Ƿ�����ײ
template <typename T1,typename T2>
bool collide(T1 A,T2 B){
    return (A.pos-B.pos).length()<=(A.radius+B.radius);
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
        char info[20];
        extern plane Player;
        sprintf(info, "LIFE: %2d", (int)(Player.life));
        drawText( info, 200,430);
}

void drawBullet()
{
    for(std::vector<bullet>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();++iter)
	  plotbullet((*iter),0);
   for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();++iter)
	  plotbullet((*iter),1);
 /*
   for(std::vector<bullet>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();++iter)
	  drawImage(imageBullet, (*iter).pos.x, (*iter).pos.y, 0.5, 0.5 );
   for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();++iter)
	  drawImage(imageEnemyBullet, (*iter).pos.x, (*iter).pos.y, 0.5, 0.5 );
*/
}

int lastAnime = 0;
void drawEnemy()
{
   int w=10,h=10;
   for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter){
            plotplane(*iter,1);
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



void drawflower(){
    for(std::vector<flower>::iterator iter=flowerlist.begin();iter!=flowerlist.end();++iter){
            int w,h;
            getImageSize(imageflower[(*iter).type], w, h );
            Rect clip = {w/24*(*iter).life,0, w/24,h};
            drawImage(imageflower[(*iter).type],(*iter).pos.x, (*iter).pos.y, 1, 1,0, NULL, FLIP_NONE, &clip );
            (*iter).life++;
    }
}


//�˺����������ɵ���
void  newEnemy(){
    if(Enemy.size()<4){
        plane tmp(0,(rand()%640),10,lifeofEnemy,radiusEnemy,0,0,1);
        Enemy.push_back(tmp);
    }
}

//�ٶȸ���,����ƶ�
void moveEnemy(){
for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();)
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
 for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
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

	drawBullet();
	drawEnemy();
	drawPlayer();
	drawflower();
	//drawForeground();
	drawHint();
	drawStatus();//��ʾ�û�����ֵ������״̬�ĺ���
}


void deal()
{
    Player.period++;//��period��ʾ�������ʱ��
    std::cout<<(Player).pos.x<<" "<<(Player).pos.y<<std::endl;
    for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
	{
         (*iter).period++;
	}
    bool shoot=false;
	bool move = false;
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
//�������ڸ�����ҵ��ӵ�
for(std::vector<bullet>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();)
{
        (*iter).pos=(*iter).pos+(*iter).speed;
        for(std::vector<plane>::iterator iter2=Enemy.begin();iter2!=Enemy.end();++iter2)
        {
            if(collide((*iter),(*iter2)))
            {
                //�����ײ�����ӵ�ȥ����������ײЧ��
                (*iter).life-=powerofCollide;
                (*iter2).life-=powerofPlayer;
                (*iter2).speed.y-=0.5;
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

for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();)
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
                    flower tmp(0,(*iter).pos.x,(*iter).pos.y,23,1);
                    flowerlist.push_back(tmp);
                    EnemyBullet.erase(iter);
                    //std::cout<<"YEnemybullet out!"<<std::endl;
                }
                else{
                    iter++;
                }
}
	//��ȥ�����ٵĵл����жϵл��Ƿ��������ײ
for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();)
    {
        if(collide((*iter),Player))
            {
                //�����ײ�����ӵ�ȥ����������ײЧ��
                (*iter).life-=powerofCollide;
                 Player.life-=powerofCollide;
                //std::cout<<"You are Shot!"<<std::endl;
            }

        if((*iter).life<0){
            (*iter).type=3;
            ++iter;
            //Enemy.erase(iter);
            ++SCORE;
        }
        else{
             iter++;
        }
    }
  std::cout<<flowerlist.size()<<std::endl;
  for(std::vector<flower>::iterator iter=flowerlist.begin();iter!=flowerlist.end();)
    {
         if((*iter).life>=(*iter).maxtime){
              flowerlist.erase(iter);
         }
         else{
            ++iter;
         }
    }
    //��������������������һ���µ��ӵ�
	if(shoot){
         	Playershoot();
	         }
	moveEnemy();
	//�л������ӵ�
    Enemyshoot();


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
    for(int i=0;i<10;++i){
        cleanup( imageplane[i]);
        cleanup( imagebullet[i]);
        cleanup( imageflower[i]);
    }
}

void showresult()
{
     SDL_RenderClear(renderer);
     char info[20];
     sprintf(info, "YOUR SCORE: %2d", (int)(SCORE));
     drawText(info,200,430);
}





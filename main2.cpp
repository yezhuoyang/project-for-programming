#include "SDL2_header.h"
#include <cstdio>
#include <map>
#include<ctime>

const std::string Game::TitleName = "Not A Simple Game Demo";
const int Game::SCREEN_WIDTH	= 640;//��ȣ�x����
const int Game::SCREEN_HEIGHT	= 480;//����,y����
using namespace Game;
std::map<int, bool> keyboard;
PointD posPlayer, velocityPlayer;
std::vector <PointD> posEnemy={};
std::vector <PointD> posBullet={};
std::vector <PointD> EnemyBullet={};//���ڴ�ŵ����ӵ�


PointD velocityBullet;
PointD velocityEnemyBullet;

double speedPlayer;
double radiusPlayer=5,radiusEnemy=50;//��ҷɻ���л��İ뾶
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
	imageEnemy=loadImage( "player.png" );
}

void initialize()
{
	FPS_DISPLAY = true;
	posPlayer = PointD( SCREEN_WIDTH/2, SCREEN_HEIGHT/2 );
	//posEnemy[0] = posPlayer;
	velocityBullet= PointD(0,-5);
	lifeofEnemy=10;//���˳�ʼ����ֵ
	lifeofPlayer=10;//��ҳ�ʼ����ֵ
	powerofPlayer=10;//����ӵ��Ĺ�����
	powerofCollide=100;
	powerofEnemy=1;//����ӵ��Ĺ�����
	speedPlayer=5;
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
	drawImage( imagePlayer, posPlayer.x-w/2, posPlayer.y-h/2, 0.5, 0.5 );
}

//�ж��Ƿ�����ײ

bool collide(PointD A,double RA,PointD B,double RB){
    return (B-A).length()<=(RA+RB);
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

   int w=10,h=10;
   for(std::vector<PointD>::iterator iter=posEnemy.begin();iter!=posEnemy.end();++iter){
        /*
          getImageSize( imageEnemy, w, h );
	      lastAnime = (lastAnime+15)%(4*60);
          Rect clip = {w/16*(lastAnime/60), 0, w/16,h/16};
	      setImageAlpha( imageEnemy, 255 );
        */
          //Rect clip = {w/16*(lastAnime/60), 0, w/16,h/16};
	      drawImage(imageEnemy, (*iter).x,(*iter).y, 0.5,0.5);
   }

/*
	int w,h;
	getImageSize( imageEnemy, w, h );
	lastAnime = (lastAnime+15)%(4*60);
	Rect clip = {w/16*(lastAnime/60), 0, w/16,h/16};
	setImageAlpha( imageEnemy, 255 );
	drawImage( imageEnemy, posEnemy[0].x, posEnemy[0].y, 2, 2, 0, NULL, FLIP_NONE, &clip );
*/
}

//�˺����������ɵ���
void  newEnemy(){
    std::cout<<"create a new enemy"<<std::endl;
    PointD newposition;
    newposition.x=(rand()%640);
    newposition.y=10;
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
	PointD tmp;
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
        for(std::vector<PointD>::iterator iter2=posEnemy.begin();iter2!=posEnemy.end();++iter2)
        {
            if(collide(*iter,0,*iter2,radiusEnemy))
            {
                //�����ײ�����ӵ�ȥ����������ײЧ��
                (*iter).life-=powerofCollide;
                (*iter2).life-=powerofPlayer;
                std::cout<<"Shoot!"<<std::endl;
            }
        }
        //�ж�ÿ����ҵ��ӵ�����ĳ����������ײ
	}


for(std::vector<PointD>::iterator iter=posBullet.begin();iter!=posBullet.end();)
	{
	        //���������Ļ�������ӵ�������С��0���Ͱ��ӵ����ڴ��ͷŵ�
                if(-(*iter).y>SCREEN_WIDTH||(*iter).life<0){
                    std::cout<<"Bullet out"<<std::endl;
                    posBullet.erase(iter);
                }
                else{
                    iter++;
                }
	}
	//��ȥ�����ٵĵػ�
    for(std::vector<PointD>::iterator iter=posEnemy.begin();iter!=posEnemy.end();)
    {
        if((*iter).life<0){
            std::cout<<"Enemy out"<<std::endl;
            posEnemy.erase(iter);
        }
        else{
             iter++;
        }
    }

    //��������������������һ���µ��ӵ�
	if(shoot){
       tmp=posPlayer;
       tmp.life=1;
       posBullet.push_back(tmp);
	}

    //�ж�����Ƿ���Ϸ�߽�
    if(posPlayer.x+ velocityPlayer.x<0||posPlayer.y+velocityPlayer.y<0||posPlayer.y+velocityPlayer.y>SCREEN_HEIGHT||
       posPlayer.x+ velocityPlayer.x>SCREEN_WIDTH){
        velocityPlayer=velocityPlayer*0;
    }
	else{
    posPlayer = posPlayer + velocityPlayer;
	}


    //����ƶ�
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



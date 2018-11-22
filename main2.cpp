#include "SDL2_header.h"
#include <SDL2/SDL_mixer.h>
#include <cstdio>
#include <map>
#include<ctime>
#include <string>
const std::string Game::TitleName = "Not A Simple Game Demo";
const int Game::SCREEN_WIDTH	= 600;//��ȣ�x����
const int Game::SCREEN_HEIGHT	= 700;//����,y����
using namespace Game;
std::map<int, bool> keyboard;
PointD velocityPlayer;
double velocityBullet;
double velocityEnemyBullet;
double speedPlayer;
double radiusPlayer=30,radiusEnemy=30;//��ҷɻ���л��İ뾶
double lifeofPlayer;//��ҵ�����ֵ
double lifeofEnemy;//��ҵ�����ֵ
int powerofPlayer;//����ӵ��Ĺ�����
int  powerofEnemy;//�����ӵ��Ĺ�����
int  powerofcollide;//��������������ײ���˺�ֵ
int SCORE;
Image *imageplane[10],*imagebullet[10],*imageflower[10],*imageobject,*imagemouse;

//extern plane Player;
std::vector <plane> Enemy={};
std::vector <bullet> PlayerBullet={};
std::vector <bullet> EnemyBullet={};//���ڴ�ŵ����ӵ�
std::vector <flower> flowerlist={};//ʵ����ײ�Ļ�Ч��
std::vector <object> objectlist={};

//�������
plane mouse(0,50,50,50,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,1000,100,0,0,0,3);
int lastAnime=0;
int choice()
{
  bool start=false;
  Rect rect = {100, 300,400, 100};//��һ����ɫ,��߽磬�ϱ߽磬�ұ߽� ���±߽�
  if(100<mouse.pos.x&&mouse.pos.x<500&&300<mouse.pos.y&&mouse.pos.y<400){
    	setPenColor((Color){50, 100, 0,250});
    	start=true;
  }
  else{
        setPenColor((Color){50, 100, 250,250});
  }
	drawRect( rect, true );
    Image *text = textToImage( "START GAME");
	int w,h;
	getImageSize( text, w, h );
	drawImage( text,100,300, 1, 1,0);
	cleanup(text);
    mouse.pos=PointD(mouseX, mouseY);
	getImageSize(imagemouse, w, h );
	lastAnime = (lastAnime+15)%(4*60);
	Rect clip = {w/16*(lastAnime/60), 0, w/16,h/16};
	setImageAlpha( imagemouse, 255 );
	drawImage( imagemouse, mouse.pos.x, mouse.pos.y, 2, 2, 0, NULL, FLIP_NONE, &clip );
    if(mousePressed&&start){
        return 1;
    }
	return 0;
}

int Sound_Init()
{
        const int    TMP_FREQ = MIX_DEFAULT_FREQUENCY;
        const Uint16 TMP_FORMAT = MIX_DEFAULT_FORMAT;
        const int    TMP_CHAN = 2;
        const int    TMP_CHUNK_SIZE = 512;
        return Mix_OpenAudio(TMP_FREQ,TMP_FORMAT,TMP_CHAN,TMP_CHUNK_SIZE);
}

int Sound_Play(const char *file_name)
{
        Mix_Music *mix_music;
        if((mix_music = Mix_LoadMUS(file_name)) == NULL)
        {
                printf("call Mix_LoadMUS failed:%s/n",Mix_GetError());
                return -1;
        }
        if(Mix_PlayMusic(mix_music,-1) == -1)
        {
                printf("call Mix_PlayMusic failed/n");
                return -1;
        }
        printf("after call Mix_PlayMusic/n");
        return 0;
}


void loadPictures()
{
    imageplane[0]=loadImage( "player.png" );
    imagebullet[0]=loadImage( "bullet.png" );
    imageflower[1]=loadImage("red_strip24.png");
    imageflower[0]=loadImage("poison_strip24.png");
    imageobject=loadImage("etama.png");
    imagemouse=loadImage("mouse.png");
    //shootmusic= Mix_LoadMUS( "shoot.mp3" );
}

void plotbullet(const bullet &A, int flip){
      int w,h;
     getImageSize(imagebullet[A.type], w, h );
     switch(flip){
          case 0:
               drawImage(imagebullet[A.type],A.pos.x, A.pos.y,A.width*1.0/w,A.height*1.0/h,A.speed.x*4,NULL,FLIP_NONE);
          break;
          case 1:
               drawImage(imagebullet[A.type],A.pos.x, A.pos.y,A.width*1.0/w,A.height*1.0/h,-A.speed.x*4,NULL,FLIP_VERTICAL);
     }
}

void plotplane(const plane &A, int flip){
     int w,h;
     getImageSize(imageplane[A.type], w, h );
     switch(flip){
          case 0:
          //drawImage(imagebullet[0],A.pos.x,A.pos.y);
          drawImage(imageplane[A.type],A.pos.x, A.pos.y,A.width*1.0/w,A.height*1.0/h,A.speed.x*4,NULL,FLIP_NONE);
          //drawImage(imagebullet[0],A.pos.x+w/2,A.pos.y+h/2);
          break;
          case 1:
          drawImage(imageplane[A.type],A.pos.x, A.pos.y,A.width*1.0/w,A.height*1.0/h,-A.speed.x*4,NULL,FLIP_VERTICAL);
     }
}

playerplane Player(0,50,50,50,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,10000,500,0,0,0,7);


void initialize()
{
 /*
    lifeofPlayer=10;//��ҳ�ʼ����ֵ
	Player.pos.x=SCREEN_WIDTH/2;Player.pos.y=SCREEN_HEIGHT/2;Player.radius=radiusPlayer;Player.life=lifeofPlayer;
	Player.type=0;Player.bulletnum=3;
    Player.life=lifeofPlayer*10000;
	powerofPlayer=50;//����ӵ��Ĺ�����
	Player.powerofcollide=powerofcollide;
	speedPlayer=10;
*/
    Sound_Init();
    Sound_Play("shoot.wav");
    speedPlayer=10;
	FPS_DISPLAY = true;
	velocityBullet=-5;
	velocityEnemyBullet=-10;
	lifeofEnemy=100;//���˳�ʼ����ֵ
    powerofEnemy=1;//����ӵ��Ĺ�����
    powerofcollide=100;
	canvasColor = {0, 0, 0, 255};
	loadPictures();
    //�������������
    srand((unsigned)time(NULL));
}

void draw_Player()
{
    int w,h;
    getImageSize(imageplane[Player.type], w, h );
    if(Player.shotperiod){
        --Player.shotperiod;
        Player.pos.x+=(rand()%5-2);
        Player.pos.y+=(rand()%5-2);
    }
    drawImage(imageplane[Player.type],Player.pos.x,Player.pos.y,Player.width*1.0/w,Player.height*1.0/h,Player.speed.x*4,NULL,FLIP_NONE);
    if(Player.properiod==0){
    getImageSize(imagemouse,w,h);
    //--Player.properiod;
    Rect clip={0,h/4, w/8,h/8};
    drawImage(imagemouse,Player.pos.x-110,Player.pos.y-100, (Player.width)*1.0/12, (Player.height)*1.0/12,0, NULL, FLIP_NONE, &clip );
    }
}

void Enemy_shoot(){
    for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
	{
       if((*iter).period%81==1){
            int k=-((*iter).bulletnum-1)/2;
            for(int i=0;i<(*iter).bulletnum;++i){
               bullet tmp(0,20,20,powerofEnemy,(*iter).pos.x,(*iter).pos.y,0,1,3*k,velocityEnemyBullet);
               EnemyBullet.push_back(tmp);
               k++;
            }
       }
	}
}

void Player_shoot(){
            int k=-(Player.bulletnum-1)/2;
            for(int i=0;i<Player.bulletnum;++i){
               bullet tmp(0,20,20,powerofPlayer,Player.pos.x-25,Player.pos.y-50,0,1,3*k,velocityBullet*4);
               PlayerBullet.push_back(tmp);
               k++;
            }
}


//�ж��Ƿ�����ײ
template <typename T1,typename T2>
void collide(T1 &A,T2 &B){
    if((A.pos-B.pos).length()<=(A.radius+B.radius))
    {
        A.life=A.life-B.powerofcollide;
        B.life=B.life-A.powerofcollide;
        A.shotperiod=5;
        B.shotperiod=5;
        //std::cout<<A.life<<"  "<<B.life<<std::endl;
    }
}

//���������������ڻ���Ϸ��ʼ�����
void draw_Background()
{
	Rect rect = {70, 50, 80, 90};
	setPenColor((Color){255, 255, 0, 255});
	//	Pay attention: (Color){255,255,0} means (Color){255,255,0,0}
	//	and means you will draw nothing
	drawRect( rect, true );
}

void draw_Foreground()
{
	Rect rect = {400, 0, 85, 800};//��һ����ɫ,��߽磬�ϱ߽磬�ұ߽� ���±߽�
	setPenColor((Color){0, 255, 0, 200});
	drawRect( rect, true );
}

//���º������ڵ�����ʾ�ַ�
void draw_Hint()
{
	Image *text = textToImage( "< This is not a simple game demo. >");
	int w,h;
	getImageSize( text, w, h );
	drawImage( text, SCREEN_WIDTH-h/2-w/2, SCREEN_HEIGHT/2-h/2, 1, 1, 90+180 );
	cleanup(text);
}

//��ʾ��ҵ�ǰ״̬
void draw_Status(){
        char info[20];
        sprintf(info, "LIFE: %2d", (int)(Player.life));
        drawText( info, 200,430);
}

void draw_Bullet()
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


void draw_Enemy()
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

void draw_flower(){
    for(std::vector<flower>::iterator iter=flowerlist.begin();iter!=flowerlist.end();++iter){
            int w,h;
            getImageSize(imageflower[(*iter).type], w, h );
            Rect clip = {w/24*(*iter).life,0, w/24,h};
            drawImage(imageflower[(*iter).type],(*iter).pos.x, (*iter).pos.y, ((*iter).width)*1.0/w*24, ((*iter).height)*1.0/h,0, NULL, FLIP_NONE, &clip );
            (*iter).life++;
    }
}

void draw_object(){
    for(std::vector<object>::iterator iter=objectlist.begin();iter!=objectlist.end();++iter){
            int w,h;
            getImageSize(imageobject,w,h);
            Rect clip = {0,0,w/9,h/5};
            drawImage(imageobject,(*iter).pos.x, (*iter).pos.y, ((*iter).width)*1.0/w*9, ((*iter).height)*1.0/h*5,0, NULL, FLIP_NONE, &clip );
            (*iter).life++;
    }
}



//�˺����������ɵ���
void  newEnemy(){
    if(Enemy.size()<4){
        plane tmp(0,50,50,powerofcollide,(rand()%640),10,lifeofEnemy,0,0,1,0,5);
        Enemy.push_back(tmp);
    }
}

//�ٶȸ���,����ƶ�
void Enemy_move(){
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
//�л�����ı��ٶ�
int judge;
for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
{
    //ʵ��ײ��ʱ�Ķ���Ч��
    if((*iter).shotperiod){
        --(*iter).shotperiod;
        (*iter).pos.x+=(rand()%5-2);
        (*iter).pos.y+=(rand()%5-2);
    }
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
	draw_Bullet();
	draw_Enemy();
	draw_Player();
	draw_flower();
	draw_object();
	//drawForeground();
	draw_Hint();
	draw_Status();//��ʾ�û�����ֵ������״̬�ĺ���
}

//�ж��Ƿ����
template <typename T>
bool check(const T &A){
  return (A.pos.x>0&&A.pos.x<SCREEN_WIDTH&&A.pos.y>0&&A.pos.y<SCREEN_HEIGHT);
}

void collide_all(){
    if(Player.properiod==0){
       for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
        {
            if( ((*iter).pos-Player.pos).length()<=100){
                (*iter).life=-1;
            }
        }
       for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();)
        {
            if( ((*iter).pos-Player.pos).length()<=100){
                PlayerBullet.push_back(*iter);
                EnemyBullet.erase(iter);
                continue;
            }
            ++iter;
        }
    }
     else
    { for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
        {
               collide(Player,*iter);
        }
       for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();++iter)
        {
               collide(*iter,Player);
        }
     }
    for(std::vector<bullet>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();++iter)
	{
        for(std::vector<plane>::iterator iter2=Enemy.begin();iter2!=Enemy.end();++iter2)
           collide(*iter2,*iter);
	}

}

void create_flower(){
   for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
	{
         if((*iter).life<0){
                    flower tmp(1,30,30,(*iter).pos.x,(*iter).pos.y,23);
                    flowerlist.push_back(tmp);
                 if(rand()%3==1){
                     object tmp(0,30,30,(*iter).pos.x,(*iter).pos.y,100);
                     objectlist.push_back(tmp);
                 }
           }
	}

   for(std::vector<bullet>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();++iter)
	{
         if((*iter).life<0){
                 flower tmp(0,30,30,(*iter).pos.x,(*iter).pos.y,23);
                 flowerlist.push_back(tmp);
           }
	}
   for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();++iter)
	{
         if((*iter).life<0){
                 flower tmp(0,80,80,(*iter).pos.x,(*iter).pos.y,23);
                 flowerlist.push_back(tmp);
           }
	}
}

void clear_all(){
  for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();)
	{
	       std::cout<<((*iter).life)<<"  "<<(((*iter).life)<0)<<std::endl  ;
           if(((*iter).life)<0){
                Enemy.erase(iter);
           }
           else{
            ++iter;
           }
	}
	for(std::vector<bullet>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();)
    {
	      if((*iter).life<0){
                PlayerBullet.erase(iter);
           }
           else{
            ++iter;
           }
    }
     for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();)
    {
	      if((*iter).life<0||!check(*iter)){
                EnemyBullet.erase(iter);
           }
           else{
            ++iter;
           }
    }
    for(std::vector<flower>::iterator iter=flowerlist.begin();iter!=flowerlist.end();)
    {
	      if((*iter).life>(*iter).maxtime){
                flowerlist.erase(iter);
           }
           else{
            ++iter;
           }
    }
    for(std::vector<object>::iterator iter=objectlist.begin();iter!=objectlist.end();)
    {
	      if((*iter).life>(*iter).maxtime){
                objectlist.erase(iter);
           }
           else{
                ++iter;
           }
    }

}



void bullet_move(){
   for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();++iter)
    {
             (*iter).pos=(*iter).pos-(*iter).speed;
    }

    for(std::vector<bullet>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();++iter)
    {
             (*iter).pos=(*iter).pos+(*iter).speed;
    }
}


void deal()
{
    Player.period++;//��period��ʾ�������ʱ��
    //std::cout<<(Player).pos.x<<" "<<(Player).pos.y<<std::endl;
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
    //��������������������һ���µ��ӵ�
	if(shoot){
         	Player_shoot();
	         }
	Enemy_move();
    bullet_move();
	//�л������ӵ�
    Enemy_shoot();
    collide_all();
    create_flower();
    clear_all();

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





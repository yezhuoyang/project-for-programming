#include "SDL2_header.h"
#include <SDL2/SDL_mixer.h>
#include <cstdio>
#include <map>
#include<ctime>
#include <string>

const std::string Game::TitleName = "Not A Simple Game Demo";
const int Game::SCREEN_WIDTH	= 600;//宽度，x坐标
const int Game::SCREEN_HEIGHT	= 600;//长度,y坐标
using namespace Game;
std::map<int, bool> keyboard;
PointD velocityPlayer;
double velocityBullet;
double velocityEnemyBullet;
double speedPlayer;
double radiusPlayer=30,radiusEnemy=30;//玩家飞机与敌机的半径
double lifeofPlayer;//玩家的生命值
double lifeofEnemy;//玩家的生命值
int powerofPlayer;//玩家子弹的攻击力
int  powerofEnemy;//敌人子弹的攻击力
int  powerofcollide;//任意两个物体相撞的伤害值
int SCORE;
int sanperiod;//最大开启保护伞时间
int leiperiod;
int powerofthunder;
Image *imageplane[10],*imagebullet[10],*imageflower[10],*imageobject,*imagemouse,*thunder[3];
Mix_Chunk *effect[10];
//extern plane Player;
std::vector <plane> Enemy={};
std::vector <bullet> PlayerBullet={};
std::vector <bullet> EnemyBullet={};//用于存放敌人子弹
std::vector <flower> flowerlist={};//实现碰撞的火花效果
std::vector <flower> explodelist={};//实现爆炸效果
std::vector <object> objectlist={};

//生成鼠标
plane mouse(0,50,50,50,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,1000,100,0,0,0,3);
int lastAnime=0;

int Sound_Init()
{
        int    TMP_FREQ = MIX_DEFAULT_FREQUENCY*32;
        const Uint16 TMP_FORMAT = MIX_DEFAULT_FORMAT;
        const int    TMP_CHAN =1000;
        const int    TMP_CHUNK_SIZE = 512;
        return Mix_OpenAudio(TMP_FREQ,TMP_FORMAT,TMP_CHAN,TMP_CHUNK_SIZE);
}


void effect_Play(int num,int loop)
{
      Mix_PlayChannel(-1,effect[num],loop);
      std::cout<<Mix_GetError()<<std::endl;
}

int choice()
{
  bool start=false;
  Rect rect = {100, 300,400, 100};//画一块颜色,左边界，上边界，右边界 ，下边界
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
void load_all()
{
    imageplane[0]=loadImage( "player.png" );
    imageplane[1]=loadImage( "enemy1.png" );
    imageplane[2]=loadImage( "enemy2.png" );
    imagebullet[0]=loadImage( "bullet.png" );
    imagebullet[3]=loadImage( "fire.png" );
    imageflower[1]=loadImage("red_strip24.png");
    imageflower[0]=loadImage("poison_strip24.png");
    imageobject=loadImage("etama.png");
    imagemouse=loadImage("mouse.png");
    imageflower[2]=loadImage("explode.png");
    thunder[0]=loadImage("ele0.png");
    thunder[1]=loadImage("ele1.png");
    thunder[2]=loadImage("ele2.png");
    thunder[3]=loadImage("ele3.png");
    effect[0]=Mix_LoadWAV_RW(SDL_RWFromFile("res/music/shoot.ogg", "rb"),1 );
    effect[1]=Mix_LoadWAV_RW(SDL_RWFromFile("res/music/beshot.ogg", "rb"),1 );
    effect[2]=Mix_LoadWAV_RW(SDL_RWFromFile("res/music/shooten.ogg", "rb"),1 );
    effect[3]=Mix_LoadWAV_RW(SDL_RWFromFile("res/music/lei.ogg", "rb"),1 );
    std::cout<<Mix_GetError()<<std::endl;
    //shootmusic= Mix_LoadMUS( "shoot.mp3" );
}


bool lei;
playerplane Player(0,50,50,50,SCREEN_WIDTH/2,SCREEN_HEIGHT/2,10000,500,0,0,0,7);
void draw_thunder(){
    if(Player.thunder>0&&lei){
         int w,h;
         effect_Play(3,0);
         getImageSize(thunder[(Player.thunder/5)%4],w,h);
         drawImage(thunder[(Player.thunder/5)%4],Player.pos.x-340, Player.pos.y-280,1.2,1.2,60,NULL,FLIP_NONE);
         drawImage(thunder[(Player.thunder/5)%4],Player.pos.x-270, Player.pos.y-280,1.2,1.2,90,NULL,FLIP_NONE);
         drawImage(thunder[(Player.thunder/5)%4],Player.pos.x-200, Player.pos.y-280,1.2,1.2,120,NULL,FLIP_NONE);
         --Player.thunder;
    }
}


void plotbullet(bullet &A, int flip){
     int w,h;
      getImageSize(imagebullet[A.type], w, h );
     if(A.type==3){
            Rect clip={((A.period/16)%2)/2*w,((A.period/16)/4)%2*h,w/2,h/2};
            drawImage(imagebullet[A.type],A.pos.x, A.pos.y, (A.width)*1.0/w*2, (A.height)*1.0/h*2,0,NULL,FLIP_NONE,&clip);
            (A.period)=(A.period)+1;
            return;
     }
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



void initialize()
{
    Sound_Init();
  //music_Play("res/music/bac.mp3",-1);
    // start SDL with audio support
    if(SDL_Init(SDL_INIT_AUDIO)==-1) {
    printf("SDL_Init: %s\n", SDL_GetError());
    exit(1);
    }
// open 44.1KHz, signed 16bit, system byte order,
// stereo audio, using 1024 byte chunks
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
    printf("Mix_OpenAudio: %s\n", Mix_GetError());
    exit(2);
    }
    //Mix_AllocateChannels(2);
    //sound=Mix_LoadWAV("res/music/bac.wav");
    lifeofPlayer=10;//玩家初始生命值
	Player.pos.x=SCREEN_WIDTH/2;Player.pos.y=SCREEN_HEIGHT/2;Player.radius=radiusPlayer;Player.life=lifeofPlayer;
	Player.type=0;Player.bulletnum=3;
    Player.life=lifeofPlayer;
	powerofPlayer=50;//玩家子弹的攻击力
    powerofcollide=100;
	Player.powerofcollide=powerofcollide;
    speedPlayer=10;
    sanperiod=100;
    leiperiod=100;
    powerofthunder=1000;
	FPS_DISPLAY = true;
	velocityBullet=-5;
	velocityEnemyBullet=-10;
	lifeofEnemy=100;//敌人初始生命值
    powerofEnemy=1;//玩家子弹的攻击力

	canvasColor = {0, 0, 0, 255};
	load_all();
    //设置随机数种子
    srand((unsigned)time(NULL));
}
bool san;//判断是否开保护伞//是否发雷电
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

    if(Player.properiod>0&&san){
    getImageSize(imagemouse,w,h);
    //--Player.properiod;
    Rect clip={0,h/4, w/8,h/8};
    drawImage(imagemouse,Player.pos.x-110,Player.pos.y-100, (Player.width)*1.0/12, (Player.height)*1.0/12,0, NULL, FLIP_NONE, &clip );
    --Player.properiod;
    }

}

void Enemy_shoot(){
    for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
	{
       if((*iter).period%81==1){
            int k=-((*iter).bulletnum-1)/2;
            for(int i=0;i<(*iter).bulletnum;++i){
               bullet tmp(0,20,20,powerofEnemy,(*iter).pos.x,(*iter).pos.y,0,1,3*k,-velocityEnemyBullet);
               EnemyBullet.push_back(tmp);
               k++;
            }
       }
	}
}

void Player_shoot(){
            int k=-(Player.bulletnum-1)/2;
            for(int i=0;i<Player.bulletnum;++i){
               bullet tmp(3,100,100,powerofPlayer,Player.pos.x-25,Player.pos.y-50,0,1,3*k,velocityBullet);
               PlayerBullet.push_back(tmp);
               k++;
            }
}


//判断是否发生碰撞
template <typename T1,typename T2>
int collide(T1 &A,T2 &B){
    if((A.pos-B.pos).length()<=(A.radius+B.radius))
    {
        A.life=A.life-B.powerofcollide;
        B.life=B.life-A.powerofcollide;
        A.shotperiod=5;
        B.shotperiod=5;
        return 1;
        //std::cout<<A.life<<"  "<<B.life<<std::endl;
    }
    return 0;
}





//以下两个函数用于画游戏开始与结束
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
	Rect rect = {400, 0, 85, 800};//画一块颜色,左边界，上边界，右边界 ，下边界
	setPenColor((Color){0, 255, 0, 200});
	drawRect( rect, true );
}

//以下函数用于弹出提示字符
void draw_Hint()
{
	Image *text = textToImage( "< This is not a simple game demo. >");
	int w,h;
	getImageSize( text, w, h );
	drawImage( text, SCREEN_WIDTH-h/2-w/2, SCREEN_HEIGHT/2-h/2, 1, 1, 90+180 );
	cleanup(text);
}

//显示玩家当前状态
void draw_Status(){
  Rect rect1 = {10,520,Player.life*150/lifeofPlayer,20};//画一块颜色,左边界，上边界，右边界 ，下边界
  setPenColor((Color){250, 0, 0,250});
  drawRect( rect1, true );
  Rect rect2 = {10+Player.life*100/lifeofPlayer,520,150-Player.life*100/lifeofPlayer,20};//画一块颜
  setPenColor((Color){250, 0, 0,50});
  drawRect( rect2, true );
  Image *text = textToImage( "LIFE");
  drawImage( text,10,520, 1, 1,0);
  rect1= {10,550,Player.properiod*150/sanperiod,20};//画一块颜色,左边界，上边界，右边界 ，下边界
  setPenColor((Color){0, 250, 0,250});
  drawRect( rect1, true );
  rect2= {10+Player.properiod*100/sanperiod,550,150-Player.properiod*100/sanperiod,20};//画一块颜
  setPenColor((Color){0, 250, 0,50});
  drawRect( rect2, true );
  text=textToImage( "Umbrella");
  drawImage( text,10,550, 1, 1,0);
  rect1= {10,580,Player.thunder*150/leiperiod,20};//画一块颜色,左边界，上边界，右边界 ，下边界
  setPenColor((Color){0, 0, 250,250});
  drawRect( rect1, true );
  rect2= {10+Player.thunder*100/leiperiod,580,150-Player.thunder*100/leiperiod,20};//画一块颜
  setPenColor((Color){0, 0, 250,50});
  drawRect( rect2, true );
  text=textToImage( "Thunder");
  drawImage( text,10,580,1,1,0);
  cleanup(text);
}


void draw_Bullet()
{
    for(std::vector<bullet>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();++iter)
	  plotbullet((*iter),0);
   for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();++iter)
	  plotbullet((*iter),1);
}


void draw_Enemy()
{
   for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter){
            plotplane(*iter,1);
   }
}

void draw_flower(){
    for(std::vector<flower>::iterator iter=flowerlist.begin();iter!=flowerlist.end();++iter){
            int w,h;
            getImageSize(imageflower[(*iter).type], w, h );
            Rect clip = {w/24*(*iter).life,0, w/24,h};
            drawImage(imageflower[(*iter).type],(*iter).pos.x, (*iter).pos.y, ((*iter).width)*1.0/w*24, ((*iter).height)*1.0/h,0, NULL, FLIP_NONE, &clip );
            (*iter).life++;
    }

    for(std::vector<flower>::iterator iter=explodelist.begin();iter!=explodelist.end();++iter){
            int w,h;
            getImageSize(imageflower[(*iter).type],w,h);
            Rect clip = {w/5*((*iter).life/4),0, w/5,h};
           // std::cout<<w/5*((*iter).life/4)<<" "<<w<<std::endl;
            drawImage(imageflower[(*iter).type],(*iter).pos.x, (*iter).pos.y, ((*iter).width)*1.0/w*5, ((*iter).height)*1.0/h,0,NULL,FLIP_NONE,&clip);
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

//此函数用于生成敌人
void  newEnemy(){
    if(Enemy.size()<4){
        plane tmp(1,50,50,powerofcollide,(rand()%640),10,lifeofEnemy,0,0,1,0,5);
        Enemy.push_back(tmp);
    }
}

//速度更新,随机移动
void Enemy_move(){
for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();)
{
     if((*iter).speed.y<1){
         (*iter).speed.y+=0.2;
     }
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
//敌机随机改变速度

int judge;
for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
{
    //实现撞击时的抖动效果
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
	draw_Status();//显示用户生命值，其他状态的函数
}

//判断是否出界
template <typename T>
bool check(const T &A){
  return (A.pos.x>0&&A.pos.x<SCREEN_WIDTH&&A.pos.y>0&&A.pos.y<SCREEN_HEIGHT);
}

void collide_all(){
    if(san&&Player.properiod>0){
       for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
        {
            if( ((*iter).pos-Player.pos).length()<=100){
                (*iter).life=-1;
            }
            if(lei&&(*iter).pos.y<Player.pos.y)
                {(*iter).life-=powerofthunder;}
        }
       for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();)
        {
            if( ((*iter).pos-Player.pos).length()<=130){
                PlayerBullet.push_back(*iter);
                EnemyBullet.erase(iter);
                continue;
            }
            ++iter;
        }
    }
     else
    {
        int tmp;
        for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
        {
                  if(lei&&(*iter).pos.y<Player.pos.y)
                      {(*iter).life-=powerofthunder;
                        std::cout<<"雷击"<<std::endl;
                      }

                  if(((*iter).pos-Player.pos).length()<=((*iter).radius+Player.radius))
                        {
                        (*iter).life=(*iter).life-Player.powerofcollide;
                        Player.life=Player.life-(*iter).powerofcollide;
                        tmp=Player.speed.x;
                        Player.speed.x=(*iter).speed.x;
                        (*iter).speed.x=tmp;
                        Player.speed.y=5;
                        (*iter).speed.y=-5;
                        }
        }
       for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();++iter)
        {
                if(lei&&(*iter).pos.y<Player.pos.y)
                {(*iter).life-=powerofthunder;}
                 collide(*iter,Player);
        }
     }
    for(std::vector<bullet>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();++iter)
	{
        for(std::vector<plane>::iterator iter2=Enemy.begin();iter2!=Enemy.end();++iter2)
        {
            if(collide(*iter2,*iter)){
               effect_Play(2,0);
            }
        }
	}
}

void create_flower(){
   for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
	{
         if((*iter).life<0){
                    flower tmp(2,100,100,(*iter).pos.x,(*iter).pos.y,16);
                    explodelist.push_back(tmp);
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
                 std::cout<<"Enemy"<<std::endl;
                 flower tmp(0,80,80,(*iter).pos.x,(*iter).pos.y,23);
                 flowerlist.push_back(tmp);
           }
	}
}

void clear_all(){
  for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();)
	{
	       //std::cout<<((*iter).life)<<"  "<<(((*iter).life)<0)<<std::endl  ;
           if(((*iter).life)<0||(*iter).pos.y>SCREEN_HEIGHT){
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

    for(std::vector<flower>::iterator iter=explodelist.begin();iter!=explodelist.end();)
    {
	      if((*iter).life>=(*iter).maxtime){
                explodelist.erase(iter);
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
             (*iter).pos=(*iter).pos+(*iter).speed;
    }
    for(std::vector<bullet>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();++iter)
    {
             (*iter).pos=(*iter).pos+(*iter).speed;
    }
}

void deal()
{
    Player.period++;//用period表示对象存活的时间
    for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
	{
         (*iter).period++;
	}
    bool shoot=false;
	bool move = false;
	san=false;//是否开保护伞
	lei=false;
	if(keyboard['k']&&Player.period%5==1)
    {
          if(keyboard['k']){
            effect_Play(0,0);
           }
        shoot=true;
    }
	if( keyboard[KEY_UP]||keyboard['w'] )
	{
		Player.speed=Player.speed+ PointD(0,-1)*speedPlayer;
		move = true;
	}
	if( keyboard[KEY_DOWN]||keyboard['s'] )
	{
		Player.speed=Player.speed+ PointD(0,+1)*speedPlayer;
		move = true;
	}
	if( keyboard[KEY_LEFT]|| keyboard['a'] )
	{
		Player.speed=Player.speed+PointD(-1,0)*speedPlayer;
		move = true;
	}
	if( keyboard[KEY_RIGHT]||keyboard['d'] )
	{
		Player.speed=Player.speed+PointD(+1,0)*speedPlayer;
		move = true;
	}
    if(keyboard['r'] )
	{
	    san=true;
	}
	if(keyboard['t']){
        shoot=false;
        lei=true;
	}
	double len = Player.speed.length();
	if( len > speedPlayer )
	{
		Player.speed = Player.speed/len*speedPlayer;
	}
    //如果按了射击键，则生成一个新的子弹
	if(shoot){
         	Player_shoot();
	         }
	Enemy_move();
    bullet_move();
    draw_thunder();
	//敌机发射子弹
    Enemy_shoot();
    collide_all();
    create_flower();
    clear_all();

    if(Player.pos.x+ Player.speed.x<0||Player.pos.y+Player.speed.y<0||Player.pos.y+Player.speed.y>SCREEN_HEIGHT||
       Player.pos.x+ Player.speed.x>SCREEN_WIDTH){
        Player.speed=Player.speed*0;
    }
	else{
    Player.pos=Player.pos+Player.speed;
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





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
int lifeofPlayer;//玩家的生命值
double lifeofEnemy;//玩家的生命值
int powerofPlayer;//玩家子弹的攻击力
int powerofBoss;
int lifeofBoss;
int  powerofEnemy;//敌人子弹的攻击力
int  powerofcollide;//任意两个物体相撞的伤害值
int  SCORE;
int sanperiod;//最大开启保护伞时间
int leiperiod;
int powerofthunder;
int revive;
Image *imageplane[3],*imagebullet[3],*imageflower[4],*imageobject[2],*imagemouse,*thunder[4];
Mix_Chunk *effect[4];
//extern plane Player;
std::vector <plane> Enemy;
std::vector <bullet> PlayerBullet;
std::vector <bullet> EnemyBullet;//用于存放敌人子弹
std::vector <flower> flowerlist;//实现碰撞的火花效果
std::vector <flower> explodelist;//实现爆炸效果
std::vector <object> objectlist;
std::vector <Hint>  Hintlist;
//生成鼠标
plane mouse;
playerplane Player;
bossplane Boss;
int lastAnime=0;
int Sound_Init()
{
        int    TMP_FREQ = MIX_DEFAULT_FREQUENCY;
        const Uint16 TMP_FORMAT = MIX_DEFAULT_FORMAT;
        const int    TMP_CHAN =2;
        const int    TMP_CHUNK_SIZE = 512;
        return Mix_OpenAudio(TMP_FREQ,TMP_FORMAT,TMP_CHAN,TMP_CHUNK_SIZE);
}
void effect_Play(int num,int loop)
{
      Mix_PlayChannel(-1,effect[num],loop);
      std::cout<<Mix_GetError()<<std::endl;
}
template <typename T>
void renewcenter(T &A){
       A.center.x=A.pos.x+A.width/2;A.center.y=A.pos.y+A.height/2;
}


int choice(bool & quit)
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
	drawRect(rect,true);
    Image *text = textToImage("START GAME");
	int w,h;
	getImageSize( text, w, h );
	drawImage( text,200,330, 1, 1,0);
	cleanup(text);

    text = textToImage("THUNDER-BE A MAN AND HOLD 100 SECONDS!");
	getImageSize( text, w, h );
	drawImage( text,50,130, 1, 1,0);
	cleanup(text);



    mouse.pos=PointD(mouseX, mouseY);
	getImageSize(imagemouse, w, h );
	lastAnime = (lastAnime+15)%(4*60);
	Rect clip = {w/16*(lastAnime/60), 0, w/16,h/16};
	setImageAlpha( imagemouse, 255 );
	drawImage( imagemouse, mouse.pos.x, mouse.pos.y, 2, 2, 0, NULL, FLIP_NONE, &clip );
    if(mousePressed&&start){
        return 0;
    }
    if( keyboard[KEY_ESC])
		quit = true;
	return 2;
}

void load_all()
{
    imageplane[0]=loadImage( "player.png" );
    imageplane[1]=loadImage( "enemy1.png" );
    imageplane[2]=loadImage( "enemy2.png" );
    //imageplane[3]=loadImage( "enemy3.png" );
    imagebullet[0]=loadImage( "bullet2.png" );
    imageflower[0]=loadImage("red_strip24.png");
    imagebullet[1]=loadImage( "bullet3.png" );
    imageflower[1]=loadImage("red_strip24.png");
    imagebullet[2]=loadImage( "bullet4.png");
    imageflower[2]=loadImage("poison_strip24.png");
    imageflower[3]=loadImage("explode.png");
    imageobject[0]=loadImage("blood.png");
    imageobject[1]=loadImage("bulletplus.png");
    imagemouse=loadImage("mouse.png");
    thunder[0]=loadImage("ele0.png");
    thunder[1]=loadImage("ele1.png");
    thunder[2]=loadImage("ele2.png");
    thunder[3]=loadImage("ele3.png");
    effect[0]=Mix_LoadWAV_RW(SDL_RWFromFile("res/music/shoot.ogg", "rb"),1 );
    effect[1]=Mix_LoadWAV_RW(SDL_RWFromFile("res/music/beshot.ogg", "rb"),1 );
    effect[2]=Mix_LoadWAV_RW(SDL_RWFromFile("res/music/shooten.ogg", "rb"),1 );
    effect[3]=Mix_LoadWAV_RW(SDL_RWFromFile("res/music/lei.ogg", "rb"),1 );
    std::cout<<Mix_GetError()<<std::endl;
}

bool lei,san,Boss_exist,Boss_killed,Player_die,gameover;
void draw_thunder(){
    if(Player.thunder>0){
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
     getImageSize(imageplane[A.type],w,h);
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
     revive=3;
     SCORE=0;
     Enemy={};
     PlayerBullet={};
     EnemyBullet={};//用于存放敌人子弹
     flowerlist={};//实现碰撞的火花效果
     explodelist={};//实现爆炸效果
     objectlist={};
     Hintlist={};
     Boss_exist=false;
    Boss_killed=false;
    Player_die=false;
    gameover=false;
    lifeofPlayer=500;//玩家初始生命值
	Player.pos.x=SCREEN_WIDTH/2;Player.pos.y=SCREEN_HEIGHT/2;Player.life=lifeofPlayer;
	Player.type=0;Player.bulletnum=1;Player.properiod=500;
	Player.width=50;Player.height=50;
    renewcenter(Player);
	powerofPlayer=10;
	powerofBoss=50;
	lifeofBoss=15000;
    powerofcollide=30;
	Player.powerofcollide=powerofcollide;
    speedPlayer=10;
    sanperiod=500;
    leiperiod=100;
    powerofthunder=1;
	FPS_DISPLAY = true;
	velocityBullet=-3;
	velocityEnemyBullet=-4;
	lifeofEnemy=200;
    powerofEnemy=5;
	canvasColor = {0, 0, 0, 255};
    srand((unsigned)time(NULL));
}

void draw_Player()
{
    int w,h;
    getImageSize(imageplane[Player.type],w,h);
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
    Player.period++;
}

void draw_Boss()
{
    int w,h;
    getImageSize(imageplane[Boss.type], w, h );
    if(Boss.shotperiod){
        --Boss.shotperiod;
        Boss.pos.x+=(rand()%5-2);
        Boss.pos.y+=(rand()%5-2);
    }
    if(Boss.life<lifeofBoss/20){
        Boss.pos.x+=(rand()%5-2)*5;
        Boss.pos.y+=(rand()%5-2)*5;
    }
    drawImage(imageplane[Boss.type],Boss.pos.x,Boss.pos.y,Boss.width*1.0/w,Boss.height*1.0/h,Boss.speed.x*4,NULL,FLIP_NONE);
    Rect rect1 = {Boss.pos.x+100,Boss.pos.y+200,std::max(0,Boss.life*150/lifeofBoss),20};//画一块颜色,左边界，上边界，右边界 ，下边界
    setPenColor((Color){250, 0, 0,250});
    drawRect( rect1, true );
    Rect rect2 = {Boss.pos.x+100+Boss.life*150/lifeofBoss,Boss.pos.y+200,150-std::max(0,Boss.life*150/lifeofBoss),20};//画一块颜
    setPenColor((Color){250, 0, 0,50});
    drawRect( rect2, true );
    Boss.period++;
}

void Boss_shoot(){
        if(Boss.period%8==1){
            int k=-(Boss.bulletnum-1)/2;
            for(int i=0;i<Boss.bulletnum;++i){
               bullet tmp(1,50,50,powerofBoss,Boss.center.x,Boss.center.y,1,3*k,-velocityBullet*2);
               EnemyBullet.push_back(tmp);
               k++;
            }
        }
}

void Boss_move(){
    if(Boss.center.y>50){
        Boss.speed.y=0;
    }
    if(rand()%30==1){
        if(Boss.center.x>Player.center.x){
            Boss.speed.x=-1;
        }
        else{
            Boss.speed.x=1;
        }
    }
    Boss.pos=Boss.pos+Boss.speed;
    renewcenter(Boss);
}

void Enemy_shoot(){
    for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
	{
       if((*iter).period%81==1){
            int k=-((*iter).bulletnum-1)/2;
            for(int i=0;i<(*iter).bulletnum;++i){
               bullet tmp(2,40,40,powerofEnemy,(*iter).center.x,(*iter).center.y,1,3*k,-velocityEnemyBullet);
               EnemyBullet.push_back(tmp);
               k++;
            }
       }
	}
}


void Player_shoot(){
            if(Player.bulletperiod>0){
                --Player.bulletperiod;
            }
            else if(Player.bulletnum>3){
                Player.bulletnum=3;
            }
            int k=-(Player.bulletnum-1)/2;
            for(int i=0;i<Player.bulletnum;++i){
               bullet tmp(0,50,50,powerofPlayer,Player.center.x,Player.center.y,1,3*k,velocityBullet*2);
               PlayerBullet.push_back(tmp);
               k++;
            }
}


//判断是否发生碰撞
template <typename T1,typename T2>
int collide(T1 &A,T2 &B){
    if((A.center-B.center).length()<=(A.radius+B.radius))
    {
        A.life=A.life-B.powerofcollide;
        B.life=B.life-A.powerofcollide;
        A.shotperiod=5;
        B.shotperiod=5;
        //Hint tmp("Collide",A.pos,40,40,10);
        //Hintlist.push_back(tmp);
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
void plotHint(Hint &A)
{
    Image *text = textToImage(A.info);
    int w,h;
    getImageSize(text,w,h);
    drawImage(text,A.pos.x,A.pos.y,A.width*1.0/w,A.height*1.0/h,0);
    cleanup(text);
    A.period++;
}

void draw_Hint(){
  for(std::vector<Hint>::iterator iter=Hintlist.begin();iter!=Hintlist.end();++iter){
      plotHint(*iter);
  }
}

//Show the status of player
void draw_Status(){
  Rect rect1 = {10,520,Player.life*200/lifeofPlayer,20};
  setPenColor((Color){250, 0, 0,250});
  drawRect( rect1, true );
  Rect rect2 = {10+Player.life*200/lifeofPlayer,520,200-std::max(0,Player.life*200/lifeofPlayer),20};//画一块颜
  setPenColor((Color){250, 0, 0,50});
  drawRect( rect2, true );
  Image *text1 = textToImage( "Your Life");
  drawImage( text1,10,520, 1, 1,0);
  rect1= {10,550,Player.properiod*200/sanperiod,20};//画一块颜色,左边界，上边界，右边界 ，下边界
  setPenColor((Color){0, 250, 0,250});
  drawRect( rect1, true );
  rect2= {10+Player.properiod*200/sanperiod,550,200-std::max(0,Player.properiod*200/sanperiod),20};//画一块颜
  setPenColor((Color){0, 250, 0,50});
  drawRect( rect2, true );
  Image *text2=textToImage( "Umbrella. Press R");
  drawImage( text2,10,550, 1, 1,0);
  rect1= {10,580,Player.thunder*200/leiperiod,20};//画一块颜色,左边界，上边界，右边界 ，下边界
  setPenColor((Color){0, 0, 250,250});
  drawRect( rect1, true );
  rect2= {10+Player.thunder*200/leiperiod,580,200-std::max(0,Player.thunder*200/leiperiod),20};//画一块颜
  setPenColor((Color){0, 0, 250,50});
  drawRect( rect2, true );
  Image *text3=textToImage("Thunder. Press T");
  drawImage( text3,10,580,1,1,0);
  cleanup(text1);cleanup(text2);cleanup(text3);
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
            (*iter).period++;
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
    int w,h;
    for(std::vector<object>::iterator iter=objectlist.begin();iter!=objectlist.end();++iter){
            getImageSize(imageobject[(*iter).type],w,h);
            drawImage(imageobject[(*iter).type],(*iter).pos.x,(*iter).pos.y,1.0*(*iter).width/w,1.0*(*iter).height/h,0);
    }
}

void eat_object(){
     for(std::vector<object>::iterator iter=objectlist.begin();iter!=objectlist.end();){
            //std::cout<<((*iter).center-Player.center).length()<<" "<<(Player.radius+(*iter).radius)<<std::endl;
            if(((*iter).center-Player.center).length()<=(Player.radius+(*iter).radius+50))
            {
                switch((*iter).type){
                 case 0:
                           Player.life=std::min(Player.life+50,lifeofPlayer);break;
                 case 1:
                           Player.bulletnum+=2;Player.bulletperiod=100;
                }
                objectlist.erase(iter);
            }
            else
            ++iter;
     }
}

void move_object(){
 for(std::vector<object>::iterator iter=objectlist.begin();iter!=objectlist.end();++iter){
    (*iter).pos=(*iter).pos+(*iter).speed;
    renewcenter(*iter);
 }
}


//Create a new enemy with some probability
void  newEnemy(){
    if(Enemy.size()<8){
        plane tmp(1,80,80,powerofcollide,(rand()%640),10,lifeofEnemy,0,0,1,0,5);
        Enemy.push_back(tmp);
    }
}

//Move all the enemies
void Enemy_move(){
for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();)
{
     if((*iter).speed.y<1){
         (*iter).speed.y+=0.2;
     }
     if((*iter).center.x+(*iter).speed.x<0||((*iter).center.x+(*iter).speed.x>SCREEN_WIDTH-30)){
          (*iter).speed.x=0;
     }
     (*iter).pos=(*iter).pos+(*iter).speed;
     if((*iter).center.y>SCREEN_HEIGHT){
        Enemy.erase(iter);
     }
     else
        iter++;
}


/*
 * Enable all enemies to move randomly.
 */
int judge;
for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
{
    //Vibration of a plane after being shot.
    if((*iter).shotperiod){
        --(*iter).shotperiod;
        (*iter).pos.x+=(rand()%5-2);
        (*iter).pos.y+=(rand()%5-2);
    }
    //Move Enemy horizontally randomly
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
   renewcenter(*iter);
    }
}


//The function to create the Boss.
void create_boss(){
    Boss.pos.x=SCREEN_WIDTH/2;
    Boss.pos.y=-200;
    renewcenter(Boss);
    Boss.speed.y=1;
	Boss.type=2;
	Boss.bulletnum=3;
	Boss.height=300;
    Boss.width=300;
    Boss.radius=150;
    Boss.life=lifeofBoss;
    Boss_exist=true;
    Hint tmp("BOSS COMMING!",150,SCREEN_HEIGHT/2,350,100,300);
    Hintlist.push_back(tmp);
}

//Draw all the pictures.
void draw()
{
	draw_Bullet();
    if(lei)
    draw_thunder();
	draw_Enemy();
	if(!Player_die){
        draw_Player();
	}
	draw_flower();
	draw_object();
	draw_Hint();
	draw_object();
	if(Boss_exist&&!Boss_killed){
        draw_Boss();
	}
	draw_Hint();
	draw_Status();
}


//When 'T' is pressed, all enemies ahead are struck by the lightning
void leiji_all(){
  for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
        {
             if((*iter).center.y<Player.center.y)
                {
                 (*iter).life-=powerofthunder;(*iter).shotperiod=5;
                 Player.properiod=std::min(Player.properiod+1,sanperiod);
                }
        }
  for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();++iter)
        {
                if(lei&&(*iter).center.y<Player.center.y)
                {
                    (*iter).life-=powerofthunder;
                    Player.properiod=std::min(Player.properiod+1,sanperiod);
                }
        }
  if(Boss_exist&&!Boss_killed&&Boss.center.y<Player.center.y)
    {
      Boss.life-=powerofthunder;
      Boss.shotperiod=5;
      Player.properiod=std::min(Player.properiod+1,sanperiod);
    }
}

//Collide bullets with planes, planes with planes.
void collide_all(){
    //When 'R' is pressed, the umbrella is on, all the bullets are bounced back
    if(san&&Player.properiod>0){
       for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
        {
            if( ((*iter).center-Player.center).length()<=100){
                (*iter).life=-1;
            }
        }
       for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();)
        {
            if(((*iter).center-Player.center).length()<=130){
                (*iter).speed=(*iter).speed*-1;
                (*iter).powerofcollide=(*iter).powerofcollide/3;
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
                  if(((*iter).center-Player.center).length()<=((*iter).radius+Player.radius))
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
                 collide(*iter,Player);
        }
    }
    for(std::vector<bullet>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();++iter)
	{
	    if(Boss_exist&&!Boss_killed){
            if(collide(Boss,(*iter))){
               Player.thunder=std::min(Player.thunder+1,leiperiod);
            }
	    }
        for(std::vector<plane>::iterator iter2=Enemy.begin();iter2!=Enemy.end();++iter2)
        {
            if(collide(*iter2,*iter)){
               effect_Play(2,0);
               Player.thunder=std::min(Player.thunder+1,leiperiod);
            }
        }
	}
}

//When a plane or bullet dies, a spark is created.
void create_flower(){
   for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();++iter)
	{
         if((*iter).life<0){
                    flower tmp(3,100,100,(*iter).center.x,(*iter).center.y,16);
                    explodelist.push_back(tmp);
                 if(rand()%3==1){
                     object tmp(rand()%2,50,50,(*iter).center.x,(*iter).center.y);
                     objectlist.push_back(tmp);
                 }
         }
	}
   for(std::vector<bullet>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();++iter)
	{
         if((*iter).life<0){
                 flower tmp((*iter).type,30,30,(*iter).center.x,(*iter).center.y,23);
                 flowerlist.push_back(tmp);
           }
	}
   for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();++iter)
	{
         if((*iter).life<0){
                 flower tmp((*iter).type,80,80,(*iter).center.x,(*iter).center.y,23);
                 flowerlist.push_back(tmp);
           }
	}
}

//Clear all objects with minus life value.
void clear_all(){
  for(std::vector<plane>::iterator iter=Enemy.begin();iter!=Enemy.end();)
	{
           if(((*iter).life)<0||(*iter).center.y>SCREEN_HEIGHT){
                Enemy.erase(iter);
                Hint tmp("GOOD JOB!",(*iter).center,100,50);
                ++SCORE;
                Hintlist.push_back(tmp);
           }
           else{
            ++iter;
           }
	}
	for(std::vector<bullet>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();)
    {
	      if((*iter).life<0||(*iter).pos.y<0){
                PlayerBullet.erase(iter);
           }
           else{
            ++iter;
           }
    }
     for(std::vector<bullet>::iterator iter=EnemyBullet.begin();iter!=EnemyBullet.end();)
    {
	      if((*iter).life<0||(*iter).pos.y>SCREEN_HEIGHT){
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
	      if((*iter).pos.y>SCREEN_HEIGHT){
                objectlist.erase(iter);
           }
           else{
                ++iter;
           }
    }
    for(std::vector<Hint>::iterator iter=Hintlist.begin();iter!=Hintlist.end();)
    {
	      if((*iter).period>(*iter).maxtime){
                Hintlist.erase(iter);
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
             renewcenter(*iter);
    }
    for(std::vector<bullet>::iterator iter=PlayerBullet.begin();iter!=PlayerBullet.end();++iter)
    {
             (*iter).pos=(*iter).pos+(*iter).speed;
              renewcenter(*iter);
    }
}

void deal()
{
    bool shoot=false;
	bool move = false;
	//whether the umbrella is on(if 'T' is pressed and sanperiod>0)
	san=false;
	//whether the thunder is on.
	lei=false;
	if(keyboard['k']&&Player.period%10==1)
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
	if(shoot){
         	Player_shoot();
	         }
	Enemy_move();
	if(Boss_exist&&Boss.life>0){
         Boss_shoot();
         Boss_move();}
    if(Boss_exist&&!gameover&&Boss.life<0){
        Boss_killed=true;
        gameover=true;
        flower tmp(3,100,100,Boss.center.x,Boss.center.y,128);
        explodelist.push_back(tmp);
    }
    if(!Boss_killed&&Player.life<0&&!gameover){
        Player_die=true;
        gameover=true;
        flower tmp(3,100,100,Player.center.x,Player.center.y,128);
        explodelist.push_back(tmp);
    }
    if(lei&&Player.thunder){
         leiji_all();
    }
    bullet_move();
    Enemy_shoot();
    collide_all();
    eat_object();
    move_object();
    create_flower();
    clear_all();
    if(Player.center.x+ Player.speed.x<0||Player.center.y+Player.speed.y<0||Player.center.y+Player.speed.y>SCREEN_HEIGHT||
       Player.center.x+ Player.speed.x>SCREEN_WIDTH){
        Player.speed=Player.speed*0;}
	else{
    Player.pos=Player.pos+Player.speed;
    renewcenter(Player);
	}
	if(!move)
	{
		Player.speed=Player.speed* 0.8;
		if( Player.speed.length() < 0.1 )
			Player.speed=PointD();
	}
}

int work(bool &quit)
{
	deal();
	draw();
	if( keyboard[KEY_ESC])
		quit = true;
    if(Boss_killed&&explodelist.empty()){
        return 1;
    }
    if(Player_die&&explodelist.empty()){
        if(revive==1){
           return -1;
        }
        else{
            return 4;
        }
    }
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
    cleanup(imagemouse);
    for(int i=0;i<4;++i){
        cleanup(thunder[i]);
        cleanup(imageflower[i]);
    }
    for(int i=0;i<2;++i){
        cleanup(imageobject[i]);
    }
    for(int i=0;i<3;++i){
        cleanup(imageplane[i]);
        cleanup(imagebullet[i]);
    }
    for(int i=0;i<4;++i){
            Mix_FreeChunk(effect[i]);
    }
    Mix_CloseAudio();
}

//Revive the player.
void player_revive()
{
   --revive;
   gameover=false;
   Player_die=false;
   Hint tmp("LOST ONE LIFE",SCREEN_WIDTH/2-200,SCREEN_HEIGHT/2,300,100,100);
   Hintlist.push_back(tmp);
   Player.life=lifeofPlayer;
}

//Show the result when player win.
int showwin( bool &quit )
{
  bool restart=false;
  Rect rect = {100, 300,400, 100};
  if(100<mouse.pos.x&&mouse.pos.x<500&&300<mouse.pos.y&&mouse.pos.y<400){
    	setPenColor((Color){50, 100, 0,250});
    	restart=true;
  }
  else{
        setPenColor((Color){50, 100, 250,250});
  }
	drawRect(rect,true);
    Image *text = textToImage("RETRY?");
	int w,h;
	getImageSize( text, w, h );
	drawImage( text,150,350, 1, 1,0);
	cleanup(text);
    mouse.pos=PointD(mouseX, mouseY);
	getImageSize(imagemouse, w, h );
	lastAnime = (lastAnime+15)%(4*60);
	Rect clip = {w/16*(lastAnime/60), 0, w/16,h/16};
	setImageAlpha( imagemouse, 255 );
	drawImage( imagemouse, mouse.pos.x, mouse.pos.y, 2, 2, 0, NULL, FLIP_NONE,&clip);
     char info[20];
     sprintf(info, "YOU WIN! YOUR SCORE IS: %2d", (int)(SCORE));
     drawText(info,200,430);
    if(mousePressed&&restart){
        return 3;
    }
    if( keyboard[KEY_ESC])
		quit = true;
	return 1;
}

int showlose( bool &quit )
{
   bool restart=false;
  Rect rect = {100, 300,400, 100};
  if(100<mouse.pos.x&&mouse.pos.x<500&&300<mouse.pos.y&&mouse.pos.y<400){
    	setPenColor((Color){50, 100, 0,250});
    	restart=true;
  }
  else{
        setPenColor((Color){50, 100, 250,250});
  }
	drawRect(rect,true);
    Image *text = textToImage("RETRY?");
	int w,h;
	getImageSize( text, w, h );
	drawImage( text,150,350, 1, 1,0);
	cleanup(text);
    mouse.pos=PointD(mouseX, mouseY);
	getImageSize(imagemouse, w, h );
	lastAnime = (lastAnime+15)%(4*60);
	Rect clip = {w/16*(lastAnime/60), 0, w/16,h/16};
	setImageAlpha( imagemouse,255);
	drawImage( imagemouse, mouse.pos.x, mouse.pos.y, 2, 2, 0, NULL, FLIP_NONE,&clip);
     char info[20];
     sprintf(info, "YOU LOSE,YOUR SCORE IS: %2d", (int)(SCORE));
     drawText(info,200,430);
    if(mousePressed&&restart){
        return 3;
    }
    if( keyboard[KEY_ESC])
		quit = true;
	return -1;
}



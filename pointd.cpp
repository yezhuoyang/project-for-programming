#include  "pointd.h"

/*
  此程序用于实现pointd.h中的成员函数
*/

PointD PointD::operator+(const PointD &a) const
{
    PointD tmp;
    tmp.x=x+a.x;
    tmp.y=y+a.y;
    return tmp;
}


PointD PointD::operator-(const PointD &a) const
{
    PointD tmp;
    tmp.x=x-a.x;
    tmp.y=y-a.y;
    return tmp;
}

PointD PointD::operator/(const double &a) const
{
    PointD tmp;
    tmp.x=x/a;
    tmp.y=y/a;
    return tmp;
}

PointD PointD::operator*(const double &a) const
{
    PointD tmp;
    tmp.x=x*a;
    tmp.y=y*a;
    return tmp;
}


PointD::PointD (double x0,double y0):x(x0),y(y0){}


template<class T1,class T2>
double distance(const T1 &a,const T2 &b)
{
   return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}


template<class T1,class T2>
void collide(T1 &A,T2 &B){
    if(distance(A,B)<=(A.radius+B.radius)){
        A.life-=A.powerofCollide;
        B.life-=B.powerofCollide;
        A.speed=(A.speed*(A.mass-B.mass)+2*B.mass*B.speed)/(A.mass+B.mass);
        B.speed=(B.speed*(B.mass-A.mass)+2*A.mass*A.speed)/(A.mass+B.mass);
    }
}

//检查对象是否应该继续存在
bool object::check(){
    return((pos.x-radius)>0&&(pos.x+radius)<SCREEN_WIDTH&&(pos.y-radius)>0&&(pos.y+radius)<SCREEN_HEIGHT&&life>0);
}

//object::object(const double &X=0,const double &Y=0,const double &R=0,const double &Life=0,const double M;
//           const double &Vx=0,const double &Vy=0):pos(PointD(X,Y)),radius(R),life(Life),mass(M),speed(PointD(Vx,Vy)) {}

void object::object_move(){
    pos=pos+speed;
} //实现物体的移动


bullet::bullet(int bult,const double &X,const double &Y,const double &Vx,const double &Vy){
    radius=0;
    mass=10;
    bullettype=bult;
    switch(bullettype)
     {
     case 1:
          pos=PointD(X,Y);
          speed=PointD(Vx,Vy);
          period=0;
     }
}



void bullet::draw_onebullet(){
     switch(bullettype)
     {
     case 1:
            drawImage(imageBullet,pos.x,pos.y,0.5,0.5);
     }
};

enemyplane::enemyplane(int typ,const double &X,const double &Y,const double &Vx,const double &Vy){
    type=typ;
    radius=50;
    mass=100;
    bulletnum=1;
    bulletangle=0;
    switch(type)
     {
     case 1:
          pos=PointD(X,Y);
          speed=PointD(Vx,Vy);
          period=0;
     }
}


void enemyplane::draw_oneenemy(){
     switch(type)
     {
     case 1:
         	int w,h;
            getImageSize( imagePlayer, w, h );
	        setImageAlpha( imagePlayer, 150);
            drawImage( imagePlayer, pos.x-w/2, pos.y-h/2, 0.5, 0.5 );
     }
}


void playerplane::drawplayer(){
     switch(type)
     {
     case 1:
         	int w,h;
            getImageSize( imagePlayer, w, h );
	        setImageAlpha( imagePlayer, 150);
            drawImage( imagePlayer, pos.x-w/2, pos.y-h/2, 0.5, 0.5 );
     }
}


playerplane::playerplane(){
          radius=50;
          mass=100;
          bulletnum=1;
          bulletangle=0;
          pos=PointD(100,100);
          speed=PointD(0,0);
          period=0;
}

void enemyplane::shoot(){
    double theta=-bulletangle;
    double bulletspeed=10;
    for(int i=0;i<bulletnum;++i){
        theta+=(2*bulletangle)/(bulletnum-1);
        bullet tmp(type,pos.x,pos.y,sin(theta)*bulletspeed,cos(theta)*bulletspeed);
        EnemyBullet.push_back(tmp);
    }
}


void playerplane::shoot(){
    double theta=-bulletangle;
    double bulletspeed=10;
    for(int i=0;i<bulletnum;++i){
        theta+=(2*bulletangle)/(bulletnum-1);
        bullet tmp(type,pos.x,pos.y,sin(theta)*bulletspeed,cos(theta)*bulletspeed);
        playerBullet.push_back(tmp);
    }
}












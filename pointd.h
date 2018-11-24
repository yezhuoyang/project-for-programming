/*
 * Coded by Jingxiao Chen.
 *
 */
#ifndef POINTD_HPP
#define POINTD_HPP
#include <cmath>
#define Image  SDL_Texture
struct PointD
{
	PointD(const double &X=0, const double &Y=0)
		:x(X),y(Y){}
	double x,y;
	double length() const
	{
		return sqrt(x*x+y*y);
	}
	PointD operator + ( const PointD &a ) const
	{
		return PointD(x+a.x, y+a.y);
	}
	PointD operator - ( const PointD &a ) const
	{
		return PointD(x-a.x, y-a.y);
	}
	PointD operator * ( const double &a ) const
	{
		return PointD(x*a, y*a);
	}
	PointD operator / ( const double &a ) const
	{
		return PointD(x/a, y/a);
	}
};

struct Hint
{
    //position of Hint
    PointD pos;
    //period of its existence
    int period;
    //the longeset period it can survive
    int maxtime;
    //The size of it.
    int width;
    int height;
    std::string info;
    Hint(const std::string &I,const PointD &A,const int &wid,const int &hei,const int &maxt=100):info(I),
    pos(A),width(wid),height(hei),maxtime(maxt){period=0;}
    Hint(const std::string &I,const int &X,const int &Y,const int &wid,const int &hei,const int &maxt=100):info(I),
    pos(PointD(X,Y)),width(wid),height(hei),maxtime(maxt){period=0;}
};
struct bullet
{
    //The position of the upper-left point of it's image
    PointD pos;
    //The position of it's center
    PointD center;
    PointD speed;
    double theta;
    double radius;
    //When it collide with B, B will lose it's life of value powerofcollide.
    int powerofcollide;
    //The type of the bullet.
    int  type;
    //The width of the bullet.
    int width;
    //The height of the bullet.
    int height;
    //Current value of its life.
    int life;
    int shotperiod;
    bullet(const int &t,const int &wid,const int &hei,const int &Pow,const double &X=0,const double &Y=0,const int &Life=0,
           const double &Vx=0,const double &Vy=0,const double &th=0)
           :type(t),width(wid),height(hei),powerofcollide(Pow),pos(PointD(X,Y)),life(Life),speed(PointD(Vx,Vy)),theta(th)
           {
                radius=std::min(width*1.0/2,height*1.0/2);center.x=pos.x+width/2;center.y=pos.y+height/2;
           }
};


struct plane
{
    PointD pos;
    PointD speed;
    PointD center;
    double theta;
    double radius;
    //Number of its bullet in one shot.
    int bulletnum;
    int powerofcollide;
    int width;
    int height;
    //Type of its image.
    int  type;
    int life;
    //The period for its'trembling after being shot.
    int shotperiod;
	int period;
    plane(const int &t,const int &wid,const int &hei,const int &Pow=100,const double &X=0,const double &Y=0,const int &Life=0,const int &P=0,
           const double &Vx=0,const double &Vy=0,const double &th=0,const int &n=1)
           :type(t),width(wid),height(hei),powerofcollide(Pow),pos(PointD(X,Y)),life(Life),period(P),speed(PointD(Vx,Vy)),theta(th),bulletnum(n)
           {
              radius=std::min(width*1.0/2,height*1.0/2);shotperiod=0;
              center.x=pos.x+width/2;center.y=pos.y+height/2;
           }
    plane(){};
};




struct bossplane
{
    PointD pos;
    PointD speed;
    PointD center;
    double theta;
    double radius;
    int bulletnum;
    int powerofcollide;
    int width;
    int height;
    int  type;//加载图像类别
    int life;//存储生命值
    int shotperiod;//记录撞击效果的时间
	int period;//存储在游戏中已存活的时间
    bossplane(const int &t,const int &wid,const int &hei,const int &Pow=100,const double &X=0,const double &Y=0,const int &Life=0,const int &P=0,
           const double &Vx=0,const double &Vy=0,const double &th=0,const int &n=1)
           :type(t),width(wid),height(hei),powerofcollide(Pow),pos(PointD(X,Y)),life(Life),period(P),speed(PointD(Vx,Vy)),theta(th),bulletnum(n)
           {
              radius=std::min(width*1.0/2,height*1.0/2);shotperiod=0;
              center.x=pos.x+width/2;center.y=pos.y+height/2;
           }
    bossplane(){};
};




struct playerplane
{
    PointD pos;
    PointD center;
    PointD speed;
    double theta;
    double radius;
    int bulletnum;
    int powerofcollide;
    int width;
    int height;
    int type;//加载图像类别
    int properiod;
    int bulletperiod;//记录加子弹的时间
    int shotperiod;//记录撞击效果的时间
    int thunder;//大招时间记时
    int life;//存储生命值
	int period;//存储在游戏中已存活的时间
    playerplane(const int &t,const int &wid,const int &hei,const int &Pow=100,const double &X=0,const double &Y=0,const int &Life=0,const int &P=0,
           const double &Vx=0,const double &Vy=0,const double &th=0,const int &n=1)
           :type(t),width(wid),height(hei),powerofcollide(Pow),pos(PointD(X,Y)),life(Life),period(P),speed(PointD(Vx,Vy)),theta(th),bulletnum(n)
           {
              radius=std::min(width*1.0/2,height*1.0/2);shotperiod=0;
              center.x=pos.x+width/2;center.y=pos.y+height/2;
              properiod=100;
              thunder=0;
              bulletperiod=0;
           }
    playerplane(){};
};


//碰撞效果对象
struct flower
{
    PointD pos;
    PointD center;
    int  type;//加载图像类别
    int maxtime;//最大存活时间
    int life;//已经存在的时间
    int width;
    int height;
    flower(const int &t,const int &wid,const int &hei,const double &X=0,const double &Y=0,const int maxt=23)
           :type(t),width(wid),height(hei),pos(PointD(X,Y)),maxtime(maxt){life=1;
            center.x=pos.x+width/2;center.y=pos.y+height/2;}
    flower(){};
};

struct object
{
    PointD pos;
    PointD speed;
    int  type;//加载图像类别 1加生命，2加子弹
    int width;
    PointD center;
    int height;
    double radius;
    object(const int &t,const int &wid,const int &hei,const double &X=0,const double &Y=0)
           :type(t),width(wid),height(hei),pos(PointD(X,Y))
           {center.x=pos.x+width/2;center.y=pos.y+height/2; speed.y=1;
           radius=std::min(width*1.0/2,height*1.0/2);}
    object(){};
};




double cross( const PointD &a, const PointD &b );
double dot( const PointD &a, const PointD &b );
#endif

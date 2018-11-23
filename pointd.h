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
	double radius;
	double length() const
	{
		return sqrt(x*x+y*y);
	}
	PointD& rotate( double rad )
	{
		double a,b;
		a = x;
		b = y;
		x = a*cos(rad) - b*sin(rad);
		y = a*sin(rad) + b*cos(rad);
		return *this;
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

struct bullet
{
    PointD pos;
    PointD speed;
    double theta;
    double radius;
    int mass;
    int powerofcollide;
    int  type;//加载图像类别
    Image *picture;//指向加载图像的指针；
    int width;
    int height;
    int life;//存储生命值
    int shotperiod;//记录撞击效果的时间
	int period;//存储在游戏中已存活的时间
    bullet(const int &t,const int &wid,const int &hei,const int &Pow,const double &X=0,const double &Y=0,const int &Life=0,const int &P=0,
           const double &Vx=0,const double &Vy=0,const double &th=0,const int &M=1)
           :type(t),width(wid),height(hei),powerofcollide(Pow),pos(PointD(X,Y)),life(Life),period(P),speed(PointD(Vx,Vy)),theta(th),mass(M)
           {
                radius=std::min(width*1.0/2,height*1.0/2);
           }
    bullet(){};
};

struct plane
{
    PointD pos;
    PointD speed;
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
    plane(const int &t,const int &wid,const int &hei,const int &Pow=100,const double &X=0,const double &Y=0,const int &Life=0,const int &P=0,
           const double &Vx=0,const double &Vy=0,const double &th=0,const int &n=1)
           :type(t),width(wid),height(hei),powerofcollide(Pow),pos(PointD(X,Y)),life(Life),period(P),speed(PointD(Vx,Vy)),theta(th),bulletnum(n)
           {
              radius=std::min(width*1.0/2,height*1.0/2);shotperiod=0;
           }
    plane(){};
};




struct playerplane
{
    PointD pos;
    PointD speed;
    double theta;
    double radius;
    int bulletnum;
    int powerofcollide;
    int width;
    int height;
    int  type;//加载图像类别
    int properiod;
    int shotperiod;//记录撞击效果的时间
    int thunder;//大招时间记时
    int life;//存储生命值
	int period;//存储在游戏中已存活的时间
    playerplane(const int &t,const int &wid,const int &hei,const int &Pow=100,const double &X=0,const double &Y=0,const int &Life=0,const int &P=0,
           const double &Vx=0,const double &Vy=0,const double &th=0,const int &n=1)
           :type(t),width(wid),height(hei),powerofcollide(Pow),pos(PointD(X,Y)),life(Life),period(P),speed(PointD(Vx,Vy)),theta(th),bulletnum(n)
           {
              radius=std::min(width*1.0/2,height*1.0/2);shotperiod=0;
              properiod=100;
              thunder=100;
           }
    playerplane(){};
};

//碰撞效果对象
struct flower
{
    PointD pos;
    int  type;//加载图像类别
    int maxtime;//最大存活时间
    int life;//已经存在的时间
    int width;
    int height;
    flower(const int &t,const int &wid,const int &hei,const double &X=0,const double &Y=0,const int maxt=23)
           :type(t),width(wid),height(hei),pos(PointD(X,Y)),maxtime(maxt){life=1;}
    flower(){};
};

struct object
{
    PointD pos;
    int  type;//加载图像类别 1加生命，2加攻击
    int maxtime;//最大存活时间
    int life;//已经存在的时间
    int width;
    int height;
    object(const int &t,const int &wid,const int &hei,const double &X=0,const double &Y=0,const int maxt=23)
           :type(t),width(wid),height(hei),pos(PointD(X,Y)),maxtime(maxt){life=1;}
    object(){};
};




double cross( const PointD &a, const PointD &b );
double dot( const PointD &a, const PointD &b );
#endif

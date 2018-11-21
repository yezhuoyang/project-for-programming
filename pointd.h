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
    int  type;//加载图像类别
    Image *picture;//指向加载图像的指针；
    double life;//存储生命值
	int period;//存储在游戏中已存活的时间
    bullet(const int &t,const double &X=0,const double &Y=0,const double &R=0,const double &Life=0,const double &P=0,
           const double &Vx=0,const double &Vy=0,const double &th=0)
           :type(t),pos(PointD(X,Y)),radius(R),life(Life),period(P),speed(PointD(Vx,Vy)),theta(th){}
    bullet(){};
};

struct plane
{
    PointD pos;
    PointD speed;
    double theta;
    double radius;
    int bulletnum;
    int  type;//加载图像类别
    double life;//存储生命值
	int period;//存储在游戏中已存活的时间
    plane(const int &t,const double &X=0,const double &Y=0,const double &R=0,const double &Life=0,const double &P=0,
           const double &Vx=0,const double &Vy=0,const double &th=0,const int &n=1)
           :type(t),pos(PointD(X,Y)),radius(R),life(Life),period(P),speed(PointD(Vx,Vy)),theta(th),bulletnum(n){}
    plane(){};
};


//碰撞效果对象
struct flower
{
    PointD pos;
    int  type;//加载图像类别
    int maxtime;//最大存活时间
    int life;//已经存在的时间
	int period;//存储在游戏中已存活的时间
    flower(const int &t,const double &X=0,const double &Y=0,const int maxt=23,const double &Life=1)
           :type(t),pos(PointD(X,Y)),maxtime(maxt),life(Life){}
    flower(){};
};




double cross( const PointD &a, const PointD &b );
double dot( const PointD &a, const PointD &b );
#endif

/*
 * Coded by Jingxiao Chen.
 *
 */

#ifndef POINTD_HPP
#define POINTD_HPP
#include <cmath>

struct PointD
{
	PointD( const double &X=0, const double &Y=0 )
		:x(X),y(Y){}
	double x,y;
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

//对象类结构体
struct object
{
    PointD pos;
    double life;//存储生命值
	int period;//存储在游戏中已存活的时间
    object( const double &X=0,const double &Y=0,const double &Life=0,const double &P=0)
		:pos(PointD(X,Y)),life(Life),period(P){}
};



double cross( const PointD &a, const PointD &b );
double dot( const PointD &a, const PointD &b );
#endif

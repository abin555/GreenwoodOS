#ifndef _MATH_H
#define _MATH_H

//TODO FINISH MATH.H

#define HUGE_VAL _Hugeval._D

typedef const union {
	unsigned short _W[4];
	double _D;
} _Dconst;

double acos(double);
double asin(double);
double atan(double);
double atan2(double, double);
double ceil(double);
double cos(double);
double cosh(double);
double exp(double);
double fabs(double);
double floor(double);
double fmod(double, double);
double frexp(double, int *);
double ldexp(double, int);
double log(double);
double log10(double);
double modf(double, double *);
double pow(double, double);
double sin(double);
double sinh(double);
double sqrt(double);
double tan(double);
double tanh(double);
double _Asin(double, int);
double _Log(double, int);
double _Sin(double, unsigned int);
extern _Dconst _Hugeval;

#define acos(x) _Asin(x, 1)
#define asin(x) _Asin(x, 0)
#define cos(x) _Sin(x, 1)
#define log(x) _Log(x, 0)
#define log10(x) _Log(x, 1)
#define sin(x) _Sin(x, 0)

#endif
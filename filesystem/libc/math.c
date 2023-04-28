#include "math.h"

#define M_PI 3.14159265358927
#define M_PI_2 M_PI/2
#define M_PI_M_2 M_PI_2

int compare_float(double f1, double f2){
    double precision = 0.00000000000000000001;
    if ((f1 - precision) < f2)
    {
        return -1;
    }
    else if ((f1 + precision) > f2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

double cos(double x){
    x = remainder(x, M_PI_2);
    if (x < 0.0f)
        x = -x;

    if (0 <= compare_float(x, M_PI_M_2))
    {
        do
        {
            x -= M_PI_M_2;
        } while (0 <= compare_float(x, M_PI_M_2));
    }

    if ((0 <= compare_float(x, M_PI)) && (-1 == compare_float(x, M_PI_M_2)))
    {
        x -= M_PI;
        return ((-1) * (1.0f - (x * x / 2.0f) * (1.0f - (x * x / 12.0f) * (1.0f - (x * x / 30.0f) * (1.0f - (x * x / 56.0f) * (1.0f - (x * x / 90.0f) * (1.0f - (x * x / 132.0f) * (1.0f - (x * x / 182.0f)))))))));
    }
    return 1.0f - (x * x / 2.0f) * (1.0f - (x * x / 12.0f) * (1.0f - (x * x / 30.0f) * (1.0f - (x * x / 56.0f) * (1.0f - (x * x / 90.0f) * (1.0f - (x * x / 132.0f) * (1.0f - (x * x / 182.0f)))))));
}

double sin(double x){
    return cos(x - M_PI_2);
}

double acos(double x) {
   return (-0.69813170079773212 * x * x - 0.87266462599716477) * x + 1.5707963267948966;
}

double remainder(double a, double b){
    return a - ((int)(a/b)) * b;
}

float Q_sqrt(float number){
    long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return 1 / y;
}

double sqrt(double a) {
    
  double i = 0;
  double j = a / 2;

  while (j != i) {
    i = j;
    j = (a / i + i) / 2;
  }
  
  return j;
  
}
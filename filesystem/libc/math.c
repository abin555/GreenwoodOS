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

double remainder(double a, double b){
    return a - ((int)(a/b)) * b;
}
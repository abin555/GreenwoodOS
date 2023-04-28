#include "math.h"

#define LN10 2.3025850929940456840179914546844

double log10(double x){
    return ln(x) / LN10;
}

double ln(double x){
    double old_sum = 0.0;
    double xmlxpl = (x - 1) / (x + 1);
    double xmlxpl_2 = xmlxpl * xmlxpl;
    double denom = 1.0;
    double frac = xmlxpl;
    double term = frac;                 // denom start from 1.0
    double sum = term;

    while ( sum != old_sum )
    {
        old_sum = sum;
        denom += 2.0;
        frac *= xmlxpl_2;
        sum += frac / denom;
    }
    return 2.0 * sum;
}

int abs(int data){
    return (data < 0 ? -1 * data : data);
}
int fabs(double data){
    return (data < 0 ? -1 *data : data);
}
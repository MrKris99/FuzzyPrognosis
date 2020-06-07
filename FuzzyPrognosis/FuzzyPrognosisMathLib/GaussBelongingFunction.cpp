#include "GaussBelongingFunction.h"

fuzzyPrognosisMath::GaussBelongingFunction::GaussBelongingFunction(double b, double c)
    : BelongingFunction(2)
{
    SetParam(0,b);
    SetParam(1,c);
}

double fuzzyPrognosisMath::GaussBelongingFunction::GetDerivedByParam(size_t param, double x) const
{
    if (param >= 2)
    {
        throw std::runtime_error("Invalid parameter.");
    }

    double c = GetParam(1);
    double c2 = c*c;//c^2
    double x_b = x - GetParam(0); //x-b
    double x_b2 = x_b*x_b;//(x-b)^2
    double sqrt_denom = c2+x_b2;//c^2+(x-b)^2
    double denom = sqrt_denom * sqrt_denom; //(c^2+(x-b)^2)^2

    if(param == 0)
    {
        //derived by b
        return 2*c2*x_b/denom;
    }
    else
    {
        //derived by c
        return 2*c*x_b2/denom;
    }
}

double fuzzyPrognosisMath::GaussBelongingFunction::GetBelogningMeasure(double x) const
{
    const double tmp = (x - GetParam(0)) / GetParam(1); // (x - b) / c
    return 1.0 / (1.0 + tmp * tmp);
}

double fuzzyPrognosisMath::GaussBelongingFunction::GetMaxValue() const
{
    return GetParam(0);  //b
}



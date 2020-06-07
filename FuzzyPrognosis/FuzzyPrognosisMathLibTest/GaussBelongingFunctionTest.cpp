#include <gtest/gtest.h>
#include "GaussBelongingFunction.h"

TEST(GaussBelongingFunctionTest, MaxValueIsB)
{
    fuzzyPrognosisMath::GaussBelongingFunction gauss(100, 200);
    ASSERT_EQ(gauss.GetMaxValue(), 100);
    gauss.SetParam(0, 150);
    ASSERT_EQ(gauss.GetMaxValue(), 150);
}

TEST(GaussBelongingFunctionTest, ReturnsCorrectMeasure)
{
    fuzzyPrognosisMath::GaussBelongingFunction gauss(10, 20);
    ASSERT_DOUBLE_EQ(gauss.GetBelogningMeasure(15), 1.0 / (1.0 + std::pow(5.0 / 20, 2)));

    gauss.SetParam(1, 40);
    ASSERT_DOUBLE_EQ(gauss.GetBelogningMeasure(15), 1.0 / (1.0 + std::pow(5.0 / 40, 2)));
}

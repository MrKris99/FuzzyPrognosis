#include <gtest/gtest.h>
#include "GaussBelongingFunction.h"

TEST(GaussBelongingFunctionTest, MaxValueIsB)
{
    fuzzyPrognosisMath::GaussBelongingFunction gauss(100, 200);
    ASSERT_EQ(gauss.GetMaxValue(), 100);
    gauss.SetParam(0, 150);
    ASSERT_EQ(gauss.GetMaxValue(), 150);
}

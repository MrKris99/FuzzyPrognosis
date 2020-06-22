#pragma once
#include <vector>

namespace fuzzyPrognosisMath
{
    class PeriodDependencies
    {
    public:
        explicit PeriodDependencies(size_t nVarCount = 1);
        void SetVarCount(size_t nVarCount);
        size_t GetVarCount() const;
        size_t GetVarOffset(size_t nVar) const;
        void SetVarOffset(size_t nVar, size_t nOffset);
        size_t GetMaxOffset() const;

    public:
        std::vector<size_t> m_varConvertions;
    };
}

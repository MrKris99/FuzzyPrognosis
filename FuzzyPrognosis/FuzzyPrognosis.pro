TEMPLATE = subdirs
SUBDIRS += FuzzyPrognosisGUI \
    FuzzyModelSerializationLib
SUBDIRS += FuzzyPrognosisMathLib
SUBDIRS += FuzzyPrognosisMathLibTest

FuzzyPrognosisMathLibTest.depends = FuzzyPrognosisMathLib

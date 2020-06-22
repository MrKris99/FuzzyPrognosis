TEMPLATE = subdirs
SUBDIRS += \
    FuzzyPrognosisGUI \
    FuzzyModelSerializationLib \
    FuzzyPrognosisMathLib \
    FuzzyPrognosisMathLibTest

FuzzyPrognosisMathLibTest.depends = FuzzyPrognosisMathLib
FuzzyPrognosisGUI.depends = FuzzyPrognosisMathLib FuzzyModelSerializationLib

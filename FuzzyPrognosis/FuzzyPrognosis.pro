TEMPLATE = subdirs
SUBDIRS += FuzzyPrognosisGUI
SUBDIRS += FuzzyPrognosisMathLib
SUBDIRS += FuzzyPrognosisMathLibTest

FuzzyPrognosisMathLibTest.depends = FuzzyPrognosisMathLib

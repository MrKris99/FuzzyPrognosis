TEMPLATE = app
CONFIG += console conan_basic_setup
include(../conanbuildinfo.pri)
SOURCES += \
    GaussBelongingFunctionTest.cpp \
    main.cpp

CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FuzzyPrognosisMathLib/release/ -lFuzzyPrognosisMathLib
else: LIBS += -L$$OUT_PWD/../FuzzyPrognosisMathLib/debug/ -lFuzzyPrognosisMathLib

INCLUDEPATH += $$PWD/../FuzzyPrognosisMathLib

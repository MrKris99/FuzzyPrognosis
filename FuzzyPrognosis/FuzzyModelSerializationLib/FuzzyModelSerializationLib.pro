TEMPLATE = lib
CONFIG += staticlib conan_basic_setup
include(../conanbuildinfo.pri)

CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FuzzyPrognosisMathLib/release/ -lFuzzyPrognosisMathLib
else: LIBS += -L$$OUT_PWD/../FuzzyPrognosisMathLib/debug/ -lFuzzyPrognosisMathLib

SOURCES += \
    FuzzyDoc.cpp

HEADERS += \
    FuzzyDoc.h

INCLUDEPATH += $$PWD/../FuzzyPrognosisMathLib

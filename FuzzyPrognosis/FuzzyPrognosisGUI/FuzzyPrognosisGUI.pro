QT += core gui widgets charts
CONFIG += conan_basic_setup
include(../conanbuildinfo.pri)

SOURCES += \
    DependenciesDialog.cpp \
    ModelTuningWorkerThread.cpp \
    TermsTableModel.cpp \
    TermsTableWidget.cpp \
    TrendTableModel.cpp \
    TrendTableWidget.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    DependenciesDialog.h \
    MainWindow.h \
    ModelTuningWorkerThread.h \
    TermsTableModel.h \
    TermsTableWidget.h \
    TrendTableModel.h \
    TrendTableWidget.h

FORMS += \
    DependenciesDialog.ui \
    MainWindow.ui \
    TermsTableWidget.ui \
    TrendTableWidget.ui

TRANSLATIONS += \
    FuzzyPrognosis_uk_UA.ts

CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FuzzyPrognosisMathLib/release/ -lFuzzyPrognosisMathLib
else: LIBS += -L$$OUT_PWD/../FuzzyPrognosisMathLib/debug/ -lFuzzyPrognosisMathLib

CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FuzzyModelSerializationLib/release/ -lFuzzyModelSerializationLib
else: LIBS += -L$$OUT_PWD/../FuzzyModelSerializationLib/debug/ -lFuzzyModelSerializationLib

INCLUDEPATH += $$PWD/../FuzzyPrognosisMathLib
INCLUDEPATH += $$PWD/../FuzzyModelSerializationLib

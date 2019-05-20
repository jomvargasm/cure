TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        cure/cureclusterdata.cpp \
        cure/curetest.cpp \
        cureAlgorithm/curealgorithm.cpp \
        cureAlgorithm/cureclustermodel.cpp \
        cureAlgorithm/curemanager.cpp \
        files/csvdatareader.cpp \
        files/filestest.cpp \
        files/utils.cpp \
        kmeans/kmeanscluster.cpp \
        main.cpp

INCLUDEPATH += C:\Armadillo\armadillo-9.400.3\include

LIBS += \
    -LC:\Armadillo\armadillo-9.400.3\examples\lib_win64 \
    -llapack_win64_MT  \
    -lblas_win64_MT
    # -larmadillo \
    # -llapack \
    # -lblas

HEADERS += \
    cure/cureclusterdata.h \
    cure/curetest.h \
    cureAlgorithm/curealgorithm.h \
    cureAlgorithm/cureclustermodel.h \
    cureAlgorithm/curemanager.h \
    files/csvdatareader.h \
    files/filestest.h \
    files/utils.h \
    kmeans/kmeanscluster.h

DISTFILES += \
    files/data_test.csv

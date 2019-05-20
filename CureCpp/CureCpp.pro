TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        cure/curecluster.cpp \
        cure/curetest.cpp \
        cureAlgorithm/curealgorithm.cpp \
        cureAlgorithm/cureclustermodel.cpp \
        files/csvdatareader.cpp \
        files/filestest.cpp \
        files/utils.cpp \
        kmeans/kmeanscluster.cpp \
        main.cpp

INCLUDEPATH += C:\Armadillo\armadillo-9.400.3\include

LIBS += \
    # -LC:\Armadillo\armadillo-9.400.3\examples\lib_win64 \ # windows only
    # -llapack_win64_MT  \
    # -lblas_win64_MT
    # linux
    -larmadillo \
    -lopenblas \
    -llapack

HEADERS += \
    cure/curecluster.h \
    cure/curetest.h \
    cureAlgorithm/curealgorithm.h \
    cureAlgorithm/cureclustermodel.h \
    files/csvdatareader.h \
    files/filestest.h \
    files/utils.h \
    kmeans/kmeanscluster.h

DISTFILES += \
    files/data_test.csv

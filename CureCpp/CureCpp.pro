QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


CONFIG += console c++11
CONFIG += app_bundle
CONFIG += qt

SOURCES += \
        cure/cureclusterdata.cpp \
        cure/curetest.cpp \
        cureAlgorithm/curealgorithm.cpp \
        cureAlgorithm/cureclustermodel.cpp \
        cureAlgorithm/curemanager.cpp \
        cureui.cpp \
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
    cureui.h \
    files/csvdatareader.h \
    files/filestest.h \
    files/utils.h \
    kmeans/kmeanscluster.h

DISTFILES += \
    files/data_test.csv

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


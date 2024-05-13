CONFIG	    += console 
CONFIG	    += release

TEMPLATE      = app

TARGET = cdoan1

QT += widgets


HEADERS       = ..\..\..\qtsrc\key_filter.h \
                ..\..\..\qtsrc\MainWindow.h 

LIBS += "D:\MRZV_HOME\Xtended\dnp3\CODE_opendnp3-mrzs\opendnp3-mrzs\dll\libqt_dnp3.a"

include(../../../include_tests.pri)

SOURCES      += MainWindow.cpp \
                main.cpp \
                \
                dnpdataMrzs.cpp \
                timesliceMrzs.cpp \
                serialMrzs.cpp \
                dnpMrzs.cpp \
                utilitiesMrzs.cpp \
                commMrzs.cpp
#                \
#                txtLogEntry.cpp

#    onReadInputPacketEx.cpp \

DESTDIR = d:\QT_5.12\Examples\release_programs



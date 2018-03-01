QT += widgets
DEFINES += QWT_DLL
TEMPLATE = app
FORMS = analog_io.ui \
    digital_io.ui \
    eeprom.ui \
    hardware_test.ui \
    qtdemo.ui \
    secure.ui
HEADERS = application.h \
    analog_io.h \
    qtdemo.h \
    digital_io.h \
    eeprom.h \
    plot.h \
    hardware_test.h \
    secure.h
SOURCES = application.cpp \
    main.cpp \
    qtdemo.cpp \
    analog_io.cpp \
    digital_io.cpp \
    eeprom.cpp \
    plot.cpp \
    hardware_test.cpp \
    secure.cpp
LIBS += avaspec.lib \
    qwt.lib          

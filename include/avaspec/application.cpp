#include <QtGui>
#include "application.h"
#include "avaspec.h"
#include "qtdemo.h"

application::application(int &argc, char *argv[])
	: QApplication(argc, argv)
{
    setStyle("plastique");
}

void application::emitsignal(int WParam, int LParam)
{
    emit DataIsHere(WParam, LParam);
}

void application::callback(AvsHandle *handle, int *result )
{
    if (*result >= 0) {
    application *m = static_cast<application*>(qApp);
    // qApp is a predefined pointer to the QApplication instance
    if (m != NULL)
        m->emitsignal(*result, *handle); 
    }
}

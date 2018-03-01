#ifndef APPLICATION_H
#define APPLICATION_H

#include <QtWidgets\QApplication>
#include "avaspec.h"

class application : public QApplication
{
    Q_OBJECT

public:
    application(int &argc, char *argv[]);
    void emitsignal(int WParam, int LParam);
    static void callback(AvsHandle *handle, int *result );
    
signals:
    void DataIsHere(int WParam, int LParam);
	
};

#endif // APPLICATION_H

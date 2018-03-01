#ifndef HARDWARE_TEST_H
#define HARDWARE_TEST_H

#include <QtWidgets\QMainWindow>
#include "ui_hardware_test.h"

class hardware_test : public QDialog
{
	Q_OBJECT

public:
	hardware_test(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~hardware_test();

protected:

private:
	Ui::hardware_test ui;

private slots:


public slots:


};

#endif // HARDWARE_TEST_H

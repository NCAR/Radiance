#ifndef ANALOG_IO_H
#define ANALOG_IO_H

#include <QtWidgets\QMainWindow>
#include "ui_analog_io.h"

class analog_io : public QDialog
{
	Q_OBJECT

public:
	analog_io(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~analog_io();

protected:

private:
	Ui::analog_io ui;

private slots:
	void on_GetOnboardAIBtn_clicked();
	void on_GetAnalogInBtn_clicked();
	void on_SetAnalogOutBtn_clicked();


public slots:


};

#endif // ANALOG_IO_H

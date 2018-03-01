#ifndef DIGITAL_IO_H
#define DIGITAL_IO_H

#include <QtWidgets\QMainWindow>
#include "ui_digital_io.h"

class digital_io : public QDialog
{
	Q_OBJECT

public:
	digital_io(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~digital_io();

protected:

private:
	Ui::digital_io ui;
	void OutputClick(unsigned char a_Id, bool a_Value);

private slots:
	void on_DO10Chk_stateChanged(int);
	void on_DO9Chk_stateChanged(int);
	void on_DO8Chk_stateChanged(int);
	void on_PWM7Chk_stateChanged(int);
	void on_DO7Chk_stateChanged(int);
	void on_PWM6Chk_stateChanged(int);
	void on_DO6Chk_stateChanged(int);
	void on_PWM5Chk_stateChanged(int);
	void on_DO5Chk_stateChanged(int);
	void on_DO4Chk_stateChanged(int);
	void on_PWM3Chk_stateChanged(int);
	void on_DO3Chk_stateChanged(int);
	void on_PWM2Chk_stateChanged(int);
	void on_DO2Chk_stateChanged(int);
	void on_PWM1Chk_stateChanged(int);
	void on_DO1Chk_stateChanged(int);
	void on_GetDigitalInputsBtn_clicked();


public slots:


};

#endif // DIGITAL_IO_H

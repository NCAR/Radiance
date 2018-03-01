#ifndef QTDEMO_H
#define QTDEMO_H

#include <QtWidgets\QMainWindow>
#include "ui_qtdemo.h"

class Qtdemo : public QMainWindow
{
	Q_OBJECT

public:
	Qtdemo(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~Qtdemo();
	void ConnectGui();
	void DisconnectGui();
	void UpdateButtons();

private:
	Ui::QtdemoClass ui;

private slots:
	void on_DigitalIoBtn_clicked();
	void on_AnalogIoBtn_clicked();
	void on_WriteEepromBtn_clicked();
	void on_DisableGraphChk_stateChanged(int);
	void on_DeactivateBtn_clicked();
	void on_ReadEepromBtn_clicked();
	void on_ActivateBtn_clicked();
	void on_UpdateListBtn_clicked();
	void on_StopMeasBtn_clicked();
	void on_StartMeasBtn_clicked();
	void on_ShowEepromBtn_clicked();
    void on_ShowSecureBtn_clicked();
	void on_OpenCommBtn_clicked();
    void on_HW_TestBtn_clicked();

public slots:
	void ReceiveDataIsHere(int WParam, int LParam);
	// avoid a name starting with "on_" here, otherwise you will get a
	// warning, as there is no defined signal, just an emit in "application.cpp"

};

#endif // QTDEMO_H

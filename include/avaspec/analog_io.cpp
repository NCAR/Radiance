#include <QtGui>
#include <QtWidgets\QApplication>
#include <QtWidgets\QMessageBox>
#include <QtWidgets\QPushButton>
#include "qtdemo.h"
#include "analog_io.h"
#include "avaspec.h"
#include "application.h"


analog_io::analog_io(QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	ui.AO1Edt->setText(QString("%1").arg(0.0,4,'f',2));
	ui.AO2Edt->setText(QString("%1").arg(0.0,4,'f',2)); 
}

analog_io::~analog_io()
{

}

void analog_io::on_SetAnalogOutBtn_clicked()
{
	#define NR_OF_ANALOG_OUTPUTS     2
	float l_Value[NR_OF_ANALOG_OUTPUTS];
	extern long m_DeviceHandle;
	bool l_bNoError = true;
	bool ok;

	l_Value[0] = (float) QString(ui.AO1Edt->text()).toDouble(&ok);
	l_Value[1] = (float) QString(ui.AO2Edt->text()).toDouble(&ok);
	for (int i = 0; (i < NR_OF_ANALOG_OUTPUTS) && l_bNoError; ++i)
	{
		int l_Res = AVS_SetAnalogOut(m_DeviceHandle, i, l_Value[i]);
		if (l_Res != ERR_SUCCESS)
		{
			l_bNoError = false;
			QMessageBox::critical(this, tr("QT4 Demo"),
				tr("AVS_SetAnalogOut failed, code %1").arg(l_Res),
                QMessageBox::Ok, QMessageBox::Ok);
		}
    }
}

void analog_io::on_GetAnalogInBtn_clicked()
{
	#define NR_OF_ANALOG_INPUTS     2
	float l_Value[NR_OF_ANALOG_INPUTS];
	extern long m_DeviceHandle;
        int l_Res = AVS_GetAnalogIn(m_DeviceHandle, 5, &l_Value[0]);
        if (l_Res != ERR_SUCCESS)
        {
                QMessageBox::critical(this, tr("QT4 Demo"),
                        tr("AVS_GetAnalogIn failed, ID=5 (pin 9): code %1").arg(l_Res),
                        QMessageBox::Ok, QMessageBox::Ok);
        }
        else
                ui.AI1Edt->setText(QString("%1").arg(l_Value[0],4,'f',2));
        l_Res = AVS_GetAnalogIn(m_DeviceHandle, 4, &l_Value[1]);
        if (l_Res != ERR_SUCCESS)
        {
                QMessageBox::critical(this, tr("QT4 Demo"),
                        tr("AVS_GetAnalogIn failed, ID=4 (pin 18): code %1").arg(l_Res),
                        QMessageBox::Ok, QMessageBox::Ok);
        }
        else
                ui.AI2Edt->setText(QString("%1").arg(l_Value[1],4,'f',2));

}

void analog_io::on_GetOnboardAIBtn_clicked()
{
	float l_Value;
	extern long m_DeviceHandle;
	int l_Res = AVS_GetAnalogIn(m_DeviceHandle, 0, &l_Value);
	switch (l_Res) {
	case ERR_SUCCESS:
		ui.ThermistorX11Edt->setText(QString("%1").arg(l_Value, 4, 'f', 2));
		break;
	case ERR_INVALID_PARAMETER:
		ui.ThermistorX11Edt->setText("Not Available");
		break;
	default:
		QMessageBox::critical(this, tr("QT4 Demo"),
			tr("AVS_GetAnalogIn failed, ID=0 code %1").arg(l_Res),
			QMessageBox::Ok, QMessageBox::Ok);
		break;
	}

	l_Res = AVS_GetAnalogIn(m_DeviceHandle, 1, &l_Value);
	switch (l_Res) {
	case ERR_SUCCESS:
		ui.E1V2Edt->setText(QString("%1").arg(l_Value, 4, 'f', 2));
		break;
	case ERR_INVALID_PARAMETER:
		ui.E1V2Edt->setText("Not Available");
		break;
	default:
		QMessageBox::critical(this, tr("QT4 Demo"),
			tr("AVS_GetAnalogIn failed, ID=1 code %1").arg(l_Res),
			QMessageBox::Ok, QMessageBox::Ok);
		break;
	}

	l_Res = AVS_GetAnalogIn(m_DeviceHandle, 2, &l_Value);
	switch (l_Res) {
	case ERR_SUCCESS:
		ui.E5VIOEdt->setText(QString("%1").arg(l_Value, 4, 'f', 2));
		break;
	case ERR_INVALID_PARAMETER:
		ui.E5VIOEdt->setText("Not Available");
		break;
	default:
		QMessageBox::critical(this, tr("QT4 Demo"),
			tr("AVS_GetAnalogIn failed, ID=2 code %1").arg(l_Res),
			QMessageBox::Ok, QMessageBox::Ok);
		break;
	}

	l_Res = AVS_GetAnalogIn(m_DeviceHandle, 3, &l_Value);
	switch (l_Res) {
	case ERR_SUCCESS:
		ui.E5VUSBEdt->setText(QString("%1").arg(l_Value, 4, 'f', 2));
		break;
	case ERR_INVALID_PARAMETER:
		ui.E5VUSBEdt->setText("Not Available");
		break;
	default:
		QMessageBox::critical(this, tr("QT4 Demo"),
			tr("AVS_GetAnalogIn failed, ID=3 code %1").arg(l_Res),
			QMessageBox::Ok, QMessageBox::Ok);
		break;

	}

	l_Res = AVS_GetAnalogIn(m_DeviceHandle, 6, &l_Value);
	switch (l_Res) {
	case ERR_SUCCESS:
		ui.NTC1X8Edt->setText(QString("%1").arg(l_Value, 4, 'f', 2));
		break;
	case ERR_INVALID_PARAMETER:
		ui.NTC1X8Edt->setText("Not Available");
		break;
	default:
		QMessageBox::critical(this, tr("QT4 Demo"),
			tr("AVS_GetAnalogIn failed, ID=6 code %1").arg(l_Res),
			QMessageBox::Ok, QMessageBox::Ok);
		break;
	}

	l_Res = AVS_GetAnalogIn(m_DeviceHandle, 7, &l_Value);
	switch (l_Res) {
	case ERR_SUCCESS:
		ui.NTC2X9Edt->setText(QString("%1").arg(l_Value, 4, 'f', 2));
		break;
	case ERR_INVALID_PARAMETER:
		ui.NTC2X9Edt->setText("Not Available");
		break;
	default:
		QMessageBox::critical(this, tr("QT4 Demo"),
			tr("AVS_GetAnalogIn failed, ID=7 code %1").arg(l_Res),
			QMessageBox::Ok, QMessageBox::Ok);
		break;
	}
}

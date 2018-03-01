#include <QtGui>
#include <QtWidgets\QApplication>
#include <QtWidgets\QMessageBox>
#include <QtWidgets\QPushButton>
#include "digital_io.h"
#include "avaspec.h"
#include "application.h"

digital_io::digital_io(QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
	ui.DutyCycle1Edt->setText(QString("%1").arg(50));
	ui.DutyCycle2Edt->setText(QString("%1").arg(50));
	ui.DutyCycle3Edt->setText(QString("%1").arg(50));
	ui.DutyCycle5Edt->setText(QString("%1").arg(50));
	ui.DutyCycle6Edt->setText(QString("%1").arg(50));
	ui.DutyCycle7Edt->setText(QString("%1").arg(50));
	ui.Frequency123Edt->setText(QString("%1").arg(1000));
	ui.Frequency567Edt->setText(QString("%1").arg(1000));
}

digital_io::~digital_io()
{

}

void digital_io::OutputClick(unsigned char a_Id, bool a_Value)
{
	int l_Res;
	unsigned long l_Freq;
	unsigned char l_Perc;
	extern long m_DeviceHandle;
	bool ok;

	if (((a_Id==0)||(a_Id==1)||(a_Id==2)||(a_Id==4)||(a_Id==5)||(a_Id==6)) && (a_Value==true))
	{
		switch (a_Id)
		{
			case 0 :	if (ui.PWM1Chk->isChecked())
						{
							l_Perc = ui.DutyCycle1Edt->text().toInt(&ok); 
							l_Freq = ui.Frequency123Edt->text().toInt(&ok);
							l_Res = AVS_SetPwmOut(m_DeviceHandle, a_Id, l_Freq, l_Perc);
						}
						else
							l_Res = AVS_SetDigOut(m_DeviceHandle, a_Id, a_Value);
						break;
			case 1 :	if (ui.PWM2Chk->isChecked())
						{
							l_Perc = ui.DutyCycle2Edt->text().toInt(&ok); 
							l_Freq = ui.Frequency123Edt->text().toInt(&ok);
							l_Res = AVS_SetPwmOut(m_DeviceHandle, a_Id, l_Freq, l_Perc);
						}
						else
							l_Res = AVS_SetDigOut(m_DeviceHandle, a_Id, a_Value);
						break;
			case 2 :	if (ui.PWM3Chk->isChecked())
						{
							l_Perc = ui.DutyCycle3Edt->text().toInt(&ok); 
							l_Freq = ui.Frequency123Edt->text().toInt(&ok);
							l_Res = AVS_SetPwmOut(m_DeviceHandle, a_Id, l_Freq, l_Perc);
						}
						else
							l_Res = AVS_SetDigOut(m_DeviceHandle, a_Id, a_Value);
						break;
			case 4 :	if (ui.PWM5Chk->isChecked())
						{
							l_Perc = ui.DutyCycle5Edt->text().toInt(&ok); 
							l_Freq = ui.Frequency567Edt->text().toInt(&ok);
							l_Res = AVS_SetPwmOut(m_DeviceHandle, a_Id, l_Freq, l_Perc);
						}
						else
							l_Res = AVS_SetDigOut(m_DeviceHandle, a_Id, a_Value);
						break;
			case 5 :	if (ui.PWM6Chk->isChecked())
						{
							l_Perc = ui.DutyCycle6Edt->text().toInt(&ok); 
							l_Freq = ui.Frequency567Edt->text().toInt(&ok);
							l_Res = AVS_SetPwmOut(m_DeviceHandle, a_Id, l_Freq, l_Perc);
						}
						else
							l_Res = AVS_SetDigOut(m_DeviceHandle, a_Id, a_Value);
						break;
			case 6 :	if (ui.PWM7Chk->isChecked())
						{
							l_Perc = ui.DutyCycle7Edt->text().toInt(&ok); 
							l_Freq = ui.Frequency567Edt->text().toInt(&ok);
							l_Res = AVS_SetPwmOut(m_DeviceHandle, a_Id, l_Freq, l_Perc);
						}
						else
							l_Res = AVS_SetDigOut(m_DeviceHandle, a_Id, a_Value);
						break;
		}
	}
	else
	{
		l_Res = AVS_SetDigOut(m_DeviceHandle, a_Id, a_Value);
	}
	if (l_Res != ERR_SUCCESS)
	{
                QMessageBox::critical(this, tr("QT4 Demo"),
			tr("AVS_SetDigOut failed, code %1").arg(l_Res),
			QMessageBox::Ok, QMessageBox::Ok);
	}
}

void digital_io::on_GetDigitalInputsBtn_clicked()
{
	#define NR_OF_DIGITAL_INPUTS    3
	#define DIG_IN0                 0
	#define DIG_IN1                 1
	#define DIG_IN2                 2
	
	extern long m_DeviceHandle;
	unsigned char l_Value[NR_OF_DIGITAL_INPUTS];
	bool l_bNoError = true;

	for (int i = 0; (i < NR_OF_DIGITAL_INPUTS) && l_bNoError; ++i)
	{
		l_bNoError = (ERR_SUCCESS == AVS_GetDigIn(m_DeviceHandle, i, &l_Value[i]));
	}
	
	if (l_bNoError)
	{
		ui.DI1Chk->setEnabled(true);
		ui.DI1Chk->setChecked(l_Value[DIG_IN0] & 0x01);
		ui.DI1Chk->setEnabled(false);
		ui.DI2Chk->setEnabled(true);
		ui.DI2Chk->setChecked(l_Value[DIG_IN1] & 0x01);
		ui.DI2Chk->setEnabled(false);
		ui.DI3Chk->setEnabled(true);
		ui.DI3Chk->setChecked(l_Value[DIG_IN2] & 0x01);
		ui.DI3Chk->setEnabled(false);
	}
	else
	{
                QMessageBox::critical(this, tr("QT4 Demo"),
			tr("AVS_GetDigIn failed"),
			QMessageBox::Ok, QMessageBox::Ok);
	}
}

void digital_io::on_DO1Chk_stateChanged(int)
{
	OutputClick(0, ui.DO1Chk->isChecked());
}

void digital_io::on_PWM1Chk_stateChanged(int)
{
	OutputClick(0, ui.DO1Chk->isChecked());
}

void digital_io::on_DO2Chk_stateChanged(int)
{
	OutputClick(1, ui.DO2Chk->isChecked());
}

void digital_io::on_PWM2Chk_stateChanged(int)
{
	OutputClick(1, ui.DO2Chk->isChecked());
}

void digital_io::on_DO3Chk_stateChanged(int)
{
	OutputClick(2, ui.DO3Chk->isChecked());
}

void digital_io::on_PWM3Chk_stateChanged(int)
{
	OutputClick(2, ui.DO3Chk->isChecked());
}

void digital_io::on_DO4Chk_stateChanged(int)
{
	OutputClick(3, ui.DO4Chk->isChecked());
}

void digital_io::on_DO5Chk_stateChanged(int)
{
	OutputClick(4, ui.DO5Chk->isChecked());
}

void digital_io::on_PWM5Chk_stateChanged(int)
{
	OutputClick(4, ui.DO5Chk->isChecked());
}

void digital_io::on_DO6Chk_stateChanged(int)
{
	OutputClick(5, ui.DO6Chk->isChecked());
}

void digital_io::on_PWM6Chk_stateChanged(int)
{
	OutputClick(5, ui.DO6Chk->isChecked());
}

void digital_io::on_DO7Chk_stateChanged(int)
{
	OutputClick(6, ui.DO7Chk->isChecked());
}

void digital_io::on_PWM7Chk_stateChanged(int)
{
	OutputClick(6, ui.DO7Chk->isChecked());
}

void digital_io::on_DO8Chk_stateChanged(int)
{
	OutputClick(7, ui.DO8Chk->isChecked());
}

void digital_io::on_DO9Chk_stateChanged(int)
{
	OutputClick(8, ui.DO9Chk->isChecked());
}

void digital_io::on_DO10Chk_stateChanged(int)
{
	OutputClick(9, ui.DO10Chk->isChecked());
}

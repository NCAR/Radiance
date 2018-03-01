#include <QtGui>
#include <QtWidgets\QApplication>
#include <QtWidgets\QMessageBox>
#include <QtWidgets\QPushButton>
#include "qtdemo.h"
#include "hardware_test.h"
#include "avaspec.h"
#include "undocumented.h"
#include "application.h"

const QString result_strings[] =
{
    "Passed",
    "FAILED",
    "Wrong Hardware",
    "Not Implemented"
};

const QString enabled_strings[] =
{
    "Disabled",
    "Enabled"
};

const QString SPI_result_strings[]=
{
    "Unknown",
    "Not Busy",
    "Busy"
};

const QString QSPI_result_strings[]=
{
    "OK",
    "Programming ERROR",
    "Erase ERROR",
    "Prog & Erase ERROR"
};

void set_le_color(int status, QLineEdit *le)
// change QLineEdit backgroundcolor to red if status != 0
{
    QPalette palette;
    if (0 == status)
    {
        palette.setColor(QPalette::Base,Qt::white);
        palette.setColor(QPalette::Text,Qt::black);
    }
    else
    {
        palette.setColor(QPalette::Base,Qt::red);
        palette.setColor(QPalette::Text,Qt::black);
    }
    le->setPalette(palette);
}

hardware_test::hardware_test(QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
    extern long m_DeviceHandle;
    HWTestType testresults;
    int l_Res = AVS_HW_TestReport(m_DeviceHandle, &testresults);
    if (ERR_SUCCESS == l_Res)
    {
        set_le_color(testresults.eui24aa_TestResult,ui.eui24aa_TestResult_Edt);
        ui.eui24aa_TestResult_Edt->setText(result_strings[testresults.eui24aa_TestResult]);

        ui.eui24aa_MacAddr_Edt->setText(QString("%1:%2:%3:%4:%5:%6")
                                        .arg(testresults.eui24aa_MacAddr[0],2,16,QChar('0'))
                                        .arg(testresults.eui24aa_MacAddr[1],2,16,QChar('0'))
                                        .arg(testresults.eui24aa_MacAddr[2],2,16,QChar('0'))
                                        .arg(testresults.eui24aa_MacAddr[3],2,16,QChar('0'))
                                        .arg(testresults.eui24aa_MacAddr[4],2,16,QChar('0'))
                                        .arg(testresults.eui24aa_MacAddr[5],2,16,QChar('0')));
        ui.eui24aa_ErrorCount_Edt->setText(QString("%1").arg(testresults.eui24aa_ErrorCount));
        
        set_le_color(testresults.adp5050_TestResult,ui.adp5050_TestResult_Edt);
        ui.adp5050_TestResult_Edt->setText(result_strings[testresults.adp5050_TestResult]);
        
        ui.adp5050_Channel1Stat_Edt->setText(enabled_strings[testresults.adp5050_Channel1Stat]);
        ui.adp5050_Channel2Stat_Edt->setText(enabled_strings[testresults.adp5050_Channel2Stat]);
        ui.adp5050_Channel3Stat_Edt->setText(enabled_strings[testresults.adp5050_Channel3Stat]);
        ui.adp5050_Channel4Stat_Edt->setText(enabled_strings[testresults.adp5050_Channel4Stat]);
        ui.adp5050_ErrorCount_Edt->setText(QString("%1").arg(testresults.adp5050_ErrorCount));

        set_le_color(testresults.emc1001_TestResult,ui.emc1001_TestResult_Edt);
        ui.emc1001_TestResult_Edt->setText(result_strings[testresults.emc1001_TestResult]);

        ui.emc1001_ProductId_Edt->setText(QString("%1").arg(testresults.emc1001_ProductId,0,16));
        ui.emc1001_ManufactureId_Edt->setText(QString("%1").arg(testresults.emc1001_ManufactureId,0,16));
        ui.emc1001_RevNr_Edt->setText(QString("%1").arg(testresults.emc1001_RevNr,0,16));
        ui.emc1001_BoardTemperature_Edt->setText(QString("%1 °C").arg((testresults.emc1101_BoardTemperature / 100.0),3,'f',1));
        ui.emc1001_ErrorCount_Edt->setText(QString("%1").arg(testresults.emc1001_ErrorCount));

        set_le_color(testresults.mp25p80_TestResult,ui.mp25p80_TestResult_Edt);
        ui.mp25p80_TestResult_Edt->setText(result_strings[testresults.mp25p80_TestResult]);

        ui.mp25p80_DevStatus_Edt->setText(SPI_result_strings[testresults.mp25p80_DevStatus]);
        ui.mp25p80_ManufacturerId_Edt->setText(QString("%1").arg(testresults.mp25p80_ManufacturerId,0,16));
        ui.mp25p80_DevIdMemoryType_Edt->setText(QString("%1").arg(testresults.mp25p80_DevIdMemoryType,0,16));
        ui.mp25p80_DevIdDensity_Edt ->setText(QString("%1").arg(testresults.mp25p80_DevIdDensity,0,16));

        set_le_color(testresults.s25fl256_TestResult,ui.s25fl256_TestResult_Edt);
        ui.s25fl256_TestResult_Edt->setText(result_strings[testresults.s25fl256_TestResult]);

        ui.s25fl256_DevStatus_Edt->setText(QSPI_result_strings[testresults.s25fl256_DevStatus]);
        ui.s25fl256_ManufacturerId_Edt->setText(QString("%1").arg(testresults.s25fl256_ManufacturerId,0,16));
        ui.s25fl256_DevIdMemoryType_Edt->setText(QString("%1").arg(testresults.s25fl256_DevIdMemoryType,0,16));
        ui.s25fl256_DevIdDensity_Edt ->setText(QString("%1").arg(testresults.s25fl256_DevIdDensity,0,16));

        set_le_color(testresults.xadc_TestResult,ui.xadc_TestResult_Edt);
        ui.xadc_TestResult_Edt->setText(result_strings[testresults.xadc_TestResult]);

        ui.xadc_NtcValue_Edt->setText(QString("%1 mV").arg(testresults.xadc_NtcValue)); 

        set_le_color(testresults.gic_TestResult,ui.gic_TestResult_Edt);
        ui.gic_TestResult_Edt->setText(result_strings[testresults.gic_TestResult]);

        set_le_color(testresults.xgpioFpga_TestResult,ui.xgpioFpga_TestResult_Edt);
        ui.xgpioFpga_TestResult_Edt->setText(result_strings[testresults.xgpioFpga_TestResult]);

        set_le_color(testresults.xgpioCortex_TestResult,ui.xgpioCortex_TestResult_Edt);
        ui.xgpioCortex_TestResult_Edt->setText(result_strings[testresults.xgpioCortex_TestResult]);
    }
}

hardware_test::~hardware_test()
{

}


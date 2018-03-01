#include <QtGui>
#include <QtWidgets\QApplication>
#include <QtWidgets\QMessageBox>
#include <QtWidgets\QPushButton>
#include "secure.h"
#include "avaspec.h"
#include "undocumented.h"

secure::secure(QWidget *parent, Qt::WindowFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);
}

secure::~secure()
{

}

void secure::showEvent(QShowEvent * /* event */)
{
    SecureType sectype;
    extern AvsIdentityType m_Identity;
    int l_Res = AVS_GetSecureConfig((AvsIdentityType*) &m_Identity, &sectype);
    if (l_Res==0) {
        if (sectype.m_aSwVersion[1]==255) sectype.m_aSwVersion[1]=0; // patch for uninitialized eeprom of Mini
        if (sectype.m_aSwVersion[2]==255) sectype.m_aSwVersion[2]=0;
        if (sectype.m_aSwVersion[3]==255) sectype.m_aSwVersion[3]=0;
        QByteArray version=QByteArray::fromRawData((char*)sectype.m_aSwVersion,4);
        ui.VersionEdt->setText(version);
        QByteArray serial=QByteArray::fromRawData((char*)sectype.m_aSerialId,9);
        ui.SerialEdt->setText(serial);
        QByteArray timestamp=QByteArray::fromRawData((char*)sectype.m_aTimeStamp,4);
        float stamp=*reinterpret_cast<const float*>(timestamp.data());
        ui.TimeStampEdt->setText(QString("%1").arg(stamp,10,'f',4));
        QByteArray customer=QByteArray::fromRawData((char*)sectype.m_aCustomerID,64);
        ui.CustomerEdt->setText(customer);
        ui.BasicRBtn->setChecked(sectype.m_aAppl[2] == '0');
        ui.FullRbtn->setChecked(sectype.m_aAppl[2] == '1');
        ui.NotDedRBtn->setChecked(sectype.m_aAppl[12] == '1');
        ui.DedSlaveRBtn->setChecked(sectype.m_aAppl[12] == '2');
        ui.DedMasterRBtn->setChecked(sectype.m_aAppl[12] == '3');
        ui.ColorChkB->setChecked(sectype.m_aAppl[3] == '1');
        ui.ProcessChkB->setChecked(sectype.m_aAppl[4] == '1');
        ui.IrradianceChkB->setChecked(sectype.m_aAppl[5] == '1');
        ui.OxygenChkB->setChecked(sectype.m_aAppl[6] == '1');
        ui.ExcelChkB->setChecked(sectype.m_aAppl[7] == '1');
        ui.PhotonChkB->setChecked(sectype.m_aAppl[8] == '1');
        ui.ThinFilmChkB->setChecked(sectype.m_aAppl[9] == '1');
        ui.ChemoChkB->setChecked(sectype.m_aAppl[10] == '1');
        ui.FastTChkB->setChecked(sectype.m_aAppl[11] == '1');
        ui.BlackLeftEdt->setText(QString("%1").arg(sectype.m_aNrOfBlackenedLeft[0]));
        ui.BlackRightEdt->setText(QString("%1").arg(sectype.m_aNrOfBlackenedRight[0]));
        QByteArray pval=QByteArray::fromRawData((char*)sectype.m_aTecCtrl_P,2);
        uint16 pvalword=*reinterpret_cast<const uint16*>(pval.data());
        ui.TEC_P_Edt->setText(QString("%1").arg(pvalword));
        QByteArray ival=QByteArray::fromRawData((char*)sectype.m_aTecCtrl_I,2);
        uint16 ivalword=*reinterpret_cast<const uint16*>(ival.data());
        ui.TEC_I_Edt->setText(QString("%1").arg(ivalword));
        QByteArray dval=QByteArray::fromRawData((char*)sectype.m_aTecCtrl_D,2);
        uint16 dvalword=*reinterpret_cast<const uint16*>(dval.data());
        ui.TEC_D_Edt->setText(QString("%1").arg(dvalword));
        ui.MAC_Edt->setText(QString("%1:%2:%3:%4:%5:%6")
                            .arg(sectype.m_aMacAddr[0],2,16,QChar('0'))
                            .arg(sectype.m_aMacAddr[1],2,16,QChar('0'))
                            .arg(sectype.m_aMacAddr[2],2,16,QChar('0'))
                            .arg(sectype.m_aMacAddr[3],2,16,QChar('0'))
                            .arg(sectype.m_aMacAddr[4],2,16,QChar('0'))
                            .arg(sectype.m_aMacAddr[5],2,16,QChar('0')));
        QByteArray devicetype=QByteArray::fromRawData((char*)sectype.m_aDeviceType,4);
        ui.DeviceTypeEdt->setText(devicetype);
        ui.NotSetRBtn->setChecked((sectype.m_aNirSensitivity[0] == 0) || (sectype.m_aNirSensitivity[0] == 255));
        ui.LowNoiseRBtn->setChecked(sectype.m_aNirSensitivity[0] == 100);
        ui.HighSensRBtn->setChecked(sectype.m_aNirSensitivity[0] == 101);
    }
}

void secure::on_SaveSecureBtn_clicked()
{
    SecureType sectype;
    extern AvsIdentityType m_Identity;
    extern long m_DeviceHandle;

    int l_Res = AVS_GetSecureConfig((AvsIdentityType*) &m_Identity, &sectype);
    if (l_Res==0) {
		QByteArray version = ui.VersionEdt->text().toLatin1();
        memcpy((char*)sectype.m_aSwVersion,version.data(),4);
		QByteArray serial = ui.SerialEdt->text().toLatin1();
        memcpy((char*)sectype.m_aSerialId,serial.data(),9);
        float timestamp=ui.TimeStampEdt->text().toFloat();
        memcpy((char*)sectype.m_aTimeStamp,&timestamp,4);
		QByteArray customer = ui.CustomerEdt->text().toLatin1();
        memcpy((char*)sectype.m_aCustomerID,customer.data(),64);
        if (ui.BasicRBtn->isChecked())  // Full button is mutually exclusive
            sectype.m_aAppl[2]='0';
        else
            sectype.m_aAppl[2]='1';
        if (ui.NotDedRBtn->isChecked()) 
            sectype.m_aAppl[12] = '1';
        if (ui.DedSlaveRBtn->isChecked()) 
            sectype.m_aAppl[12] = '2';
        if (ui.DedMasterRBtn->isChecked())
            sectype.m_aAppl[12] = '3';
        if (ui.ColorChkB->isChecked())
            sectype.m_aAppl[3] = '1';
        else
            sectype.m_aAppl[3] = '0';
        if (ui.ProcessChkB->isChecked())
            sectype.m_aAppl[4] = '1';
        else
            sectype.m_aAppl[4] = '0';
        if (ui.IrradianceChkB->isChecked())
            sectype.m_aAppl[5] = '1';
        else
            sectype.m_aAppl[5] = '0';
        if (ui.OxygenChkB->isChecked())
            sectype.m_aAppl[6] = '1';
        else
            sectype.m_aAppl[6] = '0';
        if (ui.ExcelChkB->isChecked())
            sectype.m_aAppl[7] = '1';
        else
            sectype.m_aAppl[7] = '0';
        if (ui.PhotonChkB->isChecked())
            sectype.m_aAppl[8] = '1';
        else
            sectype.m_aAppl[8] = '0';
        if (ui.ThinFilmChkB->isChecked())
            sectype.m_aAppl[9] = '1';
        else
            sectype.m_aAppl[9] = '0';
        if (ui.ChemoChkB->isChecked())
            sectype.m_aAppl[10] = '1';
        else
            sectype.m_aAppl[10] = '0';
        if (ui.FastTChkB->isChecked())
            sectype.m_aAppl[11] = '1';
        else
            sectype.m_aAppl[11] = '0';
        uint8 blackleft=ui.BlackLeftEdt->text().toInt();
        sectype.m_aNrOfBlackenedLeft[0]=blackleft;
        uint8 blackright=ui.BlackRightEdt->text().toInt();
        sectype.m_aNrOfBlackenedRight[0]=blackright;
        uint16 pval=ui.TEC_P_Edt->text().toInt();
        memcpy((char*)sectype.m_aTecCtrl_P,&pval,2);
        uint16 ival=ui.TEC_I_Edt->text().toInt();
        memcpy((char*)sectype.m_aTecCtrl_I,&ival,2);
        uint16 dval=ui.TEC_D_Edt->text().toInt();
        memcpy((char*)sectype.m_aTecCtrl_D,&dval,2);
        bool ok1, ok2, ok3, ok4, ok5, ok6;
        sectype.m_aMacAddr[0]=ui.MAC_Edt->text().mid(0,2).toInt(&ok1,16);
        sectype.m_aMacAddr[1]=ui.MAC_Edt->text().mid(3,2).toInt(&ok2,16);
        sectype.m_aMacAddr[2]=ui.MAC_Edt->text().mid(6,2).toInt(&ok3,16);
        sectype.m_aMacAddr[3]=ui.MAC_Edt->text().mid(9,2).toInt(&ok4,16);
        sectype.m_aMacAddr[4]=ui.MAC_Edt->text().mid(12,2).toInt(&ok5,16);
        sectype.m_aMacAddr[5]=ui.MAC_Edt->text().mid(15,2).toInt(&ok6,16);
        if ((ok1 && ok2 && ok3 && ok4 && ok5 && ok6) == false)
            QMessageBox::critical(this, tr("QT4 Demo"),
                     tr("MAC Address is invalid!"),
                     QMessageBox::Ok, QMessageBox::Ok);
		QByteArray devicetype = ui.DeviceTypeEdt->text().toLatin1();
        memcpy((char*)sectype.m_aDeviceType,devicetype.data(),4);
        if (ui.NotSetRBtn->isChecked()) 
            sectype.m_aNirSensitivity[0] = 0;
        if (ui.LowNoiseRBtn->isChecked()) 
            sectype.m_aNirSensitivity[0] = 100;
        if (ui.HighSensRBtn->isChecked())
            sectype.m_aNirSensitivity[0] = 101;

        l_Res = AVS_SetSecure(m_DeviceHandle,"5216",&sectype);
            if (ERR_SUCCESS != l_Res) 
                QMessageBox::critical(this, tr("QT4 Demo"),
                     tr("AVS_SetSecure failed, code %1").arg(l_Res),
                     QMessageBox::Ok, QMessageBox::Ok);

    }
}
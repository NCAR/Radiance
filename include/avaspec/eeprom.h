#ifndef EEPROM_H
#define EEPROM_H

#include <QtWidgets\QMainWindow>
#include <QTimer>
#include "ui_eeprom.h"

class eeprom : public QDialog
{
	Q_OBJECT

public:
	eeprom(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~eeprom();

protected:

private:
	Ui::Eeprom ui;
	QTimer *timer;

private slots:
	void on_SaveEepromBtn_clicked();
	void showEvent(QShowEvent *event);

public slots:

};

#endif // EEPROM_H

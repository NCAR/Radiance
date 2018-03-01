#ifndef SECURE_H
#define SECURE_H

#include <QtWidgets\QMainWindow>
#include "ui_secure.h"

class secure : public QDialog
{
	Q_OBJECT

public:
	secure(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~secure();

protected:

private:
	Ui::secure ui;

private slots:
	void on_SaveSecureBtn_clicked();
    void showEvent(QShowEvent *event);

public slots:

};

#endif // SECURE_H
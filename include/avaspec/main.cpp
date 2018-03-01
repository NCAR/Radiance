#include <QtWidgets\QApplication>
#include <QTime>
#include "qtdemo.h"
#include "application.h"
#include "avaspec.h"

// some globals

long m_DeviceHandle=INVALID_AVS_HANDLE_VALUE;
AvsIdentityType m_Identity;
unsigned int l_Time=0;				    // timestamp value
unsigned int m_PreviousTimeStamp=0;		// timestamp value
QTime m_Time;					        // time value
unsigned short m_NrPixels;
unsigned short m_StartPixels;
unsigned short m_StopPixels;
unsigned int m_Measurements;
unsigned int m_Failures;
bool m_GraphicsDisabled;
double m_pLambda[MAX_NR_PIXELS];
double l_pSpectrum[MAX_NR_PIXELS];
DeviceConfigType DeviceConfig;

int main(int argc, char *argv[])
{
	application a(argc, argv);
	Qtdemo w;
	w.show();
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	return a.exec();
}

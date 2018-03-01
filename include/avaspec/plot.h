#ifndef PLOT_H
#define PLOT_H

#include <qwt_plot.h>

class QwtPlotCurve;

class Plot : public QwtPlot
{
	Q_OBJECT  

	// if you get a QMetaObject link error here, make sure your
	// Visual Studio project adds QWT_DLL to the list of 
	// preprocessor defines
	// (project properties->c++->preprocessor->definitions)

public:
    Plot(QWidget * = NULL);

private:
    QwtPlotCurve *curve;

public slots:
    void update_plot();
};

#endif

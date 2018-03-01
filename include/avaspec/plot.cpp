#include <QtGui>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include "plot.h"
#include "avaspec.h"

Plot::Plot(QWidget *parent):
    QwtPlot(parent)
{
    extern double m_pLambda[MAX_NR_PIXELS];
    extern double l_pSpectrum[MAX_NR_PIXELS];

    setAxisTitle(xBottom, "Wavelength [nm]");
    setAxisTitle(yLeft, "Counts");
    setAxisScale(xBottom, 300.0, 900.0);
    setAxisScale(yLeft, -500.0, 66000.0);

    curve = new QwtPlotCurve("Testcurve");
    curve->setRenderHint(QwtPlotItem::RenderAntialiased);
    curve->setPen(QPen(Qt::black));
    curve->setSamples(m_pLambda,l_pSpectrum,MAX_NR_PIXELS);
    curve->attach(this);

    // LeftButton for the zooming
    // MidButton for the panning
    // RightButton: zoom out by 1
    // Ctrl+RighButton: zoom out to full size

    QwtPlotZoomer* zoomer = new QwtPlotZoomer(canvas());
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,
    Qt::RightButton, Qt::ControlModifier);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,
    Qt::RightButton);

    QwtPlotPanner *panner = new QwtPlotPanner(canvas());  
    panner->setMouseButton(Qt::MidButton);
    zoomer->setRubberBandPen(QColor(Qt::black));
    zoomer->setTrackerPen(QColor(Qt::black));
    this->show();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_plot()));
    timer->start(100);  // 100 msec
}

void Plot::update_plot()
{
    extern double m_pLambda[MAX_NR_PIXELS];
    extern double l_pSpectrum[MAX_NR_PIXELS];
    extern unsigned short m_NrPixels;
    extern bool m_GraphicsDisabled;

    if (!m_GraphicsDisabled)
    {
        curve->setSamples(m_pLambda,l_pSpectrum,m_NrPixels);
        replot();
    }
}


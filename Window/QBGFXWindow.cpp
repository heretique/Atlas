#include "QBGFXWindow.h"
#include <QtGui/private/qpaintdevicewindow_p.h>
#include <bgfx/bgfx.h>
#include <bgfx/bgfxplatform.h>
#include <QGuiApplication>
#include <qpa/qplatformnativeinterface.h>


class QBGFXWindowPrivate : public QPaintDeviceWindowPrivate
{
    Q_DECLARE_PUBLIC(QBGFXWindow)
public:
    QBGFXWindowPrivate() :
        _initialized(false)
    { }
    ~QBGFXWindowPrivate() { }

    void initialize();
    void resize(const QSize &size);
    void shutdown();

    void beginPaint(const QRegion &region) Q_DECL_OVERRIDE;
    void endPaint() Q_DECL_OVERRIDE;
    void flush(const QRegion &region) Q_DECL_OVERRIDE;
    void platformInitBGFX();

private:
    bool _initialized;
    uint32_t _reset;
    uint32_t _debug;
};


void QBGFXWindowPrivate::initialize()
{
    Q_Q(QBGFXWindow);
    if (_initialized)
        return;

    _debug  = BGFX_DEBUG_TEXT;
    _reset  = BGFX_RESET_NONE;


    platformInitBGFX();
    bgfx::init(bgfx::RendererType::Count, BGFX_PCI_ID_NONE);
    bgfx::reset(q->size().width(), q->size().height());

    // Enable debug text.
    bgfx::setDebug(_debug);

    // Set view 0 clear state.
    bgfx::setViewClear(0
                       , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                       , 0x303030ff
                       , 1.0f
                       , 0
                       );

    _initialized = true;

    q->initializeBGFX();

    resize(q->size());
}

void QBGFXWindowPrivate::resize(const QSize &size)
{
    if (!_initialized)
        return;

    // Set view 0 default viewport.
    bgfx::reset(size.width(), size.height());
    bgfx::setViewRect(0, 0, 0, size.width(), size.height());
}

void QBGFXWindowPrivate::shutdown()
{
    Q_Q(QBGFXWindow);
    q->releaseBGFX();
    bgfx::shutdown();
}

void QBGFXWindowPrivate::beginPaint(const QRegion &region)
{
    Q_UNUSED(region);
    initialize();
}

void QBGFXWindowPrivate::endPaint()
{

}

void QBGFXWindowPrivate::flush(const QRegion &region)
{
    Q_UNUSED(region);
    bgfx::frame();
}


void QBGFXWindowPrivate::platformInitBGFX()
{
    Q_Q(QBGFXWindow);
#if BX_PLATFORM_WINDOWS
    // platform specific that needs to be called before bgfx::init to set global window handle
    // and context
    // TODO: do this for other platforms as well
    bgfx::winSetHwnd(reinterpret_cast<HWND>(q->winId()));
#elif BX_PLATFORM_LINUX

    QPlatformNativeInterface *interface =  QGuiApplication::platformNativeInterface();
    void *display = interface->nativeResourceForIntegration("Display");
    bgfx:bgfx::x11SetDisplayWindow(display, q->winId());
#endif
}









QBGFXWindow::QBGFXWindow(QWindow *parent) :
    QPaintDeviceWindow(*(new QBGFXWindowPrivate), parent)
{
    setSurfaceType(QSurface::OpenGLSurface);
    format().setSwapInterval(0);
}


QBGFXWindow::~QBGFXWindow()
{

}


void QBGFXWindow::initializeBGFX()
{
}

void QBGFXWindow::releaseBGFX()
{
}

void QBGFXWindow::resizeBGFX(const QSize &size)
{
    Q_UNUSED(size);

}

void QBGFXWindow::paintBGFX()
{
}

void QBGFXWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    paintBGFX();
}

void QBGFXWindow::resizeEvent(QResizeEvent *event)
{
    Q_D(QBGFXWindow);

    if (!isExposed() || size().isEmpty())
        return;

    d->resize(event->size());
    resizeBGFX(event->size());
}

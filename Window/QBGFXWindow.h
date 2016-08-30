#ifndef QBGFXWINDOW_H
#define QBGFXWINDOW_H

#include <QPaintDeviceWindow>

class QBGFXWindowPrivate;

class QBGFXWindow : public QPaintDeviceWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QBGFXWindow)

public:
    QBGFXWindow(QWindow *parent = Q_NULLPTR);
    ~QBGFXWindow();

protected:
    virtual void initializeBGFX();
    virtual void releaseBGFX();
    virtual void resizeBGFX(const QSize &size);
    virtual void paintBGFX();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *) Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(QBGFXWindow)
};

#endif // QBGFXWINDOW_H

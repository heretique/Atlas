#ifndef WINDOW_H
#define WINDOW_H

#include <bgfx/bgfx.h>
#include <bx/uint32_t.h>
#include <QBGFXWindow.h>

class MainWindow : public QBGFXWindow
{
public:
    MainWindow();
    ~MainWindow();

protected:
    void initializeBGFX() Q_DECL_OVERRIDE;
    void releaseBGFX() Q_DECL_OVERRIDE;
    void resizeBGFX(const QSize &size) Q_DECL_OVERRIDE;
    void paintBGFX() Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_debug;
    uint32_t m_reset;

    bool     m_autoAdjust;
    int32_t  m_scrollArea;
    int32_t  m_dim;
    int32_t  m_maxDim;
    uint32_t m_transform;

    int64_t  m_timeOffset;

    int64_t  m_deltaTimeNs;
    int64_t  m_deltaTimeAvgNs;
    int64_t  m_numFrames;

    bgfx::ProgramHandle m_program;
    bgfx::VertexBufferHandle m_vbh;
    bgfx::IndexBufferHandle  m_ibh;
};

#endif // WINDOW_H

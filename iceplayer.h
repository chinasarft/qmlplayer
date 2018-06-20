#ifndef ICEPLAYER_H
#define ICEPLAYER_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QtMultimedia>
#include <memory>
#include <input.hpp>

class QGLRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    QGLRenderer();
    ~QGLRenderer();

    void setT(qreal t) { m_t = t; }
    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }
    void setDrawRect(QRectF & s, QRectF &it);

    const int TextureYIndex = 0;
    const int TextureUIndex = 1;
    const int TextureVIndex = 2;

public slots:
    void paint();

private:
    QSize m_viewportSize;
    qreal m_t;
    QOpenGLShaderProgram *m_program;
    QQuickWindow *m_window;
    GLuint m_textures[3];
    GLfloat xleft;
    GLfloat xright;
    GLfloat ytop;
    GLfloat ybottom;
};

class AudioRender{
public:
    const static int ulawType = 1;
    const static int alawType = 2;
    AudioRender();
    void PushData(void *pcmData,int size);
    void PushG711Data(void *g711Data, int size, int lawType);
private:
    QAudioFormat m_audioConfig;
    std::shared_ptr<QAudioOutput> m_audioOutput;
    bool m_canPlay;
    QIODevice *m_device;
};

class IcePlayer : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)

public:
    explicit IcePlayer();

    qreal t() const { return m_t; }
    void setT(qreal t);

    std::shared_ptr<QTimer> testTimer;
    void testpcm();

signals:
    void tChanged();
    void pictureReady();

public slots:
    void sync();
    void cleanup();
    void testTimeout();
    void Stop();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    static void getFrameCallback(void * userData, const std::shared_ptr<MediaFrame> & frame);

private:
    qreal m_t;
    QGLRenderer *m_vRenderer;
    AudioRender m_aRenderer;

    std::shared_ptr<Input> m_stream1;
    std::shared_ptr<Input> m_stream2;
};

#endif // ICEPLAYER_H

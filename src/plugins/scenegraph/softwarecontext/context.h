/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the Qt SceneGraph Raster Add-on.
**
** $QT_BEGIN_LICENSE$
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef CONTEXT_H
#define CONTEXT_H

#include <private/qsgcontext_p.h>
#include <private/qsgrenderer_p.h>
#include <private/qsgadaptationlayer_p.h>
#include <QtCore/QElapsedTimer>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QBackingStore>

Q_DECLARE_LOGGING_CATEGORY(QSG_RASTER_LOG_TIME_RENDERLOOP)
Q_DECLARE_LOGGING_CATEGORY(QSG_RASTER_LOG_TIME_COMPILATION)
Q_DECLARE_LOGGING_CATEGORY(QSG_RASTER_LOG_TIME_TEXTURE)
Q_DECLARE_LOGGING_CATEGORY(QSG_RASTER_LOG_TIME_GLYPH)
Q_DECLARE_LOGGING_CATEGORY(QSG_RASTER_LOG_TIME_RENDERER)
Q_DECLARE_LOGGING_CATEGORY(QSG_RASTER_LOG_INFO)
Q_DECLARE_LOGGING_CATEGORY(QSG_RASTER_LOG_RENDERLOOP)

namespace SoftwareContext
{

class Renderer : public QSGRenderer
{
public:
    Renderer(QSGRenderContext *context);

    virtual void renderScene(GLuint fboId = 0);

    virtual void render();

    void nodeChanged(QSGNode *node, QSGNode::DirtyState state);

    QBackingStore *backingStore() const { return m_backingStore.data(); }

private:
    QScopedPointer<QBackingStore> m_backingStore;
    QRect m_dirtyRect;
};

class PixmapRenderer : public QSGRenderer
{
public:
    PixmapRenderer(QSGRenderContext *context);

    virtual void renderScene(GLuint fboId = 0);

    virtual void render();

    void render(QPixmap *target);

    QRect m_projectionRect;
};

class RenderContext : public QSGRenderContext
{
public:
    RenderContext(QSGContext *ctx);
    void initialize(QOpenGLContext *context);
    void initializeIfNeeded();
    void invalidate();
    void renderNextFrame(QSGRenderer *renderer, GLuint fbo);
    QSGTexture *createTexture(const QImage &image, uint flags = CreateTexture_Alpha) const;
    QSGRenderer *createRenderer();

    QWindow *currentWindow;
    bool m_initialized;
};

class Context : public QSGContext
{
    Q_OBJECT
public:
    explicit Context(QObject *parent = 0);

    QSGRenderContext *createRenderContext() { return new RenderContext(this); }

    virtual QSGRectangleNode *createRectangleNode();
    virtual QSGImageNode *createImageNode();
    virtual QSGPainterNode *createPainterNode(QQuickPaintedItem *item);
    virtual QSGGlyphNode *createGlyphNode(QSGRenderContext *rc, bool preferNativeGlyphNode);
    virtual QSGNinePatchNode *createNinePatchNode();
    virtual QSGLayer *createLayer(QSGRenderContext *renderContext);
    virtual QSurfaceFormat defaultSurfaceFormat() const;
};

} // namespace

#endif // CONTEXT_H

#ifndef VIEWPORT_CONTAINER_H
#define VIEWPORT_CONTAINER_H

#include "ViewportWidget.h"
#include "ViewportOverlayWidget.h"

class ViewportContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ViewportContainer(QWidget* parent = nullptr);
    RenderWidget* getRenderWidget() { return m_render; }

protected:
    void resizeEvent(QResizeEvent* e) override;

private:
    RenderWidget* m_render;
    RenderOverlayWidget* m_overlay;
};


#endif // !VIEWPORT_CONTAINER_H
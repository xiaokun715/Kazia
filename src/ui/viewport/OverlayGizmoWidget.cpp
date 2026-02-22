//#include "ViewportContainer.h"
//
//ViewportContainer::ViewportContainer(QWidget* parent)
//	: QWidget(parent)
//{
//	//m_render = new RenderWidget(this);
//	//m_overlay = new RenderOverlayWidget(this);
//
//	//m_render->setGeometry(0, 0, 200, 200);
//	//m_overlay->setGeometry(rect());
//	//m_overlay->raise();
//
//    // 主布局
//    QVBoxLayout* layout = new QVBoxLayout(this);
//    layout->setContentsMargins(0, 0, 0, 0);
//    layout->setSpacing(0);
//
//    // 渲染层（占满）
//    m_render = new ViewportWidget;
//    layout->addWidget(m_render);
//
//    // 覆盖层（放在最后添加 → 自动在上面）
//    m_overlay = new RenderOverlayWidget;
//    m_overlay->setParent(this);           // 显式设父对象
//    m_overlay->resize(this->size());        // 初始大小
//    m_overlay->raise();                      // 确保在最上层
//}
//
//void ViewportContainer::resizeEvent(QResizeEvent*)
//{
//	//const int overlayHeight = 32;
//	//m_render->setGeometry(0, 0, 200, 200);
//	//m_overlay->setGeometry(0, 0, width(), overlayHeight);
//}
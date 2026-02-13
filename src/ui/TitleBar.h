#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>

class MainWindow;

class TitleBar : public QWidget
{
    Q_OBJECT

private:
    QHBoxLayout* m_layout;
    QLabel* m_appIcon;
    QLabel* m_appTitle;
    QPushButton* m_minimizeButton;
    QPushButton* m_maximizeButton;
    QPushButton* m_closeButton;
    
    MainWindow* m_mainWindow;
    bool m_isDragging;
    QPoint m_dragStartPosition;
    
    void setupUI();
    void createButtons();
    void connectSignals();

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    
public:
    explicit TitleBar(MainWindow* mainWindow, QWidget* parent = nullptr);
    ~TitleBar();
    
    void updateMaximizeButton();
    void setTitle(const QString& title);
    
public slots:
    void onMinimizeClicked();
    void onMaximizeClicked();
    void onCloseClicked();
};

#endif // TITLEBAR_H

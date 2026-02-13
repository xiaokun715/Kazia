#ifndef DOCKTITLEBAR_H
#define DOCKTITLEBAR_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

class QDockWidget;

class DockTitleBar : public QWidget
{
    Q_OBJECT

private:
    QHBoxLayout* m_layout;
    QLabel* m_titleLabel;
    QPushButton* m_floatButton;
    QPushButton* m_closeButton;
    
    QDockWidget* m_dockWidget;
    
    void setupUI();
    void createButtons();
    void connectSignals();

public:
    explicit DockTitleBar(QDockWidget* dockWidget, QWidget* parent = nullptr);
    ~DockTitleBar();
    
    void setTitle(const QString& title);
    
public slots:
    void onFloatClicked();
    void onCloseClicked();
};

#endif // DOCKTITLEBAR_H

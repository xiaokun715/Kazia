#ifndef PROPERTIESPANEL_H
#define PROPERTIESPANEL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>

class PropertiesPanel : public QWidget
{
    Q_OBJECT

private:
    QVBoxLayout* m_mainLayout;
    QFormLayout* m_transformLayout;
    QLineEdit* m_nameEdit;
    QDoubleSpinBox* m_positionX;
    QDoubleSpinBox* m_positionY;
    QDoubleSpinBox* m_positionZ;
    QDoubleSpinBox* m_rotationX;
    QDoubleSpinBox* m_rotationY;
    QDoubleSpinBox* m_rotationZ;
    QDoubleSpinBox* m_scaleX;
    QDoubleSpinBox* m_scaleY;
    QDoubleSpinBox* m_scaleZ;

public:
    PropertiesPanel(QWidget *parent = nullptr);
    ~PropertiesPanel();

    void setGameObjectProperties(const QString& name, 
                                float posX, float posY, float posZ,
                                float rotX, float rotY, float rotZ,
                                float scaleX, float scaleY, float scaleZ);

    void clear();

signals:
    void propertyChanged(const QString& property, float value);
    void nameChanged(const QString& name);

private slots:
    void onPositionChanged(double value);
    void onRotationChanged(double value);
    void onScaleChanged(double value);
    void onNameChanged(const QString& text);
};

#endif // PROPERTIESPANEL_H

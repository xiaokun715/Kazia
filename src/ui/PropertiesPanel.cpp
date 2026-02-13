#include "PropertiesPanel.h"

PropertiesPanel::PropertiesPanel(QWidget *parent)
    : QWidget(parent)
    , m_mainLayout(nullptr)
    , m_transformLayout(nullptr)
    , m_nameEdit(nullptr)
    , m_positionX(nullptr)
    , m_positionY(nullptr)
    , m_positionZ(nullptr)
    , m_rotationX(nullptr)
    , m_rotationY(nullptr)
    , m_rotationZ(nullptr)
    , m_scaleX(nullptr)
    , m_scaleY(nullptr)
    , m_scaleZ(nullptr)
{
    // 创建主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setContentsMargins(10, 10, 10, 10);

    // 创建名称编辑框
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("对象名称");
    m_mainLayout->addWidget(m_nameEdit);

    // 创建变换属性布局
    m_transformLayout = new QFormLayout();
    m_transformLayout->setContentsMargins(0, 10, 0, 0);

    // 创建位置控件
    m_positionX = new QDoubleSpinBox(this);
    m_positionY = new QDoubleSpinBox(this);
    m_positionZ = new QDoubleSpinBox(this);

    m_positionX->setRange(-1000.0, 1000.0);
    m_positionY->setRange(-1000.0, 1000.0);
    m_positionZ->setRange(-1000.0, 1000.0);
    m_positionX->setSingleStep(0.1);
    m_positionY->setSingleStep(0.1);
    m_positionZ->setSingleStep(0.1);

    // 创建旋转控件
    m_rotationX = new QDoubleSpinBox(this);
    m_rotationY = new QDoubleSpinBox(this);
    m_rotationZ = new QDoubleSpinBox(this);

    m_rotationX->setRange(-180.0, 180.0);
    m_rotationY->setRange(-180.0, 180.0);
    m_rotationZ->setRange(-180.0, 180.0);
    m_rotationX->setSingleStep(1.0);
    m_rotationY->setSingleStep(1.0);
    m_rotationZ->setSingleStep(1.0);

    // 创建缩放控件
    m_scaleX = new QDoubleSpinBox(this);
    m_scaleY = new QDoubleSpinBox(this);
    m_scaleZ = new QDoubleSpinBox(this);

    m_scaleX->setRange(0.01, 100.0);
    m_scaleY->setRange(0.01, 100.0);
    m_scaleZ->setRange(0.01, 100.0);
    m_scaleX->setSingleStep(0.1);
    m_scaleY->setSingleStep(0.1);
    m_scaleZ->setSingleStep(0.1);
    m_scaleX->setValue(1.0);
    m_scaleY->setValue(1.0);
    m_scaleZ->setValue(1.0);

    // 添加控件到布局
    m_transformLayout->addRow("位置 X:", m_positionX);
    m_transformLayout->addRow("位置 Y:", m_positionY);
    m_transformLayout->addRow("位置 Z:", m_positionZ);
    m_transformLayout->addRow("旋转 X:", m_rotationX);
    m_transformLayout->addRow("旋转 Y:", m_rotationY);
    m_transformLayout->addRow("旋转 Z:", m_rotationZ);
    m_transformLayout->addRow("缩放 X:", m_scaleX);
    m_transformLayout->addRow("缩放 Y:", m_scaleY);
    m_transformLayout->addRow("缩放 Z:", m_scaleZ);

    // 添加变换布局到主布局
    m_mainLayout->addLayout(m_transformLayout);

    // 添加拉伸空间
    m_mainLayout->addStretch();

    // 设置布局
    setLayout(m_mainLayout);

    // 连接信号槽
    connect(m_nameEdit, &QLineEdit::textChanged, this, &PropertiesPanel::onNameChanged);
    connect(m_positionX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onPositionChanged);
    connect(m_positionY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onPositionChanged);
    connect(m_positionZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onPositionChanged);
    connect(m_rotationX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onRotationChanged);
    connect(m_rotationY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onRotationChanged);
    connect(m_rotationZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onRotationChanged);
    connect(m_scaleX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onScaleChanged);
    connect(m_scaleY, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onScaleChanged);
    connect(m_scaleZ, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &PropertiesPanel::onScaleChanged);
}

PropertiesPanel::~PropertiesPanel()
{
}

void PropertiesPanel::setGameObjectProperties(const QString& name, 
                                            float posX, float posY, float posZ,
                                            float rotX, float rotY, float rotZ,
                                            float scaleX, float scaleY, float scaleZ)
{
    // 设置名称
    m_nameEdit->setText(name);

    // 设置位置
    m_positionX->setValue(posX);
    m_positionY->setValue(posY);
    m_positionZ->setValue(posZ);

    // 设置旋转
    m_rotationX->setValue(rotX);
    m_rotationY->setValue(rotY);
    m_rotationZ->setValue(rotZ);

    // 设置缩放
    m_scaleX->setValue(scaleX);
    m_scaleY->setValue(scaleY);
    m_scaleZ->setValue(scaleZ);
}

void PropertiesPanel::clear()
{
    // 清空所有控件
    m_nameEdit->clear();
    m_positionX->setValue(0.0);
    m_positionY->setValue(0.0);
    m_positionZ->setValue(0.0);
    m_rotationX->setValue(0.0);
    m_rotationY->setValue(0.0);
    m_rotationZ->setValue(0.0);
    m_scaleX->setValue(1.0);
    m_scaleY->setValue(1.0);
    m_scaleZ->setValue(1.0);
}

void PropertiesPanel::onPositionChanged(double value)
{
    // 发送位置变化信号
    if (sender() == m_positionX) {
        emit propertyChanged("positionX", static_cast<float>(value));
    } else if (sender() == m_positionY) {
        emit propertyChanged("positionY", static_cast<float>(value));
    } else if (sender() == m_positionZ) {
        emit propertyChanged("positionZ", static_cast<float>(value));
    }
}

void PropertiesPanel::onRotationChanged(double value)
{
    // 发送旋转变化信号
    if (sender() == m_rotationX) {
        emit propertyChanged("rotationX", static_cast<float>(value));
    } else if (sender() == m_rotationY) {
        emit propertyChanged("rotationY", static_cast<float>(value));
    } else if (sender() == m_rotationZ) {
        emit propertyChanged("rotationZ", static_cast<float>(value));
    }
}

void PropertiesPanel::onScaleChanged(double value)
{
    // 发送缩放变化信号
    if (sender() == m_scaleX) {
        emit propertyChanged("scaleX", static_cast<float>(value));
    } else if (sender() == m_scaleY) {
        emit propertyChanged("scaleY", static_cast<float>(value));
    } else if (sender() == m_scaleZ) {
        emit propertyChanged("scaleZ", static_cast<float>(value));
    }
}

void PropertiesPanel::onNameChanged(const QString& text)
{
    // 发送名称变化信号
    emit nameChanged(text);
}

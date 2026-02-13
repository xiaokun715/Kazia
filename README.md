# Kazia - 基于 Filament 和 Qt 的 3D 渲染器

Kazia 是一款基于 Filament 渲染引擎和 Qt 框架开发的桌面 3D 渲染器，类似于 Blender 或 Unreal Engine，提供了直观的用户界面和强大的渲染功能。

## 功能特性

- **实时渲染**：基于 Filament 物理渲染引擎，提供高质量的实时光照和阴影效果
- **直观的用户界面**：基于 Qt 框架，提供类似于 Blender 或 UE 的用户界面
- **场景管理**：支持创建、编辑和管理 3D 场景
- **基本几何体**：支持创建立方体、球体和圆柱体等基本几何体
- **相机控制**：提供丰富的相机控制功能，包括旋转、缩放和平移
- **材质系统**：支持 PBR 材质

## 系统要求

- Windows 10 或更高版本
- Visual Studio 2019 或更高版本
- Qt 6.x
- Filament 渲染引擎

## 构建步骤

1. **克隆仓库**

2. **安装依赖**
   - 安装 Qt 6.x
   - 构建 Filament 渲染引擎

3. **配置 CMake**
   ```bash
   mkdir build
   cd build
   cmake .. -G "Visual Studio 16 2019" -A x64
   ```

4. **构建项目**
   - 打开生成的解决方案文件 (`Kazia.sln`)
   - 构建项目

5. **运行应用程序**

## 项目结构

```
Kazia/
├── CMakeLists.txt              # 主构建文件
├── src/
│   ├── core/                   # 核心渲染引擎集成
│   │   ├── FilamentEngine.cpp  # Filament 引擎封装
│   │   ├── FilamentEngine.h
│   │   ├── Renderer.cpp        # 渲染器实现
│   │   ├── Renderer.h
│   │   ├── Mesh.cpp            # 网格实现
│   │   ├── Mesh.h
│   │   ├── MaterialManager.cpp # 材质管理器
│   │   ├── MaterialManager.h
│   │   ├── CameraController.cpp # 相机控制器
│   │   └── CameraController.h
│   ├── ui/                     # Qt 用户界面
│   │   ├── MainWindow.cpp      # 主窗口
│   │   ├── MainWindow.h
│   │   ├── RenderWidget.cpp    # 渲染视口控件
│   │   ├── RenderWidget.h
│   │   ├── SceneTree.cpp       # 场景树面板
│   │   ├── SceneTree.h
│   │   ├── PropertiesPanel.cpp # 属性面板
│   │   └── PropertiesPanel.h
│   ├── scene/                  # 场景管理
│   │   ├── Scene.cpp           # 场景类
│   │   ├── Scene.h
│   │   ├── GameObject.cpp      # 游戏对象
│   │   ├── GameObject.h
│   │   ├── Camera.cpp          # 相机
│   │   └── Camera.h
│   └── utils/                  # 工具类
│       ├── Logger.cpp          # 日志工具
│       └── Logger.h
├── assets/                     # 资源文件
│   ├── models/                 # 3D 模型
│   ├── textures/               # 纹理
│   └── shaders/                # 着色器
├── build/                      # 构建目录
└── thirdparty/                 # 第三方库
    └── filament/               # Filament 库
```

## 使用指南

### 基本操作

1. **创建立体**：点击工具栏中的 "创建立方体"、"创建球体" 或 "创建圆柱体" 按钮
2. **选择对象**：在场景树中点击对象名称，或在渲染视口中点击对象
3. **编辑对象属性**：在属性面板中修改对象的位置、旋转和缩放
4. **相机控制**：
   - 鼠标左键拖动：旋转相机
   - 鼠标滚轮：缩放相机
   - 鼠标中键拖动：平移相机

### 场景管理

- **添加对象**：使用工具栏按钮或右键菜单
- **删除对象**：在场景树中选择对象，然后按 Delete 键
- **重命名对象**：在属性面板中修改对象名称

## 未来计划

- **模型导入/导出**：支持导入和导出常见的 3D 模型格式
- **材质编辑**：提供直观的材质编辑工具
- **动画系统**：支持基本的动画功能
- **后期处理**：添加后期处理效果
- **物理引擎集成**：集成物理引擎，支持碰撞检测和物理模拟

## 许可证

MIT 许可证

## 致谢

- [Filament](https://github.com/google/filament) - Google 开发的物理渲染引擎
- [Qt](https://www.qt.io/) - 跨平台应用程序框架

## 联系方式

如有问题或建议，请联系项目维护者。

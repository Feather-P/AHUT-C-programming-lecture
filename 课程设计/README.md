# 课程设计：酒店简易管理系统（C 语言）

本项目为安徽工业大学 C 语言课程设计，基于 C99 实现一个“酒店简易管理系统”，采用分层结构（配置层、模型层、仓储层、服务层、终端 UI 层），并提供基础单元测试。

## 1. 项目目标

- 以控制台交互（TUI）方式完成酒店日常业务管理
- 支持房型/房间、价格、住客档案、订单与入住生命周期管理
- 支持基础统计洞察（insight）能力
- 使用配置文件与 JSON 数据文件完成持久化

## 2. 技术栈与依赖

- 语言标准：C99
- 构建系统：CMake
- 第三方库：
  - cJSON（JSON 解析/序列化）
  - libconfig（配置文件加载）
  - Curses（终端 UI）

可在 [`CMakeLists.txt`](CMakeLists.txt) 中看到依赖声明与可执行目标定义。

## 3. 目录结构

```text
.
├── CMakeLists.txt
├── CMakePresets.json
├── vcpkg.json
├── vcpkg-configuration.json
├── config/
│   └── repository.cfg.example
├── data.example/
│   ├── guests.json
│   ├── orders.json
│   ├── prices.json
│   ├── room_types.json
│   └── rooms.json
├── src/
│   ├── main.c
│   ├── config/
│   ├── models/
│   ├── repository/
│   ├── service/
│   ├── ui/
│   └── utils/
└── tests/
```

### 分层说明

- `src/models`：领域模型（房间、房型、价格、住客、订单）
- `src/repository`：数据访问层（读写 JSON 数据）
- `src/service`：业务逻辑层（房间、定价、入住退房、画像、洞察等）
- `src/ui`：终端交互界面
- `src/config`：系统配置读取
- `src/utils`：链表、日志等通用组件

## 4. 功能概览

1. 房态与房间管理（可用房间、占用状态等）
2. 价格管理与价格策略相关处理
3. 住客档案管理
4. 订单与入住生命周期管理（入住/续住/退房）
5. 基础经营数据洞察

## 5. 快速开始

### 5.1 准备数据与配置

1. 复制示例配置文件：
   - `config/repository.cfg.example` → `config/repository.cfg`（按本地路径修改）
2. 准备数据文件：
   - 参考 `data.example/` 下各 JSON 文件格式

### 5.2 构建

```bash
cmake -S . -B build
cmake --build build
```

### 5.3 运行

```bash
./build/hotel-simple-manage-system
```

## 6. 测试

项目已启用 CTest，并在 [`tests/CMakeLists.txt`](tests/CMakeLists.txt) 中注册测试。

```bash
ctest --test-dir build --output-on-failure
```

## 7. 与课程提交要求的关系说明

父目录文档 [`../作业提交.pdf`](../作业提交.pdf) 给出了平时“分题源文件命名与打包提交”的通用规范（如 `1.1.cpp`、`2.x.cpp` 等）。

本仓库属于“课程设计工程化项目”形态，采用 CMake + 多目录源码组织方式，不再按单题 `x.y.c/.cpp` 平铺命名；实际提交时可按任课教师对“课程设计”阶段的最新通知执行（包含源码、配置、数据样例、说明文档等）。

## 8. 备注

- 默认编码建议使用 UTF-8。
- 终端界面依赖 curses，建议在支持 ANSI/终端控制的环境运行。
- 示例数据仅用于演示与测试。

# Linux 自动 Git 更新脚本说明

## 1. 脚本概述

`auto_update_git.py` 是一个智能的 Git 仓库自动更新脚本，专为 Linux 系统设计。该脚本能够自动检查仓库同步状态，并在需要时执行更新操作，同时保护本地未提交的修改。

## 2. 核心功能

- **智能同步判断**：精确检测仓库与远程分支的同步状态
- **本地修改保护**：自动保存和恢复本地未提交的修改
- **冲突处理机制**：在发生冲突时提供清晰的解决方案
- **详细日志记录**：记录所有操作过程，便于问题排查
- **自动错误处理**：在各种异常情况下保持仓库一致性

## 3. 适用环境

- **操作系统**：Linux
- **依赖软件**：Git
- **Python 版本**：Python 3.6 及以上

## 4. 安装与配置

### 4.1 克隆或下载脚本

将脚本文件 `auto_update_git.py` 下载到 Linux 系统的任意目录，例如：
```bash
/home/justinian/codingFile/LinuxScripts/
```

### 4.2 配置脚本

使用文本编辑器打开脚本文件，根据需要修改以下参数：

```python
# 项目配置
project_dir = "/home/justinian/codingFile"  # 你的项目目录路径
log_path = os.path.join(project_dir, "Log_Files", "auto_update_log.txt")  # 日志文件路径
```

- `project_dir`：设置为你的 Git 仓库所在的目录路径
- `log_path`：设置日志文件的保存路径（默认保存在项目目录下的 Log_Files 文件夹）

## 5. 使用方法

### 5.1 直接运行

在终端中执行以下命令运行脚本：

```bash
python /home/justinian/codingFile/LinuxScripts/auto_update_git.py
```

### 5.2 设置定时任务

使用 `crontab` 设置定时任务，实现自动更新：

1. 打开 crontab 编辑器：
```bash
crontab -e
```

2. 添加定时任务（例如每天凌晨 2 点执行）：
```bash
0 2 * * * python /home/justinian/codingFile/LinuxScripts/auto_update_git.py
```

## 6. 工作原理

脚本执行流程如下：

### 6.1 检查 Git 状态

```
1. 检查 Git 状态...
Git 状态检查成功：
On branch main
Your branch is up to date with 'origin/main'.

Untracked files:
  (use "git add <file>..." to include in what will be committed)
	new_file.txt
```

### 6.2 判断同步状态

- 检查是否有未提交的修改或未跟踪的文件
- 获取远程仓库的最新更新（`git fetch`）
- 比较本地分支与远程分支的差异

### 6.3 保存本地修改

如果检测到本地有未提交的修改，脚本会自动保存：

```
3. 保存本地修改...
本地修改已保存：
Saved working directory and index state WIP on main: xxxxxxx Commit message
```

### 6.4 执行更新操作

```
4. 执行更新操作...
仓库更新成功：
Already up to date.
```

### 6.5 恢复本地修改

```
5. 恢复本地修改...
本地修改已恢复：
On branch main
Your branch is up to date with 'origin/main'.
```

## 7. 日志说明

### 7.1 日志位置

日志文件默认保存在：
```
/home/justinian/codingFile/Log_Files/auto_update_log.txt
```

### 7.2 日志格式

```
==================================
开始时间: 2023-11-08 14:30:00
==================================

1. 检查Git状态...
Git状态检查成功：
On branch main
Your branch is up to date with 'origin/main'.

2. 检查仓库同步状态...
仓库已经与GitHub完全同步，无需更新

==================================
结束时间: 2023-11-08 14:30:05
==================================
```

## 8. 常见问题与解决方案

### 8.1 问题：脚本执行失败，提示 "git: command not found"

**解决方案**：确保 Git 已安装在系统上
```bash
sudo apt-get install git  # Ubuntu/Debian
sudo yum install git      # CentOS/RHEL
```

### 8.2 问题：更新失败，提示权限不足

**解决方案**：确保对 Git 仓库有读写权限
```bash
sudo chown -R your_username:your_group /path/to/repository
```

### 8.3 问题：本地修改恢复失败，提示冲突

**解决方案**：手动解决冲突
```bash
# 查看保存的修改
git stash list

# 应用保存的修改（不删除）
git stash apply stash@{0}

# 手动解决冲突后，提交修改
git add .
git commit -m "Resolve conflict"
```

## 9. 注意事项

1. **网络连接**：确保系统有稳定的网络连接，以便与远程仓库通信
2. **Git 配置**：确保已正确配置 Git 用户信息（用户名和邮箱）
3. **分支管理**：脚本默认使用 `main` 分支，如需使用其他分支请修改脚本中的分支名称
4. **磁盘空间**：确保有足够的磁盘空间来存储更新内容
5. **定时任务**：设置定时任务时，确保脚本路径正确且有执行权限

## 10. 脚本代码结构

```
├── run_git_command()  # 执行 Git 命令的辅助函数
└── main()             # 主函数
    ├── 检查 Git 状态
    ├── 判断同步状态
    ├── 保存本地修改
    ├── 执行更新操作
    └── 恢复本地修改
```

## 11. 更新日志

- **v1.0**：初始版本，基本的 Git 拉取功能
- **v2.0**：增加智能同步判断和本地修改保护机制
- **v2.1**：完善错误处理和日志记录功能

---

希望这份说明文件能帮助你更好地理解和使用这个自动更新脚本！如果有任何问题或建议，请随时提出。
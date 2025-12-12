import os
import sys
import subprocess
import datetime

def run_git_command(command, cwd):
    """运行git命令并返回结果"""
    try:
        result = subprocess.run(
            command,
            cwd=cwd,
            shell=True,
            capture_output=True,
            text=True,
            encoding='utf-8',
            errors='replace'  # 处理编码问题
        )
        return result.returncode, result.stdout, result.stderr
    except Exception as e:
        return -1, "", str(e)

def main():
    #设置文件目录
    project_dir=r"C:\Users\29606\codingFile"
    # 指定日志文件夹
    log_folder = os.path.join(project_dir, "Log_Files")
    
    # 创建日志文件夹（如果不存在）
    if not os.path.exists(log_folder):
        os.makedirs(log_folder)
        
    # 设置日志文件路径
    log_file=os.path.join(log_folder,"auto_update_log.txt")
    
    #记录开始时间
    now=datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    log_entries = [f"开始自动更新Git... {now}"]
    
    try:
        #1.检查git状态
        log_entries.append("\n1. 检查Git状态...")
        code, stdout, stderr = run_git_command("git status", project_dir)
        if code == 0:
            log_entries.append("Git状态检查成功：")
            log_entries.append(stdout)
        else:
            log_entries.append("Git状态检查失败：")
            log_entries.append(stderr)
        
        #2.如果git已经和github上同步，则不更新仓库，如果没有同步，则更新仓库
        log_entries.append("\n2. 检查仓库同步状态...")
        
        # 判断是否已同步
        if "Your branch is up to date with" in stdout:
            log_entries.append("仓库已经与GitHub同步，无需更新")
        else:
            log_entries.append("仓库未与GitHub同步，正在更新...")
            # 执行git pull命令更新仓库
            code, stdout, stderr = run_git_command("git pull", project_dir)
            if code == 0:
                log_entries.append("仓库更新成功：")
                log_entries.append(stdout)
            else:
                log_entries.append("仓库更新失败：")
                log_entries.append(stderr)
    
    except Exception as e:
        log_entries.append(f"\n发生未知错误：{str(e)}")
        return -1, "", str(e)
    
    # 记录结束时间
    now_end = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    log_entries.append(f"\n更新完成！{now_end}")
    log_entries.append("--------------------------------------------------")
    
    # 将日志写入文件
    log_content = "\n".join(log_entries)
    try:
        with open(log_file, "a", encoding="utf-8") as f:
            f.write(log_content + "\n")
    except Exception as e:
        log_entries.append(f"\n日志写入失败：{str(e)}")
    
    # 输出结果到控制台
    print(log_content)
    return 0, log_content, ""

if __name__ == "__main__":
    main()
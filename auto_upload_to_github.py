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
            text=True
        )
        return result.returncode, result.stdout, result.stderr
    except Exception as e:
        return -1, "", str(e)

def main():
    # 设置项目目录
    project_dir = r"C:\Users\29606\codingFile"
    log_file = os.path.join(project_dir, "auto_upload_log.txt")
    
    # 记录开始时间
    now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    log_entries = [f"开始自动上传到GitHub... {now}"]
    
    try:
        # 1. 检查Git状态
        log_entries.append("\n1. 检查Git状态...")
        code, stdout, stderr = run_git_command("git status", project_dir)
        if code == 0:
            log_entries.append("Git状态检查成功：")
            log_entries.append(stdout)
        else:
            log_entries.append("Git状态检查失败：")
            log_entries.append(stderr)
            
        # 2. 添加所有更改的文件到暂存区
        log_entries.append("\n2. 添加所有更改的文件到暂存区...")
        code, stdout, stderr = run_git_command("git add .", project_dir)
        if code == 0:
            log_entries.append("文件添加成功")
        else:
            log_entries.append("文件添加失败：")
            log_entries.append(stderr)
            
        # 3. 提交更改
        log_entries.append("\n3. 提交更改...")
        commit_message = f"自动提交 - {now}"
        code, stdout, stderr = run_git_command(f"git commit -m \"{commit_message}\"", project_dir)
        if code == 0:
            log_entries.append("提交成功：")
            log_entries.append(stdout)
        else:
            log_entries.append("提交失败：")
            log_entries.append(stderr)
            
        # 4. 推送到GitHub
        log_entries.append("\n4. 推送到GitHub...")
        code, stdout, stderr = run_git_command("git push origin main", project_dir)
        if code == 0:
            log_entries.append("推送成功：")
            log_entries.append(stdout)
        else:
            log_entries.append("推送失败：")
            log_entries.append(stderr)
            
    except Exception as e:
        log_entries.append(f"\n发生未知错误：{str(e)}")
    
    # 记录结束时间
    now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    log_entries.append(f"\n上传完成！{now}")
    log_entries.append("-" * 50)
    
    # 将日志写入文件
    with open(log_file, "a", encoding="utf-8") as f:
        f.write("\n".join(log_entries))
    
    # 输出结果到控制台
    print("\n".join(log_entries))
    print("\n脚本执行完成，请查看日志文件了解详细信息。")

if __name__ == "__main__":
    main()
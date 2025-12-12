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
    project_dir=r"/home/justinian/codingFile"
    # 指定日志文件夹
    log_folder = os.path.join(project_dir, "Log_Files")
    
    # 创建日志文件夹（如果不存在）
    if not os.path.exists(log_folder):
        os.makedirs(log_folder)
        
    # 设置日志文件路径
    log_file=os.path.join(log_folder,"auto_update_log.txt")
    
    #记录开始时间
    now=datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    log_entries = [f"[Linux]开始自动更新Git... {now}"]
    
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
            return code, stdout, stderr
        
        #2.检查仓库同步状态
        log_entries.append("\n2. 检查仓库同步状态...")
        
        # 检查是否有未提交的修改或未跟踪的文件
        has_local_changes = False
        if "Changes not staged for commit:" in stdout or "Untracked files:" in stdout:
            has_local_changes = True
            log_entries.append("发现本地有未提交的修改或未跟踪的文件")
        
        # 检查本地分支是否与远程分支同步
        is_branch_up_to_date = "Your branch is up to date with" in stdout
        
        # 检查是否有远程更新
        code, fetch_stdout, fetch_stderr = run_git_command("git fetch", project_dir)
        if code != 0:
            log_entries.append("获取远程更新失败：")
            log_entries.append(fetch_stderr)
        else:
            # 检查本地分支与远程分支的差异
            code, diff_stdout, diff_stderr = run_git_command("git diff origin/main main", project_dir)
            if code == 0 and diff_stdout.strip():
                has_remote_changes = True
            else:
                has_remote_changes = False
        
        # 根据不同情况决定是否更新
        if not is_branch_up_to_date or has_remote_changes:
            log_entries.append("仓库未与GitHub完全同步，正在更新...")
            
            # 如果有本地修改，尝试stash保存
            if has_local_changes:
                log_entries.append("\n3. 保存本地修改...")
                code, stash_stdout, stash_stderr = run_git_command("git stash", project_dir)
                if code == 0:
                    log_entries.append("本地修改已保存：")
                    log_entries.append(stash_stdout)
                else:
                    log_entries.append("保存本地修改失败：")
                    log_entries.append(stash_stderr)
                    return code, stash_stdout, stash_stderr
            
            # 执行git pull命令更新仓库
            log_entries.append("\n4. 执行更新操作...")
            code, pull_stdout, pull_stderr = run_git_command("git pull", project_dir)
            if code == 0:
                log_entries.append("仓库更新成功：")
                log_entries.append(pull_stdout)
                
                # 如果有保存的本地修改，尝试恢复
                if has_local_changes:
                    log_entries.append("\n5. 恢复本地修改...")
                    code, pop_stdout, pop_stderr = run_git_command("git stash pop", project_dir)
                    if code == 0:
                        log_entries.append("本地修改已恢复：")
                        log_entries.append(pop_stdout)
                    else:
                        log_entries.append("恢复本地修改失败（可能有冲突）：")
                        log_entries.append(pop_stderr)
                        log_entries.append("请手动解决冲突：git stash list 查看保存的修改，git stash apply 应用修改")
            else:
                log_entries.append("仓库更新失败：")
                log_entries.append(pull_stderr)
                
                # 如果有保存的本地修改，恢复它们
                if has_local_changes:
                    run_git_command("git stash pop", project_dir)
        else:
            log_entries.append("仓库已经与GitHub完全同步，无需更新")
    
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

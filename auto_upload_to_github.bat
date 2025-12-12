@echo off

REM Set project directory
set PROJECT_DIR=C:\Users\29606\codingFile

REM Set log file path
set LOG_FILE=%PROJECT_DIR%\auto_upload_log.txt

REM Create log file if it doesn't exist
if not exist "%LOG_FILE%" type nul > "%LOG_FILE%"

REM Change to project directory
cd /d "%PROJECT_DIR%"

echo Start auto upload to GitHub... %date% %time% >> "%LOG_FILE%"

echo 1. Checking Git status...
git status >> "%LOG_FILE%" 2>&1

echo 2. Adding all changes to staging...
git add . >> "%LOG_FILE%" 2>&1

echo 3. Committing changes...
git commit -m "Auto commit - %date% %time%" >> "%LOG_FILE%" 2>&1

echo 4. Pushing to GitHub...
git push origin main >> "%LOG_FILE%" 2>&1

echo Upload completed! %date% %time% >> "%LOG_FILE%"
echo. >> "%LOG_FILE%"

echo Script execution completed. Please check the log file for details.
pause
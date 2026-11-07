# 编译祖玛游戏与单元测试（Windows / PowerShell，使用 g++；如使用 MSVC 请见 README.md）
#
# 用法：
#   powershell -ExecutionPolicy Bypass -File .\build.ps1

$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$buildDir = Join-Path $root "build"
$srcDir = Join-Path $root "src"
$testDir = Join-Path $root "tests"

New-Item -ItemType Directory -Force -Path $buildDir | Out-Null

$compiler = (Get-Command "g++" -ErrorAction SilentlyContinue)
if (-not $compiler) {
    throw "未找到 g++。请安装 MinGW-w64 / MSYS2，或参考 README.md 使用 MSVC 编译。"
}
$gpp = $compiler.Source

$flags = @("-std=c++17", "-Wall", "-Wextra", "-O2", "-I$srcDir")

$gameSources = Get-ChildItem -Path $srcDir -Recurse -Filter *.cpp |
    Where-Object { $_.Name -ne "main.cpp" } | ForEach-Object { $_.FullName }
$allGameSources = Get-ChildItem -Path $srcDir -Recurse -Filter *.cpp | ForEach-Object { $_.FullName }
$testSources = Get-ChildItem -Path $testDir -Filter *.cpp | ForEach-Object { $_.FullName }

$gameExe = Join-Path $buildDir "zuma.exe"
$testExe = Join-Path $buildDir "zuma_tests.exe"

Write-Host "[1/3] 编译游戏 -> $gameExe"
& $gpp @flags @allGameSources "-o" $gameExe
if ($LASTEXITCODE -ne 0) { throw "游戏编译失败" }

Write-Host "[2/3] 编译单元测试 -> $testExe"
& $gpp @flags @gameSources @testSources "-o" $testExe
if ($LASTEXITCODE -ne 0) { throw "单元测试编译失败" }

Write-Host "[3/3] 复制关卡文件 -> $buildDir\levels"
Copy-Item -Recurse -Force (Join-Path $root "levels") $buildDir

Write-Host ""
Write-Host "构建完成："
Write-Host "  运行游戏：  $gameExe"
Write-Host "  运行测试：  $testExe"

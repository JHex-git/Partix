# 获取目录的绝对路径
param(
    [string]$shaderPath,
    [string]$outputRootPath,
    [string]$cl
)
$basePath = (Get-Item $shaderPath).FullName

# 递归搜索所有.vert和.frag文件
$files = Get-ChildItem -Path $basePath -Recurse -Include '*.vert', '*.geom', '*.frag', '*.comp' -ErrorAction SilentlyContinue

# 遍历文件并输出相对路径
foreach ($file in $files) {
    # 计算相对于基准目录的路径
    Push-Location $basePath
    $relativePath = $file.FullName | Resolve-Path -Relative
    $outputFile = Join-Path $outputRootPath $relativePath
    $outputPath = Split-Path $outputFile -Parent
    $outputFileName = Split-Path $outputFile -Leaf
    $outputFileNameBase = $outputFileName.Substring(0, $outputFileName.LastIndexOf('.'))
    $outputFileNameExt = $outputFileName.Substring($outputFileName.LastIndexOf('.'))
    $outputFile = Join-Path $outputPath "$outputFileName"
    if (-not (Test-Path $outputPath)) {
        New-Item -Path $outputPath -ItemType Directory -Force | Out-Null
    }
    & $cl /I ./ /EP $file /nologo | Out-File -Encoding ASCII -FilePath $outputFile
    Pop-Location
}
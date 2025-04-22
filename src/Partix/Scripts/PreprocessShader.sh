#!/bin/sh
# 参数说明：
# $1: 着色器源目录（如 src/Partix/Shaders）
# $2: 目标输出目录（如 build/Shaders）

SHADER_DIR="$1"
OUTPUT_DIR="$2"

# find $SHADER_DIR -name "*.comp" -o -name "*.vert" -o -name "*.geom" -o -name "*.frag" \
#     -exec sh -c 'file={}; mkdir -p "build/Shaders/$(dirname "$file")" && g++ -E -P -xc++ -I src/Partix/Shaders/ "$file" -o "build/Shaders/${file#src/Partix/Shaders/}"' \;
find "$SHADER_DIR" \( -name "*.comp" -o -name "*.vert" -o -name "*.geom" -o -name "*.frag" \) -exec sh -c '
    file="{}"
    # 计算相对路径（剥离源目录前缀）
    rel_path="${file#'"$SHADER_DIR"'}"
    # 创建目标目录
    mkdir -p "'"$OUTPUT_DIR"'/Shaders/$(dirname "$rel_path")"
    # 执行预处理
    g++ -E -P -xc++ -I "'"$SHADER_DIR"'" "$file" -o "'"$OUTPUT_DIR"'/Shaders/$rel_path"
' _ {} \;
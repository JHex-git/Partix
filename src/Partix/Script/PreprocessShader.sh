#!/bin/sh
# 参数说明：
# $1: 着色器源目录（如 src/Partix/Shader）
# $2: 目标输出目录（如 build/Shader）

SHADER_DIR="$1"
OUTPUT_DIR="$2"

# find $SHADER_DIR -name "*.comp" -o -name "*.vert" -o -name "*.geom" -o -name "*.frag" \
#     -exec sh -c 'file={}; mkdir -p "build/Shader/$(dirname "$file")" && g++ -E -P -xc++ -I src/Partix/Shader/ "$file" -o "build/Shader/${file#src/Partix/Shader/}"' \;
find "$SHADER_DIR" \( -name "*.comp" -o -name "*.vert" -o -name "*.geom" -o -name "*.frag" \) -exec sh -c '
    file="{}"
    # 计算相对路径（剥离源目录前缀）
    rel_path="${file#'"$SHADER_DIR"'}"
    # 创建目标目录
    mkdir -p "'"$OUTPUT_DIR"'/Shader/$(dirname "$rel_path")"
    # 执行预处理
    g++ -E -P -xc++ -I "'"$SHADER_DIR"'" "$file" -o "'"$OUTPUT_DIR"'/Shader/$rel_path"
' _ {} \;
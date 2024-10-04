# コンパイラの設定
CC = icc
CXX = icpc

# ディレクトリ設定
BUILD_DIR = build

# ソースファイル
SRCS = t4_amx.c t4_avx512.c

# オブジェクトファイル (build ディレクトリ内)
OBJS = $(SRCS:.c=.o)
BUILD_OBJS = $(patsubst %.o,$(BUILD_DIR)/%.o,$(OBJS))

# バイナリ出力ファイル名
TARGETS = build/t4_amx t4_avx512

# デフォルトのターゲット
all: $(TARGETS)

# 各ターゲットのルール
build/t4_amx: $(BUILD_DIR)/t4_amx.o
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $(BUILD_DIR)/t4_amx.o

# オブジェクトファイルのルール
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) -c $< -o $@

# クリーンターゲット
clean:
	rm -rf $(BUILD_DIR)

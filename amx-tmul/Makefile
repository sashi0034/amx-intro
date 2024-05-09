# コンパイラの設定
CC = icc

# ディレクトリ設定
BUILD_DIR = build

# ソースファイル
SRCS = t1.c t2.c

# オブジェクトファイル (build ディレクトリ内)
OBJS = $(SRCS:.c=.o)
BUILD_OBJS = $(patsubst %.o,$(BUILD_DIR)/%.o,$(OBJS))

# バイナリ出力ファイル名
TARGETS = $(BUILD_DIR)/t1 $(BUILD_DIR)/t2

# デフォルトのターゲット
all: $(TARGETS)

# 各ターゲットのルール
$(BUILD_DIR)/t1: $(BUILD_DIR)/t1.o
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $(BUILD_DIR)/t1.o

$(BUILD_DIR)/t2: $(BUILD_DIR)/t2.o
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $(BUILD_DIR)/t2.o

# オブジェクトファイルのルール
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@

# クリーンターゲット
clean:
	rm -rf $(BUILD_DIR)

# コンパイラの設定
CC = icc
CXX = icpc

# ディレクトリ設定
BUILD_DIR = build

# ソースファイル
SRCS = t1.c t2.c t3.cpp t4.c

# オブジェクトファイル (build ディレクトリ内)
OBJS = $(SRCS:.c=.o)
BUILD_OBJS = $(patsubst %.o,$(BUILD_DIR)/%.o,$(OBJS))

# バイナリ出力ファイル名
TARGETS = build/t1 build/t2 build/t3

# デフォルトのターゲット
all: $(TARGETS)

# 各ターゲットのルール
build/t1: $(BUILD_DIR)/t1.o
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $(BUILD_DIR)/t1.o

build/t2: $(BUILD_DIR)/t2.o
	@mkdir -p $(BUILD_DIR)
	$(CC) -o $@ $(BUILD_DIR)/t2.o

build/t3: $(BUILD_DIR)/t3.o
	@mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ $(BUILD_DIR)/t3.o

build/t4: $(BUILD_DIR)/t4.o
	@mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ $(BUILD_DIR)/t4.o

build/t5: $(BUILD_DIR)/t5.o
	@mkdir -p $(BUILD_DIR)
	$(CXX) -o $@ $(BUILD_DIR)/t5.o

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

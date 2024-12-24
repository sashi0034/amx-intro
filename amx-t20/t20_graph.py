import os
import re
import matplotlib.pyplot as plt


# extract_result_from_data 関数の定義
def extract_result_from_data(file, fn_name):
    with open(file, "r") as file:
        for line in file:
            if line.startswith(fn_name):
                elements = line.split()
                if len(elements) > 1:
                    try:
                        return float(elements[1].rstrip("s"))
                    except ValueError:
                        print(f"Cannot convert '{elements[1]}' to float.")
                        return 0
    return 0


# メイン処理
if __name__ == "__main__":
    # データ収集用ディレクトリとファイル名の正規表現
    data_dir = "data"
    amx_pattern = re.compile(r"data_amx_(\d+)\.txt")
    avx512_pattern = re.compile(r"data_avx512_(\d+)\.txt")
    avx2_pattern = re.compile(r"data_avx2_(\d+)\.txt")
    sse42_pattern = re.compile(r"data_sse42_(\d+)\.txt")

    # データを格納する辞書
    test_cases = {}
    amx_times = []
    avx512_times = []
    avx2_items = []
    sse42_items = []

    # ディレクトリ内のファイルを列挙してテストケース番号と実行時間を収集
    for filename in os.listdir(data_dir):
        filepath = os.path.join(data_dir, filename)
        case_number = None

        if amx_pattern.match(filename):
            case_number = int(amx_pattern.match(filename).group(1))
            test_cases.setdefault(case_number, {})["AMX"] = extract_result_from_data(filepath, "compute_dot_products")
        elif avx512_pattern.match(filename):
            case_number = int(avx512_pattern.match(filename).group(1))
            test_cases.setdefault(case_number, {})["AVX-512"] = extract_result_from_data(filepath, "dot_product")
        elif avx2_pattern.match(filename):
            case_number = int(avx2_pattern.match(filename).group(1))
            test_cases.setdefault(case_number, {})["AVX2"] = extract_result_from_data(filepath, "dot_product")
        elif sse42_pattern.match(filename):
            case_number = int(sse42_pattern.match(filename).group(1))
            test_cases.setdefault(case_number, {})["SSE4.2"] = extract_result_from_data(filepath, "dot_product")

    # 各テストケースのデータをリストにまとめる
    sorted_cases = sorted(test_cases.keys())
    for case in sorted_cases:
        amx_times.append(test_cases[case].get("AMX", 0))
        avx512_times.append(test_cases[case].get("AVX-512", 0))
        avx2_items.append(test_cases[case].get("AVX2", 0))
        sse42_items.append(test_cases[case].get("SSE4.2", 0))

    # グラフ作成
    plt.figure(figsize=(10, 6))
    plt.plot(sorted_cases, amx_times, marker='o', label="AMX")
    plt.plot(sorted_cases, avx512_times, marker='o', label="AVX-512")
    plt.plot(sorted_cases, avx2_items, marker='o', label="AVX2")
    plt.plot(sorted_cases, sse42_items, marker='o', label="SSE4.2")

    # グラフの装飾
    plt.xlabel("Test Case Number")
    plt.ylabel("Execution Time (seconds)")
    plt.title("Execution Time Comparison for AMX, AVX-512, AVX2, and SSE4.2")
    plt.legend()
    plt.grid(True)

    # 画像の保存
    plt.savefig("execution_time_comparison.png")

    # グラフの表示
    plt.show()

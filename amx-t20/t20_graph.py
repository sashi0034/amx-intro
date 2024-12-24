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
    noamx_pattern = re.compile(r"data_noamx_(\d+)\.txt")

    # データを格納する辞書
    test_cases = {}
    amx_times = []
    avx512_times = []
    noamx_times = []

    # ディレクトリ内のファイルを列挙してテストケース番号と実行時間を収集
    for filename in os.listdir(data_dir):
        filepath = os.path.join(data_dir, filename)
        case_number = None

        if amx_pattern.match(filename):
            case_number = int(amx_pattern.match(filename).group(1))
            test_cases.setdefault(case_number, {})["AMX"] = extract_result_from_data(filepath, "compute_all_tests")
        elif avx512_pattern.match(filename):
            case_number = int(avx512_pattern.match(filename).group(1))
            test_cases.setdefault(case_number, {})["AVX512"] = extract_result_from_data(filepath, "compute_dot_product")
        elif noamx_pattern.match(filename):
            case_number = int(noamx_pattern.match(filename).group(1))
            test_cases.setdefault(case_number, {})["No-AMX"] = extract_result_from_data(filepath, "compute_dot_product")

    # 各テストケースのデータをリストにまとめる
    sorted_cases = sorted(test_cases.keys())
    for case in sorted_cases:
        amx_times.append(test_cases[case].get("AMX", 0))
        avx512_times.append(test_cases[case].get("AVX512", 0))
        noamx_times.append(test_cases[case].get("No-AMX", 0))

    # グラフ作成
    plt.figure(figsize=(10, 6))
    plt.plot(sorted_cases, amx_times, marker='o', label="AMX")
    plt.plot(sorted_cases, avx512_times, marker='o', label="AVX512")
    plt.plot(sorted_cases, noamx_times, marker='o', label="No-AMX")

    # グラフの装飾
    plt.xlabel("Test Case Number")
    plt.ylabel("Execution Time (seconds)")
    plt.title("Execution Time Comparison for AMX, AVX512, and No-AMX")
    plt.legend()
    plt.grid(True)

    # 画像の保存
    plt.savefig("execution_time_comparison.png")

    # グラフの表示
    plt.show()

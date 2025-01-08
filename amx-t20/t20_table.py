import os
import re
import csv


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

    # 各テストケースのデータを標準出力
    sorted_cases = sorted(test_cases.keys())

    # ヘッダーの出力
    print("Size, AMX, AVX2, SSE4.2, AVX-512")

    # データの出力
    for case in sorted_cases:
        print(", ".join(map(str, [
            case,
            test_cases[case].get("AMX", 0),
            test_cases[case].get("AVX2", 0),
            test_cases[case].get("SSE4.2", 0),
            test_cases[case].get("AVX-512", 0)
        ])))

    # AMX と AVX2 の比率を計算して出力
    amx_sum = 0
    avx2_sum = 0
    count = 0
    for case in sorted_cases:
        amx_time = test_cases[case].get("AMX", 0)
        avx2_time = test_cases[case].get("AVX2", 0)
        if amx_time > 0 and avx2_time > 0:
            amx_sum += amx_time
            avx2_sum += avx2_time
            count += 1
    if count > 0:
        ratio = amx_sum / avx2_sum
        print(f"\nAverage AMX to AVX2 ratio: {ratio:.3f} ({1.0 / ratio:.3})")
    else:
        print("\nNo valid data for AMX and AVX2 ratio calculation.")

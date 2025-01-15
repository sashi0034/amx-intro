import os
import re


def calculate_execution_time(file_path):
    """
    指定されたテキストファイルから
      Elapsed Time: {数値}s
      init_all_tests_from_buffer {数値}s
      check_result_validation {数値}s
      fread {数値}s
    を正規表現で取得し、execution_time = 上記を用いて計算して返す。
    """
    # 正規表現パターンのコンパイル
    pattern_elapsed = re.compile(r'Elapsed\s*Time:\s*([\d\.]+)\s*s')
    pattern_init = re.compile(r'init_all_tests_from_buffer\s*([\d\.]+)\s*s')
    pattern_check = re.compile(r'check_result_validation\s*([\d\.]+)\s*s')
    pattern_fread = re.compile(r'fread\s*([\d\.]+)\s*s')

    elapsed_time = None
    init_time = 0.0
    check_time = 0.0
    fread_time = 0.0

    with open(file_path, encoding='utf-8') as f:
        for line in f:
            if elapsed_time is None:
                match_elapsed = pattern_elapsed.search(line)
                if match_elapsed:
                    elapsed_time = float(match_elapsed.group(1))
            # 以下3つは初回マッチだけを想定
            if init_time == 0.0:
                match_init = pattern_init.search(line)
                if match_init:
                    init_time = float(match_init.group(1))
            if check_time == 0.0:
                match_check = pattern_check.search(line)
                if match_check:
                    check_time = float(match_check.group(1))
            if fread_time == 0.0:
                match_fread = pattern_fread.search(line)
                if match_fread:
                    fread_time = float(match_fread.group(1))

    # いずれかが未取得だった場合は None のまま
    if elapsed_time is None:
        return None

    execution_time = elapsed_time - init_time - check_time - fread_time
    return execution_time


def get_txt_files_in_data20():
    """
    data20 ディレクトリ内の .txt ファイルパスを一覧で取得し、
    名前順でソートして返す。
    """
    dir_name = 'data20'
    result = []
    if not os.path.isdir(dir_name):
        return result

    for file_name in os.listdir(dir_name):
        if file_name.endswith('.txt'):
            result.append(os.path.join(dir_name, file_name))

    # ファイル名でソート(フルパスではなくbasenameでソートしたい場合は分けて処理)
    # ここではフルパスのリストを「ファイル名の名前順」にソート
    result.sort(key=lambda path: os.path.basename(path))
    return result


def main():
    txt_files = get_txt_files_in_data20()

    for file_path in txt_files:
        execution_time = calculate_execution_time(file_path)
        # ファイル名 (拡張子抜き) を取得
        base_name = os.path.splitext(os.path.basename(file_path))[0]

        if execution_time is not None:
            print(f"{base_name}, {execution_time}")
        else:
            print(f"{base_name}, Not Found")


if __name__ == "__main__":
    main()

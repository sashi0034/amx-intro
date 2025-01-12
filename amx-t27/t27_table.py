import os


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


data_dir = "data"

if __name__ == "__main__":
    content = ""

    for filename in os.listdir(data_dir):
        filepath = os.path.join(data_dir, filename)
        content += f"{filename} & {extract_result_from_data(filepath, 'convolution')}s\n"

    print(content)

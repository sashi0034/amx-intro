import os
import subprocess
import glob
import shutil

# Define test cases
# test_cases = [
#     100000,
#     200000,
#     400000,
#     800000,
#     1600000,
#     3200000,
#     6400000,
#     12800000,
# ]

# test_cases = [100000 + 990000 * (i + 1) for i in range(10)]

# test_cases = [13000000, 13500000]

# test_cases = [14000000, 14500000, 15000000]

test_cases = [16000000, 17000000, 18000000, 19000000, 20000000]


def clear_results_directory():
    results_dir = "./results"
    if os.path.exists(results_dir):
        shutil.rmtree(results_dir)
    os.makedirs(results_dir, exist_ok=True)


def run_command(command):
    print(f"Executing: {command}")
    subprocess.run(command, shell=True, check=True)


def find_latest_result_dir(base_name):
    # Find the directory matching `base_name` with the highest suffix
    dirs = glob.glob(f"{base_name}*")
    if dirs:
        latest_dir = max(dirs, key=os.path.getctime)
        return latest_dir
    else:
        raise FileNotFoundError(f"No result directory found matching {base_name}")


if __name__ == '__main__':
    # Clear the results directory
    clear_results_directory()

    for cases in test_cases:
        # 1. Compile and run t7_gen with each test case
        gen_command = f"icpc -O3 t7_gen.cpp -o build/t7_gen && build/t7_gen {cases}"
        run_command(gen_command)

        # 2. Run the VTune profiling for t7_amx.c
        amx_command = (
            f"icc -g -O3 t7_amx.c -o build/t7_amx && "
            f"module load intel-vtune && "
            f"tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./results/result_amx_{cases} build/t7_amx"
        )
        run_command(amx_command)
        result_amx_dir = find_latest_result_dir(f"./results/result_amx_{cases}")
        amx_report_command = (
            f"module load intel-vtune && "
            f"vtune -report hotspots -result-dir {result_amx_dir} -format text -filter compute_all_tests > data/data_amx_{cases}.txt"
        )
        run_command(amx_report_command)

        # 3. Run the VTune profiling for AVX512 in t7_noamx.c
        avx512_command = (
            f"icc -g -xCORE-AVX512 -qopt-zmm-usage=high -O3 t7_noamx.c -o build/t7_avx512 && "
            f"module load intel-vtune && "
            f"tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./results/result_avx512_{cases} build/t7_avx512"
        )
        run_command(avx512_command)
        result_avx512_dir = find_latest_result_dir(f"./results/result_avx512_{cases}")
        avx512_report_command = (
            f"module load intel-vtune && "
            f"vtune -report hotspots -result-dir {result_avx512_dir} -format text -filter compute_all_tests > data/data_avx512_{cases}.txt"
        )
        run_command(avx512_report_command)

        # 4. Run the VTune profiling for NoAMX in t7_noamx.c
        noamx_command = (
            f"icc -g -O3 t7_noamx.c -o build/t7_noamx && "
            f"module load intel-vtune && "
            f"tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./results/result_noamx_{cases} build/t7_noamx"
        )
        run_command(noamx_command)
        result_noamx_dir = find_latest_result_dir(f"./results/result_noamx_{cases}")
        noamx_report_command = (
            f"module load intel-vtune && "
            f"vtune -report hotspots -result-dir {result_noamx_dir} -format text -filter compute_all_tests > data/data_noamx_{cases}.txt"
        )
        run_command(noamx_report_command)

        print(f"Completed loop for cases = {cases}")

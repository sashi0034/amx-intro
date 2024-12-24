import os
import subprocess
import glob
import shutil

# Define test cases
test_cases = [
    # 100000,
    # 200000,
    # 400000,
    # 800000,
    # 1600000,
    # 3200000,
    6400000,
    # 12800000,
]


# test_cases = [100000 + 990000 * (i + 1) for i in range(10)]

# test_cases = [13000000, 13500000]

# test_cases = [14000000, 14500000, 15000000]

# test_cases = [16000000, 17000000, 18000000, 19000000, 20000000]


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


def run_vtune_profile(label, compile):
    build = f"build/t20_{label}"
    run_command(
        compile + f" -o {build} && "
                  f"module load intel-vtune && "
                  f"tssrun -p gr10034a --rsc m=50G vtune -collect hotspots -r=./results/result_{label}_{cases} {build}")
    result_dir = find_latest_result_dir(f"./results/result_{label}_{cases}")
    report_command = (
        f"module load intel-vtune && "
        f"vtune -report hotspots -result-dir {result_dir} -format text -filter compute_dot_products > data/data_{label}_{cases}.txt"
    )
    run_command(report_command)


if __name__ == '__main__':
    # Clear the results directory
    clear_results_directory()

    for cases in test_cases:
        # 1. Compile and run t20_generate with each test case
        gen_command = f"icpc -O3 t20_generate.cpp -o build/t20_generate && build/t20_generate {cases}"
        run_command(gen_command)

        # 2. Run the VTune profiling
        run_vtune_profile(
            "amx",
            f"icc -g -O3 t20_amx.c"
        )

        run_vtune_profile(
            "avx512",
            f"icc -g -xCOMMON-AVX512 -O3 t20_avx.c"
        )

        run_vtune_profile(
            "avx2",
            f"icc -g -xAVX2 -O3 t20_avx.c"
        )

        run_vtune_profile(
            "sse42",
            f"icc -g -xSSE4.2 -O3 t20_avx.c"
        )

        print(f"Completed loop for cases = {cases}")

import os
import subprocess
import glob
import shutil


def clear_results_directory():
    results_dir = "./results1"
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


def run_vtune_profile(label, compile_cmd):
    build = f"build/t27_{label}"
    run_command(
        compile_cmd + f" -o {build} && "
                      f"module load intel-vtune && "
                      f"tssrun -p gr10034a --rsc m=1G vtune -collect hotspots -r=./results1/result_{label} {build}")
    result_dir = find_latest_result_dir(f"./results1/result_{label}")
    report_command = (
        f"module load intel-vtune && "
        f"vtune -report hotspots -result-dir {result_dir} -format text -filter convolution > data/data_{label}.txt"
    )
    run_command(report_command)


if __name__ == '__main__':
    clear_results_directory()

    run_vtune_profile(
        "amx_row1",
        f"icc -g -O3 t27_amx.c"
    )

    run_vtune_profile(
        "amx_row1_tile3",
        f"icc -g -O3 t27_amx_o2.c"
    )

    run_vtune_profile(
        "amx_row16",
        f"icc -g -O3 t27_amx_o3.c"
    )

    run_vtune_profile(
        "amx_row16_tile3",
        f"icc -g -O3 t27_amx_o3.c"
    )

    run_vtune_profile(
        "avx512",
        f"icc -g -O3 t27_naive.c -xCOMMON-AVX512"
    )

    run_vtune_profile(
        "avx2",
        f"icc -g -O3 t27_naive.c -xAVX2"
    )

    run_vtune_profile(
        "sse42",
        f"icc -g -O3 t27_naive.c -xSSE4.2"
    )

    print(f"Completed profiling.")

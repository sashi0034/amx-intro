import os
import subprocess


def run_command(command):
    # Executes a shell command and prints it.
    print(f"Executing: {command}")
    subprocess.run(command, shell=True, check=True)


if __name__ == "__main__":
    results_dir = "results20"
    data_dir = "data20"

    # Ensure the output directory exists
    os.makedirs(data_dir, exist_ok=True)

    # Get the list of directories in results20
    directories = [d for d in os.listdir(results_dir) if os.path.isdir(os.path.join(results_dir, d))]

    for dir_name in directories:
        result_path = os.path.join(results_dir, dir_name)
        output_file = os.path.join(data_dir, f"{dir_name}.txt")

        # Build the command
        command = (f"module load intel-vtune && "
                   f"vtune -result-dir {result_path} -report summary > {output_file} &&"
                   f"vtune -result-dir {result_path} -report callstacks -group-by function >> {output_file}")

        # Execute the command
        run_command(command)

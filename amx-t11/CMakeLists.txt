
add_executable(t11_mhd_amx t11_task_amx.c t11_mhd.c t11_sim.c)

add_executable(t11_mhd_gpu t11_task_gpu.c t11_mhd.c t11_sim.c)

add_executable(t11_mhd_naive t11_task_naive.c t11_sim.c)

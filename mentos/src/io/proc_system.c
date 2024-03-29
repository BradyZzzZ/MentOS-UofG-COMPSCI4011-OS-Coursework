/// @file proc_system.c
/// @brief Contains callbacks for procfs system files.
/// @copyright (c) 2014-2024 This file is distributed under the MIT License.
/// See LICENSE.md for details.

#include "fs/procfs.h"
#include "hardware/timer.h"
#include "io/debug.h"
#include "process/process.h"
#include "stdio.h"
#include "string.h"
#include "sys/errno.h"
#include "version.h"
// * Task 1: Add cpuid.h
#include "hardware/cpuid.h"
// * Task 2: Add scheduler.h
#include "process/scheduler.h"

// * Task 2: Define functions to calculate integer and decimal parts separately for loadavg
#define LOAD_INT(x) ((x) >> FSHIFT)
#define LOAD_FRAC(x) LOAD_INT(((x) & (FIXED_1-1)) * 100)

static ssize_t procs_do_uptime(char *buffer, size_t bufsize);

static ssize_t procs_do_version(char *buffer, size_t bufsize);

static ssize_t procs_do_mounts(char *buffer, size_t bufsize);

static ssize_t procs_do_cpuinfo(char *buffer, size_t bufsize);

static ssize_t procs_do_meminfo(char *buffer, size_t bufsize);

static ssize_t procs_do_stat(char *buffer, size_t bufsize);

// * Task 2: Add a new function for loadavg
static ssize_t procs_do_loadavg(char *buffer, size_t bufsize);

static ssize_t __procs_read(vfs_file_t *file, char *buf, off_t offset, size_t nbyte)
{
    if (!file) {
        pr_err("We received a NULL file pointer.\n");
        return -EFAULT;
    }
    proc_dir_entry_t *entry = (proc_dir_entry_t *)file->device;
    if (entry == NULL) {
        pr_err("The file is not a valid proc entry.\n");
        return -EFAULT;
    }
    // Prepare a buffer.
    char buffer[BUFSIZ];
    memset(buffer, 0, BUFSIZ);
    // Call the specific function.
    int ret = 0;
    if (strcmp(entry->name, "uptime") == 0) {
        ret = procs_do_uptime(buffer, BUFSIZ);
    } else if (strcmp(entry->name, "version") == 0) {
        ret = procs_do_version(buffer, BUFSIZ);
    } else if (strcmp(entry->name, "mounts") == 0) {
        ret = procs_do_mounts(buffer, BUFSIZ);
    } else if (strcmp(entry->name, "cpuinfo") == 0) {
        ret = procs_do_cpuinfo(buffer, BUFSIZ);
    } else if (strcmp(entry->name, "meminfo") == 0) {
        ret = procs_do_meminfo(buffer, BUFSIZ);
    } else if (strcmp(entry->name, "stat") == 0) {
        ret = procs_do_stat(buffer, BUFSIZ);
    } else if (strcmp(entry->name, "loadavg") == 0) {
        // * Task 2: The new function for loadavg
        ret = procs_do_loadavg(buffer, BUFSIZ); 
    }
    // Perform read.
    ssize_t it = 0;
    if (ret == 0) {
        size_t name_len = strlen(buffer);
        size_t read_pos = offset;
        if (read_pos < name_len) {
            while ((it < nbyte) && (read_pos < name_len)) {
                *buf++ = buffer[read_pos];
                ++read_pos;
                ++it;
            }
        }
    }
    return it;
}

/// Filesystem general operations.
static vfs_sys_operations_t procs_sys_operations = {
    .mkdir_f   = NULL,
    .rmdir_f   = NULL,
    .stat_f    = NULL,
    .creat_f   = NULL,
    .symlink_f = NULL,
};

/// Filesystem file operations.
static vfs_file_operations_t procs_fs_operations = {
    .open_f     = NULL,
    .unlink_f   = NULL,
    .close_f    = NULL,
    .read_f     = __procs_read,
    .write_f    = NULL,
    .lseek_f    = NULL,
    .stat_f     = NULL,
    .ioctl_f    = NULL,
    .getdents_f = NULL,
    .readlink_f = NULL,
};

int procs_module_init(void)
{
    proc_dir_entry_t *system_entry;
    // == /proc/uptime ========================================================
    if ((system_entry = proc_create_entry("uptime", NULL)) == NULL) {
        pr_err("Cannot create `/proc/uptime`.\n");
        return 1;
    }
    pr_debug("Created `/proc/uptime` (%p)\n", system_entry);
    // Set the specific operations.
    system_entry->sys_operations = &procs_sys_operations;
    system_entry->fs_operations  = &procs_fs_operations;

    // == /proc/version ========================================================
    if ((system_entry = proc_create_entry("version", NULL)) == NULL) {
        pr_err("Cannot create `/proc/version`.\n");
        return 1;
    }
    pr_debug("Created `/proc/version` (%p)\n", system_entry);
    // Set the specific operations.
    system_entry->sys_operations = &procs_sys_operations;
    system_entry->fs_operations  = &procs_fs_operations;

    // == /proc/mounts ========================================================
    if ((system_entry = proc_create_entry("mounts", NULL)) == NULL) {
        pr_err("Cannot create `/proc/mounts`.\n");
        return 1;
    }
    pr_debug("Created `/proc/mounts` (%p)\n", system_entry);
    // Set the specific operations.
    system_entry->sys_operations = &procs_sys_operations;
    system_entry->fs_operations  = &procs_fs_operations;

    // == /proc/cpuinfo ========================================================
    if ((system_entry = proc_create_entry("cpuinfo", NULL)) == NULL) {
        pr_err("Cannot create `/proc/cpuinfo`.\n");
        return 1;
    }
    pr_debug("Created `/proc/cpuinfo` (%p)\n", system_entry);
    // Set the specific operations.
    system_entry->sys_operations = &procs_sys_operations;
    system_entry->fs_operations  = &procs_fs_operations;

    // == /proc/meminfo ========================================================
    if ((system_entry = proc_create_entry("meminfo", NULL)) == NULL) {
        pr_err("Cannot create `/proc/meminfo`.\n");
        return 1;
    }
    pr_debug("Created `/proc/meminfo` (%p)\n", system_entry);
    // Set the specific operations.
    system_entry->sys_operations = &procs_sys_operations;
    system_entry->fs_operations  = &procs_fs_operations;

    // == /proc/stat ========================================================
    if ((system_entry = proc_create_entry("stat", NULL)) == NULL) {
        pr_err("Cannot create `/proc/stat`.\n");
        return 1;
    }
    pr_debug("Created `/proc/stat` (%p)\n", system_entry);
    // Set the specific operations.
    system_entry->sys_operations = &procs_sys_operations;
    system_entry->fs_operations  = &procs_fs_operations;

    // * Task 2: Register the /proc/loadavg entry
    // == /proc/loadavg ========================================================
    if ((system_entry = proc_create_entry("loadavg", NULL)) == NULL) {
        pr_err("Cannot create `/proc/loadavg`.\n");
        return 1;
    }
    pr_debug("Created `/proc/loadavg` (%p)\n", system_entry);
    // Set the specific operations for loadavg.
    system_entry->sys_operations = &procs_sys_operations;
    system_entry->fs_operations  = &procs_fs_operations;
    
    return 0;
}

static ssize_t procs_do_uptime(char *buffer, size_t bufsize)
{
    sprintf(buffer, "%d", timer_get_seconds());
    return 0;
}

static ssize_t procs_do_version(char *buffer, size_t bufsize)
{
    sprintf(buffer,
            "%s version %s (site: %s) (email: %s)",
            OS_NAME,
            OS_VERSION,
            OS_SITEURL,
            OS_REF_EMAIL);
    return 0;
}

static ssize_t procs_do_mounts(char *buffer, size_t bufsize)
{
    return 0;
}

// * Task 1: Hook up to procs_do_cpuinfo function
static ssize_t procs_do_cpuinfo(char *buffer, size_t bufsize)
{
    get_cpuid(&sinfo);
    sprintf(buffer,
             "Vendor ID: %s\n"
             "CPU Type: %s\n"
             "CPU Family: %u\n"
             "CPU Model: %u\n"
             "APIC ID: %u\n"
             "Brand String: %s\n",
             sinfo.cpu_vendor, sinfo.cpu_type, sinfo.cpu_family,
             sinfo.cpu_model, sinfo.apic_id, sinfo.brand_string);

    return 0;
}

static ssize_t procs_do_meminfo(char *buffer, size_t bufsize)
{
    double total_space = get_zone_total_space(GFP_KERNEL) +
                         get_zone_total_space(GFP_USER),
           free_space = get_zone_free_space(GFP_KERNEL) +
                        get_zone_free_space(GFP_USER),
           cached_space = get_zone_cached_space(GFP_KERNEL) +
                          get_zone_cached_space(GFP_USER),
           used_space = total_space - free_space;
    total_space /= (double)K;
    free_space /= (double)K;
    cached_space /= (double)K;
    used_space /= (double)K;
    sprintf(
        buffer,
        "MemTotal : %12.2f Kb\n"
        "MemFree  : %12.2f Kb\n"
        "MemUsed  : %12.2f Kb\n"
        "Cached   : %12.2f Kb\n",
        total_space, free_space, used_space, cached_space);
    return 0;
}

static ssize_t procs_do_stat(char *buffer, size_t bufsize)
{
    return 0;
}

// * Task 2: Add a new function for loadavg
static ssize_t procs_do_loadavg(char *buffer, size_t bufsize) 
{
    // Get the loadavg number with fixed-point and scaling
    size_t load1 = scheduler_get_loadavg_1min() * (FIXED_1/200);
    size_t load5 = scheduler_get_loadavg_5min()* (FIXED_1/200);
    size_t load15 = scheduler_get_loadavg_15min()* (FIXED_1/200);

    // Get the number of running processes and total processes
    int running_tasks = scheduler_get_running_processes();
    int total_tasks = scheduler_get_total_processes();

    // Get the last pid
    pid_t last_pid = get_last_pid();

    sprintf(buffer, "%d.%02d %d.%02d %d.%02d %d/%d %d\n",
                       LOAD_INT(load1), LOAD_FRAC(load1),
                       LOAD_INT(load5), LOAD_FRAC(load5),
                       LOAD_INT(load15), LOAD_FRAC(load15), 
                       running_tasks, total_tasks, last_pid);

    return 0;
}

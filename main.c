#include "main.h"
#include "libconf.h"

int get_process_pid(char *proces_name)
{
    char bufer[PATH_SIZE];
    char comm[PATH_SIZE];
    char proc_comm_name[PATH_SIZE];
    int num[PATH_SIZE] = { 0 };
    int n = 0;
    FILE *fp;
    DIR *dir;
    struct dirent *ptr;
    dir = opendir("/proc");
    while ((ptr = readdir(dir)) != NULL) {
        if (ptr->d_type == 4 && strcasecmp(ptr->d_name, ".") && strcasecmp(ptr->d_name, "..")) {
            bzero(bufer, 0);
            sprintf(comm, "/proc/%s/comm", ptr->d_name);
            if (access(comm, F_OK) == 0) {
                fp = fopen(comm, "r");
                if (fgets(bufer, PATH_SIZE - 1, fp) == NULL) {
                    fclose(fp);
                    continue;
                }
                sscanf(bufer, "%s", proc_comm_name);
                if (!strcmp(proces_name, proc_comm_name)) {
                    num[n] = atoi(ptr->d_name);
                    n += 1;
                }
                fclose(fp);
            }
        }

    }

    n -= 1;                     // 去除最后一个搜索时的本身进程
    closedir(dir);
    if (num[0] > 0)
        return num[0];
    else
        return 0;
}

char *times()
{
    time_t t;
    struct tm *timeinfo;
    time(&t);
    timeinfo = localtime(&t);
    return asctime(timeinfo);
}

int logs(char *str, char *configfile)
{
    FILE *fp = NULL;
    fp = fopen(read_conf(configfile, "global", "LOGFILE"), "a+");
    fprintf(fp, str);
    return fclose(fp);
}

int loop(char *configfile)
{
    FILE *fp;
    char buffer[PATH_SIZE];
    while (1) {
        if (get_process_pid(read_conf(configfile, "global", "PROCESS_NAME")) <= 0) {
            logs(times(), configfile);
            logs("没有运行\n", configfile);
            fp = _popen(read_conf(configfile, "global", "COMMAND"), "r");

            logs(times(), configfile);
            logs("执行结果\n", configfile);
            while (fgets(buffer, sizeof(buffer), fp)) {
                //printf("%s", buffer);
                logs(buffer, configfile);
            }
            _pclose(fp);
        } else {
            logs(times(), configfile);
            logs("运行\n", configfile);
        }
        sleep(atoi(read_conf(configfile, "global", "TIME")));
    }
}

int main(int argc, char **argv, char **env)
{
    char configfile[PATH_SIZE];
    int opt;
    
    memset(configfile, 0, PATH_SIZE);
    while ((opt = getopt(argc, argv, "c:")) != -1) {
        switch (opt) {
            case 'c':
                strcpy(configfile, optarg);
                break;
                
            default:
                break;
        }
    }
    
    if (strlen(configfile) == 0) {
        strcpy(configfile, "daemon.conf");
    }
    printf("%s\n", configfile);
    
    if (daemon(1, 1)) {
        perror("daemon");
        return 1;
    }

    loop(configfile);

    return 0;
}

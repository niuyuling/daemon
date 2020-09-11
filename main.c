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
    struct tm *timeinfo;        //结构体
    time(&t);
    timeinfo = localtime(&t);
    return asctime(timeinfo);   //以字符串形式输出localtime本地时间
}

int logs(char *str)
{
    FILE *fp = NULL;
    fp = fopen(read_conf("daemon.conf", "global", "LOFFILE"), "a+");
    fprintf(fp, str);
    return fclose(fp);
}

int loop()
{
    FILE *fp;
    char buffer[PATH_SIZE];
    while (1) {
        if (get_process_pid(read_conf("daemon.conf", "global", "PROCESS_NAME")) <= 0) {
            logs(times());
            logs("没有运行\n");
            fp = _popen(read_conf("daemon.conf", "global", "COMMAND"), "r");

            logs(times());
            logs("执行结果\n");
            while (fgets(buffer, sizeof(buffer), fp)) {
                //printf("%s", buffer);
                logs(buffer);
            }
            _pclose(fp);
        } else {
            logs(times());
            logs("运行\n");
        }
        sleep(atoi(read_conf("daemon.conf", "global", "TIME")));
    }
}

int main(int argc, char **argv)
{
    if (daemon(1, 1)) {
        perror("daemon");
        return 1;
    }

    loop();

    return 0;
}

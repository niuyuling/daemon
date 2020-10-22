### Daemon
    根据程序名称和时间为判断条件的进程守护程序.  
    配置文件默认 daemon.conf  
    global {
        PROCESS_NAME = "frpc";      //进程名称条件变量
        COMMAND = "ls -al";         //进程名称条件变量不成立执行的命令
        TIME = "10";                //时间条件变量, 单位秒
        LOGFILE = "log.txt";        //日志记录
    }

### Build
    Linux编译:  
    git clone https://github.com/niuyuling/daemon.git
    git clone https://github.com/niuyuling/libconf.git
    cd libconf
    make clean; make
    cd ../daemon
    make clean; make

### Help
    #启动
    ./daemon
    #关闭
    killall daemon

### Linux多线程

**Linux 0 1 2 号进程**

- idle进程， 系统创建的第一个进程，加载系统
- systemd， 系统初始化，类似进程的根节点
- kthreadd，负责所有内核线程的调度、管理

**fork函数**

- 一个当前存在的进程调用函数fork创建新的进程
- fork调用一次，执行两次
  - 子进程返回0，父进程返回子进程的PID
  - 注意子进程和父进程共享的资源，忘记了看OS
  - 子进程是父进程的一个副本
  - 子进程拥有父进程的资源副本（堆、栈，数据区等）

### 守护进程

ipcs -m 查看共享内存段

ipcrm -m 编号       删除

man shmget    创建、获取共享内存

```
   #include <sys/ipc.h>
   #include <sys/shm.h>

   int shmget(key_t key, size_t size, int shmflg);
```

man shmdt    连接到共享内存

       #include <sys/types.h>
       #include <sys/shm.h>
    
       void *shmat(int shmid, const void *shmaddr, int shmflg);
    
       int shmdt(const void *shmaddr);

man shmctl    删除

       #include <sys/ipc.h>
       #include <sys/shm.h>
    
       int shmctl(int shmid, int cmd, struct shmid_ds *buf);

### Linux信号量

ipcs -s   查看信号量

ipcrm sem  编号  删除信号量

man semget		获取信号量

       #include <sys/types.h>
       #include <sys/ipc.h>
       #include <sys/sem.h>
    
       int semget(key_t key, int nsems, int semflg);

man semctl

       #include <sys/types.h>
       #include <sys/ipc.h>
       #include <sys/sem.h>
    
       int semctl(int semid, int semnum, int cmd, ...);



       #include <sys/ipc.h>
       #include <sys/shm.h>



###  调用操作系统的命令

man system

      #include <stdlib.h>
      int system(const char *command);

excel()、execv()

exec 的功能更强大



### 一些内容



exit函数和析构函数

- exit函数不会调用局部对象的析构函数
- exit函数会调用全局对象的析构函数
- return会调用局部和全局变量
- 在编写checkproc.cpp的时候使用book.cpp 测试   CPActive在局部声明并调用，退出进程，程序并没有结束。使用全局会退出

### 函数说明

crtsurfdata.cpp 		服务程序，生成模拟测试数据

procctl.cpp   服务程序调度

learn.cpp   共享内存的使用

PV.cpp    	PV操作来互斥，该功能封装成了类PActive   放在_puhlic.cpp  中

checkproc.cpp   处理服务程序超时

book.cpp		测试服务程序

gzipfiles.cpp		打包程序

seart.sh	启动脚本

killall.sh	停止脚本



ftpgetfiles.cpp		文件传输系统

ftpgetfiles1.cpp 	完成基础架构解析xml、加载待获取文件目录+文件名.....

ftpgetfiles2.cpp  完成取文件操作

ftpgetfiles3.cpp   完成删除远程服务器文件，和转存备份

ftpgetfiles4.cpp     每次只下载新增和修改过的文件





i/o复用

select、 poll、 epoll 

### dairy



#### 1 实现不停机执行的服务程序

##### may-11 

crtsurfdata2.cpp    完成 stcode.ini 数据写入crtsurfdata2.log 日志

##### may-12

crtsurfdata3.cpp     完成  CrtSurfData()
- 研究时间函数

crtsurfdata4.cpp  完成   CrtSurfFile()   写入xsv文件

- 未完成写入格式  json，xml

##### may-15

crtsurfdata5.cpp 完成 json   xml格式写入

##### may-16

**linux多线程**

linux signal     64种   kill -l 

- 自定义信号
  - SIGUSR1  10
  - SIGUSR2  12
- 子程序结束信号
  - SIGCHLD

##### may-5-17

完成procctl.cpp   

一个调度程序，用来启动crtsurfdata.cpp  生产的测试数据

例如 每60秒生产一次       注意测试的时候要终止进程， 否则程序一直执行 会占满服务器空间

`~/project/tools1/bin/procctl 60 ~/project/idc1/bin/crtsurfdata5 ~/project/idc1/ini/stcode.ini ~/project/idc1/psurfdata ~/project/idc1/log/crtsurfdata5.log xml,json,csv`

**开始实现守护进程**

book.cpp

##### may-5-19

learn.cpp   使用共享变量， PV.cpp 实现最多1000个进程互斥使用共享变量



##### may 5-22

checkproc.cpp 完成     检测后台服务程序是否超时，若超时就终止。

checkproc.cpp 完成   处理服务程序超时

设计的目的是，用于守护进程中结束不必要占用共享内存的进程。

需要注意两点。

1.----------

exit函数不会调用局部对象的析构函数

exit函数会调用全局对象的析构函数

return会调用局部和全局变量

在编写checkproc.cpp的时候使用book.cpp 测试   CPActive在局部声明并调用，退出进程，程序并没有结束。使用全局会退

2.-----------

服务程序，使用了  CloseIOAndSignal(true);忽略全部的信号和IO，不希望程序被打扰，同时关闭IO资源，主要有标准输入、输出。因为程序最大支持1000个进程，会有一定的开销

book.cpp 用于测试crtsurfdata.cpp

##### may5-23

gzipfiles.cpp   完成 作为一个打包程序

使用的system 来执行系统的功能，之前使用的是execv来执行的

start.sh    killall.sh

注意  kill  调度程序  要放在服务程序上面先终止，否则周期太短可能还会启动。



##### 总结

信号量、守护进程、服务程序用法



#### 2. ftp文件传输系统





### 问题

tools1的history目录下的ftpgetfiles2.cpp

在获取远程服务的待下载文件名，写入到日志的时候将服务器的home目录也写进来了，这样使用文件名进行下载的时候会下载home中可以下载的所有内容。

目前在LoadListFile()函数中将home过滤了，这样不会下载

主要搞清楚为什么，获取下载文件名，会将home也获取到，怎么过滤？

**解决了**

问题是之前测试ftp的时候在存放目录添加了。

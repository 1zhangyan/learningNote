# Linux Common Commands
## find 查找文件数量
find [path] / -name "xxxx"
> [例子] 统计当前目录下以.c结尾的所有文件个数
> find . -name "*.c"|wc -l

## wc 
-l 只显示行数
-w 只显示字数
-c 只显示字节数

## kill 
kill -[Signal value] [pid]
|Signal Name|	Signal Value|	Effect|
|----|----|----|
|SIGHUP	|1	|挂起|
|SIGINT	|2	|键盘的中断信号|
|SIGKILL|	9	|发出杀死信号|
|SIGTERM|	15	|发出终止信号|
|SIGSTOP|	17, 19, 23	|停止进程|
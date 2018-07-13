# 相似性查询

### 使用说明

#### 函数部分：

这部分是两个函数的实现

直接将当前目录运行
```bash
$ USE_PGXS=1 make install
```


打开psql
执行create extension similarity;
请确保无其他重名函数存在！


若要卸载，
psql中执行drop extension similarity;
若要彻底卸载，执行
```bash
$ USE_PGXS=1 make uninstall
```


请确保postgresql的bin目录正确设置了环境变量

检查方式
```bash
$ pg_config
```
否则请把\usr\bin\psql\bin（取决与编译配置）文件夹添加到系统环境变量


#### 源码部分：


这部分将原有的nest loop join改为nest block join。


使用方法：

用execnodes.h替换src/include/nodes中的同名文件
用nodeNestloop.c替换替换src/backend/executor中的同名文件


nodeNestloop.c中的宏JC\_BLOCKSIZE定义的是blocksize的大小，
可自己修改！

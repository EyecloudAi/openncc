Python Demo：

**Note：$ 后面是命令，如果复制不需要复制$.**

1.安装Anaconda5.3.0

https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/Anaconda3-5.3.0-Linux-x86_64.sh

在下载完的路径里，使用命令行

```shell
$ bash Anaconda3-5.3.0-Linux-x86_64.sh
```

一直按回车，直到你需要输入yes or no的时候，要选择yes，后面会让你选择路径，默认按回车，后面还有一个yes，是询问你是否需要将Anaconda加入环境的，最后一个询问的时候看自己的需求。

2.可能这时候会存在一个问题，就是在~/.bashrc里面就会有Anaconda的环境变量了，环境变量里面可能会有多余的斜杠，需要你手动将他去掉，如果比如下面这样

```
\export PATH="/home/beam/anaconda3/bin:$PATH"
```

这时候需要去掉export前面的`\`。

完成修改后，需要进行下面这个命令行操作

```shell
source ~/.bashrc
```

3.这时候可以创建一个conda的虚拟环境，用下面的命令

```shell
conda create -n opencv_python python=3.7 # 这里的opencv_python只是一个名字，你可以自己取
```

创建完成后需要进入这个，用下面的命令

```shell
source activate opencv_python #这里的opencv_python就是你上面创建的
```

4.安装python-opencv

输入好上面这条后，你的命令行应该是我这样的

```shell
(opencv_python) beam@beam:~$ 
```

前面有(opencv_python)代表我在opencv_python这个虚拟空间里

这时候，我运行下面这条命令来安装python-opencv

```shell
conda install -c http://conda.anaconda.org/menpo opencv3
```

它需要你按y确定需要安装，如果你觉得速度过慢可以将conda换成其他源，这就靠大家自己去搜索了。

5.在Python Demo下面，输入python会显示下面部分

```shell
(opencv_python) beam@beam:~/gitlab/openncc_cdk/Platform/Linux/Example/How_to/Python_demo$ python
Python 3.6.13 |Anaconda, Inc.| (default, Jun  4 2021, 14:25:59) 
[GCC 7.5.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> 

```

输入`import cv2`可以看下opencv是否安装

```shell
(opencv_python) beam@beam:~/gitlab/openncc_cdk/Platform/Linux/Example/How_to/Python_demo$ python
Python 3.6.13 |Anaconda, Inc.| (default, Jun  4 2021, 14:25:59) 
[GCC 7.5.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import cv2
>>> 

```

没有提示错误就是正常的。

这时候我们可以用sudo ./run.sh来跑下demo看是不是对了。

<font color="red">**NOTE：这些操作我都是在前面有`(opencv_python)`的时候操作的。**</font>

发现出现了错误，错误如下：

```shell
(opencv_python) beam@beam:~/gitlab/openncc_cdk/Platform/Linux/Example/How_to/Python_demo$ sudo ./run.sh 
[sudo] password for beam: 
Traceback (most recent call last):
  File "hello_ncc.py", line 7, in <module>
    import cv2
ImportError: No module named 'cv2'

```

提示没有cv2，可以看下run.sh里面的内容，发现里面是python3 hello_ncc.py，这时候，我们可以修改run.sh文件为python hello_ncc.py，也可以自己敲命令，这里我推荐自己敲命令。

在敲命令的时候要先做一件事，因为sudo的python跟直接运行python不一样，所以我们需要先将/usr/bin里面的python先删除，或者进行备份，然后ln下我们的python，先用which来看下python的路径

```shell
(opencv_python) beam@beam:~/gitlab/openncc_cdk/Platform/Linux/Example/How_to/Python_demo$ which python
/home/beam/anaconda3/envs/opencv_python/bin/python #这里是上面which出来的 $后面是命令
```

后面再备份下原来的python

```shell
$ sudo mv /usr/bin/python /usr/bin/python_bak
$ sudo ln -s /home/beam/anaconda3/envs/opencv_python/bin/python /usr/bin/python
```

那么我用命令行输入

```shell
sudo python hello_ncc.py
```

这样就python的demo就可以在linux下进行运行了。
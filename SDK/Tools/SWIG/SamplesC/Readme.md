$ swig -python -py3  example.i
$ gcc -c -fpic example.c example_wrap.c -I/usr/include/python3.5
$ gcc -shared example.o example_wrap.o -o _example.so
$ python
Python 2.7.12 (default, Oct  8 2019, 14:14:10) 
[GCC 5.4.0 20160609] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import example
>>> example.fact(4)
24
>>> example.my_mod(23,7)
2
>>> example.cvar.My_variable + 4.5
7.5



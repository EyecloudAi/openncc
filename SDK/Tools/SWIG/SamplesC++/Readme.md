http://www.swig.org/Doc3.0/SWIGDocumentation.html#Introduction_nn4

$ sudo apt-get install swig
$ swig -c++ -python example.i
$ python setup.py build_ext --inplace
$ python
Python 2.7.12 (default, Oct  8 2019, 14:14:10) 
[GCC 5.4.0 20160609] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import example

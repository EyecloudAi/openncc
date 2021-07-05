/* module name */
%module example  
%{
extern double My_variable;
#include "example.h"  
%}
extern double My_variable;
%include "example.h"  
/* import source file */

#include "example.hpp"

Example::Example()
{    
	My_variable = 2;
}

Example::~Example()
{

}

/* Compute factorial of n */
int Example::fact(int n) {
  if (n <= 1)
    return 1;
  else
    return n*fact(n-1);
}

/* Compute n mod m */
int Example::my_mod(int n, int m) {
  return(n % m);
}

int Example::add_my_variable(int *n){
   int t = *n;
   return My_variable + t;	
}

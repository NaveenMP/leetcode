#include <iostream>
#include <stdint.h>

using namespace std;

class Solution;
//double Pow(double x, unsigned int n);
//double myPow(double x, int n);


class Solution {
public:
    double Pow(double x, unsigned int n)
    {
        if (n == 0)
        {
            return 1.0;
        }
        if (n == 1)
        {
            return x; 
        }    
        else
        {
            if (n%2 == 0)
            {
              return Pow(x,n/2)*Pow(x,n/2);
            }
            else
            {
                return Pow(x,(n-1)/2)*Pow(x,(n-1)/2)*x;
            }
        }
    }
    
    double myPow(double x, int n) {
        
        double sign = 1.0;
        if (x < 0)
        {
           x = -x;
           if (n%2 != 0)
           {
               sign = -1.0;
           }
        }
        
        if (x == 1.0)
            return 1.0;
        
        if (n == INT_MAX)
        {
            if (x > 0 && x < 1)
                return 0.0;
        }
        else if(n == INT_MIN)
        {
            if (x > 0 && x < 1)
                return sign*INT_MAX;
            else
                return 0.0;
        }
        
        if ( n < 0)
        {
            int m = 0;
            double overflow = 1.0;
            if (n < -INT_MAX){
                m = (n+1);
                m = -m;
                overflow = x;
            }
            else{
                m = -n;
            }
            
            return sign/(overflow * Pow(x,m));            
        }
        else
        {

            return sign*Pow(x,n);
        }
        
        
    }
};

int main()
{
  double x = 2.00000; //0.00001; //-13.62608;
  int n = -2147483648; // 2147483647; //3
  Solution pwrObj;
  double res = pwrObj.myPow(x, n);
  cout << "result = " << res << " | expected = " << "" << endl;
  
}

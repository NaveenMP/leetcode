class Solution {
public:
    double myPow(double x, int n) {
       
        /*  IEEE-754 floating-point standards
            https://docs.microsoft.com/en-us/cpp/build/ieee-floating-point-representation?view=msvc-170
            For double-precision, 1 sign bit, 11 exponent bits and 52 significand bits
            ( s * 2^y * 1.xxx.. ) ^ n = s^n * (2^(y*n)) * (1.xxx...)^n
        */
       
        double out;
       
        udouble ud_in, ud_out, ud_sfgd;
        unsigned long long llzero,llone, bias, sign, expn, sgfd;
        llzero = 0;
        llone = 1;
        bias = 1023;
        ud_in.d = x;
       
        // Define Masks
        long long int signMaskLLint = LLONG_MIN;
        long long int exptMaskLLint = ~(~(LLONG_MIN >> 11) | LONG_MIN);
        long long int sgfdMaskLLint = ~(LLONG_MIN >> 11);       
        
        // Sign bit
        sign = 0;
        if (( ((ud_in.u & signMaskLLint) == signMaskLLint) && n%2>0)){
            sign = ud_in.u & signMaskLLint;
        }
                
        // Exponant bits
        expn = (bias << 52);

        // ..bool isNzExponent = ((ud_in.u & exptMaskLLint) != (bias << 52));
        bool isNegPower = (n < 0);
        if (isNegPower)
            expn = (ud_in.u & exptMaskLLint) - ((long long int)(-n+1) << 52);
        else
            expn = (ud_in.u & exptMaskLLint) + ((long long int)(n-1) << 52);
               
        // Significand bits
        udouble one, sgfdNumber;
        unsigned long long powSgfdExp; // Exponent of extracted significant raised to the power n 
        one.d = 1;
        sgfdNumber.u = (ud_in.u & sgfdMaskLLint) | one.u;
        double be  = binomialExpansion(sgfdNumber.d, n);
                
        ud_sfgd.d = be;
        // .. Add exponent part of extracted significand to that of previous exponent raised to required power
        powSgfdExp = ((ud_sfgd.u - (bias << 52)) & exptMaskLLint); // This exponent in unbiased
                    
        expn += powSgfdExp;            
        expn &= exptMaskLLint;        
        sgfd = ud_sfgd.u & sgfdMaskLLint;

        // Combine sign, exponent and significand bits that are shifted to appropriate positions
        ud_out.u = sign ^ expn ^ sgfd;        
       
        // Prints
        /*
        printVec(int2bitvec(signMaskLLint), "signMask");
        printVec(int2bitvec(exptMaskLLint), "exptMask");
        printVec(int2bitvec(sgfdMaskLLint), "sgfdMask");
        */
       
        return ud_out.d;
           
    }
   
    union udouble {
            double d;
            unsigned long long u;
    };
    union idouble {
            double d;
            long long int i;
    };
   
    vector<bool> double2bitvec(const double x)
    {
       
        udouble ud;
        ud.d = x;
        bitset<sizeof(double) * 8> b(ud.u);
        vector<bool> bvec(sizeof(double) * 8);
        for(std::size_t n = 0; n < bvec.size(); ++n){
            bvec[n] = b[bvec.size() - n - 1];
        }
        return bvec;
    }
   
    vector<bool> int2bitvec(const long long int x)
    {
        bitset<sizeof(long long int) * 8> b(x);
        vector<bool> bvec(sizeof(long long int) * 8);
        for(std::size_t n = 0; n < bvec.size(); ++n){
            bvec[n] = b[bvec.size() - n - 1];
        }
        return bvec;
    }
   
   
    void printVec(const vector<bool> &vec, const std::string &str = std::string(""))
    {
        std::cout << str << " :: ";
        for (auto i=0; i<vec.size(); ++i)
            std::cout << vec[i];
        std::cout << endl;
    }
    
    double binomialExpansion(double sgfdNum, int power)
    {
        udouble ud;
        ud.d = sgfdNum;
        double x = (ud.d > 1)?(ud.d - 1):(1 - ud.d);
        double factCoeff = 1;
        double sum = 1;
        
        if (power < 0){
            sgfdNum /= 1;
            power *= -1;
        }
        
        int s = 1;
        double exponent = x;
                
        factCoeff = power;
        
        int lim = 10;
        
        int sign = s;
        for (int i=1; i<=lim; ++i)
        {
            sum +=  sign * factCoeff * exponent;
            //cout<<"fact "<<factCoeff<<"|exp "<<exponent<<"|sum = "<<sum<<endl;
            factCoeff *= double(power-i)/double(i+1);
            exponent *= x;
            sign *= s;
        }
        
        return sum;
    }
};
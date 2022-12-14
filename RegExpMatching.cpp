#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

/*
 *
 * Given an input string s and a pattern p, implement regular expression matching
 * with support for '.' and '*' where:
 * '.' Matches any single character.​​​​
 * '*' Matches zero or more of the preceding element.
 * The matching should cover the entire input string (not partial).
 *
 * Ref : https://www.geeksforgeeks.org/write-regular-expressions/
 *
 * g++ -o RegExpMatching RegExpMatching.cpp -std=c++20 && ./RegExpMatching
 *
 */

class Solution
{
public:
    bool isMatch(string s, string p)
    {
        size_t pos = 0;

        while(p.find('*') < p.length() || pos==0)
        {
            if (p == ".*")
                    return true;

            if (p.length() > s.length()) // prune to match sizes
            {
                while (pos++ < p.length())
                {
                    if ((p[pos] == '*') && (s.find(p[pos-1]) > s.length()))
                    {
                        p.erase(pos-1,2);
                        pos = 0;
                    }
                    
                    if(p[pos] == '.')
                    {
                        if (p[pos+1] == '*'){
                            p.erase(pos,2);
                        }
                        else
                        {
                            if (pos > s.length()-1)
                                p[pos] = s[s.length()-1];
                            else
                                p[pos] = s[pos];
                        }
                    }
                }

                pos = 0;
                while(p.length() > s.length())
                {
                    if (p[pos] != s[pos])
                    {
                        p.erase(pos,1);
                    }
                    else if (p[p.length()-1] != s[s.length()-1] && p[p.length()-1] != '*')
                    {
                        p.erase(p.length()-1, 1);
                    }
                    else
                    {
                        break;
                    }
                }

                size_t lDiff = 0;
                while (p.length() > s.length() && (pos++ < p.length())) //(pos++ < s.length())
                {
                    if (p[pos] == '*' && (s.find(p[pos-1]) < s.length()))
                    {
                        lDiff = p.length() - s.length();
                        if (lDiff > 0)
                        {
                            if (p[pos-1]!=s[pos-1])
                                p.erase(pos-1, lDiff); //p.erase(pos,1);
                            else
                                if (count(p.begin(),p.end(),'*')==1)
                                    p.erase(pos, lDiff);
                                else
                                    p.erase(pos, 1);
                            //pos = 0;
                        }
                    }
                }

                if (s == p)
                    return true;

            }

            if (s.length() >= p.length())
            {
                if (s == p)
                    return true;

                size_t pos = 0;
                while (pos < s.length())
                {
                    if(p[pos] == s[pos])
                    {
                        ++pos;
                        continue;
                    }
                    else if (p[pos] == '*' && pos > 0)
                    {
                        p[pos] = p[pos-1];
                        if (pos+1 < s.length())
                        {
                            if ((p[pos+1] != s[pos+1]) && (p[pos] == s[pos+1])) //&& (p.length() < s.length())
                            {
                                p.insert(pos+1, 1, '*');
                                ++pos;
                                continue;
                            }
                        }
                    }
                    else if (p[pos] == '.')
                    {
                        p[pos] = s[pos];
                    }
                    else  // non-matching character
                    {
                        return false;
                    }
                    
                    ++pos;

                }
            }
        }

        if (s == p)
            return true;
        else
            return false;
    }
};

int main()
{
    // Failed at 160/353 |    s = "mississippi" p = "mis*is*p*."; Output: true Expected: false
                                // mississippi
                                // mis*is*p*.
                                // missis*p*.
                                // mississp*.
                                // mississpp.
                                // mississppi
    // Failed at test case 169/353 | s = "aab"   p = "c*a*b"    ; exp: true

    // Failed at test case 275/353 | s = "aaa"   p = "aaaa"     ; Output: true  Expected: false

    // Failed at test case 277/353 | s = "aaa"   p = "ab*ac*a"  ; Output: false Expected: true

    // Failed at test case 280/353 | s = "aaca"  p = "ab*a*c*a" ; Output: false Expected: true (detected after 285/353)

    // Failed at test case 281/353 | s = "aaa"   p = "ab*a*c*a" ; Output: false Expected: true

    // Failed at test case 284/353 | s = "a"     p =".*"        ; Output: false Expected: true

    // Failed at test case 285/353 | s = "bbbba" p = ".*a*a"    ; Output: false Expected: true

    // Failed at test case 285/353 | s = "a"     p =".*..a*"    ; Output: true  Expected: false

    // 285 / 353 testcases passed  | s = "aasdfasdfasdfasdfas" p = "aasdf.*asdf.*asdf.*asdf.*s" Output false Expected true


    Solution S;
    clock_t start = clock();
    bool ret = S.isMatch("aasdfasdfasdfasdfas", "aasdf.*asdf.*asdf.*asdf.*s");
    double elapsedSecs = (clock() - start) / ((double)CLOCKS_PER_SEC);
    double elapsedMilliSecs = elapsedSecs*1000;
    cout << ret << endl;
    
    cout << "---------- SUMMARY ----------" << endl;
    cout << "Elapsed time : " << elapsedMilliSecs << "ms" << endl;
    return 0;
}
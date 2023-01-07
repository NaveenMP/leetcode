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
        while (p.length() > s.length()) // prune to match sizes
        {
            if (p[pos+1] == '*')
                p[pos+1] = p[pos];

            p.erase(pos,1);

            if (s == p)
            {
                return false;
            }

        }

        if (s.length() >= p.length())
        {
            if (p == ".*")
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
                        if ((p[pos+1] != s[pos+1]) && (p[pos] == s[pos+1]) && p.length() < s.length())
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
    // Failed at test case 169/353 | s = "aab" p = "c*a*b" ; exp: true

    // Failed at test case 275/353   | s = "aaa" p = "aaaa" ; Output: true Expected: false

    // Failed at test case 277/353   | s = "aaa" p = "ab*ac*a";  Output: false Expected: true

    Solution S;
    bool ret = S.isMatch("mississippi", "mis*is*p*.");
    cout << ret << endl;
    return 0;
}
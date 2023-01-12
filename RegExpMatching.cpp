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
        size_t s_idx(0), p_idx(0);
        size_t s_len(s.length());// p_len(p.length());

        while(p_idx < s_len)
        {
            if ((s[s_idx] == p[p_idx]) && (s_idx==p_idx))
            {
                ++s_idx;
                ++p_idx;
            }
            else if (p[p_idx] == '.')
            {
                if(p[p_idx+1] == '*') // '.*' sequence
                {
                    // find index in p to the next . or * starting from after current '.*'
                    size_t p_idx_next(p_idx+2);
                    while(p[p_idx_next] != '.' && p[p_idx_next] != '*' && p_idx_next < p.length())
                    {
                        ++p_idx_next;
                    }
                    size_t p_substr_len = p_idx_next - (p_idx+2);
                    string p_substr = p.substr(p_idx+2, p_substr_len);

                    // find index of first occurance of p_substr in s starting from s_idx.
                    size_t s_idx_match = 0; 
                    while (s.find(p_substr.c_str(), s_idx, p_substr_len) != string::npos)
                    {
                        s_idx_match = s.find(p_substr.c_str(), s_idx, p_substr_len);
                        if (s_idx_match > s_idx)
                        {
                            size_t diff = s_idx_match - s_idx;
                            p.insert(p_idx, s.substr(s_idx, diff)); // Insert portion of s between current position till string match into current position of p
                            p_idx += diff;
                            s_idx += diff;
                        } 
                        else
                        {
                            if (s.find(p_substr.c_str(), s_idx+p_substr_len, p_substr_len) != string::npos) // check if another match to p_substr is found right after first match
                            {
                                if ((countNonSpecial(s, s_idx) - countNonSpecial(p, p_idx)) < p_substr_len)
                                {
                                    break;
                                } 
                                p.insert(p_idx, p_substr);
                                p_idx += p_substr.length();
                                s_idx += p_substr.length();
                                continue;
                            }
                            else
                            {
                                break;
                            }
                        }  
                    }

                    p.erase(p_idx, 2); // erase '.*' from p once substring matching is completed
                }
                else
                {
                    if (s_idx==p_idx)
                        p[p_idx] = s[s_idx];
                }
            }
            else if (p[p_idx] == '*')
            {
                if (s[s_idx] == p[p_idx-1])
                {
                    if (s[s_idx+1] == s[s_idx]){
                        p.insert(p_idx, 1, p[p_idx-1]);
                    }
                    else{
                        if (s_idx < s_len-1)
                            p[p_idx] = p[p_idx-1];
                        else{
                            p.erase(p_idx,1);
                            continue;
                        }
                    }
                    ++p_idx;
                    ++s_idx;
                }
            }
            else
            {
                if (p[p_idx+1] == '*')
                {
                    p.erase(p_idx,2);
                }
            }
        }

        if (s == p)
            return true;
        else
            return false;
    }
    
    size_t countNonSpecial(string str, size_t pos = 0)
    {
        size_t count = 0;
        for (int i = pos; i < str.length(); ++i)
        {
            if (str[i] != '.' && str[i] != '*')
            {
                ++count;
            }
        }
        return count;
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

    // Failed at test case 180/353 | s = "abcdede" p = "ab.*de" ; Output: false Expected: true


    Solution S;
    clock_t start = clock();
    bool ret = S.isMatch("aaa", "ab*a*c*a");
    double elapsedSecs = (clock() - start) / ((double)CLOCKS_PER_SEC);
    double elapsedMilliSecs = elapsedSecs*1000;
    cout << ret << endl;
    
    cout << "---------- SUMMARY ----------" << endl;
    cout << "Elapsed time : " << elapsedMilliSecs << "ms" << endl;
    return 0;
}
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
        size_t s_len(s.length()), p_len(p.length()); // NOTE: p_len will keep changing as we build the pattern towards input string and must be re-calculated

        // Quick check to find if end of pattern string can never be matched with end of input string
        if (p.length()>=3 && s.length()>=1){
            if (!((p.back()==s.back()) || (p.back()=='.') 
                || (p[p_len-2]==s.back() && p.back()=='*') 
                || (p[p_len-2]=='.' && p.back()=='*') 
                || (p[p_len-2]=='*' && p.back()=='*')
                || (p[p_len-3]==s.back() && p[p_len-2]!=s.back() && p.back()=='*')
                || (p[p_len-3]=='.' && p[p_len-2]!=s.back() && p.back()=='*')
                || (p[p_len-3]=='*' && p[p_len-2]!=s.back() && p.back()=='*'))
                ){        
                    return false;
                }
        }

        // Chip away at the end of pattern string p till it becomes the last character of input string s or something that will reduce to the last character of s
        while(!patternBackMatch(s, p))
        {
            if (p.back()=='.')
            {
                p.back() = s.back();
            }
            else if (p.back()=='*' && p[p.length()-2]!='*' && p[p.length()-2]!='.' && p[p.length()-2]!=s.back())
            {
                p.erase(p.length()-2,2);
            }
            else if (p.back()=='*' && p[p.length()-2]=='*')
            {
                p.erase(p.length()-1,1);
            }
            else
            {
                break;
            }
        }

        // Now process from start of pattern string p
        while(p_idx < p.length())
        {
            if ((s[s_idx] == p[p_idx]) && (s_idx==p_idx) && (p_idx <= s_idx))
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

                    if (p_substr == "")
                    {
                        size_t p_dotBefChar_count(0);
                        for (size_t i = p_idx+2; i < p.length(); ++i)
                        {
                            if (p[i] == '.')
                                ++p_dotBefChar_count;
                            else
                                break;
                        }
                        if ((p_idx > s.length()-1) || (p_dotBefChar_count == (s_len - s_idx)))
                        {
                            p.erase(p_idx,2);
                        }
                        else if ((p_idx+1) < p.length()-1 || (s_idx >= s.length()))
                        {
                            p.erase(p_idx,2);
                            if (p.length() < s_len)
                                p.insert(p_idx, 1, s[s_idx]);
                        }
                        else
                        {
                            p.insert(p_idx, 1, s[s_idx]);
                            ++p_idx;
                            ++s_idx;
                        }
                        continue;
                    }

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
                            if (s.substr(s_idx+p_substr_len, p_substr_len).find(p_substr, 0) != string::npos)  // check if another match to p_substr is found right after first match
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
                if (p_idx >= s_len)
                {
                    if (p_idx - p.length() >= 1)
                        p.erase(p_idx, 2);
                    else
                        p.erase(p_idx, 1);

                    continue; 
                }
                else
                {
                    size_t p_char_reps(0), s_char_reps(0), diff_reps(0);
                    for (size_t i = p_idx-1; i < p.length(); ++i)
                    {
                        if (p[i] == p[p_idx-1] && i!= p_idx)
                           ++p_char_reps;
                        else if (p[i] != p[p_idx-1] && i!= p_idx)
                           break; 
                    }
                    for (size_t i = p_idx-1; i < s.length(); ++i)
                    {
                        if (s[i] == p[p_idx-1])
                           ++s_char_reps;
                        else if (s[i] != p[p_idx-1])
                           break; 
                    }

                    size_t p_char_occur(0);
                    for (size_t i = p_idx-1; i < p.length(); ++i)
                    {
                        if (p[i] == p[p_idx-1]){
                            ++p_char_occur;
                        }
                        else{
                            if ((p[i] != '*') && (p[i] != '.') && (p[i+1] != '*') && (p[i+1] != '.')) // search region for potential character repetitions ends when this condition is met
                                break;
                        }
                    }

                    if (p_char_reps > s_char_reps)
                    {
                        p.erase(p_idx-1,2);
                    }
                    else
                    {
                        diff_reps = s_char_reps - p_char_reps;
                        p.erase(p_idx,1);
                        if (diff_reps > 0 && (s_char_reps > p_char_occur))
                            p.insert(p_idx, diff_reps, p[p_idx-1]);
                    }

                }
            }
            else
            {
                if (p_idx < p.length()-1)
                {
                    if (p[p_idx+1] == '*') 
                        p.erase(p_idx,2);
                    else
                        break;
                }
                else
                {
                    break;
                }
            }
        }

        //std::cout << "Input s    = " << s << std::endl;
        //std::cout << "Modified p = " << p << std::endl;
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

    bool patternBackMatch(const string &inputString, const string &patternString)
    {
        const size_t p_length = patternString.length();
        const size_t s_length = inputString.length();
        if (!(patternString.back() == inputString.back()))
        {
            if (p_length >= 2)
            {
                if ((patternString[p_length-2]==inputString.back() && inputString.back()=='*') || (patternString[p_length-2]=='.' && inputString.back()=='*'))
                    return true;
                else
                    return false;
            }
        }

        return true; 
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

    // Failed for                  | s = "aa" p = "a*"          ; output false, expected: true

    // Failed for                  | s = "ab" p = ".*"          ; output false, expected: true

    // Failed for                  | s = "abbbcd" p = "ab*bbbcd"; Output false, Expected: true

    // Failed for                  | s = "ba"     p = ".*."
    // Failed for                  | s = "ab"     p = ".*"
    // Failed for                  | s = "ab"     p = ".*.."
    // Failed for                  | s = "ab"     p = ".*..c*"

    // Failed for                  | s = "acaabbaccbbacaabbbb" p = "a*.*b*.*a*aa*a*"; Output true, Expected false => check also for s = "abcdede" p = "ab.*de"
    // Failed for                  | s = "a" p = "ab*"; Output false, Expected true
    // Failed for                  | s = "ab" p = ".*..c*"; output false, Expected true
    // Failed for                  | s = "aabcbcbcaccbcaabc" p = ".*a*aa*.*b*.c*.*a*"; Output false, Expected true
    // Failed for                  | s = "aa" p = "."

    Solution S;
    clock_t start = clock();
    bool ret = S.isMatch("ab", ".*..c*");
    double elapsedSecs = (clock() - start) / ((double)CLOCKS_PER_SEC);
    double elapsedMilliSecs = elapsedSecs*1000;
    cout << ret << endl;
    
    cout << "---------- SUMMARY ----------" << endl;
    cout << "Elapsed time : " << elapsedMilliSecs << "ms" << endl;
    return 0;
}

/*
if (s[s_idx] == p[p_idx-1])
{
    if (s[s_idx+1] == s[s_idx])
    {
        p.insert(p_idx, 1, p[p_idx-1]);
    }
    else
    {
        if (s_idx < s_len - 1)
        {
            p[p_idx] = p[p_idx-1];
        }
        else
        {
            if ((p_idx == s_idx) && (p_idx == p.length()-1))
            {
                p[p_idx] = p[p_idx -1];
            }
            else
            {
                p.erase(p_idx,1);
                continue;
            }
        }
    }
    ++p_idx;
    ++s_idx;
}
else
{
    p.erase(p_idx,1);
    continue;
}
*/
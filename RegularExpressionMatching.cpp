#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::literals::chrono_literals;

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
 * g++ -o RegularExpressionMatching RegularExpressionMatching.cpp -std=c++20 && ./RegularExpressionMatching
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
            if (p.find('*',0) == string::npos)
            {
              break;  
            }

            if (p.back()!=s.back() && p.back()!='*' && p.back()!='.')
            {
                return false;
            }
            else if (p.back()=='.')
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

        size_t minLen = std::min(s.length(), p.length());
        /*
        if (minLen > 0)
        {
            size_t i = 0;
            while (i <= minLen-1)
            {
                if (p[p.length()-1-i] != '.' && p[p.length()-1-i] != '*')
                {
                    if (p[p.length()-1-i] != s[s.length()-1-i])
                        return false;
                    else
                        ++i;
                }
                else if (p[p.length()-1-i] == '.')
                {
                    p[p.length()-1-i] = s[s.length()-1-i];
                    i++;
                }
                else if (p[p.length()-1-i] == '*')
                {
                    char pprev = p[p.length()-1-(i+1)];
                    char scurr = s[s.length()-1-i];
                    if ((p.length()-1-(i+1)) >= 0)
                    {
                        if (p[p.length()-1-(i+1)] == '.')
                        {
                            break;
                        }
                        else if(p[p.length()-1-(i+1)] == '*')
                        {
                            p.erase(p.length()-1-(i+1),1);
                        }
                        else
                        {
                            p.erase(p.length()-1-(i+1),2);
                            if (p[p.length()-1-(i+1)] == s[s.length()-1-i])
                            {
                                size_t charRecurrance = countCharReccurance(s, s[s.length()-1-i], s.length()-1-i, true);
                                if (charRecurrance > 0)
                                    p.insert(p.length()-1-(i+1),charRecurrance,s[s.length()-1-i]);
                                ++i;
                            }
                        }
                    }
                }
            }
        }
        */

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
                    if (p_idx+2 < p.length())
                    {
                        if (p[p_idx+2] == '.')
                        {
                            p.erase(p_idx, 2);
                            if (p_idx+1 < p.length())
                            {
                                if (p[p_idx+1]=='*')
                                    p.insert(p_idx, 1, s[p_idx]);
                                else
                                    p[p_idx] = s[p_idx];
                            }
                            else
                                p[p_idx] = s[p_idx];
                        }
                        else if (p[p_idx+2] == '*')
                        {
                            p.erase(p_idx, 2);
                            p[p_idx] = s[p_idx];
                        }
                        else
                        {
                            for (size_t i = p_idx; i < s_len; ++i)
                            {                        
                                if (s[i] == p[p_idx+2])
                                {
                                    //cout << "Match to match upto "<< s[i] << " at " << i  << endl;
                                    auto p_tmp = p;
                                    p_tmp.erase(p_idx, 2);
                                    p_tmp.insert(p_idx, s.substr(p_idx,i-p_idx));
                                    size_t sSubStrStart = p_idx + (i-p_idx);
                                    size_t pSubStrStart = sSubStrStart;
                                    size_t sSubStrLen   = s.length() - sSubStrStart;
                                    size_t pSubStrLen   = p_tmp.length() - pSubStrStart;
                                    string s_tmp_substr = s.substr(sSubStrStart, sSubStrLen);
                                    string p_tmp_substr = p_tmp.substr(pSubStrStart, pSubStrLen);
                                    if (isMatch(s_tmp_substr, p_tmp_substr))
                                    {
                                        return true;
                                    }
                                    else if(s.find_last_of(s[i]) == i)
                                    {
                                        return false;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        p.erase(p_idx, 2);
                        if (p_idx < s.length())
                            p.insert(p_idx, s.substr(p_idx, s.length() - p_idx));
                    }
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

        modifiedPattern = p;

        if (s == p)
            return true;
        else
            return false;
    }

    bool patternBackMatch(const string &inputString, const string &patternString)
    {
        const size_t p_length = patternString.length();
        const size_t s_length = inputString.length();
        if (!(patternString.back() == inputString.back()))
        {
            if (p_length >= 2)
            {
                if ((patternString[p_length-2]==inputString.back() && patternString.back()=='*') || (patternString[p_length-2]=='.' && patternString.back()=='*'))
                    return true;
                else
                    return false;
            }
        }

        return true; 
    }

    size_t countCharReccurance(const string Str, const char Ch, const size_t start_idx, bool revDirection = false)
    {
        size_t count = 0;
        if (Str[start_idx] == Ch)
            count++;
        else
            return count;

        size_t idx = start_idx;
        if (revDirection)
        {
            if (idx > 0 && idx <= (Str.length()-1))
            {
                while(Str[--idx] == Ch)
                {
                    ++count;
                    if (idx == 0)
                        break;
                }
            }
        }
        else
        {
            if (idx >= 0 && idx < (Str.length()-1))
            {
                while(Str[++idx] == Ch)
                {
                    ++count;
                    if (idx == Str.length()-1)
                        break;
                }
            }
        }

        return count;

    }

    string modifiedPattern;
};

void TEST(bool printAll = false)
{
    struct testCase
    {
        string inputString;
        string patternString;
        bool output;
        bool expected;
        bool result;
        float executionTime;
    };

    vector<testCase> testCases;
    testCases.push_back({"mississippi"          ,"mis*is*p*."                , false, false , false, 1000.0}); //1
    testCases.push_back({"aab"                  ,"c*a*b"                     , false, true  , false, 1000.0});
    testCases.push_back({"aaa"                  ,"aaaa"                      , false, false , false, 1000.0});
    testCases.push_back({"aaa"                  ,"ab*ac*a"                   , false, true  , false, 1000.0});
    testCases.push_back({"aaca"                 ,"ab*a*c*a"                  , false, true  , false, 1000.0});
    testCases.push_back({"aaa"                  ,"ab*a*c*a"                  , false, true  , false, 1000.0});
    testCases.push_back({"a"                    ,".*"                        , false, true  , false, 1000.0}); //7
    testCases.push_back({"aasdfasdfasdfasdfas"  ,"aasdf.*asdf.*asdf.*asdf.*s", false, true  , false, 1000.0});
    testCases.push_back({"abcdede"              ,"ab.*de"                    , false, true  , false, 1000.0});
    testCases.push_back({"aa"                   ,"a*"                        , false, true  , false, 1000.0});
    testCases.push_back({"ab"                   ,".*"                        , false, true  , false, 1000.0});
    testCases.push_back({"abbbcd"               ,"ab*bbbcd"                  , false, true  , false, 1000.0});
    testCases.push_back({"ba"                   ,".*."                       , false, true  , false, 1000.0});
    testCases.push_back({"ab"                   ,".*"                        , false, true  , false, 1000.0});
    testCases.push_back({"ab"                   ,".*.."                      , false, true  , false, 1000.0});
    testCases.push_back({"ab"                   ,".*..c*"                    , false, true  , false, 1000.0}); //16
    testCases.push_back({"acaabbaccbbacaabbbb"  ,"a*.*b*.*a*aa*a*"           , false, false , false, 1000.0});
    testCases.push_back({"a"                    ,"ab*"                       , false, true  , false, 1000.0});
    testCases.push_back({"aabcbcbcaccbcaabc"    ,".*a*aa*.*b*.c*.*a*"        , false, true  , false, 1000.0});
    testCases.push_back({"abbabaaaaaaacaa"      ,"a*.*b.a.*c*b*a*c*"         , false, true  , false, 1000.0}); //20
    testCases.push_back({"aba"                  ,".*.*"                      , false, true  , false, 1000.0});
    testCases.push_back({"acaabbaccbbacaabbbb"  ,"a*.*b*.*a*aa*a*"           , false, false , false, 1000.0});
    testCases.push_back({"a"                    ,".b"                        , false, false , false, 1000.0});
    testCases.push_back({"caaaaccabcacbaac"     ,"b*.*..*bba.*bc*a*bc"       , false, false , false, 1000.0});
    testCases.push_back({"baaabaacaacaacbca"    ,"b*c*c*.*.*bba*b*"          , false, false , false, 1000.0});
    testCases.push_back({"abbaaaabaabbcba"      ,"a*.*ba.*c*..a*.a*."        , false, true  , false, 1000.0});
    testCases.push_back({"abcd"                 ,"d*"                        , false, false , false, 1000.0});
    testCases.push_back({"aaa"                  ,"ab*a*c*a"                  , false, true  , false, 1000.0}); // 28

    size_t maxSlength(0), maxPlength(0);
    for (auto testCase:testCases)
    {
        maxSlength = ((testCase.inputString.size() > maxSlength)?testCase.inputString.size():maxSlength);
        maxPlength = ((testCase.patternString.size() > maxPlength)?testCase.patternString.size():maxPlength); 
    }
    maxSlength += 4;
    maxPlength += 4;

    size_t testCaseNumber(0);
    for (auto testCase:testCases)
    {
        bool isTimeout(false);

        std::thread timeoutThread([&]()
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            isTimeout = true;
        });

        Solution S;
        clock_t start = clock();
        string s = testCase.inputString;
        string p = testCase.patternString;
        testCase.output = S.isMatch(s, p);

        timeoutThread.join();
        
        if (!isTimeout){
            testCase.result = (testCase.output == testCase.expected);
            double elapsedSecs = (clock() - start) / ((double)CLOCKS_PER_SEC);
            double elapsedMilliSecs = elapsedSecs*1000;
            testCase.executionTime = elapsedMilliSecs;
        }

        ++testCaseNumber;

        size_t sSpacing = maxSlength - testCase.inputString.size();
        size_t pSpacing = maxPlength - testCase.patternString.size();
        if (!testCase.result || isTimeout)
        {
            cout << "Test case " << testCaseNumber << "/" << testCases.size() << " : " 
                 << " s = " << testCase.inputString << string(sSpacing,' ') << ", p = " << testCase.patternString << string(pSpacing,' ');
            if (!testCase.result) 
                cout << ", Output = " << testCase.output << ", Expected = " << testCase.expected << "; FAILURE !!!" << endl;
            else if (isTimeout){
                timeoutThread.join();
                cout << "Time Limit Exceeded !!! " << endl;
            }
        }
        else
        {
            if (printAll)
            {
                cout << "Test case " << testCaseNumber << "/" << testCases.size() << " : " 
                 << " s = " << testCase.inputString << string(sSpacing,' ') << ", p = " << testCase.patternString << string(pSpacing,' ')
                 << ", Output = " << testCase.output << ", Expected = " << testCase.expected << "; PASS" << endl;
            }
        }

    }

    return;

}

int main() 
{
    // TEST Log
    /*
     Failed at 160/353 |    s = "mississippi" p = "mis*is*p*."; Output: true Expected: false
                                // mississippi => mis*is*p*. => missis*p*. => mississp*. => mississpp. => mississppi
     Failed at test case 169/353 | s = "aab"   p = "c*a*b"    ; exp: true
     Failed at test case 275/353 | s = "aaa"   p = "aaaa"     ; Output: true  Expected: false
     Failed at test case 277/353 | s = "aaa"   p = "ab*ac*a"  ; Output: false Expected: true
     Failed at test case 280/353 | s = "aaca"  p = "ab*a*c*a" ; Output: false Expected: true (detected after 285/353)
     Failed at test case 281/353 | s = "aaa"   p = "ab*a*c*a" ; Output: false Expected: true
     Failed at test case 284/353 | s = "a"     p =".*"        ; Output: false Expected: true
     Failed at test case 285/353 | s = "bbbba" p = ".*a*a"    ; Output: false Expected: true
     Failed at test case 285/353 | s = "a"     p =".*..a*"    ; Output: true  Expected: false
     285 / 353 testcases passed  | s = "aasdfasdfasdfasdfas" p = "aasdf.*asdf.*asdf.*asdf.*s" Output false Expected true
     Failed at test case 180/353 | s = "abcdede" p = "ab.*de" ; Output: false Expected: true
     Failed for                  | s = "aa" p = "a*"          ; output false, expected: true
     Failed for                  | s = "ab" p = ".*"          ; output false, expected: true
     Failed for                  | s = "abbbcd" p = "ab*bbbcd"; Output false, Expected: true
     Failed for                  | s = "ba"     p = ".*."
     Failed for                  | s = "ab"     p = ".*"
     Failed for                  | s = "ab"     p = ".*.."
     Failed for                  | s = "ab"     p = ".*..c*"
     Failed for                  | s = "acaabbaccbbacaabbbb" p = "a*.*b*.*a*aa*a*"; Output true, Expected false => check also for s = "abcdede" p = "ab.*de"
     Failed for                  | s = "a" p = "ab*"; Output false, Expected true
     Failed for                  | s = "ab" p = ".*..c*"; output false, Expected true
     Failed for                  | s = "aabcbcbcaccbcaabc" p = ".*a*aa*.*b*.c*.*a*"; Output false, Expected true
     Failed for                  | s = "aa" p = "."
     Failed for    (197/353)     | s = "abbabaaaaaaacaa"   p = "a*.*b.a.*c*b*a*c*" ; Output false, Expected true
                                                                a*.*b.a.*c*b*a*c* => a.*b.a.*c*b*a* => a.*b.a.*c*a* => a.*b.a.*caa => abbab.a.*caa => abbabaa.*caa => "abbabaaaaaaacaa" 
    
     Failed for    (152/353)     | s = "aba" p = ".*.*"; Output false,  Expected true

     Failed for    (153/353)     | s = "acaabbaccbbacaabbbb" p = "a*.*b*.*a*aa*a*" ; Time limit exceeded
     Failed for    (170/353)     | s = "a" p = ".b"; Output true Expected false
     Failed for    (172/353)     | s = "caaaaccabcacbaac" p = "b*.*..*bba.*bc*a*bc" ; Time limit exceeded
     Failed  for    (177/353)    | s = "baaabaacaacaacbca" p = "b*c*c*.*.*bba*b*"   ; Time limit exceeded
     Failed for (205/353)        | s = "abbaaaabaabbcba" p = "a*.*ba.*c*..a*.a*." ; Time limit exceeded
     Failed for                  | s = "abcd" p = "d*"
     Failed for (240/353)        | s = "aaa"  p = "ab*a*c*a" ; Output false Expected true
    */

    Solution S;
    clock_t start = clock();
    string s = "aa";
    string p = "a";
    //string pattern = p;
    bool ret = S.isMatch(s, p);
    double elapsedSecs = (clock() - start) / ((double)CLOCKS_PER_SEC);
    double elapsedMilliSecs = elapsedSecs*1000;
    
    cout << "---------- SUMMARY ----------" << endl;
    cout << "Input s    = " << s << endl;
    cout << "Input p    = " << p << endl;
    cout << "Modified p = " << S.modifiedPattern << endl;
    cout << "Result     = "<< ret << endl;
    cout << "Elapsed time : " << elapsedMilliSecs << "ms" << endl;


    TEST(true);

    return 0;
}


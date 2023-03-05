#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include <set>
#include <chrono>
#include <thread>
#include <atomic>
#include <future>
#include <mutex>

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
 * g++ -o RegularExpressionMatching1 RegularExpressionMatching1.cpp -std=c++20 && ./RegularExpressionMatching1
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
                break;  

            if (p.back()!=s.back() && p.back()!='*' && p.back()!='.')
                return false;
            else if (p.back()=='.')
                p.back() = s.back();
            else if (p.back()=='*' && p[p.length()-2]!='*' && p[p.length()-2]!='.' && p[p.length()-2]!=s.back())
                p.erase(p.length()-2,2);
            else if (p.back()=='*' && p[p.length()-2]=='*')
                p.erase(p.length()-1,1);
            else
                break;
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

    vector<size_t> findPatternIndices(const string &patternString, const string pattern = string("."))
    {
        vector<size_t> patternIndices;
        if (pattern != string(".") && pattern != ".*" && pattern != "*")
            return patternIndices;

        for (auto i = 0; i < patternString.size(); ++i)
        {
            if (pattern == string("."))
            {
                if (patternString[i] == pattern[0] && patternString[i+1] != '*')
                    patternIndices.push_back(i);    
            }
            else if(pattern == string("*"))
            {
                if (patternString[i] == pattern[0] && patternString[i-1] != '.')
                    patternIndices.push_back(i);
            }
            else if(pattern == ".*")
            {
                if (patternString[i] == pattern[0] && patternString[i+1] != pattern[1])
                    patternIndices.push_back(i);
            }
        }

        return patternIndices;
    }

    vector<size_t> findCharIndices(const string inputString, const char ch)
    {
        vector<size_t> indices;
        if (!inputString.find(ch))
            return indices;

        for (auto i=0; i < inputString.size(); ++i)
        {
            if (inputString[i] == ch)
            {
                indices.push_back(i);
            }
        }

        return indices;
    }

    bool finishedPreprocessing = false;
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
    testCases.push_back({"aaa"                  ,"ab*a"                      , false, false , false, 1000.0});
    testCases.push_back({"a"                    ,".*"                        , false, true  , false, 1000.0}); //8
    testCases.push_back({"aasdfasdfasdfasdfas"  ,"aasdf.*asdf.*asdf.*asdf.*s", false, true  , false, 1000.0});
    testCases.push_back({"abcdede"              ,"ab.*de"                    , false, true  , false, 1000.0});
    testCases.push_back({"aa"                   ,"a*"                        , false, true  , false, 1000.0}); //11
    testCases.push_back({"ab"                   ,".*"                        , false, true  , false, 1000.0});
    testCases.push_back({"abbbcd"               ,"ab*bbbcd"                  , false, true  , false, 1000.0});
    testCases.push_back({"ba"                   ,".*."                       , false, true  , false, 1000.0});
    testCases.push_back({"ab"                   ,".*"                        , false, true  , false, 1000.0});
    testCases.push_back({"ab"                   ,".*.."                      , false, true  , false, 1000.0});
    testCases.push_back({"ab"                   ,".*..c*"                    , false, true  , false, 1000.0}); //17
    testCases.push_back({"acaabbaccbbacaabbbb"  ,"a*.*b*.*a*aa*a*"           , false, false , false, 1000.0});
    testCases.push_back({"a"                    ,"ab*"                       , false, true  , false, 1000.0});
    testCases.push_back({"aabcbcbcaccbcaabc"    ,".*a*aa*.*b*.c*.*a*"        , false, true  , false, 1000.0});
    testCases.push_back({"abbabaaaaaaacaa"      ,"a*.*b.a.*c*b*a*c*"         , false, true  , false, 1000.0}); //21
    testCases.push_back({"aba"                  ,".*.*"                      , false, true  , false, 1000.0});
    testCases.push_back({"acaabbaccbbacaabbbb"  ,"a*.*b*.*a*aa*a*"           , false, false , false, 1000.0});
    testCases.push_back({"a"                    ,".b"                        , false, false , false, 1000.0});
    testCases.push_back({"caaaaccabcacbaac"     ,"b*.*..*bba.*bc*a*bc"       , false, false , false, 1000.0});
    testCases.push_back({"baaabaacaacaacbca"    ,"b*c*c*.*.*bba*b*"          , false, false , false, 1000.0});
    testCases.push_back({"abcd"                 ,"d*"                        , false, false , false, 1000.0});
    testCases.push_back({"aaa"                  ,"ab*a*c*a"                  , false, true  , false, 1000.0}); 
    testCases.push_back({"abbaaaabaabbcba"      ,"a*.*ba.*c*..a*.a*."        , false, true  , false, 1000.0}); //29
    testCases.push_back({"bbcacbabbcbaaccabc"   ,"b*a*a*.c*bb*b*.*.*"        , false, true  , false, 1000.0}); //30
    testCases.push_back({"bbab"                 ,"b*a*"                      , false, false , false, 1000.0}); //31
    testCases.push_back({"aabccbcbacabaab"      ,".*c*a*b.*a*ba*bb*"         , false, true  , false, 1000.0}); //32
    testCases.push_back({"cbacbbabbcaabbb"      ,"b*c*.*a*..a.*c*.*"         , false, true  , false, 1000.0}); //33

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
        bool isTimeout = false;
        //auto start = std::chrono::steady_clock::now();
        //std::thread timeoutThread(timeoutChecker, 2);


        Solution S;
        clock_t startFunc = clock();
        string s = testCase.inputString;
        string p = testCase.patternString;
        testCase.output = S.isMatch(s, p);
        /*
        isTimeout = true;
        cv.notify_all();
        timeoutThread.join();
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> diff = end - start;
        */
        
        if (!isTimeout){
            testCase.result = (testCase.output == testCase.expected);
            double elapsedSecs = (clock() - startFunc) / ((double)CLOCKS_PER_SEC);
            double elapsedMilliSecs = elapsedSecs*1000;
            testCase.executionTime = elapsedMilliSecs;
        }

        ++testCaseNumber;

        size_t sSpacing = maxSlength - testCase.inputString.size();
        size_t pSpacing = maxPlength - testCase.patternString.size();
        size_t dSpacing(1);
        if (testCaseNumber > 9)
            dSpacing = 0;

        if (!testCase.result || isTimeout)
        {
            cout << "Test case " << testCaseNumber << "/" << testCases.size() << string(dSpacing,' ') << " : "  
                 << " s = " << testCase.inputString << string(sSpacing,' ') << ", p = " << testCase.patternString << string(pSpacing,' ');
            if (!testCase.result){ 
                cout << ", Output = " << testCase.output << ", Expected = " << testCase.expected << "; FAILURE !!!" << endl;
            }
            else if (isTimeout){
                cout << "Time Limit Exceeded !!! " << endl;
            }
        }
        else
        {           
            if (printAll)
            {
                cout << "Test case " << testCaseNumber << "/" << testCases.size() << string(dSpacing,' ') << " : "
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
        s = "aabccbcbacabaab" p = ".*c*a*b.*a*ba*bb*"; Output false Expected true | 243 / 353 testcases passed
        s = "cbacbbabbcaabbb" p = "b*c*.*a*..a.*c*.*"; Output false Expected true | 246 / 353 testcases passed

    */

    Solution S;
    clock_t start = clock();
    string s = "cbacbbabbcaabbb";
    //string p = "b*c*.*a*..a.*c*.*";
    string p = "c.*bca.*c*.*";
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

    //TEST(true);

    return 0;
}


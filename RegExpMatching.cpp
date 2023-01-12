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

        while(p_idx < s_len-1)
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
                    if (s[s_idx+1] == s[s_idx])
                        p.insert(p_idx, 1, p[p_idx-1]);
                    else
                        p[p_idx] = p[p_idx-1];

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
    bool ret = S.isMatch("aaa", "ab*ac*a");
    double elapsedSecs = (clock() - start) / ((double)CLOCKS_PER_SEC);
    double elapsedMilliSecs = elapsedSecs*1000;
    cout << ret << endl;
    
    cout << "---------- SUMMARY ----------" << endl;
    cout << "Elapsed time : " << elapsedMilliSecs << "ms" << endl;
    return 0;
}



/*
int main () {
  
  string s = "aasdfasdfasdfasdfas"; 
  string p = "aasdf.*asdf.*asdf.*asdf.*s";
  //cout << "s = " << s << endl << "p = " << p << endl;
  
  size_t posPdotStar = 0; 
  size_t posSpreDotStarMatch = 0;
  
  while (p.find(".*") != string::npos)
  {
      posPdotStar = p.find(".*"); 
      posSpreDotStarMatch = s.find(p.substr(0,posPdotStar-1));
      
      cout << "s = " << s << endl << "p = " << p << endl << endl;
      
      cout << "pos .*            :: " << posPdotStar << endl;
      cout << "pos pre (in S) .* :: " << posSpreDotStarMatch << endl;
      
      if (posSpreDotStarMatch!=string::npos)
      {
         s.erase(posSpreDotStarMatch, posPdotStar);
         if (s[0] == p[posPdotStar+2])
         {
            p.erase(posSpreDotStarMatch, posPdotStar+2);
         }
         else
         {
           p.erase(posSpreDotStarMatch, posPdotStar);
           p.insert(0,1,s[0]);
         }
      }
      else
      {
          break;
      }
      
      cout << endl;
  }
  
  cout << "s = " << s << endl << "p = " << p << endl << endl;
  
  return 0;
}
*/


/*
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
                        if (p[pos+1] == '*' && p[pos+2] == s[pos])
                        {
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

*/
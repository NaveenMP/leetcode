/*
* Given an integer array nums, return all the triplets [nums[i], nums[j], nums[k]] such that i != j, i != k, and j != k, and nums[i] + nums[j] + nums[k] == 0.
*
* Notice that the solution set must not contain duplicate triplets.
*
* g++ -o 3Sum 3Sum.cpp -std=c++20 && ./3Sum 
*
* (a + b + c) = sum
* (b+c) = sum-a
* complement = sum-a
* (b+c) = complement
* b = complement - c
* 
*/

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <ctime>

using namespace std;

ostream& operator<<(ostream& os, const vector<vector<int>>& obj);
ostream& operator<<(ostream& os, const vector<int>& obj);
vector<int> findMatchingIndices(const vector<int>& vec, const int elem);

class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        //vector<vector<int>> resultIndices;

        int sum = 0;
        while (!nums.empty())
        {
            int complement = sum-nums.back();
            int a = nums.back();
            int a_idx = nums.size() - 1; // Discard this if twoSumIndicesVec returns empty
            nums.pop_back();
            set<set<int>> twoSumIndicesSetOfSets = twoSumIndices(nums, complement);

            if (!twoSumIndicesSetOfSets.empty())
            {
                for(auto twoSumIndicesSet: twoSumIndicesSetOfSets)
                {
                    vector<int> twoSumIndicesVec = vector<int>(twoSumIndicesSet.begin(), twoSumIndicesSet.end()); 
                    int b_idx = twoSumIndicesVec[0];
                    int c_idx = twoSumIndicesVec[1];
                    vector<int> res{a, nums[b_idx], nums[c_idx]};
                    result.push_back(res);
                    //resultIndices.push_back(vector<int>{a_idx, b_idx, c_idx});
                    //cout << "nums : " << vector<int>{a, nums[b_idx], nums[c_idx]} << " | Indices : " << vector<int>{a_idx, b_idx, c_idx} << endl;
                }
            }

            removePermutations(result);
        }
        return result;
    }

    /*
     * b = sum - a
     * find a and b, exclude -sum
     * 
    */

    set<set<int>> twoSumIndices(vector<int>& nums, int complement)
    {    
        set<set<int>> result;

        for (auto c_idx = 0; c_idx < nums.size(); ++c_idx)
        { 
            int c = nums[c_idx];
            int b = complement - c;
            vector<int> b_idx_vec = findMatchingIndices(nums, b); // Find indices of two elements that add up to complement
            if (!b_idx_vec.empty())
            {
               for (auto b_idx:b_idx_vec)
               {
                 if (b_idx != c_idx)
                 {
                    result.insert(set<int>{b_idx, c_idx});
                 }
               }           
            }

        }
        
        return result;
    }


    vector<int> findMatchingIndices(const vector<int>& vec, const int elem)
    {
        vector<int> res;
        for (auto i = 0; i < vec.size(); ++i)
        {
            if (vec[i] == elem)
            {
                res.push_back(i);
            }
        }
        
        return res;
    }

    void removePermutations(vector<vector<int>> &result)
    {
        set<vector<int>> resultSet;
        for (auto i = 0; i < result.size(); ++i)
        { 
            auto vec = result[i];
            sort(vec.begin(),vec.end());
            resultSet.insert(vec);
        }

        result.clear();
        for (auto vec:resultSet)
        {
            result.push_back(vec);
        }
    }


};

ostream& operator<<(ostream& os, const vector<vector<int>>& obj)
{
    for (auto rowVec:obj)
    {
        for (auto elem:rowVec)
            os << elem << ", ";
        os << endl;
    }
    return os;
}

ostream& operator<<(ostream& os, const vector<int>& obj)
{

    for (auto elem:obj)
        os << elem << ", ";
    os << endl;
    return os;
}

int main()
{
    //vector<int> nums{-1,0,1,2,-1,-4}; // {-1,0,1,2,-1,-4} | {0,0,0}

    /* Leetcode test pass after commit 71af64d14d82a3133012c8d65ceb02fd6693b62a */
    /*
    vector<int> nums{5,-9,-11,9,9,-4,14,10,-11,1,-13,11,10,14,-3,-3,-4,6,-15,6,6,-13,7,-11,-15,10,-8,13,-14,-12,12,6,-6,8,0,10,-11,-8,
    -2,-6,8,0,12,3,-9,-6,8,3,-15,0,-6,-1,3,9,-5,-5,4,2,-15,-3,5,13,-11,7,6,-4,2,11,-5,7,12,-11,-15,1,-1,-9,10,-8,1,2,8,
    11,-14,-4,-3,-12,-2,8,5,-1,-9,-4,-3,-13,-12,-12,-10,-3,6,1,12,3,-3,12,11,11,10};
    */

    /* Leetcode test fail(Time limit exceeded) after commit 71af64d14d82a3133012c8d65ceb02fd6693b62a ( test 260/311 )
       Leetcode test pass after commit
    */
    vector<int> nums{7,-10,7,3,14,3,-2,-15,7,-1,-7,6,-5,-1,3,-13,6,-15,-10,14,8,5,-10,-1,1,1,11,6,8,5,-4,0,3,10,-12,-6,-2,-6,-6,-10,8,-5,12,10,1,-8,4,-8,
    -8,2,-9,-15,14,-11,-1,-8,5,-13,14,-2,0,-13,14,-12,12,-13,-3,-13,-12,-2,-15,4,8,4,-1,-6,11,11,-7,-12,-2,-8,10,-3,-4,-6,4,-14,-12,-5,0,
    3,-3,-9,-2,-6,-15,2,-11,-11,8,-11,8,-7,8,14,-5,4,10,3,-1,-15,10,-6,-11,13,-5,1,-15};

    cout << "Input : " << endl << nums << endl;
    const size_t inputSize(nums.size());

    Solution S;
    clock_t start = clock();
    vector<vector<int>> result = S.threeSum(nums);
    double elapsedSecs = (clock() - start) / ((double)CLOCKS_PER_SEC);
    double elapsedMilliSecs = elapsedSecs*1000;

    //cout << "result : " << endl << result << endl;
    cout << "---------- SUMMARY ----------" << endl;
    cout << "Input size   : " << inputSize << endl;
    cout << "Output rows  : " << result.size() << endl;
    cout << "Elapsed time : " << elapsedMilliSecs << "ms" << endl;
    
    return 0;
}
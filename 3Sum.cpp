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
            set<set<int>> twoSumIndicesSetOfSet = twoSumIndices(nums, complement);

            if (!twoSumIndicesSetOfSet.empty())
            {
                for(auto twoSumIndicesSet: twoSumIndicesSetOfSet)
                {
                    vector<int> twoSumIndicesVec = vector<int>(twoSumIndicesSet.begin(), twoSumIndicesSet.end()); 
                    int b_idx = twoSumIndicesVec[0];
                    int c_idx = twoSumIndicesVec[1];
                    vector<int> res{a, nums[b_idx], nums[c_idx]};
                    if (!permutationExists(result, res))
                    {
                        result.push_back(vector<int>{a, nums[b_idx], nums[c_idx]});
                    }
                    //resultIndices.push_back(vector<int>{a_idx, b_idx, c_idx});
                    //cout << "nums : " << vector<int>{a, nums[b_idx], nums[c_idx]} << " | Indices : " << vector<int>{a_idx, b_idx, c_idx} << endl;
                }
            }
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
                 //vector<int> newRes{b_idx, c_idx};
                 //vector<int> revNewRes{c_idx, b_idx};
                 //if (b_idx != c_idx && count(result.begin(),result.end(),newRes)==0 && count(result.begin(),result.end(),revNewRes)==0)
                 if (b_idx != c_idx)
                 {
                    //result.push_back(vector<int>{b_idx, c_idx});
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

    bool permutationExists(const vector<vector<int>> resultVec, const vector<int> newResult)
    {
        bool isPerm(false);
        for (auto result:resultVec)
        {
            vector<int> vec1 = result;
            vector<int> vec2 = newResult;
            sort(vec1.begin(),vec1.end());
            sort(vec2.begin(),vec2.end());
            isPerm = (vec1 == vec2);
            if (isPerm)
            {
                return true;                    
            }
        }
        return false;
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

    vector<int> nums{5,-9,-11,9,9,-4,14,10,-11,1,-13,11,10,14,-3,-3,-4,6,-15,6,6,-13,7,-11,-15,10,-8,13,-14,-12,12,6,-6,8,0,10,-11,-8,
    -2,-6,8,0,12,3,-9,-6,8,3,-15,0,-6,-1,3,9,-5,-5,4,2,-15,-3,5,13,-11,7,6,-4,2,11,-5,7,12,-11,-15,1,-1,-9,10,-8,1,2,8,
    11,-14,-4,-3,-12,-2,8,5,-1,-9,-4,-3,-13,-12,-12,-10,-3,6,1,12,3,-3,12,11,11,10};

    cout << "Input : " << nums << endl;

    Solution S;
    vector<vector<int>> result = S.threeSum(nums);

    cout << result << endl;
    
    return 0;
}
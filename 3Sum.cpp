/*
* Given an integer array nums, return all the triplets [nums[i], nums[j], nums[k]] such that i != j, i != k, and j != k, and nums[i] + nums[j] + nums[k] == 0.
*
* Notice that the solution set must not contain duplicate triplets.
*
* g++ -o 3Sum 3Sum.cpp -std=c++20 && ./3Sum 
*
* (a + b + c) = 0
* (b+c) = -a
* sum = -a
* (b+c) = sum
* b = sum - c
* 
*/

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

ostream& operator<<(ostream& os, const vector<vector<int>>& obj);
ostream& operator<<(ostream& os, const vector<int>& obj);
vector<int> findMatchingIndices(const vector<int>& vec, const int elem);

class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;

        int sum = 0;
        for (auto i = 0; i < nums.size(); ++i)
        {
            sum = -nums[i];
            vector<vector<int>> twoSumRes = twoSum(nums, sum);
            if (!twoSumRes.empty())
            {
                for (auto j:twoSumRes){
                    if (count(j.begin(), j.end(), nums[i]) == 0)
                    {
                        j.push_back(nums[i]);
                        sort(j.begin(),j.end());
                        if (count(result.begin(), result.end(), j) == 0)
                            result.push_back(j);
                    }
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

    vector<vector<int>> twoSum(vector<int>& nums, int sum)
    {    
        vector<vector<int>> result;
        vector<int> exclusion_list;
        for (auto i = 0 ; i < nums.size(); ++i){
           vector<int> complement_indices = findMatchingIndices(nums, sum-nums[i]); // find indices where sum-nums[i] occurs
           if (!complement_indices.empty()) 
           {             
                for (auto j = 0; j < complement_indices.size(); ++j)
                {
                    if (complement_indices[j] != i && count(exclusion_list.begin(), exclusion_list.end(), complement_indices[j]) == 0 && count(exclusion_list.begin(), exclusion_list.end(), i) == 0)
                    {
                       result.push_back(vector<int>{nums[i], nums[complement_indices[j]]});
                       exclusion_list.push_back(i);
                       exclusion_list.push_back(complement_indices[j]);
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
    vector<int> nums{-1,0,1,2,-1,-4};

    cout << "Input : " << nums << endl;

    Solution S;
    vector<vector<int>> result = S.threeSum(nums);

    cout << result << endl;
    
    return 0;
}
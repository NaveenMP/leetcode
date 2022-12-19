/*
* Given an array nums of n integers, return an array of all the unique quadruplets [nums[a], nums[b], nums[c], nums[d]] such that:
*
* 0 <= a, b, c, d < n
* a, b, c, and d are distinct.
* nums[a] + nums[b] + nums[c] + nums[d] == target
* You may return the answer in any order.
*
* g++ -o 4Sum 4Sum.cpp -std=c++20 && ./4Sum 
*
*/

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {

        set<vector<int>> resultSet;
        vector<vector<int>> resultVec;

        sort(nums.begin(), nums.end());

        vector<int> currentResultVec;
        Nsum(nums, target, 0, 4, resultVec, currentResultVec);

        /*
        resultVec.reserve(resultSet.size());
        for (auto it = resultSet.begin(); it != resultSet.end(); ) {
            resultVec.push_back(std::move(resultSet.extract(it++).value()));
        }
        */
        
        return resultVec;
    }

    void Nsum(vector<int>& nums, int target, unsigned firstIndex, unsigned N, vector<vector<int>>& resultVec, vector<int>& currentResultVec)
    {

        auto numsLength = nums.size();

        if (N==2)
        {
            int l(0),r(numsLength-1);
            while (l < r)
            {
                int sum = nums[firstIndex + l] + nums[r];

                if (sum < target)
                {
                    ++l;
                }
                else if (sum > target)
                {
                    --r;
                }
                else
                {
                    currentResultVec.push_back(nums[firstIndex + l]);
                    currentResultVec.push_back(nums[r]);
                    resultVec.push_back(currentResultVec);                 
                    while (l < r && nums[l]==nums[l-1]){
                            ++l;
                    }
                    while (l < r && nums[r]==nums[r-1]){
                            --r;
                    }
                }
            }
        }
        else
        {
            for(auto i=0; i < numsLength; ++i)
            {
                Nsum(nums, target - nums[i], i+1, N-1, resultVec, currentResultVec);
                currentResultVec.push_back(nums[i]);
            } 
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
    //Input: nums = [1,0,-1,0,-2,2], target = 0 | Basic
    //Input: nums = [2,2,2,2,2], target = 8     | Basic 
    //Input: nums = {-3,-1,0,2,4,5}, target = 2 | 186/292
    vector<int> nums{-3,-1,0,2,4,5};     //{1,0,-1,0,-2,2};
    int target = 2;

    if (nums.size() < 200)
    {
        cout << "Input : " << endl << nums << endl;
    }
    const size_t inputSize(nums.size());

    Solution S;
    clock_t start = clock();
    vector<vector<int>> result = S.fourSum(nums, target);
    double elapsedSecs = (clock() - start) / ((double)CLOCKS_PER_SEC);
    double elapsedMilliSecs = elapsedSecs*1000;

    if (result.size() < 20)
    {
        cout << "result : " << endl << result << endl;
    }
    cout << "---------- SUMMARY ----------" << endl;
    cout << "Input size   : " << inputSize << endl;
    cout << "Output rows  : " << result.size() << endl;
    cout << "Elapsed time : " << elapsedMilliSecs << "ms" << endl;
    
    return 0;
}
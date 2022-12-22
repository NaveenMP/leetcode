/*
* Given an array nums of n integers, return an array of all the unique quadruplets [nums[a], nums[b], nums[c], nums[d]] such that:
*
* 0 <= a, b, c, d < n
* a, b, c, and d are distinct.
* nums[a] + nums[b] + nums[c] + nums[d] == target
* You may return the answer in any order.
*
* Recursion algorithm:
* (nums[a] + nums[b] + nums[c] + nums[d]) = t
* nums[b] + nums[c] + nums[d] = (t - nums[a])
* nums[c] + nums[d] = ((t - nums[a]) - nums[b])
* 
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

    const int int_numbits = sizeof(int)*8;
    const int int_min = (1 << int_numbits-1);
    const int int_max = ~int_min;

    vector<vector<int>> fourSum(vector<int>& nums, int target) {

        vector<vector<int>> resultVec;
        sort(nums.begin(), nums.end());

        vector<int> currentResultVec;
        Nsum(nums, target, 4, resultVec, currentResultVec, 0);

        /*
        resultVec.reserve(resultSet.size());
        for (auto it = resultSet.begin(); it != resultSet.end(); ) {
            resultVec.push_back(std::move(resultSet.extract(it++).value()));
        }
        */
        
        return resultVec;
    }

    void Nsum(vector<int>& nums, int target, unsigned N, vector<vector<int>>& resultVec, vector<int>& currentResultVec, int firstIndex)
    {

        int numsLength = nums.size();

        if (any_of(nums.begin(), nums.end(), [](int x){ return (x < int_min || x > int_max);} )) // move thus condition outside function
        {
            return;
        }
        if ((target < int_min) || (target > int_max) || (target < N*nums[0]) || (target > N*nums[numsLength-1]) || (nums.size() < N))
        {
            return;
        }

        if (N < 2)
        {
            for (auto i=0; i<nums.size(); ++i)
            {
                if (target == nums[i])
                {
                    currentResultVec.push_back(nums[i]);
                    resultVec.push_back(currentResultVec);
                    return;
                }
            }
            return;
        }

        if (N==2)
        {
            int l(firstIndex),r(numsLength-1);
            while (l < r)
            {
                int sum = nums[l] + nums[r];

                if (sum < target)
                {
                    ++l;
                    while (l < r && nums[l]==nums[l-1] && nums[l]!=nums[r]){
                            ++l;
                    }
                }
                else if (sum > target)
                {
                    --r;
                }
                else
                {
                    currentResultVec.push_back(nums[l]);
                    currentResultVec.push_back(nums[r]);
                    resultVec.push_back(currentResultVec);
                    // Free up space in current results vector for more possible combinations of the last two sum components
                    currentResultVec.pop_back(); 
                    currentResultVec.pop_back();
                    ++l;
                    --r;            
                    while (l < r && nums[l]==nums[l-1]){
                            ++l;                      
                    }
                    
                }
            }
            currentResultVec.pop_back();
            return;
        }
        else
        {
            for(auto i=firstIndex; i < numsLength - (N - 2); ++i) // restrict search-space in the range (0, L - (N-1))
            {
                if ((i==firstIndex) || ((i > firstIndex) && (nums[i]!=nums[i-1])))
                {
                    currentResultVec.push_back(nums[i]);
                    
                    if (((target < 0) && (nums[i] > (target - int_min))) || ((target > 0) && (nums[i] < (target - int_max))))
                    {
                        return;
                    }
                    
                    Nsum(nums, target - nums[i], N-1, resultVec, currentResultVec, i+1); //i+1
                }
            }
            currentResultVec.clear(); 
        }

        return;
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
    //Input: nums = {2,2,2,2,2}, target = 8     | Basic 
    //Input: nums = {-3,-1,0,2,4,5}, target = 2 | 186/292
    //Input: nums = {-2,-1,-1,1,1,2,2}, target = 0 | 215/292 
    //Input: nums = {-3,-2,-1,0,0,1,2,3}, target = 0 | 268/292 | missing quad: [-3,0,1,2]
    //Input: nums = {-1,0,-5,-2,-2,-4,0,1,-2}, target = -9 | 278/292 | missing quad: [-5,-4,0,0]
    //Input: nums = {2,-4,-5,-2,-3,-5,0,4,-2}, target = -14 | 280/292 | missing quad: [-5,-5,-2,-2]
    //Input: nums = {0,2,2,2,10,-3,-9,2,-10,-4,-9,-2,2,8,7}, target = 6 | 285/292 | duplicate quad: [0,2,2,2]
    //Input: nums = { 1000000000, 1000000000, 1000000000, 1000000000}, target = -294967296 | 290/292 | signed integer overflow
    //Input: nums = {-1000000000,-1000000000, 1000000000,-1000000000,-1000000000} , target = 294967296 | 292/292 | signed integer overflow
    vector<int> nums{1000000000,1000000000,1000000000,1000000000};     
    int target = -294967296;

    cout << "INT MIN      = " << int_min << endl;
    cout << "INT MAX      = " << int_max << endl;
    if (nums.size() < 200)
    {
        cout << "Input        : " << endl << nums << endl;
        sort(nums.begin(), nums.end());
        cout << "Input sorted : " << endl << nums << endl;
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
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
        size_t N = nums.size();

        sort(nums.begin(), nums.end());

        int sum = 0;
        size_t b(1), c(N-2), d(N-1);

        for (size_t a=0; a<N; ++a)
        {
            if (a > 0 && nums[a] == nums[a-1])
                continue;

            b = a+1;
            d = N-1;
            c = d-1;

            set<size_t> distinctIndices{a, b, c, d};

            if (distinctIndices.size() != 4)
                continue;

            while ((a < b) && (b < c) && (c < d))
            {
                sum = nums[a] + nums[b] + nums[c] + nums[d];

                if(sum < target)
                {
                    if (b == c-1)
                    {
                        if (a < b)
                        {
                            ++a;
                            while (a < b && nums[a] == nums[a-1])
                            {
                                ++a;
                            }
                        }
                    }
                    else
                    {
                        ++b;
                        while ((b < c) && (nums[b] == nums[b-1]))
                        {
                            ++b;
                        }
                    }
                    
                }
                else if(sum > target)
                {
                    
                    if (c == b + 1 && c < d)
                    {
                        --d;
                    }
                    else
                    {
                        --c;
                    }

                }
                else
                {
                    resultSet.emplace(vector<int>{nums[a], nums[b], nums[c], nums[d]});
                    ++b;
                }

            }

        }

        resultVec.reserve(resultSet.size());
        for (auto it = resultSet.begin(); it != resultSet.end(); ) {
            resultVec.push_back(std::move(resultSet.extract(it++).value()));
        }
        
        return resultVec;
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
    //Input: nums = {[-3,-1,0,2,4,5]}, target = 2 | 186/292
    vector<int> nums{2,2,2,2,2};     //{1,0,-1,0,-2,2};
    int target = 8;

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
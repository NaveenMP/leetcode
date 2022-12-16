/*
* Given an array nums of n integers, return an array of all the unique quadruplets [nums[a], nums[b], nums[c], nums[d]] such that:
*
* 0 <= a, b, c, d < n
* a, b, c, and d are distinct.
* nums[a] + nums[b] + nums[c] + nums[d] == target
* You may return the answer in any order.
*
*
*
*/

#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Solution {
public:
    vector<vector<int>> fourSum(vector<int>& nums, int target) {

        vector<vector<int>> result;
        size_t N = nums.size();

        sort(target.begin(), target.end());

        int sum = 0;
        int b(1), c(N-1), d(N);

        for (a=0; a<N; ++a)
        {
            if (a > 0 && nums[a] == nums[a-1])
                continue;

            b = a+1;
            d = N;
            c = d-1;

            set<int> distinctIndices{a, b, c, d};

            if (distinctIndices.size()! = 4)
                continue;

            while ((a < b) && (b < c) && (c < d))
            {
                sum = nums[a] + nums[b] + nums[c] + nums[d];


                if(sum < target)
                {
                    ++b;
                    while ((b < c-1) && (nums[b] == nums[b-1]))
                        ++b;
                    else if (b == c-1 && a < b){
                        ++a;
                        while (a < b && nums[a] == nums[a-1])
                            ++a;
                    }
                }
                else if(sum > target)
                {
                    --c;
                    if (c == b + 1 && c < d)
                    {
                        --d;
                    }

                }
                else
                {
                    result.push_back(vector<int>{nums[a], nums[b], nums[c], nums[d]});
                }

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
    vector<int> nums{};

    if (nums.size() < 200)
    {
        cout << "Input : " << endl << nums << endl;
    }
    const size_t inputSize(nums.size());

    Solution S;
    clock_t start = clock();
    vector<vector<int>> result = S.fourSum(nums);
    double elapsedSecs = (clock() - start) / ((double)CLOCKS_PER_SEC);
    double elapsedMilliSecs = elapsedSecs*1000;

    //cout << "result : " << endl << result << endl;
    cout << "---------- SUMMARY ----------" << endl;
    cout << "Input size   : " << inputSize << endl;
    cout << "Output rows  : " << result.size() << endl;
    cout << "Elapsed time : " << elapsedMilliSecs << "ms" << endl;
    
    return 0;
}
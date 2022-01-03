#include <bits/stdc++.h>
#include <iostream>
#include <cmath>
#include <queue>

using namespace std;
#define MAX(a,b) (((a)>(b))?(a):(b))

class Item
{
    public:
        int value;  // 物品的價格
        int weight; // 物品的重量
        Item(int, int);
        bool operator > (const Item &other) const
        {
            return (double)value/weight > (double)other.value/other.weight;
        }
};

Item::Item(int v, int w)
{
    value  = v;
    weight = w;
}

int itemSize; // 物品(item)的個數
int capacity; // 背包容量
//int nodes_generated = 0; // 產出的node個數
int lowerBound = 0; // 節點共用的 lower bound
vector<Item> arr; // item vector
vector<int> prefixSumW; // weight 的前綴加總, 加速找到 upper bound
vector<int> prefixSumV; // value  的前綴加總, 加速找到 upper bound
int getUpperBound(int idx, int totWeight, int upperBoundIdx, int &newUpperBoundIdx, bool &stopFlag);
void knapsack(int idx, int totWeight, int profit, int upperBoundIdx);

int getUpperBound(int idx, int totWeight, int upperBoundIdx, 
    int &newUpperBoundIdx, bool &stopFlag)
{
    int lastWeight = (idx == 0) ? totWeight : totWeight + prefixSumW[idx - 1];
    newUpperBoundIdx = MAX(idx, upperBoundIdx);
    while(newUpperBoundIdx < itemSize && prefixSumW[newUpperBoundIdx] <= lastWeight)
        ++newUpperBoundIdx; // 找到最適配的新index
    if(newUpperBoundIdx == itemSize)
    {
        if(idx == 0)
            return prefixSumV[itemSize - 1];
        return prefixSumV[itemSize - 1] - prefixSumV[idx - 1];
    }
    int upperBound = 0;
    stopFlag = true;
    
    if(idx == 0)
    {
        totWeight  -= prefixSumW[newUpperBoundIdx - 1];
        upperBound += prefixSumV[newUpperBoundIdx - 1];
    }
    else
    {
        totWeight  -= prefixSumW[newUpperBoundIdx - 1] - prefixSumW[idx - 1];
        upperBound += prefixSumV[newUpperBoundIdx - 1] - prefixSumV[idx - 1];
    }
    if(totWeight != 0)
    {
        stopFlag = false;
        double ratio = (double) totWeight / arr[newUpperBoundIdx].weight;
        totWeight  -= ratio * arr[newUpperBoundIdx].weight;
        upperBound += ratio * arr[newUpperBoundIdx].value;
    }
    return upperBound;
}

void knapsack(int idx, int totWeight, int profit, int upperBoundIdx)
{
    if(idx == itemSize)
    {
        lowerBound = MAX(lowerBound, profit);
        return;
    }
    int newUpperBoundIdx = itemSize;
    bool stopFlag = false; // flag: stop the algorithm
    int tmpUpperBound = profit + 
        getUpperBound(idx, totWeight, upperBoundIdx, newUpperBoundIdx, stopFlag);
            if(tmpUpperBound <= lowerBound)
        return;
    if(stopFlag)
    {
        lowerBound = MAX(lowerBound, tmpUpperBound);
        return;
    }
    if (totWeight >= arr[idx].weight)
    {
        knapsack(idx+1, totWeight - arr[idx].weight, profit + arr[idx].value, newUpperBoundIdx);
        //++nodes_generated;
    }
    knapsack(idx+1, totWeight, profit, newUpperBoundIdx);
    //++nodes_generated;   
}

int main()
{
    cin >> capacity >> itemSize;
    for(int i = 0; i < itemSize; ++i)
    {
        int value;
        int weight;
        cin >> value >> weight;
        arr.push_back(Item(value, weight));
    }
    // Step1: 根據CP值排序
    sort(arr.begin(), arr.end(), greater<Item>()); // 根據cp值排序, 高的在前面
    //for(int i=0; i < arr.size(); ++i)
    //    cout << arr[i].value << " " << arr[i].weight 
    //         << ", cp: " << (double) arr[i].value/arr[i].weight <<endl;
    for(int i = 0, w = 0, v = 0; i < itemSize; i++) // get prefixSum 
    {
        w += arr[i].weight;
        v += arr[i].value;
        prefixSumW.push_back(w);
        prefixSumV.push_back(v);
    }
    int tmpCapacity = capacity;
    for(const Item &item: arr)
    {
        if(tmpCapacity > item.weight)
        {
            lowerBound  += item.value;
            tmpCapacity -= item.weight;
        }
    }
    //cout << "lower bound: " << lowerBound << endl;
    //++nodes_generated;
    knapsack(0, capacity, 0, 0);
    cout << lowerBound << endl;
    //cout << "node gen: " << nodes_generated << endl;
    return 0;
}
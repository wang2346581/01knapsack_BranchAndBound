#include <bits/stdc++.h>
#include <iostream>
#include <cmath>
#include <queue>

using namespace std;
#define MAX(a,b) (((a)>(b))?(a):(b))

int itemSize; // 物品(item)的個數
int capacity; // 背包容量
vector<int> prefixSumW; // weight 的前綴加總, 加速找到 upper bound
vector<int> prefixSumV; // value  的前綴加總, 加速找到 upper bound

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

vector<Item> arr; // item vector

class Node
{
    public:
        int level;  // 用來識別自己在哪 (即在tree的第幾層)
        int profit; // 目前價值
        int weight; // 目前負重
        int upperBound;
        int upperBoundIdx;
        int newUpperBoundIdx;
        Node(int, int, int, int, int);
        bool operator < (const Node &other) const;
};

Node::Node(int l, int p , int w, int uIdx, int nUIdx)
{
    level = l;
    profit = p; 
    weight = w;
    upperBoundIdx = uIdx;
    newUpperBoundIdx = nUIdx;
}

bool Node::operator < (const Node &other) const
{
    if (upperBound > other.upperBound || level > other.level)
        return false;
    return true; // upper bound 與 level 較小的稱為較小
}

int getUpperBound(int idx, int totWeight, int upperBoundIdx, int &newUpperBoundIdx, bool &stopFlag);
int knapsack();

int getUpperBound(int idx, int totWeight, int upperBoundIdx, 
    int &newUpperBoundIdx, bool &stopFlag)
{
    int lastWeight = (idx == 0) ? totWeight : (totWeight + prefixSumW[idx - 1]);
    newUpperBoundIdx = MAX(idx, upperBoundIdx);
    while(newUpperBoundIdx < itemSize && prefixSumW[newUpperBoundIdx] <= lastWeight)
        ++newUpperBoundIdx; // 找到最適配的新index
    if(newUpperBoundIdx == itemSize)
    {
        //cout << "idx: " << idx <<  " ubIdx: " << upperBoundIdx <<  ", pV: " << prefixSumV[itemSize - 1] 
        //    << ", prefixSum[idx-1]: " <<  prefixSumV[idx - 1] <<  endl;
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
//    if(upperBound > 2000)
 //       cout << "newUpperBoundIdx: " << newUpperBoundIdx << endl;
    return upperBound;
}

int knapsack()
{
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
    int lowerBound = 0; // 節點共用的 lower bound 
    bool stopFlag;
    // 設定初始lowerBound
    int tmpCapacity = capacity;
    for(const Item &item: arr)
    {
        if(tmpCapacity > item.weight)
        {
            lowerBound  += item.value;
            tmpCapacity -= item.weight;
        }
    }
    //cout << "[Init] lowerBound: " << lowerBound << endl;
    int nodes_generated = 0; // 產出的node個數
    priority_queue<Node, vector<Node>> pQueue;
    Node cur(0, 0, capacity, 0, itemSize);
    cur.upperBound = cur.profit + getUpperBound(cur.level, cur.weight, 
        cur.upperBoundIdx, cur.newUpperBoundIdx, stopFlag);
    cur.upperBoundIdx = cur.newUpperBoundIdx;
    //cout << " lowerBound: " << lowerBound << " cur.upper: " << cur.upperBound << endl;
   // cout << "upperBoundIdx: " << cur.upperBoundIdx << endl;
    //return 0;
    if(cur.upperBound <= lowerBound || stopFlag){
        //cout << "uB: " << cur.upperBound << endl;
        return cur.upperBound;
    }
    //cur.upperBound = getUpperBound(cur.level, cur.weight, 
    //    curUpperBoundIdx, cur.upperBoundIdx, stopFlag);
    //cout << "upper bound: " << cur.upperBound << endl;
    //cout << "upperBoundIdx: " << cur.upperBoundIdx << endl;
    //return 0;
    ++nodes_generated;
    pQueue.push(cur);
    // Step3: 開始進行branch and bound 
    while (!pQueue.empty()) {
        cur = pQueue.top();
        pQueue.pop();
        if (cur.level == itemSize) // 到最後一層
        {
           // cout << "[E]lower bound: " << lowerBound << " profit: " << cur.profit << endl;
            lowerBound = MAX(lowerBound, cur.profit);
            continue;
        }
        // 產生左子樹, 並拿取此level的item
        Node left = cur;
        if(cur.weight >= arr[cur.level].weight)
        {
            //cout << "[L]cur.weight: " << cur.weight << ", arr[cur.level].weight: " << arr[cur.level].weight << endl;
            ++left.level;
            left.weight -= arr[cur.level].weight;
            left.profit += arr[cur.level].value;
            //cout << "left.weight: " << left.weight << ",  left.profit: " <<  left.profit<< endl;
            //cout << "[B]cnt: " << cnt << ", left.upperBound : " << left.upperBound 
            //     << " level: " <<left.level << endl;
            left.upperBound = left.profit + getUpperBound(left.level, left.weight, 
                left.upperBoundIdx, left.newUpperBoundIdx, stopFlag);
            left.upperBoundIdx = left.newUpperBoundIdx;
            //lowerBound = MAX(lowerBound, left.profit);
            //cout << "left.upperBound: " << left.upperBound << ",  left.upperBoundIdx: " <<  left.upperBoundIdx<< endl;
            if (left.level == itemSize) // 到最後一層
            {
                //cout << "[E]lower bound: " << lowerBound << " profit: " << left.profit << endl;
                lowerBound = MAX(lowerBound, left.profit);
                continue;
            }
            if(left.upperBound > lowerBound)
            {
                if(stopFlag){
                    lowerBound = MAX(lowerBound, left.upperBound);
                    continue;
                }
                else
                {
                    ++nodes_generated;
                    pQueue.push(left);
                }
            }
        }
        // 產生右子樹, 表不拿取此level的item
        Node right = cur;
        //cout << "right.weight: " << right.weight << ",  right.profit: " <<  right.profit  
        //     << ",  right.upperBoundIdx: " <<  right.upperBoundIdx << endl;
        ++right.level;
        right.upperBound = right.profit + getUpperBound(right.level, right.weight, 
            right.upperBoundIdx, right.newUpperBoundIdx, stopFlag);
        right.upperBoundIdx = right.newUpperBoundIdx;
        //cout << "right.upperBound: " << right.upperBound << ",  right.upperBoundIdx: " <<  right.upperBoundIdx<< endl;
        if (right.level == itemSize) // 到最後一層
        {
           // cout << "[E]lower bound: " << lowerBound << " profit: " << right.profit << endl;
            lowerBound = MAX(lowerBound, right.profit);
            continue;
        }
        if(right.upperBound > lowerBound)
        {
            if(stopFlag){
                lowerBound = MAX(lowerBound, right.upperBound);
                continue;
            }
            else
            {
                ++nodes_generated;
                pQueue.push(right);
            }
        }
    }
    //cout << maxProfit << endl;
    //cout << "lowerBound: " << lowerBound << endl;
    //cout << "nodes_generated: " << nodes_generated << endl;
    return lowerBound;
}
  
int main()
{
    int value, weight;
    int lowerBound;
    vector<string> fileNames;
    for(int i = 1; i <= 15; ++i)
        fileNames.push_back("test\\test" + to_string(i) + ".input");
    //for(int i = 1; i <= 10; ++i)
    //    fileNames.push_back("data\\" + to_string(i) + ".in");
    for(int i = 0; i < 15; ++i)
    {
        // clear data
        arr.clear();
        prefixSumW.clear(); // weight 的前綴加總, 加速找到 upper bound
        prefixSumV.clear(); 
        lowerBound = 0;

        // read data
        ifstream fin(fileNames[i]);
        ofstream fout;
        fout.open("myoutput\\" + to_string(i+1) + ".out");
        //fout.open("myoutput\\" + to_string(i) + ".out");
        fin >> capacity >> itemSize;

        arr.reserve(itemSize);
        while(fin >> value >> weight)
        {
            arr.push_back(Item(value, weight));
        }
        lowerBound = knapsack();
        fout << lowerBound;
    }
    return 0;
}
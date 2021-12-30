#include <bits/stdc++.h>
#include <iostream>
#include <cmath>
#include <queue>

using namespace std;
#define MAX(a,b) (((a)>(b))?(a):(b))

int itemSize; // 物品(item)的個數
int capacity; // 背包容量
vector<int> w;
vector<int> v;

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
        Node(int, int, int);
        bool operator < (const Node &other) const;
};

Node::Node(int l, int p , int w)
{
    level = l;
    profit = p; 
    weight = w; 
}

bool Node::operator < (const Node &other) const
{
    if (upperBound > other.upperBound || level > other.level)
        return false;
    return true; // upper bound 與 level 較小的稱為較小
}

int getUpperBound(int idx, int profit, int weight);
int knapsack();

int getUpperBound(int idx, int profit, int weight)
{
    if(weight >= capacity)
        return 0;
    int upperBound = profit;
    int totWeight = weight;
    int j = idx + 1;
    while(j < itemSize)
    {
        if(totWeight + arr[j].weight <= capacity)
        {
            totWeight += arr[j].weight;
            upperBound += arr[j].value;
        }
        else
        {
            upperBound+= (double)(capacity-totWeight) * arr[j].value / arr[j].weight; // 先乘再除, 但要小心overflow
            break;
        }
        ++j;        
    }
    return upperBound;
}

int knapsack()
{
    // Step1: 根據CP值排序
    sort(arr.begin(), arr.end(), greater<Item>()); // 根據cp值排序, 高的在前面
    //for(int i=0; i < arr.size(); ++i)
    //    cout << arr[i].value << " " << arr[i].weight 
    //         << ", cp: " << (double) arr[i].value/arr[i].weight <<endl;
    int lowerBound = 0; // 節點共用的 lower bound 
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
    int maxProfit  = 0;  // 最後總價值
    int nodes_generated = 0; // 產出的node個數
    priority_queue<Node, vector<Node>> pQueue;
    Node cur(-1, 0, 0);
    ++nodes_generated;
    cur.upperBound = getUpperBound(cur.level, cur.profit, cur.weight);
    pQueue.push(cur);

    // Step3: 開始進行branch and bound 
    while (!pQueue.empty()) {
        cur = pQueue.top();
        pQueue.pop();
        if (cur.level == itemSize - 1) // 到最後一層
            continue;
        if(cur.upperBound <= lowerBound)
            continue;
        //cout << "node: " << nodes_generated << ", lb is " << lowerBound  << ", ub: " << cur.upperBound 
        //     << ", maxProfit:" << maxProfit << endl;

        // 產生左子樹, 並拿取此level的item
        Node left = cur;
        ++left.level;
        left.profit += arr[left.level].value;
        left.weight += arr[left.level].weight;
        ++nodes_generated;
        if(left.weight <= capacity && left.profit > maxProfit) //update maxProfit
            maxProfit = left.profit;
        lowerBound = MAX(lowerBound, maxProfit);
        left.upperBound = getUpperBound(left.level, left.profit, left.weight);
        if(left.upperBound > lowerBound && left.upperBound > maxProfit)
            pQueue.push(left);

        // 產生右子樹, 表不拿取此level的item
        Node right = cur;
        ++right.level;
        ++nodes_generated;
        right.upperBound = getUpperBound(right.level, right.profit, right.weight);
        if(right.upperBound > lowerBound && right.upperBound > maxProfit)
            pQueue.push(right);
    }
    if(maxProfit == 0)
        maxProfit = lowerBound;
    //cout << maxProfit << endl;
    //cout << "lowerBound: " << lowerBound << endl;
    //cout << "Maximum profit is : " << maxProfit << endl;
    //cout << "nodes_generated: " << nodes_generated << endl;
    return maxProfit;
}
  
int main()
{
    int value, weight;
    vector<string> fileNames;
    //for(int i = 1; i <= 15; ++i)
    //    fileNames.push_back("test\\test" + to_string(i) + ".input");
    for(int i = 1; i <= 10; ++i)
       fileNames.push_back("data\\" + to_string(i) + ".in");
    //for(int i = 0; i <= 15; ++i)
    //    fileNames.push_back("jg\\" + to_string(i) + ".in");
    for(int i = 0; i < 10; ++i)
    {
        w.clear(); 
        v.clear(); 
        arr.clear();
        //if(i == 9 || i == 12 || i == 14 || i == 15)
        //    continue;
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
            //v.push_back(value);
            //w.push_back(weight);
        }
        fout << knapsack();
    }
    return 0;
}
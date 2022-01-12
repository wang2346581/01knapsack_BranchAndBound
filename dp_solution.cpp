// A dynamic programming based
// solution for 0-1 Knapsack problem
#include <iostream>
#include <vector>
using namespace std;
#define MAX(A, B) (((A)>(B))?(A):(B))

vector<vector<int>> knapSack(int W, int wt[], int val[], int n);
vector<int> knapSack1DMem(int W, int wt[], int val[], int n);
vector<int> knapSack1DMemTotalCase(int W, int wt[], int val[], int n);
void knapsackCombination(int W, int wt[], int val[], int n);
void find_path(vector<int> output, vector<vector<int>> p, int n, int g, int ps, int wt[]);
int knapsackWay(int W, int wt[], int val[], int n);

vector<vector<int>> knapSack(int W, int wt[], int val[], int n)
{
    int k, g;
    vector<vector<int>> f(n + 1, vector<int>(W + 1));
    for(k = 0; k <= n; k++){
        for(g = 0; g <= W; g++){
            if(k == 0 || g == 0)
                f[k][g] = 0;
            else if(wt[k-1] <= g){ // 能放下, 使用判斷式
                f[k][g] = MAX(f[k-1][g], val[k-1] + f[k-1][g-wt[k-1]]);
            }
            else{
                f[k][g] = f[k-1][g];
            }
        }
    }
    // 找出背包裡面有什麼
    //for(int k = n, g = W; k > 0; k--){
    //    if(g - wt[k-1] >= 0 && f[k][g] == val[k-1] + f[k-1][g-wt[k-1]]){
    //        cout << "knapSack has: #" << k << ", val is: " << val[k-1] << endl;
    //        g -= wt[k-1];
    //    }
    //}
    return f;
}


vector<int> knapSack1DMem(int W, int wt[], int val[], int n)
{
    int k, g;
    vector<int> f(W+1, 0); // 右下角的空間幾乎沒用上, 考慮用1D陣列去紀錄以節省空間
    vector<vector<bool>> p(n+1, vector<bool>(W+1, false)); // 記錄放還是不放, false為不放; true為放
    for(k = 1; k <= n; k++){ // 初始條件為1, 因為k=0時根本不用考慮
        for(g = W; g >= 0; g--){ // 由於是用1D陣列來取代上一輪的結果, 所以這邊需要由後往前填
            if(wt[k-1] <= g){
                f[g] = MAX(f[g], val[k-1] + f[g-wt[k-1]]);
                p[k][g] = true;
            }
        }
    }
    // 找出背包裡面有什麼
    for(int k = n, g = W; k > 0; k--){
        if(p[k][g]){
            cout << "knapSack has: #" << k << ", val is: " << val[k-1] << endl;
            g -= wt[k-1];
        }
    }
    // 已建構好1D的表
    g = W;
    while (g-1 >= 0 && f[g-1] == f[g]) g--;
    cout << "物品總重量最少是" << g;
    return f;
}

vector<int> knapSack1DMemTotalCase(int W, int wt[], int val[], int n)
{
    int k, g;
    vector<int> f(W+1, 0); // 右下角的空間幾乎沒用上, 考慮用1D陣列去紀錄以節省空間
    vector<vector<bool>> p(n+1, vector<bool>(W+1, false)); // 記錄放還是不放, false為不放; true為放
    for(k = 1; k <= n; k++){ // 初始條件為1, 因為k=0時根本不用考慮
        for(g = W; g >= 0; g--){ // 由於是用1D陣列來取代上一輪的結果, 所以這邊需要由後往前填
            if(wt[k-1] <= g){
                f[g] = MAX(f[g], val[k-1] + f[g-wt[k-1]]);
                p[k][g] = true;
            }
        }
    }
    // 找出背包裡面有什麼
    for(int k = n, g = W; k > 0; k--){
        if(p[k][g]){
            cout << "knapSack has: #" << k << ", val is: " << val[k-1] << endl;
            g -= wt[k-1];
        }
    }
    // 利用總重W(sum(wt[0:n])), 建構好1D的表格
    int bestw = 0;
    for (int g = 1; g < W; g++)
        if (f[g] > f[bestw])
            bestw = g; // 由左至右掃, 找到最佳解
    cout << "最高的價值為" << f[bestw];
    for (int minw = 0; minw < W; minw++){
        if (f[minw] == f[bestw]){ // 由左邊掃, 遇到最佳就最輕解
            cout << "物品總重量最少是" << minw;
            break;
        }
	}
    for (int maxw = W; maxw >= 0; maxw--){
        if (f[maxw] == f[bestw]){ // 從右邊掃, 遇到最佳就最重解
            cout << "物品總重量最多是" << maxw;
            break;
        }
	}
    return f;
}

void knapsackCombination(int W, int wt[], int val[], int n)
{
    vector<int> f(W+1, 0); // 右下角的空間幾乎沒用上, 考慮用1D陣列去紀錄以節省空間
    vector<vector<int>> p(n+1, vector<int>(W+1, 0)); // 記錄放入狀態, 0為不放; 1為放; 2為可放可不放
    vector<int> output(n+1);
    int k, g;
    for(k = 1; k <= n; k++){ // 初始條件為1, 因為k=0時根本不用考慮
        for(g = W; g >= 0; g--){ // 由於是用1D陣列來取代上一輪的結果, 所以這邊需要由後往前填
            if(wt[k-1] <= g){
                if (f[g - wt[k-1]] + val[k-1] < f[g]){
                    p[k][g] = 0;
                }
                else if (f[g - wt[k-1]] + val[k-1] > f[g]){
                    f[g] = val[k-1] + f[g-wt[k-1]];
                    p[k][g] = 1;
                }
                else{
                    p[k][g] = 2;
                }
            }
        }
    }
    find_path(output, p, n, W, 0, wt);
}


void find_path(vector<int> output, vector<vector<int>> p, int n, int g, int p_size, int wt[])
{
    if (n <= 0){// 找完了
        for (int i=0; i<p_size; i++) cout << output[i] << " ";
        cout << endl;
        return;
    }
 
    if (p[n][g] == 0){    // 不放比較好：背包裡面沒有第n個物品
        find_path(output, p, n-1, g, p_size, wt);
    }
    else if (p[n][g] == 1){ // 放了比較好：背包裡面有第n個物品
        output[p_size] = n;
        find_path(output, p, n-1, g-wt[n-1], p_size+1, wt);
    }
    // 可放可不放
    else if (p[n][g] == 2){
        output[p_size] = n;
        find_path(output, p, n-1, g-wt[n-1], p_size+1, wt); // 背包裡面有第n個物品
        find_path(output, p, n-1, g, p_size, wt);  // 背包裡面沒有第n個物品
    }
}

int knapsackWay(int W, int wt[], int val[], int n)
{
    vector<int> f(W+1, 0); // 右下角的空間幾乎沒用上, 考慮用1D陣列去紀錄以節省空間
    vector<int> way(W+1, 0);
    int k, g;
    way[0] = 1;
    for(k = 1; k <= n; k++){ // 初始條件為1, 因為k=0時根本不用考慮
        for(g = W; g >= 0; g--){ // 由於是用1D陣列來取代上一輪的結果, 所以這邊需要由後往前填
            if(wt[k-1] <= g){
                if (f[g - wt[k-1]] + val[k-1] < f[g]);
                else if (f[g - wt[k-1]] + val[k-1] > f[g]){
                    f[g] = val[k-1] + f[g-wt[k-1]];
                    way[g] = way[g - wt[k-1]]; // 必放, 故沿用上次的組合
                }
                else{ // f[g - wt[k-1]] + val[k-1] == f[g]
                    way[g] = way[g] + way[g - wt[k-1]]; // 可放可不放, 增加組合
                }
            }
        }
        //for(int i =0; i < way.size();i++)
        //    cout << way[i] << " ";
        //cout << endl;
    }
    return way[W];
}


int main()
{
    int val[] = { 60, 60, 100, 120 };
    int wt[] = { 1, 1, 2, 3 };
    int W = 5;
    int n = sizeof(val) / sizeof(val[0]);

    //knapsackCombination(W, wt, val, n); // find all combinations
    //knapsackWay(W, wt, val, n); // find the ways
    
    // total wt solutaion
    //for(int i = 0; i < n; i++)
    //    W += wt[i];
    //knapSack1DMemTotalCase(W, wt, val, n);

    //vector<vector<int>> f1 = knapSack(W, wt, val, n);
    //cout << f1[n][W] << endl;
    //for(int i = 0; i <= n; i++){
    //    for(int j = 0; j <= W; j++){
    //        cout << f1[i][j] << " ";
    //    }
    //    cout << endl;
    //}
    
    //vector<int> f2 = knapSack1DMem(W, wt, val, n);
    //for(int j = 0; j <= W; j++){
    //    cout << f2[j] << " ";
    //} 
    //cout << endl;
    return 0;
}
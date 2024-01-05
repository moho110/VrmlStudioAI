#include <stdio.h>  
#include <stdlib.h>  
  
// 定义最大项目数和最大交易数  
#define MAX_ITEMS 100  
#define MAX_TRANSACTIONS 1000  
  
// 定义最小支持度和最小置信度  
#define MIN_SUPPORT 0.3  
#define MIN_CONFIDENCE 0.7  
  
// 定义结构体  
typedef struct {  
    int items[MAX_ITEMS];          // 交易中的项目  
    int item_count;                // 交易中的项目数量  
} Transaction;  
  
typedef struct {  
    int items[MAX_ITEMS];          // 项集中的项目  
    int item_count;                // 项集中的项目数量  
    int support;                   // 项集的支持度  
} FrequentItemset;  
  
// 计算项集的支持度  
int calculateSupport(FrequentItemset *itemset, Transaction *transactions, int transaction_count) {  
    int support = 0;  
    for (int i = 0; i < transaction_count; i++) {  
        bool contains_all_items = true;  
        for (int j = 0; j < itemset->item_count; j++) {  
            if (findItem(&transactions[i], itemset->items[j]) == ITEM_NOT_FOUND) {  
                contains_all_items = false;  
                break;  
            }  
        }  
        if (contains_all_items) {  
            support++;  
        }  
    }  
    return support / (transaction_count * 1.0);  
}  
  
// 查找交易中的项目  
int findItem(Transaction *transaction, int item) {  
    for (int i = 0; i < transaction->item_count; i++) {  
        if (transaction->items[i] == item) {  
            return i;  
        }  
    }  
    return ITEM_NOT_FOUND;  
}  
  
// 生成候选项集（这里简化处理，直接从交易中生成，而不是通过连接频繁项集生成）  
void generateCandidateItemsets(FrequentItemset *candidates, int *candidate_count, Transaction *transactions, int transaction_count, int itemset_length) {  
    // 初始化候选项集数组和计数器  
    *candidate_count = 0;  
    for (int i = 0; i < transaction_count; i++) {  
        for (int j = 0; j < itemset_length - 1; j++) {  
            FrequentItemset candidate = { .item_count = j + 1 };  
            for (int k = 0; k < j + 1; k++) {  
                candidate.items[k] = transactions[i].items[k];  
            }  
            if (calculateSupport(&candidate, transactions, transaction_count) >= MIN_SUPPORT) {  
                candidates[*candidate_count] = candidate;  
                (*candidate_count)++;  
            }  
        }  
    }  
}  
  
// 主函数，演示如何使用Apriori算法进行关联规则挖掘  
int main() {  
    // 初始化商品交易数据库（这里使用一个静态数组模拟）  
    Transaction transactions[] = {  
        { .items = {1, 2}, .item_count = 2 },  // 交易1: 商品1、商品2在同一个交易中  
        { .items = {3, 4}, .item_count = 2 },  // 交易2: 商品3、商品4在同一个交易中  
        { .items = {1, 2, 3}, .item_count = 3 },  // 交易3: 商品1、商品2、商品3在同一个交易中  
        { .items = {3, 4, 5}, .item_count = 3 },  // 交易4: 商品3、商品4、商品5在同一个交易中  
        { .items = {5}, .item_count = 1 }  // 交易5: 只购买了商品5的交易  
    };  
    int transaction_count = sizeof(transactions) / sizeof(Transaction);  // 总交易数（这里模拟了5个交易）  
    int itemset_length = 2;  // 项集长度（这里只考虑长度为2的项集）
    FrequentItemset candidates[MAX_ITEMS];  // 生成候选项集  
    int candidate_count = 0;  
    generateCandidateItemsets(candidates, &candidate_count, transactions, transaction_count, itemset_length);  
  
    // 输出频繁项集  
    printf("频繁项集:\n");  
    for (int i = 0; i < candidate_count; i++) {  
        printf("项集: ");  
        for (int j = 0; j < candidates[i].item_count; j++) {  
            printf("%d ", candidates[i].items[j]);  
        }  
        printf("支持度: %.2f%%\n", candidates[i].support * 100);  
    }  
  
    // 生成关联规则（这里简化处理，直接从频繁项集生成，而不是通过提升候选项集生成）  
    for (int i = 0; i < candidate_count - 1; i++) {  
        for (int j = i + 1; j < candidate_count; j++) {  
            int itemset[MAX_ITEMS];  
            int itemset_count = 0;  
            for (int k = 0; k < candidates[i].item_count; k++) {  
                itemset[itemset_count++] = candidates[i].items[k];  
            }  
            for (int k = 0; k < candidates[j].item_count; k++) {  
                itemset[itemset_count++] = candidates[j].items[k];  
            }  
            double confidence = calculateConfidence(candidates[i], candidates[j], transaction_count);  
            if (confidence >= MIN_CONFIDENCE) {  
                printf("关联规则: ");  
                for (int k = 0; k < itemset_count; k++) {  
                    printf("%d ", itemset[k]);  
                }  
                printf("=> ");  
                for (int k = 0; k < itemset_count - 1; k++) {  
                    printf("%d ", itemset[k]);  
                }  
                printf("置信度: %.2f%%\n", confidence * 100);  
            }  
        }  
    }  
    return 0;  
}


#include <stdio.h>
#include <time.h>
#include <math.h>

typedef unsigned long long u64;

// --- 核心函数：素性检测 ---

// 64位原生检测
int is_prime_64(u64 n) {
    if (n <= 3) return n > 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    for (u64 i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

// 128位分段模拟检测 (利用同余定理判断 2n^2 + 2n + 1)
int is_prime_128_split(u64 n) {
    // 判定深度 sqrt(2n^2 + 2n + 1)
    u64 limit = (u64)(1.41421356 * (double)n);
    // 只需要检测到 limit 即可
    if (n % 2 == 0) return 0; 
    for (u64 i = 3; i <= limit; i += 2) {
        // 计算 (2n^2 + 2n + 1) % i 防止溢出
        u64 n_mod_i = n % i;
        u64 term1 = (2ULL * n_mod_i) % i;
        term1 = (term1 * n_mod_i) % i;
        u64 term2 = (2ULL * n_mod_i) % i;
        u64 res = (term1 + term2 + 1) % i;
        if (res == 0) return 0;
    }
    return 1;
}

// --- 文件操作 ---

void save_n_only(u64 n) {
    FILE *f = fopen("n_list.txt", "a");
    if (f) {
        fprintf(f, "%llu\n", n);
        fclose(f);
    }
}

void save_checkpoint(u64 n, u64 count, long double sum) {
    FILE *f = fopen("checkpoint.txt", "w");
    if (f) {
        fprintf(f, "%llu %llu %.20Lf", n, count, sum);
        fclose(f);
    }
}

int load_checkpoint(u64 *n, u64 *count, long double *sum) {
    FILE *f = fopen("checkpoint.txt", "r");
    if (f) {
        if (fscanf(f, "%llu %llu %Lf", n, count, sum) == 3) {
            fclose(f);
            return 1;
        }
        fclose(f);
    }
    return 0;
}

// --- 主程序 ---

int main() {
    u64 limit_n = 10000000000ULL;
    u64 n_start = 1;
    u64 count = 0;
    long double sum = 0.0L;
    u64 border = 3037000499ULL; // 64位无符号平方溢出阈值

    // 1. 尝试断点续传
    if (load_checkpoint(&n_start, &count, &sum)) {
        printf("检测到历史进度: n=%llu, 已发现=%llu\n", n_start, count);
        printf("是否继续挖掘? (y/n): ");
        char c; scanf(" %c", &c);
        if (c == 'y' || c == 'Y') n_start++; 
        else { n_start = 1; count = 0; sum = 0.0L; }
    }

    // 2. 打印启动信息
    time_t raw_time; time(&raw_time);
    printf("\n[Tomato-NOS] 探测器引擎启动\n");
    printf("启动时刻: %s", asctime(localtime(&raw_time)));
    printf("目标维度: 100亿\n");
    printf("模式: 分段驱动 (64位原生 + 128位模拟)\n");
    printf("--------------------------------------------------\n");

    time_t start_clock = time(NULL);

    for (u64 n = n_start; n <= limit_n; n++) {
        // 判定 a = 2n + 1
        if (is_prime_64(2 * n + 1)) {
            int c_prime = 0;
            // 判定 c = 2n^2 + 2n + 1
            if (n < border) {
                c_prime = is_prime_64(2ULL * n * n + 2ULL * n + 1ULL);
            } else {
                c_prime = is_prime_128_split(n);
            }

            if (c_prime) {
                count++;
                sum += 1.0L / n;
                
                // 实时保存 n
                save_n_only(n);

                // 每 10000 条打印并存盘
                if (count % 10000 == 0) {
                    time_t now = time(NULL);
                    printf("[提示] 耦合点: %llu | n: %llu | 累积和: %.12Lf | 耗时: %.fs\n", 
                           count, n, sum, difftime(now, start_clock));
                    save_checkpoint(n, count, sum);
                }
            }
        }
    }

    printf("\n探测结束。最终倒数和: %.15Lf\n", sum);
    return 0;
}
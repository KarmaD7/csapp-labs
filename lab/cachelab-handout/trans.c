/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, ii, jj;
    // if (M == 32 && N == 32) {
    int var1, var2, var3, var4, var5, var6, var7, var8;
    if (M == 32 && N == 32) {
        for (i = 0; i < M; i += 8) {
            for (j = 0; j < N; j += 8) {
                if (i == j) {
                    var1 = A[i][j];
                    var2 = A[i + 1][j + 1];
                    var3 = A[i + 2][j + 2];
                    var4 = A[i + 3][j + 3];
                    var5 = A[i + 4][j + 4];
                    var6 = A[i + 5][j + 5];
                    var7 = A[i + 6][j + 6];
                    var8 = A[i + 7][j + 7];
                }
                for (ii = i; ii < i + 8; ++ii) {
                    for (jj = j; jj < j + 8; ++jj) {
                        if (ii == jj) {
                            if (ii - i == 0) B[ii][ii] = var1;
                            if (ii - i == 1) B[ii][ii] = var2;
                            if (ii - i == 2) B[ii][ii] = var3;
                            if (ii - i == 3) B[ii][ii] = var4;
                            if (ii - i == 4) B[ii][ii] = var5;
                            if (ii - i == 5) B[ii][ii] = var6;
                            if (ii - i == 6) B[ii][ii] = var7;
                            if (ii - i == 7) B[ii][ii] = var8;
                        }
                        else B[ii][jj] = A[jj][ii];
                    }
                }
            }
        }
    } else if (M == 64 && N == 64) {
        for (i = 0; i < 64; i += 8) {
            for (j = 0; j < 64; j += 8) {

                for (ii = i; ii < i + 4; ++ii) {
                    var1 = A[j][ii]; var2 = A[j + 1][ii]; var3 = A[j + 2][ii]; var4 = A[j + 3][ii];
                    var5 = A[j][ii + 4]; var6 = A[j + 1][ii + 4]; var7 = A[j + 2][ii + 4]; var8 = A[j + 3][ii + 4]; 
                    B[ii][j] = var1; B[ii][j + 1] = var2; B[ii][j + 2] = var3; B[ii][j + 3] = var4;
                    B[ii][j + 4] = var5; B[ii][j + 5] = var6; B[ii][j + 6] = var7; B[ii][j + 7] = var8;
                    // for (jj = j + 4; jj < j + 8; ++jj) {
                    //     B[ii][jj] = A[jj - 4][ii + 4];
                    // }
                }

                for (ii = i; ii < i + 4; ++ii) {
                    var1 = B[ii][j + 4]; var2 = B[ii][j + 5]; var3 = B[ii][j + 6]; var4 = B[ii][j + 7];
                    var5 = A[j + 4][ii]; var6 = A[j + 5][ii]; var7 = A[j + 6][ii]; var8 = A[j + 7][ii];
                    B[ii][j + 4] = var5; B[ii][j + 5] = var6; B[ii][j + 6] = var7; B[ii][j + 7] = var8; 
                    B[ii + 4][j] = var1; B[ii + 4][j + 1] = var2; B[ii + 4][j + 2] = var3; B[ii + 4][j + 3] = var4; 
                }
                for (ii = i + 4; ii < i + 8; ++ii) {
                    var1 = A[j + 4][ii]; var2 = A[j + 5][ii]; var3 = A[j + 6][ii]; var4 = A[j + 7][ii];
                    B[ii][j + 4] = var1; B[ii][j + 5] = var2; B[ii][j + 6] = var3; B[ii][j + 7] = var4; 
                }
            }
        }
    } else if (M == 61 && N == 67) {
        for (i = 0; i < M; i += 16) {
            for (j = 0; j < N; j += 15) {
                for (int ii = i; ii < i + 16 && ii < M;  ++ii) {
                    for (int jj = j; jj < j + 15 && jj < N; ++jj) {
                        B[ii][jj] = A[jj][ii];
                    }
                }
            }
        }
    }
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}


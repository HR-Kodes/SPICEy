#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "solver.h"

void solve(double **G, double *b, int n)
{
    if (debug_enabled)
    {
        printf("\n[DEBUG] Solving G * x = b:\nInitial system:\n");
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                printf("%8.3f ", G[i][j]);
            printf(" | %8.3f\n", b[i]);
        }
    }

    // Forward elimination with pivot check
    for (int i = 0; i < n; i++)
    {
        if (fabs(G[i][i]) < 1e-12)
        {
            fprintf(stderr, "Error: Singular matrix (pivot=%d).\n", i);
            exit(ERROR_SINGULAR_MATRIX);
        }

        for (int j = i + 1; j < n; j++)
        {
            double factor = G[j][i] / G[i][i];
            for (int k = i; k < n; k++)
                G[j][k] -= factor * G[i][k];
            b[j] -= factor * b[i];
        }

        if (debug_enabled)
        {
            printf("\nAfter column %d elimination:\n", i);
            for (int row = 0; row < n; row++)
            {
                for (int col = 0; col < n; col++)
                    printf("%8.3f ", G[row][col]);
                printf(" | %8.3f\n", b[row]);
            }
        }
    }

    // Back substitution
    if (debug_enabled)
    {
        printf("\nBack substitution:\n");
    }
    for (int i = n - 1; i >= 0; i--)
    {
        for (int j = i + 1; j < n; j++)
            b[i] -= G[i][j] * b[j];
        b[i] /= G[i][i];
        if (debug_enabled)
        {
            printf("x[%d] = %.3f\n", i, b[i]);
        }
    }
}
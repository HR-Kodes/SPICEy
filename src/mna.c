#include <stdio.h>
#include <math.h>
#include "common.h"
#include "mna.h"

void build_MNA(double **G, double *b)
{
    // Initialize matrix and vector to 0
    if (debug_enabled)
    {
        printf("\n[DEBUG] Initial MNA Matrix (G) and RHS (b):\n");
        for (int i = 0; i < num_vars; i++)
        {
            for (int j = 0; j < num_vars; j++)
                printf("%6.2f ", G[i][j]);
            printf(" | %6.2f\n", b[i]);
        }
    }

    for (int i = 0; i < num_components; i++)
    {
        Component c = components[i];
        int n1 = c.node1, n2 = c.node2;

        if (debug_enabled)
        {
            printf("\n[DEBUG] Processing %c%d (nodes %d-%d, value=%.2f)\n",
                   (c.type == RESISTOR) ? 'R' : (c.type == VOLTAGE_SRC) ? 'V'
                                                                        : 'I',
                   i + 1, n1, n2, c.value);
        }

        if (c.type == RESISTOR)
        {
            // Resistor contribution
            double conductance = 1.0 / c.value;
            int var_n1 = variable_indices[n1]; // -1 if fixed
            int var_n2 = variable_indices[n2]; // -1 if fixed

            // Case 1: Both nodes are variables (non-fixed)
            if (var_n1 >= 0 && var_n2 >= 0)
            {
                G[var_n1][var_n1] += conductance;
                G[var_n2][var_n2] += conductance;
                G[var_n1][var_n2] -= conductance;
                G[var_n2][var_n1] -= conductance;
            }
            // Case 2: One node is fixed, the other is variable
            else if (var_n1 >= 0 && var_n2 == -1)
            {
                // Fixed node n2 contributes to RHS (b)
                double v_fixed = fixed_voltage[n2];
                G[var_n1][var_n1] += conductance;
                b[var_n1] += conductance * v_fixed;
            }
            else if (var_n2 >= 0 && var_n1 == -1)
            {
                // Fixed node n1 contributes to RHS (b)
                double v_fixed = fixed_voltage[n1];
                G[var_n2][var_n2] += conductance;
                b[var_n2] += conductance * v_fixed;
            }
            // Case 3: Both nodes are fixed (no contribution to matrix)
            else
            {
                // Do nothing (already handled during parsing)
            }
        }
        else if (c.type == CURRENT_SRC)
        {
            // Current source contribution
            int var_n1 = variable_indices[n1];
            int var_n2 = variable_indices[n2];
            if (var_n1 >= 0)
                b[var_n1] += c.value;
            if (var_n2 >= 0)
                b[var_n2] -= c.value;
        }

        if (debug_enabled)
        {
            printf("  Updated G and b:\n");
            for (int row = 0; row < num_vars; row++)
            {
                for (int col = 0; col < num_vars; col++)
                    printf("%6.2f ", G[row][col]);
                printf(" | %6.2f\n", b[row]);
            }
        }
    }

    if (debug_enabled)
    {
        printf("\n[DEBUG] Final MNA Matrix (G) and RHS (b):\n");
        for (int i = 0; i < num_vars; i++)
        {
            for (int j = 0; j < num_vars; j++)
                printf("%6.2f ", G[i][j]);
            printf(" | %6.2f\n", b[i]);
        }
    }
}
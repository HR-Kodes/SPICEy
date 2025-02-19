#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"
#include "parse.h"

Component components[MAX_COMPONENTS];
int num_components = 0;
int num_nodes = 0;
double fixed_voltage[MAX_NODES + 1] = {0};
int variable_indices[MAX_NODES + 1] = {0};
int num_vars = 0;

int is_node_grounded(int node, int visited[])
{
    if (node == 0)
        return 1; // Ground
    if (visited[node])
        return 0;
    visited[node] = 1;

    for (int i = 0; i < num_components; i++)
    {
        Component c = components[i];
        if (c.type != RESISTOR)
            continue;

        if (c.node1 == node && is_node_grounded(c.node2, visited))
            return 1;
        if (c.node2 == node && is_node_grounded(c.node1, visited))
            return 1;
    }
    return 0;
}

void parse_netlist(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: File '%s' not found.\n", filename);
        exit(ERROR_FILE_NOT_FOUND);
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == '\n' || line[0] == '#')
            continue;

        char type[10];
        int node1, node2;
        double value;
        if (sscanf(line, "%s %d %d %lf", type, &node1, &node2, &value) == 4)
        {
            // Error: Invalid nodes
            if (node1 < 0 || node2 < 0 || node1 > MAX_NODES || node2 > MAX_NODES)
            {
                fprintf(stderr, "Error: Node %d or %d is invalid (max=%d).\n", node1, node2, MAX_NODES);
                exit(ERROR_INVALID_NODE);
            }

            Component c;
            char component_type = type[0];
            switch (component_type)
            {
            case 'R':
                c.type = RESISTOR;
                if (fabs(value) < 1e-9)
                { // Avoid zero resistance
                    fprintf(stderr, "Error: R%d has zero resistance.\n", num_components + 1);
                    exit(ERROR_ZERO_RESISTANCE);
                }
                break;
            case 'V':
                c.type = VOLTAGE_SRC;
                break;
            case 'I':
                c.type = CURRENT_SRC;
                break;
            default:
                fprintf(stderr, "Warning: Skipping invalid component '%s'.\n", type);
                continue;
            }

            c.node1 = node1;
            c.node2 = node2;
            c.value = value;
            components[num_components++] = c;

            // Track max node
            num_nodes = (node1 > num_nodes) ? node1 : num_nodes;
            num_nodes = (node2 > num_nodes) ? node2 : num_nodes;

            // Handle voltage sources
            if (c.type == VOLTAGE_SRC)
            {
                if (node2 == 0)
                    fixed_voltage[node1] = value;
                else if (node1 == 0)
                    fixed_voltage[node2] = -value;
            }

            if (debug_enabled)
            {
                printf("[DEBUG] Parsed: %c%d %d %d %.2f\n", component_type, num_components, node1, node2, value);
            }
        }
    }
    fclose(file);

    // Post-parsing checks
    fixed_voltage[0] = 0.0; // Ground node
    variable_indices[0] = -1;

    // Check for floating nodes
    for (int i = 1; i <= num_nodes; i++)
    {
        if (fixed_voltage[i] != 0)
            continue; // Skip fixed nodes

        int visited[MAX_NODES + 1] = {0};
        if (!is_node_grounded(i, visited))
        {
            fprintf(stderr, "Error: Node %d is floating (no DC path to ground).\n", i);
            exit(ERROR_FLOATING_NODE);
        }
    }

    // Assign variable indices
    num_vars = 0;
    for (int i = 1; i <= num_nodes; i++)
    {
        if (fixed_voltage[i] == 0)
        {
            variable_indices[i] = num_vars++;
        }
        else
        {
            variable_indices[i] = -1;
        }
    }

    if (debug_enabled)
    {
        printf("\n[DEBUG] Fixed Voltages:\n");
        for (int i = 0; i <= num_nodes; i++)
        {
            if (fixed_voltage[i] != 0 || i == 0)
                printf("Node %d: %.2f V\n", i, fixed_voltage[i]);
        }
        printf("\n[DEBUG] Variable Indices:\n");
        for (int i = 1; i <= num_nodes; i++)
        {
            if (variable_indices[i] != -1)
                printf("Node %d → Matrix Row %d\n", i, variable_indices[i]);
        }
        printf("Total variables: %d\n", num_vars);
    }
}
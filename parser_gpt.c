#include <stdio.h>
#include <string.h>
// #include <iostream>

int main() {
    char sql[1000];
    printf("Enter SQL statement:\n");
    fgets(sql, sizeof(sql), stdin);
    printf(sql);
    char *ptr = sql;
    while (*ptr != '\0') {
        if (strncmp(ptr, "SELECT", 6) == 0) {
            // This is a SELECT statement, parse tables and joins
            ptr += 6; // Move past SELECT keyword

            // Skip whitespace
            while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                ptr++;
            }

            // Parse column names
            if (*ptr == '*') {
                // Select all columns
            } else {
                // Parse each column name
                while (*ptr != '\0' && *ptr != ' ') {
                    // TODO: parse column name
                    ptr++;
                }
            }

            // Skip whitespace
            while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                ptr++;
            }

            // Parse FROM keyword
            if (strncmp(ptr, "FROM", 4) == 0) {
                ptr += 4; // Move past FROM keyword

                // Skip whitespace
                while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                    ptr++;
                }

                // Parse table names
                while (*ptr != '\0' && *ptr != ' ') {
                    // TODO: parse table name
                    ptr++;
                }

                // Skip whitespace
                while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                    ptr++;
                }

                // Parse JOIN keyword (if any)
                while (strncmp(ptr, "JOIN", 4) == 0) {
                    ptr += 4; // Move past JOIN keyword

                    // Skip whitespace
                    while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                        ptr++;
                    }

                    // Parse table name
                    while (*ptr != '\0' && *ptr != ' ') {
                        // TODO: parse table name
                        ptr++;
                    }

                    // Skip whitespace
                    while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                        ptr++;
                    }

                    // Parse ON keyword
                    if (strncmp(ptr, "ON", 2) == 0) {
                        ptr += 2; // Move past ON keyword

                        // Skip whitespace
                        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
                            ptr++;
                        }

                        // Parse join condition
                        while (*ptr != '\0' && *ptr != ' ') {
                            // TODO: parse join condition
                            ptr++;
                        }
                    }
                }
            }
        }

        // Move to next token
        while (*ptr != '\0' && *ptr != ' ') {
            ptr++;
        }
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
            ptr++;
        }
    }

    return 0;
}
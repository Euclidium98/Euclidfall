#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *fp;
    signed char num;
    int input;
    char choice;
	char XY = 0;

    printf("Dungeon Item Placer\n");
    printf("(-50 to 50)\n\n");

    // Open the file in binary write mode (creates new file or overwrites existing one)
    fp = fopen("numbers.bin", "wb");
    if (fp == NULL) {
        printf("Error, failed\n");
        return 1;
    }

    printf("Enter numbers one by one. Type 'q' to stop.\n\n");

    while (1) {
		if (XY==0){printf("\nX: "); XY = 1;}
        else{printf("Y: "); XY= 0;}

        // Try to read a number
        if (scanf("%d", &input) == 1) {
            // Validate range
            if (input < -50 || input > 50) {
                printf("Error: Number must be between -50 and 50. Try again.\n");
                // Clear the rest of the line
                while (getchar() != '\n');
                continue;
            }

            num = (signed char)input;

            // Write the number as signed char (1 byte)
            if (fwrite(&num, sizeof(signed char), 1, fp) != 1) {
                printf("Error: Failed to write to file\n");
                fclose(fp);
                return 1;
            }

            printf("Saved: %d\n", (int)num);
        }
        else {
            // If not a number, check if user wants to quit
            // Clear the failed input
            scanf("%c", &choice);
            if (choice == 'q' || choice == 'Q') {
                break;
            }
            else {
                printf("Invalid input. Please enter a number or 'q' to quit.\n");
                // Clear the rest of the line
                while (getchar() != '\n');
            }
        }
    }

    fclose(fp);



    return 0;
}
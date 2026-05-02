//Totalistic Cellular Automaton

#include <stdio.h>
#include <stdbool.h>

const int WORLD_SIZE = 65;

typedef struct cell_struct{
    int localSum; // total sum of local cells, [left] + [me] + [right]: possible values are 0-6
    int status;   // this cell's current status: 0, 1, or 2
    int count;    // running accumulated count of this cell's active status for all generations
} cell;

// print status of the bit
void printCell(int val){
    if (val == 2){
        printf("+"); //#
    } else if (val == 1){
        printf("-"); //+
    } else {
        printf(" ");
    }
}

// convert an integer rule (0-2186) to its base-3 representation, 
// stored in reverse order, as an array of 7 status values, i.e. valArray[7] 
//   ex: rule = 777  -> [0121001], since 777  = 1*3^1 + 2*3^2 + 1*3^3 + 1*3^6   
//   ex: rule = 177  -> [0210200], since 177  = 2*3^1 + 1*3^2 + 2*3^4
//   ex: rule = 2040 -> [0210122], since 2040 = 2*3^1 + 1*3^2 + 1*3^4 + 2*3^5 + 2*3^6
//return true if input rule is valid (0-2186)
//return false if input rule is invalid
bool setValArray(int valArray[7], int rule) {
        if (rule < 0 || rule > 2186){
            return false;
        }

        int val = rule;
        for (int i = 0; i < 7; i++){
            valArray[i] = val % 3;
            val = val /3;
        }
        return true;
}

// update the localSum subitem for each cell in the world array based on the
// current statuses for the nearby [left, me, right] cells
// note: world is periodic/cyclic, with front-to-back and back-to-front wrapping 
void setSums(cell world[WORLD_SIZE]) {

    //TODO: Task 4 - write the setSums() function
    for (int i = 1; i < WORLD_SIZE - 1; i++){
        world[i].localSum = world[i-1].status + world[i].status + world[i+1].status;
    }
    world[0].localSum = world[WORLD_SIZE-1].status + world[0].status + world[1].status;
    world[WORLD_SIZE-1].localSum = world[WORLD_SIZE-2].status + world[WORLD_SIZE-1].status + world[0].status;
}

// Evolve the world to the next generation by updating each cell's status value 
// using its localSum subitem (assumed to be updated outside of this function), where
// ruleValArray[7] stores the status values for the 7 possible local sums, in reverse order, 
// e.g. if local sum = 4, the new status value can be found at index 4 of the ruleValArray[].
// This function must also update the count subitems for all cells in world[] 
// with the new generation status values.
// This function returns the total sum of all status values in world[] after the evolution step.
int evolveWorld(cell world[WORLD_SIZE], int ruleValArray[7]) {

    //TODO: Task 6 - write the evolveWorld() function
    int totSum = 0;

    for (int i = 0; i < WORLD_SIZE; i++){
        world[i].status = ruleValArray[world[i].localSum];
        world[i].count += world[i].status;
        totSum += world[i].status;
    }

    return totSum;
}


int main() {
    cell world[WORLD_SIZE];
    int ruleNumber = -1, numGenerations, initialCell = 0, count = 0, total = 0;
    int arrayValue[7];

    printf("Welcome to the Totalistic Cellular Automaton!\n");

    while (!setValArray(arrayValue, ruleNumber)){
        printf("Enter the rule # (0-2186): ");
        scanf("%d", &ruleNumber);
        printf("\n");
    }
    
    printf("\nThe value array for rule #%d is ", ruleNumber);
    for (int j = 6; j >= 0; j--){
        printf("%d", arrayValue[j]);
    }
    printf("\n\n");
    
    printf("The evolution of all possible states are as follows:\n");
    printf("Local Sum:   6       5       4       3       2       1       0      \n");
    printf("States:    |+++|   |++-|   |++ |   |+- |   |-- |   |-  |   |   |    \n");
    printf("                   |+-+|   |+ +|   |+ -|   |- -|   | - |            \n");
    printf("                   |-++|   | ++|   |-+ |   | --|   |  -|            \n");
    printf("                           |+--|   | +-|   |+  |                    \n");
    printf("                           |-+-|   |- +|   | + |                    \n");
    printf("                           |--+|   | -+|   |  +|                    \n");
    printf("                                   |---|                            \n");
    //TODO: Task 3 - use the rule status value array to report the evolution 
    //      step for all possible cell neighbor states [left, me, right].
    //      Follow the format, including whitespaces, of the sample output EXACTLY.
    printf("New Status: ");
    for (int i = 6; i >= 0; i--){
        printf("|");
        printCell(arrayValue[i]);
        printf("|     ");
    }
    printf("\n\n");

    while (numGenerations < 1 || numGenerations > 49) {
        printf("Enter the number of generations (1-49): ");
        scanf("%d", &numGenerations);
        printf("\n");
    }

    //TODO: Task 5a - read in the total number of generation evolution steps
    //      from the user, allowing repeated attempts for invalid values.
    //      Follow the format, including whitespaces, of the sample output EXACTLY.
    while (initialCell != 1 && initialCell != 2) {
        printf("Enter the value (1 or 2) for the initial active cell: ");
        scanf("%d", &initialCell);
        printf("\n");
    }

    //TODO: Task 5b - read in the initial status value for the middle cell, 
    //      allowing repeated attempts for invalid values. 
    //      Follow the format, including whitespaces, of the sample output EXACTLY.
    printf("Initializing world & evolving...\n");

    for(int i = 0; i < WORLD_SIZE; i++){
        world[i].status = 0;
        world[i].localSum = 0;
        world[i].count = 0;
    }
    world[WORLD_SIZE / 2].status = initialCell;
    world[WORLD_SIZE / 2].count = initialCell;
    setSums(world);

    for (int i = 0; i < WORLD_SIZE; i++){
        printCell(world[i].status);
    }
    printf(" %d \n", initialCell);
    //TODO: Task 5c - initialize the world with the ONLY non-zero cell in the 
    //      exact middle of the world, whose status value is set using the 
    //      scanned-in value from the user above. Make sure to also set the 
    //      localSum subitem for each cell of the initial world.
    //      Follow the format, including whitespaces, of the sample output EXACTLY.

    for (int g = 1; g < numGenerations; g++){
        count = evolveWorld(world, arrayValue);
        for (int i = 0; i < WORLD_SIZE; i++){
            printCell(world[i].status);
        }
        printf(" %d \n", count);
        setSums(world);
    }

    for (int i = 0; i < WORLD_SIZE; i++){
        printf("_");
    }
    printf("\n");

    //TODO: Task 7 - evolve the world the user-specified number of generations,
    //      printing the world after each evolution step, where each cell prints as
    //      '+', '-', or ' ' (whitespace) for status value of 2, 1, or 0, respectively.
    //      After the world display, also print out the total sum of all status values.
    //      Follow the format, including whitespaces, of the sample output EXACTLY.
    //
    //      Ex: world = [00000000000000000020101212111001210011121210102000000000000000000]
    //          display->                  + - -+-+---  -+-  ---+-+- - +                   28 <-display
    //
    //      At the end of ALL evolution steps, print a line under the last generation. 
    //      Ex: world = [02211210000121122111001121121001210012112110011122112100001211220]
    //          display-> ++--+-    -+--++---  --+--+-  -+-  -+--+--  ---++--+-    -+--++  64 <-display
    //          display->_________________________________________________________________<-display
    for (int i = 0; i < WORLD_SIZE; i++){
        if (world[i].count < 10){
            printf(" ");
        } else {
            printf("%d", (world[i].count / 10) % 10);
        }
    }
    printf("\n");


    //TODO: Task 8 - after the line under the last generation, 
    //      print the total sum for each cell throughout all evolution steps vertically.
    //      The cell total sums should be printed as two digit numbers across two lines.
    //      Single-digit total sums should be represented as a whitespace on the top line, 
    //      and the single-digit on the lower line. 
    //      For total sums of zero, print both digits as whitespaces.
    //      Follow the format, including whitespaces, of the sample output EXACTLY.
    //
    //      Ex: consider a world after many evolution steps with the following final generation:
    //          display-> ++--+-    -+--++---  --+--+-  -+-  -+--+--  ---++--+-    -+--++  64 <-display
    //          display->_________________________________________________________________<-display   
    //          display->            11111111122121222233233222212122111111111            <-display 
    //          display-> 243666698963456647182091851471191174158190281746654369896666342 <-display 
    //                   ^ ^                             ^
    //      note: the first cell was ALWAYS 0, so its total sum is zero, and prints as two whitespaces
    //            the third cell has a total sum of 4, so it prints as a whitespace above the digit 4
    //            the middle cell has a total sum of 29, so it prints the digit 2 above the digit 9
    for (int i =0; i < WORLD_SIZE; ++i){
        if (world[i].count == 0){
            printf(" ");
        } else {
            printf("%d", world[i].count % 10);
        }
    }
    
    printf("\n"); // make sure to end your program with a newline

    return 0;
}
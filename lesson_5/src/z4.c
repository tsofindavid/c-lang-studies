#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h" // NEMENIT, inak vas kod neprejde testom !!!

// chybove hlasenia
#define ERR_UNIT_COUNT "ERR_UNIT_COUNT"
#define ERR_ITEM_COUNT "ERR_ITEM_COUNT"
#define ERR_WRONG_ITEM "ERR_WRONG_ITEM"
#define ERR_SLOTS "ERR_SLOTS"

#define MAX_ARMIES 2
#define MAX_SLOTS 2

typedef enum status { RUNNING, WIN, LOSS, TIE } STATUS;

typedef struct armada_t {
    int index;
    UNIT* units;
    int num_units;

} ARMY;
const ITEM* findItemByName(char* name) {
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
        if (strcmp(name, items[i].name) == 0) {
            return &items[i];
        }
    }
    return NULL;
}

int isCorrectUnitCount(int num_unit) {
    if (num_unit < MIN_ARMY || num_unit > MAX_ARMY) {
        return 0;
    }
    return 1;
}

int max(int a, int b) { return (a > b) ? a : b; }

int isWrongSlots(UNIT myunit) {
    int sum = 0;
    if (myunit.item1 != NULL && myunit.item2 != NULL) {
        sum = myunit.item1->slots + myunit.item2->slots;
    } else if (myunit.item1 != NULL) {
        sum = myunit.item1->slots;
    } else if (myunit.item2 != NULL) {
        sum = myunit.item2->slots;
    }
    if (sum > MAX_SLOTS) {
        return 1;
    }
    return 0;
}

int isGameOver(ARMY* armies) {
    if (armies[0].num_units <= 0 && armies[1].num_units <= 0) {
        printf("NO WINNER\n");
        return 1;
    }

    if (armies[1].num_units <= 0) {
        printf("WINNER: 1\n");
        return 1;
    }

    if (armies[0].num_units <= 0) {
        printf("WINNER: 2\n");
        return 1;
    }

    return 0;
}

char* readline(void) {
    static char buffer[MAX_NAME + 1] = {0};
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\r\n")] = '\0';
    return buffer;
}

void print_armies(ARMY* armies) {
    for (int a = 0; a < MAX_ARMIES; a++) {
        if (a != 0) {
            printf("\n");
        }

        printf("Army %d\n", armies[a].index + 1);

        for (int u = 0; u < armies[a].num_units; u++) {
            if (u != 0) {
                printf("\n");
            }

            printf("    Unit: %d\n", u);
            printf("    Name: %s\n", armies[a].units[u].name);
            printf("    HP: %d\n", armies[a].units[u].hp);

            if (armies[a].units[u].item1 != NULL) {
                printf("    Item 1: %s,%d,%d,%d,%d,%d\n", armies[a].units[u].item1->name,
                       armies[a].units[u].item1->att, armies[a].units[u].item1->def,
                       armies[a].units[u].item1->slots, armies[a].units[u].item1->range,
                       armies[a].units[u].item1->radius);
            }

            if (armies[a].units[u].item2 != NULL) {
                printf("    Item 2: %s,%d,%d,%d,%d,%d\n", armies[a].units[u].item2->name,
                       armies[a].units[u].item2->att, armies[a].units[u].item2->def,
                       armies[a].units[u].item2->slots, armies[a].units[u].item2->range,
                       armies[a].units[u].item2->radius);
            }
        }
    }
}

void print_units(ARMY* armies) {
    char* line;
    for (int a = 0; a < MAX_ARMIES; a++) {
        line = (char*)malloc((armies[a].num_units + 1) * ((sizeof(char) * MAX_NAME) + 6));

        sprintf(line + strlen(line), "%d: ", armies[a].index + 1);

        for (int u = 0; u < armies[a].num_units; u++) {
            if (u != 0) {
                sprintf(line + strlen(line), " ");
            }

            sprintf(line + strlen(line), "%s,%d ", armies[a].units[u].name, armies[a].units[u].hp);
        }

        printf("%s\n", line);
    }
}

ARMY army_attack(ARMY from, ARMY to) {
    for (int fu = 0; fu < from.num_units; fu++) {
        char* line;
        char* header;

        int headerSize = MAX_NAME * 2 + 3;

        if (from.units[fu].item1 != NULL) {
            line = (char*)malloc(headerSize);
            header = (char*)malloc(headerSize);

            sprintf(header + strlen(header), "%d,%s,%s:", from.index + 1, from.units[fu].name,
                    from.units[fu].item1->name);

            sprintf(line + strlen(line), "%-21s", header);

            for (int tu = 0; tu < to.num_units; tu++) {
                int def = 0;
                if (to.units[tu].item1 != NULL) {
                    def += to.units[tu].item1->def;
                }

                if (to.units[tu].item2 != NULL) {
                    def += to.units[tu].item2->def;
                }

                int att = max(1, from.units[fu].item1->att - def);

                if (tu <= from.units[fu].item1->radius) {
                    to.units[tu].hp -= att;

                    line = realloc(line, MAX_NAME + 6);

                    sprintf(line + strlen(line), "[%s,%d] ", to.units[tu].name, att);
                }
            }

            printf("%s\n", line);

            free(line);
            free(header);
        }

        if (from.units[fu].item2 != NULL) {
            line = (char*)malloc(headerSize);
            header = (char*)malloc(headerSize);

            sprintf(header + strlen(header), "%d,%s,%s:", from.index + 1, from.units[fu].name,
                    from.units[fu].item2->name);

            sprintf(line + strlen(line), "%-21s", header);

            for (int tu = 0; tu < to.num_units; tu++) {
                int def = 0;
                if (to.units[tu].item1 != NULL) {
                    def += to.units[tu].item1->def;
                }

                if (to.units[tu].item2 != NULL) {
                    def += to.units[tu].item2->def;
                }

                int att = max(1, from.units[fu].item2->att - def);

                // printf("%s, att: %d, def: %d\n", from.units[fu].name, att, def);
                // printf("from.units[fu]: %s\n", from.units[fu].item1->name);
                // printf("tu <= from.units[fu].item1->radius %d\n",
                //    tu <= from.units[fu].item1->radius);
                if (tu <= from.units[fu].item2->radius) {
                    to.units[tu].hp -= att;

                    line = realloc(line, MAX_NAME + 6);

                    sprintf(line + strlen(line), "[%s,%d] ", to.units[tu].name, att);
                }
            }

            printf("%s\n", line);
            free(line);
            free(header);
        }
    }

    return to;
}

ARMY army_clean(ARMY army) {
    int num_units = 0;
    UNIT* units;
    units = (UNIT*)malloc(0);

    for (int u = 0; u < army.num_units; u++) {
        if (army.units[u].hp > 0) {
            units = (UNIT*)realloc(units, sizeof(UNIT));
            units[u] = army.units[u];

            num_units += 1;
        }
    }

    army.num_units = num_units;
    army.units = units;

    return army;
}

int main(const int argc, char* argv[]) {
    ARMY armies[MAX_ARMIES];

    char* line;
    char* buff;

    for (int a = 0; a < MAX_ARMIES; a++) {
        line = readline();

        armies[a].index = a;
        armies[a].num_units = atoi(line);

        if (!isCorrectUnitCount(armies[a].num_units)) {
            printf("%s\n", ERR_UNIT_COUNT);
            return 0;
        }

        armies[a].units = (UNIT*)malloc(armies[a].num_units * sizeof(UNIT));

        for (int u = 0; u < armies[a].num_units; u++) {
            int param = 0;

            armies[a].units[u].hp = 100;

            line = readline();
            buff = strtok(line, " ");
            while (buff != NULL) {
                switch (param) {
                case 0:
                    strcpy(armies[a].units[u].name, buff);
                    break;
                case 1:
                    armies[a].units[u].item1 = findItemByName(buff);
                    if (armies[a].units[u].item1 == NULL) {
                        printf("%s", ERR_WRONG_ITEM);
                        return 0;
                    }
                    break;
                case 2:
                    armies[a].units[u].item2 = findItemByName(buff);
                    if (armies[a].units[u].item2 == NULL) {
                        printf("%s", ERR_WRONG_ITEM);
                        return 0;
                    }
                    break;
                default:
                    printf("%s", ERR_ITEM_COUNT);
                    return 0;
                }

                buff = strtok(NULL, " ");
                param++;
            }

            if (param == 1) {
                printf("%s", ERR_ITEM_COUNT);
                return 0;
            }

            if (isWrongSlots(armies[a].units[u])) {
                printf("%s\n", ERR_SLOTS);
                return 0;
            }
        }
    }

    print_armies(armies);

    int max_rounds = -1;
    if (argc == 2) {
        if (strncmp(argv[1], "N=", 2) == 0) {
            max_rounds = atoi(argv[1] + 2);
        }
    }

    int round = 0;
    ARMY army_1;
    ARMY army_2;

    printf("\n");

    while (!isGameOver(armies)) {
        if (max_rounds != -1 && round >= max_rounds) {
            return 0;
        }

        printf("Round %d\n", round + 1);

        print_units(armies);

        army_2 = army_attack(armies[0], armies[1]);
        army_1 = army_attack(armies[1], armies[0]);

        // armies[0] = army_clean(army_1);
        // armies[1] = army_clean(army_2);

        // print_units(armies);

        printf("\n");
        round++;

        if (round > 10) {
            break;
        }
    }

    return 0;
}

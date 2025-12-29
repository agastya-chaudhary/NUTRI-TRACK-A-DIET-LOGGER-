#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

//  DATABASE 
struct FoodItem {
    char name[99];
    char category[99];
    int calories;
    float protein;
    float carbs;
    float sugar;
};

struct FoodItem database[] = {
    {"Apple", "Fruit", 52, 0.3f, 14.0f, 10.0f},
    {"Banana", "Fruit", 89, 1.1f, 23.0f, 12.0f},
    {"Orange", "Fruit", 47, 0.9f, 12.0f, 9.0f},
    {"Strawberry", "Fruit", 33, 0.7f, 8.0f, 4.9f},
    {"Blueberry", "Fruit", 57, 0.7f, 14.0f, 10.0f},
    {"Grapes", "Fruit", 69, 0.7f, 18.0f, 16.0f},
    {"Pineapple", "Fruit", 50, 0.5f, 13.0f, 10.0f},
    {"Mango", "Fruit", 60, 0.8f, 15.0f, 14.0f},
    {"Watermelon", "Fruit", 30, 0.6f, 8.0f, 6.0f},
    {"Kiwi", "Fruit", 61, 1.1f, 15.0f, 9.0f},

    {"Broccoli", "Vegetable", 55, 3.7f, 11.0f, 2.2f},
    {"Spinach", "Vegetable", 23, 2.9f, 4.0f, 0.4f},
    {"Carrot", "Vegetable", 41, 0.9f, 10.0f, 4.7f},
    {"Tomato", "Vegetable", 18, 0.9f, 3.9f, 2.6f},
    {"Cucumber", "Vegetable", 16, 0.7f, 3.6f, 1.7f},
    {"Bell Pepper", "Vegetable", 31, 1.0f, 6.0f, 4.2f},
    {"Cauliflower", "Vegetable", 25, 1.9f, 5.0f, 1.9f},
    {"Onion", "Vegetable", 40, 1.1f, 9.0f, 4.2f},
    {"Garlic", "Vegetable", 149, 6.4f, 33.0f, 1.0f},
    {"Potato", "Vegetable", 77, 2.0f, 17.0f, 0.8f},

    {"Chicken Breast", "Meat", 165, 31.0f, 0.0f, 0.0f},
    {"Salmon", "Fish", 208, 20.0f, 0.0f, 0.0f},
    {"Egg", "Protein", 155, 13.0f, 1.0f, 1.0f},
    {"Turkey", "Meat", 189, 29.0f, 0.0f, 0.0f},
    {"Beef", "Meat", 250, 26.0f, 0.0f, 0.0f},
    {"Tuna", "Fish", 132, 28.0f, 0.0f, 0.0f},
    {"Shrimp", "Seafood", 99, 24.0f, 0.2f, 0.2f},
    {"Cod", "Fish", 82, 18.0f, 0.0f, 0.0f},
    {"Mackerel", "Fish", 205, 19.0f, 13.0f, 0.0f},
    {"Lamb", "Meat", 294, 25.0f, 21.0f, 0.0f},

    {"Almonds", "Nuts", 576, 21.0f, 22.0f, 4.0f},
    {"Peanuts", "Nuts", 567, 25.0f, 16.0f, 4.0f},
    {"Cashews", "Nuts", 553, 18.0f, 30.0f, 5.0f},
    {"Walnuts", "Nuts", 654, 15.0f, 14.0f, 2.6f},
    {"Oats", "Grain", 389, 17.0f, 66.0f, 1.0f},
    {"Rice (White)", "Grain", 130, 2.7f, 28.0f, 0.0f},
    {"Rice (Brown)", "Grain", 123, 2.6f, 25.6f, 0.5f},
    {"Quinoa", "Grain", 120, 4.4f, 21.3f, 0.9f},
    {"Bread (Whole Wheat)", "Grain", 247, 13.0f, 41.0f, 5.0f},
    {"Pasta", "Grain", 131, 5.0f, 25.0f, 1.1f},

    {"Milk (Whole)", "Dairy", 61, 3.1f, 5.0f, 5.0f},
    {"Cheddar Cheese", "Dairy", 403, 25.0f, 1.0f, 0.5f},
    {"Yogurt (Plain)", "Dairy", 59, 10.0f, 4.0f, 3.0f},
    {"Cottage Cheese", "Dairy", 98, 11.0f, 3.0f, 2.0f},
    {"Butter", "Dairy", 717, 0.9f, 0.0f, 0.1f},
    {"Olive Oil", "Fat", 884, 0.0f, 0.0f, 0.0f},
    {"Peanut Butter", "Spread", 588, 25.0f, 20.0f, 9.0f},
    {"Honey", "Spread", 304, 0.3f, 82.0f, 82.0f},
    {"Jam", "Spread", 250, 0.4f, 60.0f, 50.0f},
    {"Maple Syrup", "Spread", 260, 0.0f, 67.0f, 60.0f}
};

int dbSize = sizeof(database) / sizeof(database[0]);

struct FoodNode {
    char name[100];
    int calories;
    float protein;
    float carbs;
    float sugar;
    struct FoodNode* left;
    struct FoodNode* right;
};

// DAILY HISTORY 
struct HistoryNode {
    char name[100];
    int calories;
    float protein;
    float carbs;
    float sugar;
    struct HistoryNode* next;
};

struct HistoryNode* historyHead = NULL;

//STACK FOR UNDO 
struct FoodNode* undoStack[500];
int top = -1;

void push(struct FoodNode* node) {
    if (top < 499) undoStack[++top] = node;
}

struct FoodNode* pop() {
    if (top >= 0) return undoStack[top--];
    return NULL;
}

// BST FUNCTIONS 
struct FoodNode* createNodeFromItem(const struct FoodItem *item) {
    struct FoodNode* node = malloc(sizeof(struct FoodNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    strncpy(node->name, item->name, sizeof(node->name)-1);
    node->name[sizeof(node->name)-1] = '\0';
    node->calories = item->calories;
    node->protein = item->protein;
    node->carbs = item->carbs;
    node->sugar = item->sugar;
    node->left = node->right = NULL;
    return node;
}

struct FoodNode* insertNode(struct FoodNode* root, struct FoodNode* temp){
    if(root == NULL) return temp;
    if(temp->calories < root->calories)
        root->left = insertNode(root->left, temp);
    else
        root->right = insertNode(root->right, temp);
    return root;
}

struct FoodNode* deleteNode(struct FoodNode* root, int calories) {
    if (!root) return NULL;

    if (calories < root->calories)
        root->left = deleteNode(root->left, calories);
    else if (calories > root->calories)
        root->right = deleteNode(root->right, calories);
    else {
        if (!root->left) { 
            struct FoodNode* t = root->right; 
            free(root); 
            return t; 
        }
        if (!root->right){ 
            struct FoodNode* t = root->left; 
            free(root); 
            return t; 
        }

        struct FoodNode* temp = root->right;
        while (temp->left) temp = temp->left;

        root->calories = temp->calories;
        strcpy(root->name, temp->name);
        root->protein = temp->protein;
        root->carbs = temp->carbs;
        root->sugar = temp->sugar;

        root->right = deleteNode(root->right, temp->calories);
    }
    return root;
}

void displayNode(const struct FoodNode* root){
    if(root == NULL) return;
    printf("\nName: %s\n", root->name);
    printf("Calories: %d kcal\n", root->calories);
    printf("Protein: %.2fg\n", root->protein);
    printf("Carbs: %.2fg\n", root->carbs);
    printf("Sugar: %.2fg\n", root->sugar);
}

void inorderDisplay(struct FoodNode* root){
    if(root == NULL) return;
    inorderDisplay(root->left);
    displayNode(root);
    inorderDisplay(root->right);
}

void computeTotals(struct FoodNode* root, int *calSum, float *proteinSum, float *carbsSum, float *sugarSum){
    if(root == NULL) return;
    computeTotals(root->left, calSum, proteinSum, carbsSum, sugarSum);
    *calSum += root->calories;
    *proteinSum += root->protein;
    *carbsSum += root->carbs;
    *sugarSum += root->sugar;
    computeTotals(root->right, calSum, proteinSum, carbsSum, sugarSum);
}

void freeTree(struct FoodNode* root){
    if(root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

//LINKED LIST FUNCTIONS 
void addToHistory(const struct FoodItem* item) {
    struct HistoryNode* node = malloc(sizeof(struct HistoryNode));
    if (!node) return;

    strcpy(node->name, item->name);
    node->calories = item->calories;
    node->protein = item->protein;
    node->carbs = item->carbs;
    node->sugar = item->sugar;
    node->next = NULL;

    if (historyHead == NULL)
        historyHead = node;
    else {
        struct HistoryNode* temp = historyHead;
        while (temp->next) temp = temp->next;
        temp->next = node;
    }
}

void removeLastHistory(int calories) {
    if (!historyHead) return;

    if (historyHead->calories == calories) {
        struct HistoryNode* t = historyHead;
        historyHead = historyHead->next;
        free(t);
        return;
    }

    struct HistoryNode* prev = historyHead;
    struct HistoryNode* curr = historyHead->next;

    while (curr) {
        if (curr->calories == calories) {
            prev->next = curr->next;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void showHistory() {
    if (!historyHead) {
        printf("\nNo meals added today.\n");
        return;
    }

    printf("\n===== DAILY FOOD HISTORY (Linked List) =====\n");
    struct HistoryNode* temp = historyHead;
    while (temp) {
        printf("%s - %d kcal, Protein %.1fg, Carbs %.1fg, Sugar %.1fg\n",
               temp->name, temp->calories, temp->protein, temp->carbs, temp->sugar);
        temp = temp->next;
    }
}

void freeHistory() {
    struct HistoryNode* current = historyHead;
    while (current) {
        struct HistoryNode* next = current->next;
        free(current);
        current = next;
    }
    historyHead = NULL;
}

//FILE I/O
void fileInput(int totalCalories, float totalProtein, float totalCarbs, float totalSugar){
    FILE *file = fopen("DailyData.txt", "a+");
    if (file == NULL) {
        printf("Error opening file '%s' for appending\n", "DailyData.txt");
        return;
    }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char date[32];
    strftime(date, sizeof(date), "%d-%m-%Y | %H:%M:%S", t);

    fprintf(file, "%s\n", date);
    fprintf(file, "Calorie Intake: %d kcal\n", totalCalories);
    fprintf(file, "Protein: %.2fg\n", totalProtein);
    fprintf(file, "Carbohydrates: %.2fg\n", totalCarbs);
    fprintf(file, "Sugar: %.2fg\n", totalSugar);
    fprintf(file, "------------------------\n");
    fclose(file);
    printf("Daily totals appended to TXT File\n");
}

//NUTRIENT CHART
void printNutrientChart(struct FoodNode* root) {
    if (root == NULL) {
        printf("No food data available.\n");
        return;
    }

    int totalCal = 0;
    float totalProtein = 0, totalCarbs = 0, totalSugar = 0;
    computeTotals(root, &totalCal, &totalProtein, &totalCarbs, &totalSugar);

    float goalCalories, goalProtein, goalCarbs, goalSugar;

    printf("\nEnter your daily nutrient goals:\n");
    printf("Calories goal (kcal): ");
    if (scanf("%f", &goalCalories) != 1) { 
        while (getchar()!='\n'); 
        printf("Invalid input.\n"); 
        return; 
    }
    printf("Protein goal (g): ");
    if (scanf("%f", &goalProtein) != 1) { 
        while (getchar()!='\n'); 
        printf("Invalid input.\n"); 
        return; 
    }
    printf("Carbs goal (g): ");
    if (scanf("%f", &goalCarbs) != 1) { 
        while (getchar()!='\n'); 
        printf("Invalid input.\n"); 
        return; 
    }
    printf("Sugar goal (g): ");
    if (scanf("%f", &goalSugar) != 1) { 
        while (getchar()!='\n'); 
        printf("Invalid input.\n"); 
        return; 
    }
    while (getchar()!='\n');

    const char *labels[] = {"Calories", "Protein", "Carbs", "Sugar"};
    float totals[] = { (float)totalCal, totalProtein, totalCarbs, totalSugar };
    float goals[]  = { goalCalories, goalProtein, goalCarbs, goalSugar };
    const char *units[]  = {"kcal", "g", "g", "g"};
    int n = 4;

    printf("\n========= DAILY NUTRIENT PROGRESS =========\n");

    for (int i = 0; i < n; i++) {
        float percent = (goals[i] > 0.0001f) ? (totals[i] / goals[i]) * 100.0f : 0.0f;
        if (percent > 100.0f) percent = 100.0f;
        int barLength = (int)(percent / 2.0f);

        printf("%-8s | ", labels[i]);
        for (int j = 0; j < barLength; j++) printf("=");
        printf(" %.1f%%  (%.1f %s of %.1f %s)\n", 
               (goals[i] > 0.0001f) ? (totals[i] / goals[i]) * 100.0f : 0.0f,
               totals[i], units[i], goals[i], units[i]);
    }

    
}

void readLine(char *buf, size_t size){
    if (fgets(buf, (int)size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
}

int caseInsensitiveEqual(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == '\0' && *b == '\0';
}

const struct FoodItem* searchFood(const char *name) {
    for (int i = 0; i < dbSize; ++i) {
        if (caseInsensitiveEqual(database[i].name, name)) 
            return &database[i];
    }
    return NULL;
}

//MAIN 
int main(void) {
    struct FoodNode* root = NULL;
    char input[200];
    int choice;

        do {
        printf("\nFood Database Menu\n");
        printf("1) Add food item\n");
        printf("2) Display sorted food items \n");
        printf("3) Save totals to file\n");
        printf("4) Display nutrient chart\n");
        printf("5) Show Daily History \n");
        printf("6) Undo last added food\n");
        printf("7) Exit\n");
        printf("Choose an option (1-7): ");
        
        scanf("%d",&choice);
        while(getchar()!='\n');
        switch(choice)
        {
            case 1:
            printf("Enter food name: ");
            readLine(input, sizeof(input));
            
            if (input[0] == '\0') {
                printf("Empty name\n");
                continue;
            }

            const struct FoodItem *item = searchFood(input);
            if (!item) {
                printf("Food '%s' not found in database.\n", input);
                continue;
            }

            struct FoodNode* node = createNodeFromItem(item);
            if (node != NULL) {
                root = insertNode(root, node);
                push(node);
                addToHistory(item);
                printf("Added '%s'\n", item->name);
            }
            break;
            
            case 2:
            if (root == NULL) {
                printf("No food items to display.\n");
            } else {
                printf("\n-- Food items (sorted by calories) --\n");
                inorderDisplay(root);
            }
            break;

            case 3:{
            int totalCal = 0;
            float totalProtein = 0.0f, totalCarbs = 0.0f, totalSugar = 0.0f;
            computeTotals(root, &totalCal, &totalProtein, &totalCarbs, &totalSugar);
            fileInput(totalCal, totalProtein, totalCarbs, totalSugar);
            break;
            }

            case 4:
            printNutrientChart(root);
            break;

            case 5:
            showHistory();
            break;

            case 6:{
            struct FoodNode* last = pop();
            if (!last) {
                printf("Nothing to undo.\n");
            } else {
                root = deleteNode(root, last->calories);
                removeLastHistory(last->calories);
                printf("Undo completed. Removed: %s\n", last->name);
            }
            break;
        }

            case 7:
            printf("Exiting. Goodbye!\n");
            freeTree(root);
            freeHistory();
            exit(0);
            break;

            default:
            printf("Invalid choice");
            break;
        }
    }while(choice!=7);
    return 0;
}
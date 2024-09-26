#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUCTS 50
#define DEFAULT_ID 9999
#define PRODUCTS_PER_PAGE 10
#define CART_SIZE 10

// Fix the max row of displaying for formating purposes
const int max_row = 15;
// Current Product selected
int selected_id = 999;
// Current Page Selected
int current_page = 0;


// Recipt Struct containing all details about a recipt
typedef struct {
    int products_id[100]; 
    int quantities[CART_SIZE]; 
    float totalCost;
    float moneyReceived;
    float changeReturned;
    int count;
} Receipt;

// Product struct includes products details
typedef struct {
        int id;
        char name[20];
        float price;
        int quantity;
} Product;
// Storage for persistent prompts
typedef struct{
        int str_count ;
        char **str_array;
} StringBuffer;

// context for ease of accessing dynamically generated structs during runtime 
typedef struct {
        StringBuffer buffer;
        Receipt receipt;
} AppContext;

// Initialize the context in global 
static AppContext context;

void clear_input_buffer();              
void display_products();
void sorting();
void search();
void purchase_product();              
void generate_receipt();
void heading();

void ending(int);                       

       
void display(char [], int );
void sorting_choice(char , char);
int search_product(int *, char *);
void progress_bar(int );
void printing(char *);
void to_lowercase(char *);



void buffer_append( const char *new_string);
void clear_buffer();
int get_occurrences(char *);

char buffer[200];

// All available products
Product product_list[50] = {
    {0, "Soda", 1.50, 1},
    {1, "Chips", 1.00, 15},
    {2, "Candy Bar", 0.75, 20},
    {3, "Chocolate", 1.75, 12},
    {4, "Cookies", 2.00, 8},
    {5, "Peanuts", 1.25, 25},
    {6, "Gum", 0.50, 30},
    {7, "Juice", 2.50, 7},
    {8, "Granola Bar", 1.00, 14},
    {9, "Trail Mix", 22.25, 18},
    {10, "Energy Drink", 3.00, 10},
    {11, "Water Bottle", 0.80, 20},
    {12, "Fruit Snacks", 1.20, 15},
    {13, "Nuts Mix", 2.50, 10},
    {14, "Protein Bar", 2.00, 5},
    {15, "Jerky", 3.50, 8},
    {16, "Ice Cream", 2.75, 6},
    {17, "Pudding Cup", 1.50, 12},
    {18, "Mints", 0.90, 30},
    {19, "Pretzels", 1.00, 25},
    {20, "Brownies", 2.50, 10},
    {21, "Cupcake", 1.50, 15},
    {22, "Pasta", 1.75, 12},
    {23, "Rice", 0.99, 20},
    {24, "Bread", 2.50, 8},
    {25, "Pizza", 8.00, 5},
    {26, "Salad", 3.00, 20},
    {27, "Sandwich", 4.50, 15},
    {28, "Bagel", 1.25, 10},
    {29, "Toast", 0.80, 25},
    {30, "Taco", 3.00, 18},
    {31, "Nachos", 2.00, 12},
    {32, "Doughnut", 1.00, 30},
    {33, "Pancake", 2.25, 14},
    {34, "Waffle", 2.75, 10},
    {35, "Ice Tea", 1.50, 7},
    {36, "Lemonade", 1.75, 6},
    {37, "Smoothie", 3.00, 10},
    {38, "Milkshake", 4.00, 5},
    {39, "Hot Chocolate", 2.00, 8},
    {40, "Espresso", 2.50, 12},
    {41, "Coffee", 1.50, 15},
    {42, "Tea", 1.00, 20},
    {43, "Herbal Tea", 1.20, 18},
    {44, "Chai", 1.75, 10},
    {45, "Seltzer", 1.00, 25},
    {46, "Sparkling Water", 1.25, 14},
    {47, "Coconut Water", 2.50, 5},
    {48, "Fruit Juice", 2.00, 8},
    {49, "Energy Bar", 1.80, 12}
};



int main(){
        // Initialize persistent prompt storage
        StringBuffer buffer;
        context.buffer.str_count = 0;
        context.buffer.str_array = NULL;
        
        char choice;
        
        while (1) {
                system("cls");
                
                clear_buffer();
                display("Enter a choice:                  (N)ext >>", 1);
                display("", 1);
                display("(F)ind, (S)orting, (P)urchase, (R)eceipt", 1);
                display("(Q)uit", 1);
                clear_buffer();

                scanf("%c", &choice);
                clear_input_buffer();
                
                
                switch(choice){
                case '>':
                case 'N':
                case 'n':
                        current_page = (current_page + 1) % (MAX_PRODUCTS / PRODUCTS_PER_PAGE);
                        break;
                case 'F':
                case 'f':
                        search();
                        break;
                case 'S':
                case 's':
                        sorting();
                        break;
                case 'P':
                case 'p':
                        purchase_product();
                        break;
                case 'R':
                case 'r':
                        generate_receipt();
                        break;
                case 'Q':
                case 'q':
                        // Clean prompt storage before quit
                        clear_buffer();
                        // Clearing line by line              
                        for (int i = 0; i < 28; i++) {
                                printf("\033[F"); // Move the cursor up one line
                                printf("\33[2K"); // Clear that line
                                fflush(stdout); // Ensure the line is cleared immediately
                                Sleep(10);
                        }
                        return(0);
                default:
                        system("cls");
                        display("\033[1;31mInvalid choice. Please try again.\033[0m", 0);
                        Sleep(2000);
        }
        }
        


        return 0;
}

// Displaying all products
void display_products(){
        char test_buffer[300];
        // printf("\n"); 
        for (int i = 0; i < 47; i++) {
                printf("\033[4m.\033[0m"); 
        }
        // printf("\033[0m");
       
        printf("\n||\033[4m%-3s | %-15s | %-7s | %-9s\033[0m||\n", "No", "Name", "Price", "Quantity");
        
        int start = current_page * PRODUCTS_PER_PAGE;
        int end = start + PRODUCTS_PER_PAGE;


        for (int i = start; i < end; i++) {
                        
                        if(i == end-1){
                                sprintf(test_buffer, "\033[4m%-3d | %-15s | RM%5.2f | %9d\033[0m",
                                i + 1,
                                product_list[i].name,
                                product_list[i].price,
                                product_list[i].quantity);
                                
                        }else{
                                sprintf(test_buffer, "%-3d | %-15s | RM%5.2f | %9d",
                                        i + 1,
                                        product_list[i].name,
                                        product_list[i].price,
                                        product_list[i].quantity);
                        }
                                
                        if (i == selected_id) {
                        
                        char highlighted_buffer[300]; 
                        sprintf(highlighted_buffer, "\033[1;43m%s\033[0m", test_buffer); 
                        printing(highlighted_buffer); 
                        
                        }else{
                                printing(test_buffer);
                        }
                        
                        
                }
       
}

// Main Purchasing function 
void purchase_product(){
        // Setup cart for storing products selected by user
        int cart[100] = {0};
        int quantities[100] = {0};
        int cart_count = 0;
        
        // Variables for keep track of products selected by user, and initialize user balance 
        int product_number;    
        int product_amount;
        float user_balance = 0; 
        
        // Backup storage to reset the quantity back to original state
        int original_quantities[CART_SIZE];
        
        char input[50];
                
                
        while(1){
                clear_buffer();
                display("[B]ack                            [P]ayment", 1);
                display("Enter the product number to purchase :", 1);
                
                clear_buffer();

        
                scanf("%s", input);
                clear_input_buffer();
                if (input[0] == 'B' || input[0] == 'b'){
                        //Reset everthing
                        selected_id = DEFAULT_ID;
                        product_number = 0;
                        product_amount = 0;
                        user_balance = 0;

                        // Restore quantity
                        for (int i = 0; i < cart_count; i++) {
                                product_list[cart[i]].quantity += quantities[i]; 
                        }

                        // Reset cart
                        for (int i = 0; i < CART_SIZE; i++) {
                        cart[i] = 0;               
                        quantities[i] = 0;          
                        original_quantities[i] = 0; 
                        }
                        cart_count = 0; 
                        return;
                        
                }
                if (input [0] == 'P' || input [0] == 'p'){
                        if (cart_count == 0){
                                display("\033[1;31mNo products selected.\033[0m", 0);
                                Sleep(2000);
                                return;
                        }
                        float total = 0;
                        for (int i = 0; i < cart_count; i++) {
                                total += product_list[cart[i]].price * quantities[i];
                        }

                        display("You have selected the following products:", 1);
                        for (int i = 0; i < cart_count; i++) {
                                sprintf(buffer , "%s (%d) - Price: RM %.2f", 
                                        product_list[cart[i]].name, 
                                        quantities[i], 
                                        product_list[cart[i]].price * quantities[i]);
                                display(buffer , 1);
                        }
                        sprintf(buffer, "Total Price: RM%5.2f", total);
                        display(buffer, 1);
                        
                        display("Enter the amount to pay: ", 0);
                        scanf("%f", &user_balance);
                        clear_input_buffer();

                        // ensure user has enough current to pay
                        do{     
                                clear_buffer();
                                
                                //Store the current that user wnats to add
                                float add_amount = 0;

                                if(user_balance < total){
                                display("\033[1;31mInsufficient funds.\033[0m", 1);
                                sprintf(buffer, "Current balance:  %.2f", user_balance);
                                display(buffer, 1);
                                sprintf(buffer, "Total amount required:  %.2f", total);
                                display(buffer, 1);
                                display("Enter Amount to add your balance: ", 0);
                                scanf("%f", &add_amount);
                                clear_input_buffer();
                                user_balance += add_amount;
                                }
                                
                        }while(user_balance < total);

                        char temp_input;
                        clear_buffer();
                        display("Product details: ", 1);

                        for (int i = 0; i < cart_count; i++) {
                                sprintf(buffer , "%s (%d) - Price: RM %.2f", 
                                        product_list[cart[i]].name, 
                                        quantities[i], 
                                        product_list[cart[i]].price * quantities[i]);
                                display(buffer , 1);
                        }
                        sprintf(buffer, "Your balance: RM %.2f", user_balance);
                        display(buffer, 1);
                        display("Proceed with payment? (Y/N): ", 0);
                        scanf("%c", &temp_input);
                        clear_input_buffer();

                        if (temp_input == 'N' || temp_input == 'n'){

                                // Reset everthing
                                selected_id = DEFAULT_ID;
                                product_number = 0;
                                product_amount = 0;
                                user_balance = 0;

                                // Restore quantity
                                for (int i = 0; i < cart_count; i++) {
                                        product_list[cart[i]].quantity += quantities[i]; 
                                }

                                // Clear the cart
                                for (int i = 0; i < CART_SIZE; i++) {
                                cart[i] = 0;               
                                quantities[i] = 0;          
                                original_quantities[i] = 0; 
                                }
                                cart_count = 0; 

                                display("\033[1;31mPayment canceled\033[0m", 0);
                                Sleep(2000);
                        }else if (temp_input == 'Y' || temp_input == 'y'){
                                float change = user_balance - total;

                                // Saving purchasing detials into receipt
                                context.receipt.count = 0;
                                for (int i = 0; i < cart_count; i++) {
                                        context.receipt.quantities[i] = quantities[i];
                                        context.receipt.products_id[i] = cart[i];
                                        context.receipt.count++;
                                }
                                context.receipt.totalCost = total;
                                context.receipt.moneyReceived  = user_balance;
                                context.receipt.changeReturned = change;

                                // Reset everthing
                                selected_id = DEFAULT_ID;
                                product_number = 0;
                                product_amount = 0;
                                user_balance = 0;

                                // Clear the cart
                                for (int i = 0; i < CART_SIZE; i++) {
                                cart[i] = 0;               
                                quantities[i] = 0;          
                                original_quantities[i] = 0; 
                                }
                                cart_count = 0; 
                                
                                display("Purchase successful!!", 1);

                                sprintf(buffer, "Change: RM%.2f", change);
                                display(buffer, 1);

                                display("\033[1;32mThank you for purchasing\033[0m", 1);
                                display("\033[1;33m *Claim your receipt at main menu\033[0m", 1);
                                display("Proceeding", 1);
                                progress_bar(10);
                        }else{  
                                // Reset everthing
                                selected_id = DEFAULT_ID;
                                product_number = 0;
                                product_amount = 0;
                                user_balance = 0;

                                // Restore quantity
                                for (int i = 0; i < cart_count; i++) {
                                        product_list[cart[i]].quantity += quantities[i]; 
                                }

                                // Reset cart
                                for (int i = 0; i < CART_SIZE; i++) {
                                cart[i] = 0;               
                                quantities[i] = 0;          
                                original_quantities[i] = 0; 
                                }

                                cart_count = 0; 
                                display("\033[1;31mInvalid input.\033[0m", 0);
                                Sleep(2000);
                        }
                                                                        
                }else{
                        // Converting string to numbers
                        product_number = atoi(input);
                        product_number--;
                        if (product_number >= 0 && product_number < MAX_PRODUCTS) {
                                selected_id = product_number;
                                sprintf(buffer, "Item selected: %s.", product_list[product_number].name);
                                display(buffer, 1);

                                if (product_list[product_number].quantity > 0){
                                        
                                        do{     
                                                clear_buffer();
                                                sprintf(buffer, "Item selected: %s.", product_list[product_number].name);
                                                display(buffer, 1);
                                                sprintf(buffer, "Enter an amount to purchase [%d avaliable]", product_list[product_number].quantity);
                                                display(buffer, 0);
                                                scanf("%d", &product_amount);
                                                clear_input_buffer();
                                                if (product_amount > product_list[product_number].quantity) {
                                                        
                                
                                                        display("\033[1;31mInsufficient quantity.\033[0m", 0);
                                                        Sleep(2000);
                                                        product_amount = 0;
                                                        }
                                                else if(product_amount <=0  ){
                                                        

                                                        sprintf(buffer, "You can't purchase \033[1;31m[%d]\033[0m %s.", product_amount, product_list[product_number].name);
                                                        display(buffer, 0);
                                                        product_amount = 0;
                                                        Sleep(2000);
                                                }else{
                                                        sprintf(buffer, "Quantity selected: %d", product_amount);
                                                        display(buffer , 1);
                                                        
                                                }
                                                
                                                
                                        }while(!( product_amount >0 && product_amount <= product_list[product_number].quantity));
                                        if (cart_count < CART_SIZE) {

                                                // Check if the product is already in the cart and increase the quantity if yes
                                                int found = 0;
                                                for (int i = 0; i < cart_count; i++) {
                                                        if (cart[i] == product_number) {
                                                                quantities[i] += product_amount; // Increase the quantity
                                                                found = 1;
                                                                break;
                                                        }
                                                        }

                                                        // If the product is not found in the cart, add the product to the cart
                                                        if (!found) {
                                                                cart[cart_count] = product_number;
                                                                quantities[cart_count] = product_amount;
                                                                original_quantities[cart_count] = product_list[product_number].quantity;
                                                                cart_count++;
                                                        }
                                                        // Backup the original quantity 
                                                        original_quantities[cart_count] = product_list[product_number].quantity;

                                                        // Decrease available stock
                                                        product_list[product_number].quantity -= product_amount; 
                                                        display("\033[1;32mProduct added to cart.\033[0m", 0);
                                                        Sleep(500);
                                        
                                        } else {
                                                display("Cart is full. Please proceed to payment.", 0);
                                                Sleep(2000);
                                        }
                                        clear_buffer();
                                        selected_id = DEFAULT_ID;
                                }else{
                                        clear_buffer();
                                        selected_id = DEFAULT_ID;
                                        display("\033[1;31mSorry, the product is out of stock.\033[0m", 0);
                                        Sleep(2000);
                                }
                        }else{
                                clear_buffer();
                                display("\033[1;31mInvalid product number.\033[0m", 0);
                                Sleep(2000);
                                
                        }
                }
                
                
        }
        

        
                  
        
}

// Display receipt
void generate_receipt(){
        char back[10] = "N";
        float total_cost = 0; 
        do{
                
                if(context.receipt.count  <=0 ){
                        display("\033[1;31mNo receipt available\033[0m", 0);
                        Sleep(1000);
                        return;
                }
                if (!(back[0] == 'C' || back[0] == 'c')){
                        clear_buffer();
                        display("\033[1;33mReceipt###\033[0m", 1);
                        for (int i = 0; i < context.receipt.count ; i++) {
                                sprintf(buffer, "\033[1;33m%s (Quantity: %d) - RM %.2f\033[0m", 
                                product_list[context.receipt.products_id[i]].name, 
                                context.receipt.quantities[i], 
                                context.receipt.quantities[i]* product_list[context.receipt.products_id[i]].price );
                                display(buffer, 1);
                        }
                        sprintf(buffer, "\033[1;33mTotal Cost: RM %.2f\033[0m", context.receipt.totalCost);
                        display(buffer, 1);
                        sprintf(buffer, "\033[1;33mMoney Received: RM %.2f\033[0m", context.receipt.moneyReceived);
                        display(buffer, 1);
                        sprintf(buffer, "\033[1;33mChange Return: RM %.2f\033[0m", context.receipt.changeReturned);
                        display(buffer, 1);
                }
                display("\033[1;32m(C)ontinue ->\033[0m", 0);
                scanf("%s", back);
                clear_input_buffer();
                

        // Only exit if the user presses 'C'/ 'c'
        }while(!(back[0] == 'C' || back[0]== 'c'));
       



}

// Display function to decide whether the prompt need to be persistedd or nah
void display(char prompt[], int save){
        int width =43;
        if(save){
                // Store the prompt and display the whole vending
                buffer_append(prompt);

                heading();
                ending(0);
                 
                
        }else{
                // Directly display the vending and prompt wihtout savign the prompt
                heading();

                printing(prompt);
                ending(1);
             
        }

}

// Display progress bar for aesthetics purposes only
void progress_bar(int length){
        int width =43;
        
        for (int i = 0; i < length; i++) {
                char temp_str[300]="";
                heading();
                
                for(int j=0; j< i; j++) {
                        strcat(temp_str, "\033[47;30m \033[0m");
                        

                }
                for (int k=0; k< width-i;k++){
                        strcat(temp_str, " ");
                }
                printing(temp_str);
                
                ending(1);
                Sleep(500);
        }
        
}

// Main Sorting Function handler
void sorting(){
        display("Enter sorting type: ", 1);
        display("(N)ame, (P)rice, (Q)uantity: ", 1);
        clear_buffer();

        char sort_type;
       
        scanf("%c", &sort_type);
        clear_input_buffer();
        if(sort_type!= 'N' && sort_type!= 'n' && sort_type!= 'P' && sort_type!= 'p' && sort_type!= 'Q' && sort_type!= 'q'){
                display("\033[1;31mInvalid input.\033[0m", 0);
                Sleep(2000);
                return;
        }
        
        char order_by;
        display("Enter order type : ", 1);
        display(" (A)scending , (D)escending: ", 1);
        clear_buffer();
        
        scanf("%c", &order_by);
        clear_input_buffer();
        if(order_by!= 'A' && order_by!= 'a' && order_by!= 'D' && order_by!= 'd'){
                display("\033[1;31mInvalid input.\033[0m", 0);
                Sleep(2000);
                return;
        }
        
        sorting_choice(sort_type, order_by);
        display("\033[1;32mSorted successfully.\033[0m", 0);
        Sleep(2000);
}

// Bubble sorting algorithm
void sorting_choice(char sort_type, char order_by){



        switch(sort_type){
                case 'N': 
                case 'n':       
                switch(order_by){
                        case 'A':
                        case 'a': 
                                for(int i =0; i <MAX_PRODUCTS -1; i++){
                                        for(int j=0 ; j < MAX_PRODUCTS - i -1; j++){
                                                if (strcmp(product_list[j].name, product_list[j+1].name) > 0){
                                                        Product temp = product_list[j];
                                                        product_list[j] = product_list[j+1];
                                                        product_list[j+1] = temp;
                                                }

                                        }
                                }
                        break;
                        case 'D':
                        case 'd': 
                                for(int i =0; i <MAX_PRODUCTS -1; i++){
                                        for(int j=0 ; j < MAX_PRODUCTS - i -1; j++){
                                                if (strcmp(product_list[j].name, product_list[j+1].name) <0){
                                                        Product temp = product_list[j];
                                                        product_list[j] = product_list[j+1];
                                                        product_list[j+1] = temp;
                                                }

                                        }
                                }
                        break;
                        default: display("\033[1;31mInvalid order.\033[0m", 1);
                        Sleep(2000);
                        return;
                }
        
                
                
                break;
                case 'P':
                case 'p':
                switch(order_by){
                        case 'A':
                        case 'a': 
                                for(int i =0; i <MAX_PRODUCTS -1; i++){
                                        for(int j = 0; j< MAX_PRODUCTS - i -1; j++){
                                                if(product_list[j].price > product_list[j +1].price){
                                                        Product temp = product_list[j];
                                                        product_list[j] = product_list[j+1];
                                                        product_list[j+1] = temp;
                                                }
                                        }
                                }
                        break;
                        case 'D':
                        case 'd': 
                                for(int i =0; i <MAX_PRODUCTS -1; i++){
                                        for(int j = 0; j< MAX_PRODUCTS - i -1; j++){
                                                if(product_list[j].price < product_list[j +1].price){
                                                        Product temp = product_list[j];
                                                        product_list[j] = product_list[j+1];
                                                        product_list[j+1] = temp;
                                                }
                                        }
                                }
                        break;
                        default: display("\033[1;31mInvalid order.\033[0m", 0);
                        Sleep(2000);
                        return;
                        }
                        
                break;
                case 'Q': 
                case 'q':
                switch(order_by){
                        case 'A':
                        case 'a': 
                                for(int i = 0; i < MAX_PRODUCTS -1; i++){
                                        for(int j = 0; j< MAX_PRODUCTS - i- 1; j++){
                                                if(product_list[j].quantity > product_list[j +1].quantity){
                                                        Product temp = product_list[j];
                                                        product_list[j] = product_list[j+1];
                                                        product_list[j+1] = temp;
                                                }
                                        }
                                }
                        break;
                        case 'D':
                        case 'd': 
                                for(int i = 0; i < MAX_PRODUCTS -1; i++){
                                        for(int j = 0; j< MAX_PRODUCTS - i- 1; j++){
                                                if(product_list[j].quantity < product_list[j +1].quantity){
                                                        Product temp = product_list[j];
                                                        product_list[j] = product_list[j+1];
                                                        product_list[j+1] = temp;
                                                }
                                        }
                                }
                        break;
                        default: display("\033[1;31mInvalid order.\033[0m", 0);
                        Sleep(2000);
                        return;
                        }
                        
                break;
                default: 
                        display("\033[1;31mInvalid sorting type.\033[0m", 0);
                        Sleep(2000);
                return;
        }
}

// Function to store prompt that need to be persisted
void buffer_append(const char *new_string){
    context.buffer.str_count ++;
    context.buffer.str_array = realloc(context.buffer.str_array, context.buffer.str_count * sizeof(char *));

    context.buffer.str_array[context.buffer.str_count -1] = malloc(strlen(new_string)+1);
    strcpy(context.buffer.str_array[context.buffer.str_count - 1], new_string);

}

// Clear all persistent prompt 
void clear_buffer(){
    for(int i=0; i< context.buffer.str_count; i++){
        free(context.buffer.str_array[i]);
    }
    // free the dynamically allocated memory to prevent memory leaks
    free(context.buffer.str_array);
    context.buffer.str_count = 0;
    context.buffer.str_array = NULL;
}

// Dedicated printing fucntion for handling padding due to ANSI ESC Sequence involement problem 
void printing(char *str){
        int ansi_count =0;
        int width;
        
        //Get the occurrences of ANSI ESC characters used 
        ansi_count = get_occurrences(str);
        
        // Calculate the width for the string considering ANSI ESC characters
        width = 43 + (11* (ansi_count/2));
        
        printf("||%-*s||\n", width, str);
}

// Display the half upper part of vending machine
void heading(){
        //clear the screen 
        system("cls");

        //display all products
        display_products();

        //display all prompts
        for(int i=0; i< context.buffer.str_count; i++){
                printing(context.buffer.str_array[i]);
        }


}

// Display the half lower part of the vending
void ending(int eliminate_count){

        int width =43;
        // Count the numer of rows needed for the remaning below part of vending
        int rows_needed = max_row - context.buffer.str_count - eliminate_count;
        
                for (int i =1 ; i<= rows_needed; i++){
                        char temp_str[1000] ="";
                        
                        if(i== rows_needed){
                                for (int j =0 ; j< width; j++){
                                        strcat(temp_str, "\033[4m.\033[0m");
                                }
                                printing(temp_str);
                        }else{
                                for (int j =0 ; j< width; j++){
                                        strcat(temp_str, " ");
                                }
                                printing(temp_str);
                        }
                        

                }
}
// Main Searching function handler
void search(){
        // Store all found id 
        int found_ids[MAX_PRODUCTS];

        // Store user query parameters
        char query[20];
        
        // Make sure query's length is more than 3
        do{
                if (strlen(query) <3){
                        display("\033[1;31mPlease enter at least 3 characters.\033[0m",0 );
                        Sleep(1000);
                }
                display("Enter product name to search:", 0);
                scanf("%s", query);
                clear_input_buffer();
        }while(strlen(query)< 3);
        
        // Store the return value which is the number of found products
        int found = search_product(found_ids, query);

        // Display the result according to the found results
        if(found > 0){
                sprintf(buffer, "\033[1;32m%d Results\033[0m", found);
                display(buffer, 1);

        for (size_t i = 0; i < found; i++) {
                sprintf(buffer,"%-15s No. %d" , product_list[found_ids[i]].name, found_ids[i]+1);
                display(buffer, 1);
               
        }
        clear_buffer();
        }else{
                display("\033[1;31mNo results found.\033[0m", 1);
        }
        
        
        Sleep(2000);
        
}

// search through product_list, store results id and founds
int search_product(int *found_ids, char *query){
        
        // Store the numebr of found products
        int found = 0;
        
        for (size_t i = 0; i < MAX_PRODUCTS; i++) {
                // Make temporary holder for modified version of strings
                char product_name_lower[50];
                strcpy(product_name_lower, product_list[i].name);
                
                to_lowercase(product_name_lower);
                to_lowercase(query);

                // Make comaparism between user's query and products's name
                if (strstr(product_name_lower, query) != NULL) {
                        found_ids[found++] = i;
                
                }
        }

        return found;

        

}

// function to lowercase a String
void to_lowercase(char *str) {
        
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Clear all input_buffer to remove overleft chars or Newline chars
void clear_input_buffer() {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
                // Discard characters until newline or EOF
        }
}

// count the occurence of ANSI Escape Sequence
int get_occurrences(char *str){
        int count =0;
        // get the string pointer for counting process
        const char *ptr =str;
        while((ptr=strstr(ptr,"\033"))!=NULL){
                count++;
                ptr++;
        
        }

        
        // ESC Code Sequence like '_' underline is ignored due to not affecting the padding when displaying
        ptr =str;
        while((ptr=strstr(ptr,"\033[4m"))!=NULL){
                count-= 2;
                ptr++;
        
        }


    return count;

}
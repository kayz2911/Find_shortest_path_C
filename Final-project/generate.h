#ifndef __GENERATE_H__
#define __GENERATE_H__

#define MAX_NUM 15000
#define traverse(var, end) \
    for(int var = 0; var < end; var++)

typedef struct {
    long ID;
    char name[30], address[100], prod[200], tele[15];
    double price, total;
    int amount;
} order;

typedef struct {
    char code[20];
    char address[100];
} code_address;
// you must run these function before using 
// read data from file_in, then store it in orders array, return number of orders
int extract_data_to_struct(char *file_in, order *orders);
// create a struct that contains code of address and address, return number of codes
int init_code_address(char *file_in, code_address *codes);

// orders, num(number of orders) is data obtained from extract_data_to_struct function
// IDs is array where you store all ID, return 1 when succeed, 0 otherwise
int getIDs(order *orders, long *IDs, int num);
int getNames(order *orders, char (*names) [30], int num);
int getAddresses(order *orders, char (*addresses)[100], int num);
int getProducts(order *orders, char (*prods)[200], int num);
int getPrices(order *orders, double *prices, int num);
int getAmounts(order *orders, int *amount, int num);
int getTotal(order *orders, double *totals, int num);
int getTeles(order *orders, char (*teles)[15], int num);
int iscontain(const char *str1,const char *str2);
// search_data operate :
// by string can be : "id", "name", "tele", "address", 
// "product", "price", "amount", "total"
// depend on string 'by', search_key string is what you are looking for
//file in is file store data
int search_data(char *by, char *search_key, char *file_in);

// search key format : "name_ward,name_district,name_province" if you dont need ward, you replace it with SPACE
// upper or lower case is not matter, you dont need type full address(VD "Hà nội" is enough), sign is matter
// vd : valid search_key : "Tân Phú, Quốc oai, Hà nội" or " ,Quốc Oai, Hà nội" or "Hà nội"
// orders, num(number of orders) is data obtained from extract_data_to_struct function
// output is where you store data, the function return number of orders it found, otherwise return -1 
int search_address(char *search_key, order* orders, int num, order *output);

// input codes and num is from init_code_address
// NOTE THAT : you dont need type full address, upper or lower is both ok, but you have to
// use "Đ" instead of "đ" if it in your address (vd : mỹ Đức, not use mỹ đức)
// how it work : "97;an phú, mỹ Đức, Hà nội" -> 10504-97 and vice versa
void convert_address_to_code(code_address *codes, int num, char *address, char *code);
void convert_code_to_address(code_address *codes, int num, char *code, char *address);
void statistic_address(char *file_in);
void statistic_prices(order* orders, int num);
void statistic_amount(order* orders, int num);

#endif
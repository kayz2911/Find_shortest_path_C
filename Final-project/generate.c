#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "generate.h"

/* generate n lines, each line contains
ID_order, name of receiver, telephone, address, name of product, price, amount, all cost

(unique ID between 1 and n then later combine with provinces code, ... ),
name we will create randomly, telephone must start with 0 and 9 digits folowing
address : Tinh, huyen, xa, thon/ngach, so nha
name of products. price : scratch shopee website
*/

void random_name(char *name){
    int ran1, ran2, ran3;
    char first_name[][20] = {"Nguyen ", "Tran ", "Le ", "Pham ", "Hoang ", "Phan ", "Vu ",
    "Dang ", "Bui ", "Do ", "Ho ", "Ngo ", "Duong ", "Ly ", "Chu ", "Kha ", "Diep ", "An ",
    "Hoa ", "Mac ", "Vuong ", "Trinh ", "Bach ", "Anh"};
    char mid_name[][20] = {"Van ", "Huu ", "Thi ", "Xuan ", "Thu ", "Cam ", "Chau ", "Hoang ", "Duc ",
            "Hanh ", "Dinh ", "Dai "};
    char last_name[][20] = {"Nghi", "Dong", "Trang", "Si", "Nong", "Cong", "Thuong", "Nhat",
            "Nhi", "Tam", "Hong", "Lan", "Hue", "Loan", "Linh", "Hien", "Hung",
            "Dung", "Phuong", "Duyen", "Phuc", "Loc", "Tho", "Doan", "Tuyet"};

    ran1 = rand() % (sizeof(first_name) / 20);
    ran2 = rand() % (sizeof(mid_name) / 20);
    ran3 = rand() % (sizeof(last_name) / 20);
    strcpy(name, "");
    strcat(name, first_name[ran1]);
    strcat(name, mid_name[ran2]);
    strcat(name, last_name[ran3]);
}

void random_tele(char *tele){
    tele[0] = '0', tele[1] = '9';
    for(int i = 2; i < 11; i++)
        tele[i] = '0' + rand() % 10;
}

int create_random_data(long seed1, char *file_in, char *file_out){
    FILE *fp, *fp1;
    char name[30] = {}, tele_str[15] = {}, tmp[10] = {}, prods[400];
    char provin[40], district[40], so_nha[3], ward[40], *tmpp, prod[200], price[20], *end, *token; 
    long amount, ID_order, tele;
    int num;
    double priced, total_amountd;
    fp = fopen(file_in, "r");
    fp1 = fopen(file_out, "w");
    do{
        char tele_str[15] = {};
        // ensure each nunmber in range (0, 2^31-1)is random before it appears again
        ID_order = seed1 * (long)pow(7, 5) % ((long)pow(2, 31) - 1);
        seed1 = ID_order;
        //random name
        random_name(name);
        // random telephone
        random_tele(tele_str);
        //random address, products and prices
        // by take each lines in file productAndPRices.txt, then insert it
        end = fgets(prods, 400, fp);
        if(!end)    break;
        token = strtok(prods, ","); strcpy(so_nha, token);
        token = strtok(NULL, ","); strcpy(provin, token);
        token = strtok(NULL, ","); strcpy(district, token);
        token = strtok(NULL, ","); strcpy(ward, token);
        token = strtok(NULL, ","); strcpy(prod, token);
        token = strtok(NULL, "\n"); strcpy(price, token);
        priced = atof(price)*1000;
        //random amount of products, ensure that it > 0
        amount = 0;
        while(amount == 0)
            amount = rand() % 10;
        total_amountd = priced * amount;
        fprintf(fp1, "%ld, %s, %s, %s;%s;%s;%s, %s, %.0f, %ld, %.0f\n",
        ID_order, name, tele_str, so_nha,provin,district,ward, prod, priced, amount, total_amountd);
        num++;
    }while(end);
    fclose(fp);
    fclose(fp1);
    return num; // return number of orders
}

int extract_data_to_struct(char *file_in, order *orders){
    FILE *fp;
    char *id, *end, *token, str[400] = {};
    int stt = 0;
    fp = fopen(file_in, "r");
    if(!fp)
        printf("cant open file\n");
    do{
        end = fgets(str, 400, fp);
        if(!end)    break;
        token = strtok(str, ","); orders[stt].ID = atol(token);
        token = strtok(NULL, ","); strcpy(orders[stt].name, token);
        token = strtok(NULL, ","); strcpy(orders[stt].tele, token);
        token = strtok(NULL, ","); strcpy(orders[stt].address, token);
        token = strtok(NULL, ","); strcpy(orders[stt].prod, token);
        token = strtok(NULL, ","); orders[stt].price = atof(token);
        token = strtok(NULL, ","); orders[stt].amount = atoi(token);
        token = strtok(NULL, ","); orders[stt++].total = atof(token);
    }while(end);
    fclose(fp);
    return stt;
}

int init_code_address(char *file_in, code_address *codes){
    FILE *fp;
    char *end, *token, str[150];
    int i = 0;
    fp = fopen(file_in, "r");
    if(fp==NULL){
        printf("cant open file\n");
        return -1;
    }    
    do{
        end = fgets(str, 150, fp);
        if(!end)    break;
        token = strtok(str, ":");   strcpy(codes[i].code, token);
        token = strtok(NULL, "\n"); strcpy(codes[i++].address, token);
    }while(end);
    fclose(fp);
    return i;
}

void convert_address_to_code(code_address *codes, int num, char *address, char *code){
    char *token, so_nha[5], xa[20], huyen[20], tinh[20], addr[100], delimter[10]=";, .";
    int check = 0;
    strcpy(addr, address);
    token = strtok(addr, delimter);
    if(atoi(token) > 0){
        check = 1;
        strcpy(so_nha, token);
        token = strtok(NULL, delimter); strcpy(xa, token);
    }
    else
        strcpy(xa, token);
    token = strtok(NULL, delimter); strcpy(huyen, token);
    token = strtok(NULL, delimter); strcpy(tinh, token);

    traverse(i, num){
        char addrs[100];
        strcpy(addrs, codes[i].address);
        if(iscontain(xa, addrs) && iscontain(huyen, addrs) && iscontain(tinh, addrs)){
            strcpy(code, codes[i].code);
            break;
        }
    }
    if(check){
        strcat(code, "-");
        strcat(code, so_nha);
    }
}

void convert_code_to_address(code_address *codes, int num, char *code, char *address){
    char *token, so_nha[5], part_code[10], old_code[10], ans[100];
    strcpy(old_code, code);
    if(iscontain("-", code)){
        token = strtok(old_code, "-"); strcpy(part_code, token);
        token = strtok(NULL, "\n"); strcpy(so_nha, token);
        strcat(address, so_nha);
        strcat(address, ";");
    }
    else
        strcpy(part_code, old_code);
    traverse(i, num){
        if(strcmp(part_code, codes[i].code)==0){
            strcat(address, codes[i].address);
            break;
        }
    }
}

//there is a bug here, 'Đ' is different to 'đ', and both are multi-character
int iscontain(const char *str1,const char *str2){
    // return 1 if str1 is a part of str2, else 0
    int n1 = strlen(str1), n2 = strlen(str2), i1=0, i2=0, dist = 'a'-'A';
    while(i2 < n2){
        if(str1[i1]!=str2[i2] && abs(str1[i1]-str2[i2]) != dist)
            i1 = 0;
        else{
            i1++;
            if(i1 == n1)    return 1;
        }
        i2++;
    }
    return 0;
}

int search_data(char *by, char *search_key, char *file_in){
    FILE *fp;
    char *end, str[400] = {}, *token, old_str[400];
    int i = 0, search_result = 0, stt;
    fp = fopen(file_in, "r");
    if(!strcasecmp(by, "id"))
        stt = 1;
    else if(!strcasecmp(by, "name"))
        stt = 2;
    else if(!strcasecmp(by, "tele"))
        stt = 3;
    else if(!strcasecmp(by, "address"))
        stt = 4;
    else if(!strcasecmp(by, "product"))
        stt = 5;
    else if(!strcasecmp(by, "price"))
        stt = 6;
    else if(!strcasecmp(by, "amount"))
        stt = 7;
    else if(!strcasecmp(by, "total"))
        stt = 8;
    else{
        printf("wrong field data\n");
        return 0;
    }
    if(!fp)
        printf("cant open file\n");
    while(1){
        end = fgets(str, 400, fp);
        strcpy(old_str, str);
        if(!end)    
            break;
        traverse(i, stt)
            token = (i == 0) ? strtok(str, ",") : strtok(NULL, ",");
        if(iscontain(search_key, token) == 1){
            printf("%s\n", old_str);
            search_result = 1;
        }
    }
    if(!search_result)
        printf("Cant find %s, try again.\n", search_key);
    fclose(fp);
    return i;
}

int search_address(char *search_key, order* orders, int num, order *output){
    char *end, *token, xa[20], huyen[20], tinh[20], search[100], address[100];
    int i = 0, search_result = 0;
    strcpy(search, search_key);
    token = strtok(search, ",; "); strcpy(xa, token);
    token = strtok(NULL, ",; "); strcpy(huyen, token);
    token = strtok(NULL, ",; \n"); strcpy(tinh, token);
    traverse(j, num){
        strcpy(address, orders[j].address);
        if(iscontain(xa, address) && iscontain(huyen, address) && iscontain(tinh, address)){
            output[i++] = orders[j];
            search_result = 1;
        }        
    }
    if(!search_result){
        printf("Cant find %s, try again.\n", search_key);
        return -1;
    }
    return i;
}

void statistic_prices(order* orders, int num){
    int num_small=0, num_medium=0, num_big=0, num_bigger=0;
    traverse(i, num){
        if(orders[i].total < 200000)
            num_small++;
        else if(orders[i].total < 500000)
            num_medium++;
        else if(orders[i].total < 1000000)
            num_big++;
        else
            num_bigger++;
    }
    printf("number of orders < 200.000đ : %d\n", num_small);
    printf("number of orders in range 200.000đ-> 500.000đ : %d\n", num_medium);
    printf("number of orders in range 500.000đ->1.000.000đ : %d\n", num_big);
    printf("number of orders > 1.000.000đ : %d\n", num_bigger);
}

void statistic_amount(order* orders, int num){
    int nums[9] = {};
    traverse(i, num)
        nums[orders[i].amount-1]++;
    traverse(i, 9)
        printf("number of orders which order %d products is : %d\n", i+1, nums[i]);
}

// int main(){ 
//     order orders[MAX_NUM], res[1000];
//     code_address codes[600];
//     char cwd[100], file_in[100], file_out[100], file_code[100], code[20], address[100]; 
//     char names[MAX_NUM][30], teles[MAX_NUM][15], addresses[MAX_NUM][100], prods[MAX_NUM][200],output_line[100][400]; 
//     int amounts[MAX_NUM],num, i, num2, num_code;
//     long a[MAX_NUM];
//     double prices[MAX_NUM], totals[MAX_NUM];
    
//     getcwd(cwd, 100);
//     strcpy(file_in, cwd);
//     strcpy(file_out, cwd);
//     strcpy(file_code, cwd);
//     // WARNING : if you use ubuntu, use this : '/', if you use window, use this : '\\'
//     // VD : strcat(cwd, "\\Orders_tiki.csv");
//     strcat(file_code, "/code_address.txt");
//     strcat(file_in, "/address_product_price_tiki.txt");
//     strcat(file_out, "/Orders_tiki.csv");
//     num_code = init_code_address(file_code, codes);
//     // num = extract_data_to_struct(file_in, orders);
//     // convert_address_to_code(codes, num_code, "77, Phương liên, Đống Đa, Hà Nội", code);
//     convert_code_to_address(codes, num_code, "10494-19", address);
//     printf("address=%s\n", address);
//     // search_data("address", "huyện vũ thư", file_in, output_line);
//     // num2 = search_address(" ,Ba Vì,Hà nội", orders, num, res);
//     // traverse(i, num2)
//     //     printf("%s\n", res[i].address);
//     // statistic_address(file_in);
//     // statistic_prices(orders, num);
//     // statistic_amount(orders, num);
// }
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "generate.h"


void output(void *buf,size_t size, int n){
    int i;
    for (i=0; i<n; i++)
        if (size == sizeof(int)){
            printf("%d \n",*((int*)buf + i));
        }
        else if (size == sizeof(long)){
            printf("%ld \n",*((long*)buf + i));
        }
    printf("\n");
}

void outputOrder(void *buf,size_t size, int n){
    int i;
    for (i=0; i<n; i++){
        printf("%ld,",((order*)buf+i)->ID);
        printf("%s,",((order*)buf+i)->name);
        printf("%s,",((order*)buf+i)->tele);
        printf("%s,",((order*)buf+i)->address);
        printf("%s,",((order*)buf+i)->prod);
        printf("%.0lf,",((order*)buf+i)->price);
        printf("%d,",((order*)buf+i)->amount);
        printf("%.0lf\n",((order*)buf+i)->total);
    }
    printf("\n");
}

long long_compare(void const *x,void const *y){
    long m,n;
    m=*((long *)x);
    n=*((long *)y);
    if (m==n) return 0;
    return m>n ? 1:-1;
}

void swapGen(void *a, void *b, int size){
    void *dump = (void *)malloc(size);
    memcpy(dump, a, size);
    memcpy(a, b, size);
    memcpy(b, dump, size);
}

long id_cmpOrder(void const *x, void const *y){
    long m,n;
    m = ((order *)x)->ID;
    n = ((order *)y)->ID;
    if (m==n) return 0;
    return m>n ? 1:-1;
}

long name_cmpOrder(void const *x,void const* y){
    char *m, *n;
    m = ((order *)x)->name;
    n = ((order *)y)->name;
    return strcmp(m,n);
}

long tele_cmpOrder(void const *x,void const* y){
    char *m, *n;
    m = ((order *)x)->tele;
    n = ((order *)y)->tele;
    return strcmp(m,n);
}

long address_cmpOrder(void const *x,void const* y){
    char *m, *n;
    m = ((order *)x)->address;
    n = ((order *)y)->address;
    return strcmp(m,n);
}

long price_cmpOrder(void const *x,void const* y){
    double m, n;
    m = ((order *)x)->price;
    n = ((order *)y)->price;
    if(m==n) return 0;
    return m>n ? 1:-1;
}
long amount_cmpOrder(void const *x,void const* y){
    int m, n;
    m = ((order *)x)->amount;
    n = ((order *)y)->amount;
    if(m==n) return 0;
    return m>n ? 1:-1;
}

long total_cmpOrder(void const *x,void const* y){
    double m, n;
    m = ((order *)x)->total;
    n = ((order *)y)->total;
    if(m==n) return 0;
    return m>n ? 1:-1;
}

void long_swap(void *buf,size_t size,long i,long j){
    long cmp=*((long*)buf+size*i);
    *((long*)buf+size*i)=*((long*)buf+size*j);
    *((long*)buf+size*j)=cmp;
}
void quickSort3way2Gen(void *a, int size, int l, int r, long (*compare)(void const *x, void const *y)){
    if(l >= r)
        return;
    int pivot = r*size; // choose the last one is pivot
    int i = l, j = l;
    for(i = l; i < r; i++){
        if(compare(a + size * i, a + pivot) < 0){
            swapGen(a + size * i, a + size*j, size);
            j++;
        }
    }
    int j1 = j;
    for(int i = j; i < r; i++){
        if(compare(a + size * i, a + pivot) == 0){
            swapGen(a + size * i, a + size*j1, size);
            j1++;
        }
    }
    swapGen(a + size * r, a + size*j1, size);
    quickSort3way2Gen(a, size, l, j-1, compare);
    quickSort3way2Gen(a, size, j1+1, r, compare);
}

long str_cmp(void const *x,void const* y){
    return strcmp(x,y);
}

// int main(){
//     // long arr[10]={697192378,1492629944,135586615,384882553,499836108,875379063,85261519,1190259144,2084068,1123992139};
//     // // long arr[10] = {100,200,400,200,700,600,100,900,0,500};
//     // output(arr, sizeof(long), 10);
//     // quickSort3way2Gen(arr, sizeof(long),0, 10, long_compare);
//     // printf("\n After sort\n");
//     // output(arr, sizeof(long), 10);
//     order orders[MAX_NUM];
//     char output_line[400], cwd[100], file_in[100];
//     getcwd(cwd, 100);
//     // if you run program on window
//     strcat(cwd, "/Orders_sendo.csv");
//     strcpy(file_in, cwd);
//     int num_orders = extract_data_to_struct(file_in, orders);
//     // if you run program on ubuntu
//     // strcat(cwd, "/Orders.csv");
//     quickSort3way2Gen(orders , sizeof(order), 0, num_orders, total_cmpOrder);
//     printf("\n After sort\n");
//     outputOrder(orders, sizeof(order), num_orders);
        
//     // quickSort3way2Gen(names , sizeof(char)*30, 0, NUM, str_cmp);
//     // for(int i=0;i<NUM;i++){
//     //     printf("%s\n", names[i]);
//     // }
//     return 0;
// }

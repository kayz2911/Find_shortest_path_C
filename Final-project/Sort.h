#include <stdlib.h>
#ifndef _SORT_H_
#define _SORT_H_
#define NUM 7918

void output(void *buf,size_t size, int n);
void outputOrder(void *buf,size_t size, int n);
long id_cmpOrder(void const *x, void const *y);
long name_cmpOrder(void const *x,void const* y);
long tele_cmpOrder(void const *x,void const* y);
long address_cmpOrder(void const *x,void const* y);
long price_cmpOrder(void const *x,void const* y);
long total_cmpOrder(void const *x,void const* y);
long amount_cmpOrder(void const *x,void const* y);
void swapGen(void *a, void *b, int size);
void long_swap(void *buf,size_t size,long i,long j);

void quickSort3way2Gen(void *a, int size, int l, int r, long (*compare)(void const *x, void const *y));
// if you Sort for string use str_cmp
// if you Sort for number use long_compare

#endif



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



#include "graph.h"
#include "TSP.h"
#include "Sort.h"
#include "file_handler.h"
#include "generate.h"

#define INFINITIVE_VALUE 10000000

/**
 * find best path 
 * convention: shipper always start at vertext: 0
 * return total cost **/
double findBestPath(Graph graph, int *addressArr, int lengthAddressArr, int *bestPath)
{
    //build matrix cost from starting point is vertext 0;
    int start = 0;
    double **matrixCost = (double **)malloc((lengthAddressArr) * sizeof(double *));
    for (int i = 0; i < lengthAddressArr; i++)
    {
        matrixCost[i] = (double *)malloc((lengthAddressArr) * sizeof(double));
    }
    for (int row = 0; row < (lengthAddressArr); row++)
    {
        for (int col = 0; col < (lengthAddressArr); col++)
        {
            if (row == col)
            {
                matrixCost[row][col] = INFINITIVE_VALUE;
            }
            else
            {
                int path[100], length = 0;
                matrixCost[row][col] = shortestPath(graph, addressArr[row], addressArr[col], path, &length);
            }
        }
    }
    // find best path from matrix cost
    // int *bestPath = (int *)malloc((lengthAddressArr + 1) * sizeof(int)); // 1 - save end point
    double cost = TSP(matrixCost, (lengthAddressArr), (lengthAddressArr), bestPath);
    // bestpath--contain index of addressArr-> need convert into value of addressArr
    for (int i = 0; i < lengthAddressArr; i++)
    {
        int index = bestPath[i];
        bestPath[i] = addressArr[index];
    }
    bestPath[lengthAddressArr] = bestPath[0];
    return cost;
}

int convertAddresstoVertexId(Graph graph, char *address)
{
    JRB tmp;
    jrb_traverse(tmp, graph.vertices)
    {
        if (strcmp(jval_s(tmp->val), address) == 0)
        {
            return jval_i(tmp->key);
        }
    }
    return -1;
}

/**Convert vertexId into code address
 * return 1-success 0-false
*/
char* convertVertexIdtoAddress(Graph graph, int addressId)
{
    if (addressId >= 0 && addressId <= 100)
    {
        return getVertex(graph, addressId);
    }
    else
    {
        return NULL;
    }
}

void initMap(Graph g, char *regionCode)
{
    char cwd[100], fileName[100];
    getcwd(cwd, 100);
    strcpy(fileName, cwd);
    strcat(fileName, "/map/");
    strcat(fileName, regionCode);
    strcat(fileName, ".txt");
    g = read_db(fileName, g);
}

/**
 * order from Ecommerce will be deliver by this function
*/
void deliver(Graph g, char *region, order *orders, int numInOrders, code_address *addressCode, int numCodeAddress)
{
    order *resultOrederSearch = (order *)malloc(numInOrders * sizeof(order));
    char address[100]={}, code[20];
    convert_code_to_address(addressCode, numCodeAddress, region, address);
    int numResultSearch = search_address(address, orders, numInOrders, resultOrederSearch); // number orders in the region
    int *IDAddressInGraph = (int *)malloc((numResultSearch + 1) * sizeof(int));   // ô đầu tiên là nơi xuất phát, các ô sau là nơi cần đến
    int *resultPath = (int *)malloc((numResultSearch + 2) * sizeof(int));         // ô đầu & ô cuối lưu nơi bắt đầu cũng là nơi kết thúc
    IDAddressInGraph[0] = 0;
    for (int i = 0; i < numResultSearch; i++) // convert real address to vertextId in the graph
    {
        char code[20]={};
        convert_address_to_code(addressCode, numCodeAddress, resultOrederSearch[i].address, code);
        IDAddressInGraph[i+1] = convertAddresstoVertexId(g, code);
    }
    printf("\n");
    double cost = findBestPath(g, IDAddressInGraph, numResultSearch, resultPath);

    // for (int i = 0; i < (numResultSearch+2); i++) // CHECK đoạn phía trên có hoạt động không, nếu có thì in ra 1 ds các id của 
    // {                                             // các đỉnh, và giờ chỉ cần chuyển đổi qua code_address-> real_address
    //     printf("%d ", resultPath[i]);              // nó sẽ in ra 1 ds các số với 0 là nơi bắt đầu và là nơi kết thúc của hành trình
    // }
    // printf("\n");
        // dựa theo resultPath chứa các id trong graph-> address_code-> real_adress, rồi in ra thứ tự đợn cầu giao//
        // ví dụ, t đang giao trong cái máp 00001, với id ở resultrPaht là 3 -> code-address là 00001-3, từ đây cần in ra đơn hàng
        
    printf("Processing of delivering orders:\n");
    printf("\tShipper will follow the recomment to ship order in fastest way.\n");
    char *codeResult = convertVertexIdtoAddress(g, resultPath[0]);
    strcpy(address,"");
    convert_code_to_address(addressCode, numCodeAddress, codeResult, address);
    printf("\tShipper starts at PostOffice with address: ");
    printf("%s\n\n", address);
    for (int i = 1; i < (numResultSearch + 1); i++)
    {
        char address[100]={};
        order orderResult[100];
        char *codeResult = convertVertexIdtoAddress(g, resultPath[i]);
        convert_code_to_address(addressCode, numCodeAddress, codeResult, address);
        int result = search_address(address, orders, numInOrders, orderResult);
        printf("\tNo.%d: %s\n", i , address);
        printf("Id: %ld\n", orderResult[0].ID);
        printf("Name: %s\n", orderResult[0].name);
        printf("PhoneNumber: %s\n", orderResult[0].tele);
        printf("Address: %s\n", orderResult[0].address);
        printf("Produect: %s\n", orderResult[0].prod);
        printf("Total: %.0lf\n", orderResult[0].total);
        printf("Price: %.0lf", orderResult[0].price);
        printf("\n");
    }
    printf("\t Total cost of the best path: %lf (km)\n\n", cost);
}

/** express deliver a order  */
void expressDeliver(Graph g, char *addressCode, char *address)
{
    int length = 0, startPoint = 0, i=1, path[1000], ward[100], *token;
    int endPoint;
    strcpy(ward, address);
    endPoint = convertAddresstoVertexId(g, addressCode);
    double cost = shortestPath(g, startPoint, endPoint, path, &length);
    printf("Processing ...\n");
    printf("The best path which shipper should follow: \n");
    token = strtok(ward, ", "); token = strtok(NULL, "\n");
    for(i=length-1; i>=0;i--){
        if(i == length-1)
            printf("start at: ");
        else if(i> 0)
            printf("go to ");
        else
            printf("end at: ");
        printf("%d %s\n", path[i], token);
    }
    printf("total km take : %.1lf\n", cost);
}
void printGraph(Graph g){
    JRB v, treeV, treeNode;
    jrb_traverse(v, g.edges){
        if(v!= NULL){
            printf("%d: ", jval_i(v->key));
            treeV = (JRB) jval_v(v->val);
            if(treeV != NULL){
                jrb_traverse(treeNode, treeV)
                    printf("%d(%.1f) ", jval_i(treeNode->key), jval_d(treeNode->val));
            }printf("\n");
        }
    }
}
int main(void)
{
    order orders[MAX_NUM], Ordersres[1000];
    code_address codes[600];
    char cwd[100], file_in[100], file_out[100], file_code[100], code[20], address[100]={};
    int num_orders, i, num2, num_code;
    int amounts[MAX_NUM];
    getcwd(cwd, 100);
    strcpy(file_in, cwd);
    strcpy(file_out, cwd);
    strcpy(file_code, cwd);
    // WARNING : if you use ubuntu, use this : '/', if you use window, use this : '\\'
    // VD : strcat(cwd, "\\Orders_tiki.csv");
    strcat(file_code, "\\data\\code_address.txt");
    //strcat(file_in, "\\address_product_price_tiki.txt");
    printf("input file(Orders_sendo.csv, Order_shopee.csv, Orders_tiki.csv : ");
    gets(file_in);
    strcat(file_out, "\\data\\");
    strcat(file_out, file_in);

    num_code = init_code_address(file_code, codes);
    num_orders = extract_data_to_struct(file_out, orders);
    int chose;
    do
    {
        printf("\t MENU\t\n");
        printf("1. Sort Orders \n");
        printf("2. Deliver items in a region\n");
        printf("3. Express deliver a order\n");
        printf("4. Search order by ID\n");
        printf("5. Statistics\n");
        printf("6. Exit\n"); /// edit number case

        printf("Chose:");
        scanf("%d", &chose); getchar();
        switch (chose)
        {
        case 1:
        {   
            char by[10];
            //sort
            printf("Sort by(ID/name/tele/price/amount/total):? ");
            scanf("%s", by); getchar();
            if(!strcasecmp(by, "id"))
                quickSort3way2Gen(orders, sizeof(order), 0, num_orders, id_cmpOrder);
            else if(!strcasecmp(by, "name"))
                quickSort3way2Gen(orders, sizeof(order), 0, num_orders, name_cmpOrder);
            else if(!strcasecmp(by, "tele"))
                quickSort3way2Gen(orders, sizeof(order), 0, num_orders, tele_cmpOrder);
            else if(!strcasecmp(by, "address"))
                quickSort3way2Gen(orders, sizeof(order), 0, num_orders, address_cmpOrder);
            else if(!strcasecmp(by, "price"))
                quickSort3way2Gen(orders, sizeof(order), 0, num_orders, price_cmpOrder);
            else if(!strcasecmp(by, "amount"))
                quickSort3way2Gen(orders, sizeof(order), 0, num_orders, amount_cmpOrder);
            else if(!strcasecmp(by, "total"))
                quickSort3way2Gen(orders, sizeof(order), 0, num_orders, total_cmpOrder);
            else{
                printf("wrong input!\n");
            }
            outputOrder(orders, sizeof(order), num_orders);
            break;
        }
        case 2:
        {
            char codeRegion[10];
            printf("Deliver orders in a region by code of the region.\n");
            printf("Ex. When you want to see how shipper deliver orders in a valid region.\n");
            printf("You will input valid code of the region (ex: 00001 mean: Phường Phúc Xá, Quận Ba Đình, Thành phố Hà Nội)\n");
            printf("Code of the region:");
            scanf("%s", codeRegion); getchar();
            int check = 0;
            for(int i=0;i<num_code;i++){
                if(strcmp(codeRegion, codes[i].code) == 0){
                    Graph g = createGraph();
                    //init map
                    initMap(g, codeRegion);
                    deliver(g,codeRegion, orders, num_orders, codes, num_code);
                    check = 1;
                }
                
            }
            if (check == 0)
            {
                printf("\tInvalid code\n. Some valid region\n\n");
                for(int i=0;i<10;i++){
                    printf("Code: %s- Address: %s\n", codes[i].code, codes[i].address);
                }
            }
            break;
        }
        case 3:
        {
            char address[100], codeAddress[20], *token, old[20];
            printf("Express deliver a order in a specified region.\n");
            printf("Input where you want deliver item to(ex: 77,Phương liên,Đống Đa,Hà Nội).\n");
            printf("Input address: ");
            gets(address);
            convert_address_to_code(codes, num_code, address, codeAddress);
            strcpy(old, codeAddress);
            token = strtok(old, "- "); 
            Graph g = createGraph();
            initMap(g,token);
            expressDeliver(g, codeAddress, address);
            break;
        }
        case 4:
        {   
            char by[10], search_key[100], file_in[100];
            printf("what do you want to search: \n");
            printf("id, name, tele, address, product, price ?: ");
            scanf("%s", by); getchar();
            printf("enter your search key : ");
            gets(search_key);
            if(strcmp(by, "address")==0){
                order outputs[1000];
                printf("%s\n", search_key);
                int max = search_address(search_key, orders, num_orders, outputs);
                for(int i = 0; i < max;i++){
                        printf("%ld,%s,%s,%s,%.1lf,%d,%.1lf \n", outputs[i].ID,outputs[i].name,outputs[i].tele,outputs[i].address, outputs[i].prod,outputs[i].price,outputs[i].amount,outputs[i].total);
                }
            }
            else
                search_data(by, search_key, file_out);
            break;
        }
        case 5:
        {   
            printf("Statistic orders : \n");
            //statistic_address(file_out);
            statistic_prices(orders, num_orders);
            statistic_amount(orders, num_orders);
            break;
        }
        case 6:
            break;
        default:
        {
            if (chose != 6)
            {
                printf("Invalid ! try again !!\n");
            }
            else
            {
                printf("Good bye !. See you next time !!!\n");
            }
            break;
        }
        }
    } while (chose != 6); // 5- when case exit

    return 0;
}
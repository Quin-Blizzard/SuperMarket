#include <stdio.h>
#include <string.h>
#include "Product/Product.h"
#include "Order/Order.h"
#include "tools/hint.h"
#include "tools/info.h"
#include "Admin/Admin.h"
#include "User/User.h"

static const char* FILEPATH = "src/Data/Goods_Info.txt";
static int Total_ProductsNumber = 0;
static int Total_OrdersNumber = 0;
static int Total_UserNumber = 0;
Product goods[MAX_PRODUCT_NUMBER];
extern Order orders[MAX_ORDER_NUMBER];

Product* Get_Good(int idx)
{
    return goods + idx;
}

void Load_Products()
{
    FILE *ptr = fopen(FILEPATH, "r");

    if(ptr == NULL)
    {
        open_ErrorMessage();
        return ;
    }
    else
    {
        Total_ProductsNumber = 0;
        while(fscanf(ptr ,"%s%s%lf%s%s%s%d%d", goods[Total_ProductsNumber].id, \
            goods[Total_ProductsNumber].name, \
            &(goods[Total_ProductsNumber].price), goods[Total_ProductsNumber].discribe, \
            goods[Total_ProductsNumber].SellID, goods[Total_ProductsNumber].SellTime, \
            &(goods[Total_ProductsNumber].cnt), &(goods[Total_ProductsNumber].state))!= EOF)
            Total_ProductsNumber ++;
    }
    fclose(ptr);
}

void Print_Products(char type)
{
    for(int i = 0; i < Total_ProductsNumber; i ++)
    {
        printf("%10s %10s %.1f %10s %10s %10s %d %d \n", goods[i].id, \
            goods[i].name, \
            goods[i].price, goods[i].discribe, \
            goods[i].SellID, goods[i].SellTime, \
            goods[i].cnt, goods[i].state);
    }
    if(type == 'U') Buyer_Menu();
    else Admin_Menu();
}

void Out_Products()
{
    FILE *ptr = fopen(FILEPATH, "w");

    for(int i = 0; i < Total_ProductsNumber; i ++)
    {
        fprintf(ptr, "%s %s %lf %s %s %s %d %d\n", goods[i].id, \
            goods[i].name, \
            goods[i].price, goods[i].discribe, \
            goods[i].SellID, goods[i].SellTime, \
            goods[i].cnt, goods[i].state);
    }

    fclose(ptr);
}

void Add_Product(char *Now_User)
{
    char name[MAX_NAME_LENGTH];
    char discribe[MAX_DIS_LENGTH];
    double price;
    int cnt;

    printf("请输入商品名称：");
    scanf("%s", goods[Total_ProductsNumber].name);
    printf("请输入商品价格: ");
    scanf("%lf", &(goods[Total_ProductsNumber].price));
    printf("请输入对商品的描述： ");
    scanf("%s", goods[Total_ProductsNumber].discribe);
    printf("请输入需要添加商品的数量：");
    scanf("%d", &(goods[Total_ProductsNumber].cnt));

    Generate_ID(goods[Total_ProductsNumber].id, 'G');
    printf("该商品的id:%s\n",goods[Total_ProductsNumber].id);

    Get_Time(goods[Total_ProductsNumber].SellTime);
    printf("该商品的上架时间：%s\n",goods[Total_ProductsNumber].SellTime);
    
    strcpy(goods[Total_ProductsNumber].SellID, Now_User);

    goods[Total_ProductsNumber].state = 1;

    Total_ProductsNumber++;

    FILE* ptr = fopen(FILEPATH, "w");
    fprintf(ptr, "%s %s %lf %s %s %s %d %d\n", goods[Total_ProductsNumber].id, \
    goods[Total_ProductsNumber].name, goods[Total_ProductsNumber].price, \
    goods[Total_ProductsNumber].discribe, goods[Total_ProductsNumber].SellID, \
    goods[Total_ProductsNumber].SellTime, goods[Total_ProductsNumber].cnt, \
    goods[Total_ProductsNumber].state);
    fclose(ptr);
}


void Delete_Product(char *Now_User)
{
    char goodid[MAX_ID_LENGTH];

    printf("请输入需要删除的商品id: ");
    scanf("%s",goodid);


    
    for(int i = 0; i < Total_ProductsNumber; i++)
    {
        if (strcmp(goods[i].id , goodid) == 0)
        {
            if(strcmp("A", Now_User) == 0)
            {
                goods[i].state = 0 ;//下架后 商品状态状态为 已下架
                printf("下架成功！");
                Out_Products();
                Admin_Menu();
            }
            else
            {
                for(int i = 0; i < Total_ProductsNumber; i++)
                {
                    if(strcmp(goods[i].SellID , Now_User) == 0)
                    {
                        goods[i].state = 0 ;//下架后 商品状态状态为 已下架
                        printf("下架成功！");
                        return ;
                    }
                }
                printf("您不是该商品的发布者，无法下架该商品!");
                Seller_Menu(*Now_User);
                return ;
            }
        }
    }         
    printf("未查询到该商品！");
    Seller_Menu(*Now_User);
}

void Search_Product(char type)
{
//可以按照商品名称商品名称查找商品
    char a[MAX_NAME_LENGTH];

    printf("请输入您要查找的商品名称或商品id: ");
    scanf("%s", a);
    extern int Now_User;

    int flag = search(a);
    if(flag == -1)
    {
        printf("未查找到商品！");
        if(type == 'U') Buyer_Menu();
        else if (type == 'A') Admin_Menu();
        else Seller_Menu(Now_User);
    }
    else
    {
        printf("商品id:%s \n", goods[flag].id );
        printf("商品名称：%s \n",goods[flag].name);
        printf("商品价格：%.1f\n", goods[flag].price);
        printf("商品描述：%s\n",goods[flag].discribe);
        printf("上架时间：%s\n",goods[flag].SellTime);
        printf("剩余数量：%d\n",goods[flag].cnt);
        switch(goods[flag].state)
        {
            case 0: printf("商品状态： 已下架 "); break;
            case 1: printf("商品状态： 在售中 "); break;
            case 2: printf("商品状态： 已售空 "); break;
        }
        if(type == 'U') Buyer_Menu();
        if(type == 'A') Admin_Menu();
        if(type == 'S') Seller_Menu(Now_User);
    }
}

int search(char *s)
{
    for(int i = 0 ; i < Total_ProductsNumber ; i++)
    {
        if(strcmp(s , goods[i].name) == 0 || strcmp(s ,goods[i].id) == 0)
        {
            return i;
        }
    }
    return -1;
}
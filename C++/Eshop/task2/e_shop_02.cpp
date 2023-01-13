//vs code, MinGW-w64 g++
//windows 10
//class:2019211305 Name:RenChenxu ID:2019211279
//set txt_path(UserFileName,GoodsFileName) before compilation
#include<iostream>
#include<string>
#include<windows.h>
#include<fstream>
#include<vector>
#define UserFileName "user.txt"
#define GoodsFileName "goods.txt"

#define CHECKCIN if(cin.good()==0){cout<<"Data type error \nThe progaram is terminated"<<endl;system("pause");exit(0);}


using namespace std;
const short T_consumer=1;
const short T_merchant=2;
const short T_food=1;
const short T_clothes=2;
const short T_book=3;

class Goods
{
public:
    short m_type;
    string m_description,m_belong;
    double m_price;
    int m_number;
    double discount=1;
    virtual const double getPrice()=0;
};

class User
{
public:
    string m_account;//用户账号
    int m_password;//用户密码
    double m_balance;//账户余额
    short m_type;//用户类型
    virtual const short getUserType()=0;
    bool logIn(const int userType);
    bool signUp(const int userType);
    void changePassword(User** All,const int accLine);
    void logOut();
    void dealBalance(User** All,const int accLine);
    void searchGoods(Goods** All,const int gAllSize);//搜索商品
    void showGoods(Goods** All,const int gAllSize);//展示商品信息
    const int getTxtSize();
    void getAll(User** All,int& accLine,const int allSize);//从文件读入所有账户信息,找到本账户所在行，返回总行数
    void setAll(User** All,const int allSize);//将信息写入文件
    const int getGoodSize();
    void getAll(Goods** All,const int gAllsize);
    void setAll(Goods** All,const int gAllsize);
    const int getGloca(Goods** All,const int gAllsize,const string name);//获取商品所在行
    const int getUloca(User** All,const int allSize, const string name);//获取用户所在行
};

class Consumer :public User
{
public:
    Consumer(string account, int password, double balance, short type);
    Consumer();
    const short getUserType(){return T_consumer;}
    void showFuncTable();//显示可用操作
    void shopping(User** All,const int allSize,Goods** gAll,const int gAllsize);
    void addToCart(vector<Goods*>& shoppingCart,Goods** gAll,const int gAllsize);
    void removeFromCart(vector<Goods*>& shoppingCart);
    void showCart(vector<Goods*>shoppingCart);
    void changeInCart(vector<Goods*>& shoppingCart,Goods** gAll,const int gAllsize);
    void generateOrder(vector<Goods*>& shoppingCart,Goods** gAll,const int gAllsize,User** All,const int Allsize);
    void clearCart(vector<Goods*> &shoppingCart);
    /******************/
    void showInfo();//展示用户信息
};
class Merchant : public User
{
public:
    Merchant(string account, int password, double balance, short type);
    Merchant();
    const short getUserType(){return T_merchant;}
    void showFuncTable();
    void addGoods();//添加商品
    void reviseGoodsInfo(Goods** All,const int gAllSize);//管理商品(改价，改余量)
    void activity(Goods** All,const int gAllSize);
    /******************/
    void showInfo();
};

class Clothes : public Goods
{
public:
    Clothes (short type,string description,double price,int number,string belong,double discount);
    const double getPrice(){return this->m_price * this->discount;}
};
class Food : public Goods
{
public:
    Food(short type,string description,double price,int number,string belong,double discount);
    const double getPrice(){return this->m_price * this->discount;}
};
class Book : public Goods
{
public:
    Book(short type,string description,double price,int number,string belong,double discount);
    const double getPrice(){return this->m_price * this->discount;}
};

class MENU
{
public:
    void printMENU();
    void getChoice();
    void exitShop();
    void signUpGuide();
    void logInGuide();
    void showGoods();
};

void MENU::printMENU()
{
    for(int i=1;i<64;i++) cout<<"*";
    cout<<endl;
    cout<<"\t\tWelcome to e_shop!"<<endl;
    cout<<"\t\t0:Exit"<<endl;
    cout<<"\t\t1:Log in"<<endl;
    cout<<"\t\t2:Sign up"<<endl;
    cout<<"\t\t3:Show all goods"<<endl;
    for(int i=1;i<64;i++) cout<<"*";
    cout<<endl;
    ifstream uifs,gifs;
    uifs.open(UserFileName,ios::in);
    gifs.open(GoodsFileName,ios::in);
    if(!(uifs.is_open()&&gifs.is_open())){
        cout<<"No UserFile or GoodsFile, please first sign up as merchant and add  goods\n"<<endl;
    }
    uifs.close();
    gifs.close();
}
void MENU::getChoice()
{
    int choice;
    while(1){
        this->printMENU();
        cout<<"Choose one function."<<endl;
        cin>>choice;    CHECKCIN
        switch (choice)
        {
        case 0://退出
            this->exitShop();
            break;
        case 1://登录
            this->logInGuide();
            break;
        case 2://注册
             this->signUpGuide();
            break;
        case 3://显示商品信息
            this->showGoods();
        default:
            break;
        }
    }
}
void MENU::exitShop()
{
    cout<<"See you next time!"<<endl;
    system("pause");
    exit(0);
}
void MENU::logInGuide()
{
    int userType;
    cout<<"Log In :"<<endl;
    cout<<"Please input your UserType(1:consumer 2:merchant)"<<endl;
    cin>>userType;  CHECKCIN
    if(userType==T_consumer){        
        Consumer* consumer=new Consumer(); 
        bool isLogIn=consumer->logIn(T_consumer);
        if(isLogIn)
            consumer->showFuncTable();
        delete consumer;
    }
    if(userType==T_merchant){
        Merchant* merchant=new Merchant();
        bool isLogIn=merchant->logIn(T_merchant);
        if(isLogIn)
            merchant->showFuncTable();
        delete merchant;
    }
    cout<<"Turn to start menu"<<endl;
    system("pause");
    system("cls");

}
void MENU::signUpGuide()
{
    int userType;
    cout<<"Sign Up :"<<endl;
    cout<<"Please input your UserType:(1:consumer 2:merchant)"<<endl;
    cin>>userType;  CHECKCIN
    if(userType==T_consumer){        //
        Consumer* consumer=new Consumer(); 
        bool isSignUp=consumer->signUp(T_consumer);
        if(isSignUp)
            consumer->showFuncTable();
        delete consumer;
    }
    if(userType==T_merchant){
        Merchant* merchant=new Merchant();
        bool isSignUp=merchant->signUp(T_merchant);
        if(isSignUp)
            merchant->showFuncTable();
        delete merchant;
    }
}
void MENU::showGoods()
{
    ifstream gifs;
    short type;
    string desc,belong;
    int num;
    double price,discount;
    gifs.open(GoodsFileName,ios::in);
    if(!gifs.is_open()){
        cout<<"Cann't find file"<<endl;
        gifs.close();
    }
    else{
        int i=0;
        while(!gifs.eof()){
            gifs>>desc>>type>>price>>num>>belong>>discount;
            i++;
        }
        gifs.close();
        i--;
        gifs.open(GoodsFileName,ios::in);
        cout<<"\n\t\t\tShow goods"<<endl;
        cout<<"\t\ttype:1:food,2:clothes,3:book"<<endl;
        // cout<<"You can enjoy a discount if you buy more than 5 pieces of the same goods at the same time."<<endl;
        // cout<<"discountInfo:food:10%off  clothes:20%off  book:30%off\n"<<endl;
        cout<<"Description\ttype\tprice\tnumber\tbelong\tdiscount\tprice_d"<<endl;
        for(int j=0;j<i;j++){
            gifs>>desc>>type>>price>>num>>belong>>discount;
            cout<<desc<<"\t\t"<<type<<"\t"<<price<<"\t"<<num<<"\t"<<belong<<"\t"<<discount
            <<"\t\t"<<discount * price<<endl;
        }
    }
    cout<<"\n";
    cout<<"Turn to start menu"<<endl;
    system("pause");
}

Consumer::Consumer(string account, int password, double balance, short type)
{
    this->m_account=account;
    this->m_password=password;
    this->m_balance=balance;
    this->m_type=type;
}
Consumer::Consumer()
{

}
Merchant::Merchant(string account, int password, double balance, short type)
{
    this->m_account=account;
    this->m_password=password;
    this->m_balance=balance;
    this->m_type=type;
}
Merchant::Merchant()
{

}
void Consumer::showInfo()
{
    cout<<"This is debug_info"<<endl;
    cout<<"type:"<<this->m_type<<"\taccount:"<<this->m_account;
    cout<<"\tpassword:"<<this->m_password<<"\tbalance:"<<this->m_balance<<endl;

}
void Merchant::showInfo()
{
    cout<<"This is debug_info"<<endl;
    cout<<"type:"<<this->m_type<<"\taccount:"<<this->m_account;
    cout<<"\tpassword:"<<this->m_password<<"\tbalance:"<<this->m_balance<<endl;

}
Food::Food(short type,string description,double price,int number,string belong,double discount)
{
    this->m_type=type;
    this->m_description=description;
    this->m_price=price;
    this->m_number=number;
    this->m_belong=belong;
    this->discount=discount;
}
Clothes::Clothes (short type,string description,double price,int number,string belong,double discount)
{
    this->m_type=type;
    this->m_description=description;
    this->m_price=price;
    this->m_number=number;
    this->m_belong=belong;
    this->discount=discount;
}
Book::Book(short type,string description,double price,int number,string belong,double discount)
{
    this->m_type=type;
    this->m_description=description;
    this->m_price=price;
    this->m_number=number;
    this->m_belong=belong;
    this->discount=discount;
}


bool User::logIn(const int userType)
{
    string userName;
    int password;
    int tryTimes=0;
    this->m_type=userType;
    if(userType==T_consumer)
        cout<<"Log in as a consumer:"<<endl;
    if(userType==T_merchant)
        cout<<"Log in as a merchant:"<<endl;
    cout<<"Please input your username:"<<endl;
    cin>>userName;
    //todo:在文件中查找用户名
    bool nameExist=false;//用户名是在文件的标志
    string fuserName;
    int fuserType,fpassword;
    double fbalance;//文件中存放的内容
    ifstream uifs;
    uifs.open(UserFileName,ios::in);
    if(!uifs.is_open()){
        cout<<"Cann't find file"<<endl;
        uifs.close();
        return false;
    }
    else{
        while(!uifs.eof()){
            uifs>>fuserName;
            uifs>>fuserType;
            uifs>>fpassword;
            uifs>>fbalance;
            if(fuserName==userName&&fuserType==userType){
                nameExist=true;
                break;
            } 
        }
        if(nameExist==false){
            cout<<"Can't find this account"<<endl;
            return false;
        }
    }

    cout<<"Please input your password"<<endl;
    cin>>password;  CHECKCIN
    while(password!=fpassword&&tryTimes<3){
        tryTimes++;
        cout<<"The password is wrong, please input the right one"<<endl;
        cin>>password;  CHECKCIN
    }
    if(tryTimes==3){
        cout<<"Fail to log in!"<<endl;
        return false;
    }
    else{//将文件里该用户信息读入
        cout<<"Success to log in!"<<endl;
        this->m_account=fuserName;
        this->m_password=fpassword;
        this->m_type=fuserType;
        this->m_balance=fbalance;
        return true;
    }
}
bool User::signUp(const int userType)
{
    string userName;
    int password;
    this->m_type=userType;
    if(userType==T_consumer)
        cout<<"Sign up as a consumer:"<<endl;
    if(userType==T_merchant)
        cout<<"Sign up as a merchant:"<<endl;
    cout<<"Please input your username:"<<endl;
    cin>>userName;
    //todo:遍历文件，查询用户名是否已经用过
    bool nameUsed=false;//用户名是否使用过的标志
    string fuserName;
    int fuserType,fpassword;
    double fbalance;//文件中存放的内容
    ifstream uifs;
    uifs.open(UserFileName,ios::in);
    if(!uifs.is_open()){
        cout<<"Cann't find file"<<endl;
        uifs.close();
    }
    else{
         while(!uifs.eof()){
            uifs>>fuserName;
            uifs>>fuserType;
            uifs>>fpassword;
            uifs>>fbalance;
            if(fuserName==userName)
                nameUsed=true;
        }
        if(nameUsed){
            uifs.close();
            cout<<"This username is unavailable,turn to start menu and try again"<<endl;
            return false;
        }
    } 
    this->m_account=userName;
    cout<<"This username is available ,please set the password"<<endl;
    cin>>password;  CHECKCIN
    this->m_password=password;
    this->m_balance=0;//余额初始化为0
    //todo:将信息写入文件末尾
    ofstream uofs;
    uofs.open(UserFileName,ios::out|ios::app);
    uofs<<this->m_account<<"\t"<<this->m_type<<"\t"<<this->m_password<<"\t"<<this->m_balance<<endl;
    uofs.close();
    cout<<"Success to create your account!\t"<<endl;
    return true;
}
void User::logOut()
{
    cout<<"Log out!\t"<<endl;
}
void User::dealBalance(User** All,const int accLine)
{
    int op;
    double money;
    cout<<"\nChoose what you want to do:"<<endl;
    cout<<"1:Check the balance\t2:Recharge\t3:Consume"<<endl;
    cin>>op;    CHECKCIN
    switch (op)
    {
    case 1:
        cout<<"Your balance is:"<<this->m_balance<<endl;
        break;
    case 2:
        cout<<"Please input recharge amount"<<endl;
        cin>>money; CHECKCIN
        this->m_balance+=money;
        All[accLine]->m_balance=this->m_balance;
        cout<<"Success to recharge,your balance is now: "<<this->m_balance<<"\n"<<endl;
        break;
    case 3:
        cout<<"Please input how much goods cost"<<endl;
        cin>>money; CHECKCIN
        if(money>this->m_balance){
            cout<<"Sorry,you don't have enough money"<<"\n"<<endl;
            break;
        }
        else{
            this->m_balance-=money;
            All[accLine]->m_balance=this->m_balance;
            cout<<"Success to consume,your balance is now: "<<this->m_balance<<endl;
            break;
        }
        
    default:
        cout<<"Error!"<<endl;
        break;
    }
}
void User::changePassword(User** All,const int accLine)
{
    int password,tryTimes=0;
    cout<<"Your username is:"<<this->m_account<<endl;
    cout<<"Please input your old password:"<<endl;
    cin>>password;  CHECKCIN
    while(password!=this->m_password&&tryTimes<3){
        cout<<"Not the right password"<<endl;
        cin>>password;  CHECKCIN
        tryTimes++;
    }
    if(tryTimes==3){
        cout<<"Fail to change password!"<<endl;
    }
    else{
        cout<<"Please input your new password:"<<endl;
        cin>>password;  CHECKCIN
        this->m_password=password;
        All[accLine]->m_password=password;
        //cout<<"accLine="<<accLine<<endl;
        cout<<"Success to change password,your new password is: "<<All[accLine]->m_password<<endl;
        system("pause");
    }
}
void User::searchGoods(Goods** All,const int gAllSize)
{
    string goodsName,desc,belong;
    int desLine=-1;
    int type,num;
    double price,discount;
    cout<<"Please input the goods you want to find"<<endl;
    cin>>goodsName;
    for(int i=0;i<gAllSize;i++){
        desc=All[i]->m_description;
        type=All[i]->m_type;
        price=All[i]->m_price;
        num=All[i]->m_number;
        belong=All[i]->m_belong;
        discount=All[i]->discount;
        if(desc==goodsName){
            cout<<"\nFind it!"<<endl;
            cout<<"\ttype:1:food,2:clothes,3:book\n"<<endl;
            cout<<"Description\ttype\tprice\tnumber\tbelong\tdiscount\tprice_d"<<endl;
            desLine=i;
            cout<<All[i]->m_description<<"\t\t"<<All[i]->m_type<<"\t"<<
                All[i]->m_price<<"\t"<<All[i]->m_number<<"\t"<<All[i]->m_belong
                <<"\t"<<All[i]->discount<<"\t\t"<<All[i]->getPrice()<<endl;
            break;
        }
    }
    if(desLine<0){
        cout<<"Sorry,no such goods"<<endl;
        return;
    }

}
void User::showGoods(Goods** All,const int gAllSize)
{
    cout<<"\n\t\t\tShow goods"<<endl;
    cout<<"\t\ttype:1:food,2:clothes,3:book"<<endl;
    // cout<<"You can enjoy a discount if you buy more than 5 pieces of the same goods at the same time."<<endl;
    // cout<<"discountInfo:food:10%off  clothes:20%off  book:30%off\n"<<endl;
    cout<<"Description\ttype\tprice\tnumber\tbelong\tdiscount\tprice_d"<<endl;
    for(int i=0;i<gAllSize;i++){
        cout<<All[i]->m_description<<"\t\t"<<All[i]->m_type<<"\t"<<
            All[i]->m_price<<"\t"<<All[i]->m_number<<"\t"<<All[i]->m_belong
            <<"\t"<<All[i]->discount<<"\t\t"<<All[i]->getPrice()<<endl;
    }
    cout<<"\n";
}
const int User:: getTxtSize()
{
    ifstream uifs;
    int lineNum=0;
    uifs.open(UserFileName,ios::in);
    if(!uifs.is_open()){
        cout<<"Cann't find file"<<endl;
        uifs.close();
    }
    else{
        string account;
        int type,password;
        double balance;
        while(!uifs.eof()){
            uifs>>account>>type>>password>>balance;
            lineNum++;
        }
    }
    return lineNum-1;
}
void User:: getAll(User** All,int& acc,const int allSize)
{
    ifstream uifs;
    int lineNum=0;
    string account;
    int type,password;
    double balance;
    uifs.open(UserFileName,ios::in);
    if(!uifs.is_open()){
        cout<<"Cann't find file"<<endl;
        uifs.close();
    }
    else{
        while(!uifs.eof()&&lineNum<allSize){
            uifs>>account;
            uifs>>type;
            uifs>>password;
            uifs>>balance;
            User* temp=NULL;
            if(type==T_consumer)
                temp=new Consumer(account,password,balance,type);
            if(type==T_merchant)
                temp=new Merchant(account,password,balance,type);
            All[lineNum]=temp;
            if(this->m_account==account){
                acc=lineNum;
            }
            lineNum++; 
        }  
    }
    // lineNum--;
    // if(acc==lineNum)
    //     acc--;
    //cout<<"allSize="<<lineNum<<endl;
    //cout<<"accNum="<<acc<<"\t"<<endl;
    uifs.close();
    return ;
}
void User:: setAll(User** All,const int allSize)
{
    ofstream uofs;
    uofs.open(UserFileName,ios::out);
    for(int i=0;i<allSize;i++){
        uofs<<All[i]->m_account<<"\t"<<All[i]->m_type<<"\t"<<All[i]->m_password<<"\t"<<All[i]->m_balance<<"\n";
    }
    uofs.close();
}
const int User::getGoodSize()
{
    ifstream gifs;
    int lineNum=0;
    gifs.open(GoodsFileName,ios::in);
    if(!gifs.is_open()){
        cout<<"Cann't find file"<<endl;
        gifs.close();
    }
    else{
        short type;
        string desc,belong;
        int num;
        double price,discount;
        while(!gifs.eof()){
            gifs>>desc>>type>>price>>num>>belong>>discount;
            lineNum++;
        }
    }
    return lineNum-1;
}
void User::getAll(Goods** All,const int gAllsize)
{
    ifstream gifs;
    int lineNum=0;
    short type;
    string desc,belong;
    int num;
    double price,discount;
    gifs.open(GoodsFileName,ios::in);
    if(!gifs.is_open()){
        cout<<"Cann't find file"<<endl;
        gifs.close();
    }
     else{
        while(!gifs.eof()&&lineNum<gAllsize){
            gifs>>desc>>type>>price>>num>>belong>>discount;
            Goods* temp=NULL;
            if(type==T_food)
                temp=new Food(type,desc,price,num,belong,discount);
            if(type==T_clothes)
                temp=new Clothes(type,desc,price,num,belong,discount);
            if(type==T_book)
                temp=new Book(type,desc,price,num,belong,discount);
            All[lineNum]=temp;
            lineNum++; 
        }
        gifs.close();  
    }
}
void User::setAll(Goods** All,const int gAllsize)
{
    ofstream gofs;
    gofs.open(GoodsFileName,ios::out);
    for(int i=0;i<gAllsize;i++){
        gofs<<All[i]->m_description<<"\t"<<All[i]->m_type<<"\t"<<
            All[i]->m_price<<"\t"<<All[i]->m_number<<"\t"<<All[i]->m_belong
            <<"\t"<<All[i]->discount<<"\n";
    }
}
const int User:: getGloca(Goods** All,const int gAllsize,const string name)
{
    int i=0;
    for(i=0;i<gAllsize;i++){
        if(All[i]->m_description==name){
            break;
        }
    }
    if(i==gAllsize){
        return -1;
    }
    else
    {
        return i;
    }
    
}
const int User:: getUloca(User** All,const int allSize, const string name)
{
    int i=0;
    for(i=0;i<allSize;i++){
        if(All[i]->m_account==name){
            break;
        }
    }
    if(i==allSize){
        return -1;
    }
    else
    {
        return i;
    }
}


void Consumer::showFuncTable()
{
    int choice;
    cout<<"Choose what you want to do:"<<endl;
    cout<<"1:change password\t2:manage balance\t3:show goods"<<endl;
    cout<<"4:search goods\t5:shopping\t0:log out"<<endl;
    cin>>choice;    CHECKCIN
    while(choice){
        const int allSize=this->getTxtSize();
        const int gAllSize=this->getGoodSize();
        if(allSize<=0||gAllSize<=0){
            cout<<"Error:cannot find User or Goods txt,please set user and goods in advance"<<endl;
            break;
        }
        User** all=new User*[allSize];
        Goods** gAll=new Goods*[gAllSize];
        int acc=0;
        this->getAll(all,acc,allSize);
        this->getAll(gAll,gAllSize);
        switch (choice)
        {
        case 1:
            this->changePassword(all,acc);
            break;
        case 2:
            this->dealBalance(all,acc);
            break;
        case 3:
            this->showGoods(gAll,gAllSize);
            break;
        case 4:
            this->searchGoods(gAll,gAllSize);
            break;
        case 5:
            this->shopping(all,allSize,gAll,gAllSize);
            break;
        default:
            cout<<"You got a wrong choice!\tInput a right one!"<<endl;
            break;
        }
        this->setAll(all,allSize);
        delete []all;
        delete []gAll;
        cout<<"Choose what you want to do else:"<<endl;
        cout<<"1:change password\t2:manage balance\t3:show goods"<<endl;
        cout<<"4:search goods\t5:shopping\t";
        cout<<"0:log out"<<endl;
        cin>>choice;    CHECKCIN
    }
    if(choice==0){
        this->logOut();
    }
    return;
}
void Consumer::shopping(User** All,const int allSize,Goods** gAll,const int gAllsize)
{
    //创建一个购物车
    //购物车是指向抽象类的指针向量，每个元素为具体的指向派生类的指针，
    //通过用户输入确定商品名和数量,price 仍为单价，
    //通过this指针来确定消费者账户，Goods自身的belong属性来确定消费者账户
    //添加商品时先在Goods** gAll中找到该商品，根据type，new相应的商品类，拷贝初始化，填入相应的type,name,price,belong,根据用户输入填入num,
    //价格计算：单价*数量*折扣
    int choice;
    vector<Goods*> shoppingCart;



    // Goods* test=NULL;
    // test=new Book(T_book,"rice",2,30,"rita",1);
    // shoppingCart.push_back(test);
    // cout<<"Here are your shoping Cart:"<<endl;
    // cout<<"type:"<<shoppingCart[0]->m_type<<"\tdesc:"<<shoppingCart[0]->m_description
    //     <<"\tprice:"<<shoppingCart[0]->m_price<<"\tnum:"<<shoppingCart[0]->m_number
    //     <<"\tdicount:"<<shoppingCart[0]->discount<<endl;


    cout<<"Shopping"<<endl;
    cout<<"choose what you want to do"<<endl;
    cout<<"1:Add goods to shoppingCart\t\t2:Remove goods from shoppingCart"<<endl;
    cout<<"3:Change num of goods in shoppingCart\t\t4:Show all goods in e-shop"<<endl;
    cout<<"5:Search goods in e-shop\t\t6:Generate order"<<endl;
    cout<<"0:Drop out and return to Functable"<<endl;
    cin>>choice;    CHECKCIN
    while(choice){
        switch (choice)
        {
        case 1 :
            //Add goods to shoppingCart
            this->addToCart(shoppingCart,gAll,gAllsize);
            break;
        case 2 :
            this->removeFromCart(shoppingCart);
            //2:Remove goods from shoppingCart
            break;
        case 3 :
            //cout<<"type"<<shoppingCart[0]->m_type<<endl;
            this->changeInCart(shoppingCart,gAll,gAllsize);
            //3:Change num of goods in shoppingCart
            break;
        case 4 :
            this->showGoods(gAll,gAllsize);
            break; 
        case 5 :
            this->searchGoods(gAll,gAllsize);
            break;
        case 6 :
            this->generateOrder(shoppingCart,gAll,gAllsize,All,allSize);
            break;          
        default:
            cout<<"You got a wrong choice,choose the right one!"<<endl;
            break;
        }
        cout<<"choose what you want to do else"<<endl;
        cout<<"1:Add goods to shoppingCart\t\t2:Remove goods to shoppingCart"<<endl;
        cout<<"3:Change num of goods in shoppingCart\t\t4:Show all goods in e-shop"<<endl;
        cout<<"5:Search goods in e-shop\t\t6:Generate order"<<endl;
        cout<<"0:Drop out and return to Functable"<<endl;
        cin>>choice;    CHECKCIN
    }
    if(choice==0){
        cout<<"Dear "<<this->m_account<<", you have "<<this->m_balance<<"RMB left"<<endl;
        cout<<"Finish shopping!"<<endl;
       
    }

    system("pause");
    return ;
}
void Consumer::addToCart(vector<Goods*>& shoppingCart,Goods** gAll,const int gAllsize)
{

    string name;
    int gline=-1;
    int bnum=0;
    cout<<"please input goods name:"<<endl;
    cin>>name;
    gline=this->getGloca(gAll,gAllsize,name);
    if(gline>=0){
        cout<<"Find it!"<<endl;
        cout<<"please input number you want to buy"<<endl;
        cin>>bnum;  CHECKCIN
        if(gAll[gline]->m_number>=bnum){
            Goods *temp=NULL;
            switch (gAll[gline]->m_type)
            {
            case T_food:
                temp=new Food(T_food,name,gAll[gline]->m_price,bnum,gAll[gline]->m_belong,gAll[gline]->discount);
                break;
            case T_clothes:
                temp=new Clothes(T_clothes,name,gAll[gline]->m_price,bnum,gAll[gline]->m_belong,gAll[gline]->discount);
                break;
            case T_book:
                temp=new Book(T_book,name,gAll[gline]->m_price,bnum,gAll[gline]->m_belong,gAll[gline]->discount);
                break;
            default:
                break;
            }
            shoppingCart.push_back(temp);
            cout<<"Success to add goods to your shoppingCart"<<endl;
        }
        else{
            cout<<"Error:more than numbers on sale!";
        }
    }
    else{
        cout<<"No such goods"<<endl;
    }
    cout<<"\nHere are goods in your shoppingCart:"<<endl;
    this->showCart(shoppingCart);
}
void Consumer::removeFromCart(vector<Goods*>& shoppingCart)
{
    int remove;
    cout<<"Here are goods in your shoppingCart:"<<endl;
    //
    cout<<"shoppingCart.size:"<<shoppingCart.size()<<endl;
    this->showCart(shoppingCart);
    cout<<"Choose which you want to remove(please input the number before description)"<<endl;
    cin>>remove;    CHECKCIN
    remove--;
    int i=0;
    if(remove<0||remove>=shoppingCart.size()){
        cout<<"Wrong number!"<<endl;
        return;
    }
    for(auto it=shoppingCart.begin();it!=shoppingCart.end();it++){
        if(remove==i){
            it=shoppingCart.erase(it);
            if(it==shoppingCart.end())  
                break;
            cout<<"Success to remove it\n"<<endl;
        }
        i++;
    }
    cout<<"Here are goods in your shoppingCart:"<<endl;
    this->showCart(shoppingCart);
    cout<<"\n";
}
void Consumer::changeInCart(vector<Goods*>& shoppingCart,Goods** gAll,const int gAllsize)
{
    int i,cnum,cprice;
    int gline=-1;
    cout<<"Here are goods in your shoppingCart:"<<endl;
    this->showCart(shoppingCart);
    cout<<"Choose which you want to change the number(please input the number before description)"<<endl;
    cin>>i; CHECKCIN
    i--;
    if(i<0||i>=shoppingCart.size()){
        cout<<"Error:no such goods"<<endl;
        return ;
    }
    cout<<"Please input the new number"<<endl;
    cin>>cnum;  CHECKCIN
    cout<<shoppingCart[i]->m_description<<endl;
    gline=this->getGloca(gAll,gAllsize,shoppingCart[i]->m_description);
    //cout<<"gline:"<<gline<<endl;
    if((gline>=0)&&(gAll[i]->m_number>=cnum)){
        shoppingCart[i]->m_number=cnum;//更新数量
        cout<<"type:"<<shoppingCart[i]->m_type<<endl;
        cout<<"Success to change number"<<endl;
        cout<<"Here are goods in your shoppingCart:"<<endl;
        this->showCart(shoppingCart);
    }
    else{
        cout<<"Error:more than numbers on sale or no such goods"<<endl;
    }
    
}
void Consumer::showCart(vector<Goods*> shoppingCart)
{
    const int size=shoppingCart.size();
    for(int i=0;i<size;i++){
        cout<<i+1<<"\tdescription:"<<shoppingCart[i]->m_description<<"\ttype:"<<shoppingCart[i]->m_type
            <<"\tprice:"<<shoppingCart[i]->m_price<<"\tbelong:"<<shoppingCart[i]->m_belong
            <<"\tnumber:"<<shoppingCart[i]->m_number<<"\tdiscount:"<<shoppingCart[i]->discount<<endl;
    }
}
void Consumer::generateOrder(vector<Goods*> &shoppingCart,Goods** gAll,const int gAllsize,User** All,const int Allsize)
{
    //判断是否能成功生成订单
    //只有Goods** 中所有商品的数量均大于等于0，用户的余额够支付，才能生成订单
    double totalPrice=0;
    string gname;
    int size=shoppingCart.size();
    for(int i=0;i<size;i++){
        totalPrice+=shoppingCart[i]->getPrice()*shoppingCart[i]->m_number;
    }
    cout<<"Total price is:"<<totalPrice<<endl;
    if(totalPrice>this->m_balance){
        cout<<"You have "<<this->m_balance<<"RMB"<<endl;       
        cout<<"Fail to generate order: short of money"<<endl;
        return;
    }
    //冻结商品
    //flag 超量标记
    bool flag=false;
    for(int i=0;i<size;i++){
        int gline;
        gname=shoppingCart[i]->m_description;
        gline=this->getGloca(gAll,gAllsize,gname);
        gAll[gline]->m_number-=shoppingCart[i]->m_number;
        if(gAll[gline]->m_number<0) flag=true;
    }
    //若超量，还原goods内存，返回
    if(flag){
        cout<<"Fail to generate order: The number is over that on sale"<<endl;
        for(int i=0;i<size;i++){
            int gline;
            gname=shoppingCart[i]->m_description;
            gline=this->getGloca(gAll,gAllsize,gname);
            gAll[gline]->m_number+=shoppingCart[i]->m_number;
        }
        return ;
    }
    //订单支付：只有用户选择不买时失败
    int ifPay;
    cout<<"Do you want to pay for these goods:(1:yes\tothernumber:no,and goback)"<<endl;
    cin>>ifPay; CHECKCIN
    if(ifPay!=1){
        cout<<"Fail to pay:Consumer choose to leave"<<endl;
        //还原goods内存中数量
        for(int i=0;i<size;i++){
            int gline;
            gname=shoppingCart[i]->m_description;
            gline=this->getGloca(gAll,gAllsize,gname);
            gAll[gline]->m_number+=shoppingCart[i]->m_number;
        }
    }
    //以上失败情况最后内存中的信息都还原了，不必写入文件
    if(ifPay==1){
        //内存中对用户和商家的账户余额进行修改，清空购物车，将修改后的内存写入文件
        int uline,gline;
        this->m_balance-=totalPrice;
        uline=this->getUloca(All,Allsize,this->m_account);
        All[uline]->m_balance=this->m_balance;
        for(int i=0;i<size;i++){
            string mname=shoppingCart[i]->m_belong;
            uline=this->getUloca(All,Allsize,mname);
            All[uline]->m_balance+=shoppingCart[i]->getPrice()*shoppingCart[i]->m_number;
        }
        //清空购物车
        this->clearCart(shoppingCart);
        //写回文件
        this->setAll(All,Allsize);
        this->setAll(gAll,gAllsize);
        cout<<"Success to pay"<<endl;
    }
}
void Consumer::clearCart(vector<Goods*> &shoppingCart)
{
    int size=shoppingCart.size();
    for(int i=0;i<size;i++){
        delete shoppingCart[i];
        shoppingCart[i]=NULL;
    }
    shoppingCart.clear();
}

void Merchant::showFuncTable()
{
    int choice;
    cout<<"Choose what you want to do:"<<endl;
    cout<<"1:change password\t2:manage balance\t3:show goods"<<endl;
    cout<<"4:search goods\t5:add goods\t6:revise goods' info\t7:activity discount"<<endl;
    cout<<"0:log out"<<endl;
    cin>>choice;    CHECKCIN
    while(choice){
        if(choice==5)
            this->addGoods();//这里将add单独分出来是因为在没有txt文档时add功能也必须可用（创建该文档）,且add是不依赖原有txt文件内容
        const int allSize=this->getTxtSize();
        const int gAllSize=this->getGoodSize();
        if(allSize<=0||gAllSize<=0){
            cout<<"Error:cannot find User or Goods txt,please set user and goods in advance"<<endl;
            break;
        }
        User** all=new User*[allSize];
        Goods** gAll=new Goods*[gAllSize];
        int acc=0;
        this->getAll(all,acc,allSize);
        this->getAll(gAll,gAllSize);
        switch (choice)
        {
        case 1:
            this->changePassword(all,acc);
            break;
        case 2:
            this->dealBalance(all,acc);
            break;
        case 3:
            this->showGoods(gAll,gAllSize);
            break;
        case 4:
            this->searchGoods(gAll,gAllSize);
            break;
        case 5:
            break;
        case 6:
            this->reviseGoodsInfo(gAll,gAllSize);
            break;
        case 7:
            this->activity(gAll,gAllSize);
            break;
        default:
            cout<<"You got a wrong choice!\tInput a right one!"<<endl;
            break;
        }
        this->setAll(all,allSize);
        delete []all;
        delete []gAll;
        cout<<"Choose what you want to do else:"<<endl;
        cout<<"1:change password\t2:manage balance\t3:show goods"<<endl;
        cout<<"4:search goods\t5:add goods\t6:revise goods' info\t7:activity discount"<<endl;
        cout<<"0:log out"<<endl;
        cin>>choice;    CHECKCIN
    }
    if(choice==0){
        this->logOut();
    }
    return;
}
void Merchant::reviseGoodsInfo(Goods** All,const int gAllSize)
{
    string goodsName,desc,belong;
    int desLine=-1;
    int type,num;
    double price;
    cout<<"Please input the goods you want to change"<<endl;
    cin>>goodsName;
    for(int i=0;i<gAllSize;i++){
        desc=All[i]->m_description;
        type=All[i]->m_type;
        price=All[i]->m_price;
        num=All[i]->m_number;
        belong=All[i]->m_belong;
        if(desc==goodsName&&belong!=this->m_account)
            cout<<"This is not your goods"<<endl;
        if(desc==goodsName&&belong==this->m_account){
            desLine=i;
            cout<<"\nFind it!"<<endl;
            cout<<"\ttype:1:food,2:clothes,3:book\n"<<endl;
            cout<<"Description\ttype\tprice\tnumber\tbelong\tdiscount"<<endl;
            cout<<All[i]->m_description<<"\t\t"<<All[i]->m_type<<"\t"<<
                All[i]->m_price<<"\t"<<All[i]->m_number<<"\t"<<All[i]->m_belong
                <<"\t"<<All[i]->discount<<endl;
            break;
        }
    }
    if(desLine<0){
        cout<<"Sorry,no such goods"<<endl;
        return;
    }
    else{
        int kind,cnumber;
        double cprice,cdiscount;
        cout<<"\nWhat you want to deal with the goods"<<endl;
        cout<<"1:change price\t2:change number\t3:change discount"<<endl;
        cin>>kind;  CHECKCIN
        switch (kind)
        {
        case 1:
            cout<<"Please input new price of "<<All[desLine]->m_description<<endl;
            cin>>cprice;    CHECKCIN
            All[desLine]->m_price=cprice;
            cout<<"Success to change price"<<endl;
            break;
        case 2:
            cout<<"Please input new number of "<<All[desLine]->m_description<<endl;
            cin>>cnumber;   CHECKCIN
            All[desLine]->m_number=cnumber;
            cout<<"Success to change number"<<endl;
            break;
        case 3:
            cout<<"Please input new discount of "<<All[desLine]->m_description<<endl;
            cin>>cdiscount; CHECKCIN
            All[desLine]->discount=cdiscount;
            cout<<"Success to change discount"<<endl;
            break;
        default:
            cout<<"You got a wrong choice"<<endl;
            break;
        }
    }
    this->setAll(All,gAllSize);
}
void Merchant::addGoods()
{
    short type;
    int num;
    double price,discount;
    string description;
    cout<<"\nAdd Goods:"<<endl;
    cout<<"Please input type:\n1:food\t2:clothes\t3:book"<<endl;
    cin>>type;  CHECKCIN
    cout<<"Please input description,price and number and discount"<<endl;
    cin>>description>>price>>num>>discount; CHECKCIN
    Goods* newGoods=NULL;
    switch (type)
    {
    case T_food:
        newGoods=new Food(type,description,price,num,this->m_account,discount);
        break;
    case T_clothes:
        newGoods=new Clothes(type,description,price,num,this->m_account,discount);
        break;
    case T_book:
        newGoods=new Book(type,description,price,num,this->m_account,discount);
        break;
    default:
        break;
    }
    ofstream gofs;
    gofs.open(GoodsFileName,ios::out|ios::app);
    gofs<<newGoods->m_description<<"\t"<<newGoods->m_type<<"\t"
        <<newGoods->m_price<<"\t"<<newGoods->m_number<<"\t"<<newGoods->m_belong<<"\t"<<newGoods->discount<<"\n";
    gofs.close();
    delete newGoods;
    cout<<"Success to add goods"<<endl;
}
void Merchant::activity(Goods** All,const int gAllSize)
{
    int type;
    double cdiscount;
    cout<<"Set type discount\nPlease input type:"<<endl;
    cin>>type;  CHECKCIN
    cout<<"Please input new discount"<<endl;
    cin>>cdiscount; CHECKCIN
    for(int i=0;i<gAllSize;i++){
        if(All[i]->m_belong==this->m_account&&All[i]->m_type==type){
            All[i]->discount=cdiscount;
        }
    }
    cout<<"Success to change all type "<<type<<"'s discount"<<endl;
    this->setAll(All,gAllSize);
}

int main (int argc,char* argv[])
{
    MENU f;
    f.getChoice();
    system("pause");
    return 0;
}
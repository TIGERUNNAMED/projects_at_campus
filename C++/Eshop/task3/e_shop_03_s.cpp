//vs code, MinGW-w64 g++
//windows 10
//class:2019211305 Name:RenChenxu ID:2019211279
//set txt_path(UserFileName,GoodsFileName) before compilation
#include<iostream>
#include<winsock2.h>
#include<string>
#include<fstream>
#include<vector>
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll
#define CLEARBUFF memset(sendBuffer,0,8192);memset(recvBuffer,0,8192);
#define UserFileName "user.txt"
#define GoodsFileName "goods.txt"
#define MAKEMESSAGE str.copy(sendBuffer,str.length(),0);str.clear();
#define SENDMESSAGE send(clntSock,sendBuffer,strlen(sendBuffer)+sizeof(char),0);
#define RECVMESSAGE recv(clntSock,recvBuffer,8192,0); 
#define CHECKCIN if(cin.good()==0){cout<<"Data type error \nThe progaram is terminated"<<endl;system("pause");exit(0);}

using namespace std;

const short T_consumer=1;
const short T_merchant=2;
const short T_food=1;
const short T_clothes=2;
const short T_book=3;


char recvBuffer[8192]={0};
char sendBuffer[8192]={0};

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
    bool logIn(const int userType,SOCKET clntSock);
    bool signUp(const int userType,SOCKET clntSock);
    void changePassword(User** All,const int accLine,SOCKET clntSock);
    void logOut(SOCKET clntSock);
    void dealBalance(User** All,const int accLine,SOCKET clntSock);//处理余额：查询余额，充值，消费
    void searchGoods(Goods** All,const int gAllSize,SOCKET clntSock);//搜索商品
    void showGoods(Goods** All,const int gAllSize,SOCKET clntSock);//展示商品信息
    const int getTxtSize(SOCKET clntSock);
    void getAll(User** All,int& accLine,const int allSize,SOCKET clntSock);//从文件读入所有账户信息,找到本账户所在行，返回总行数
    void setAll(User** All,const int allSize);//将信息写入文件
    const int getGoodSize(SOCKET clntSock);
    void getAll(Goods** All,const int gAllsize,SOCKET clntSock);
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
    void showFuncTable(SOCKET clntSock);//显示可用操作
    void shopping(User** All,const int allSize,Goods** gAll,const int gAllsize,SOCKET clntSock);
    void addToCart(vector<Goods*>& shoppingCart,Goods** gAll,const int gAllsize,SOCKET clntSock);
    void removeFromCart(vector<Goods*>& shoppingCart,SOCKET clntSock);
    void showCart(vector<Goods*>shoppingCart,SOCKET clntSock);
    void changeInCart(vector<Goods*>& shoppingCart,Goods** gAll,const int gAllsize,SOCKET clntSock);
    void generateOrder(vector<Goods*>& shoppingCart,Goods** gAll,const int gAllsize,User** All,const int Allsize,SOCKET clntSock);
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
    void showFuncTable(SOCKET clntSock);
    void addGoods(SOCKET clntSock);//添加商品
    void reviseGoodsInfo(Goods** All,const int gAllSize,SOCKET clntSock);//管理商品(改价，改余量)
    void activity(Goods** All,const int gAllSize,SOCKET clntSock);//活动打折
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
    void printMENU(SOCKET clntSock);
    void getChoice(SOCKET clntSock);
    void exitShop(SOCKET clntSock);
    void signUpGuide(SOCKET clntSock);
    void logInGuide(SOCKET clntSock);
    void showGoods(SOCKET clntSock);
};

void MENU::printMENU(SOCKET clntSock)
{
    // for(int i=1;i<64;i++) cout<<"*";
    // cout<<endl;
    // cout<<"\t\tWelcome to e_shop!"<<endl;
    // cout<<"\t\t0:Exit"<<endl;
    // cout<<"\t\t1:Log in"<<endl;
    // cout<<"\t\t2:Sign up"<<endl;
    // cout<<"\t\t3:Show all goods"<<endl;
    // for(int i=1;i<64;i++) cout<<"*";
    string str;
    for(int i=1;i<64;i++) str+="*";
    str+="\n";
    str+="\t\tWelcome to e_shop!\n";
    str+="\t\t0:Exit\n";
    str+="\t\t1:Log in\n";
    str+="\t\t2:Sign up\n";
    str+="\t\t3:Show all goods\n";
    for(int i=1;i<64;i++) str+="*";
    str+="\n";
    ifstream uifs,gifs;
    uifs.open(UserFileName,ios::in);
    gifs.open(GoodsFileName,ios::in);
    if(!(uifs.is_open()&&gifs.is_open())){
        //cout<<"No UserFile or GoodsFile, please first sign up as merchant and add  goods\n"<<endl;
        str+="No UserFile or GoodsFile, please first sign up as merchant and add  goods\n";       
    }
    uifs.close();
    gifs.close();
    str+="Choose one function.\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
}
void MENU::getChoice(SOCKET clntSock)
{
    int choice;
    while(1){
        this->printMENU(clntSock);
        //recv(clntSock,recvBuffer,MAXBYTE,0);
        RECVMESSAGE        
        //cin>>choice;
        choice = atoi(recvBuffer);  CLEARBUFF
        //字符转整型的函数    
        switch (choice)
        {
        case 0://退出
            this->exitShop(clntSock);
            break;
        case 1://登录
            this->logInGuide(clntSock);
            break;
        case 2://注册
             this->signUpGuide(clntSock);
            break;
        case 3://显示商品信息
            this->showGoods(clntSock);
        default:
            break;
        }
        if(choice==0) 
            break;
    }
}
void MENU::exitShop(SOCKET clntSock)
{
    //cout<<"See you next time!"<<endl;
    string str;
    str+="Log out\nSee you next time!\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    // system("pause");
    // exit(0);
}
void MENU::logInGuide(SOCKET clntSock)
{
    int userType;

    // cout<<"Log In :"<<endl;
    // cout<<"Please input your UserType(1:consumer 2:merchant)"<<endl;

    string str;
    str+="Log In :\n";
    str+="Please input your UserType(1:consumer 2:merchant)\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    //cin>>userType; 
    RECVMESSAGE 
    userType = atoi(recvBuffer);    CLEARBUFF
    if(userType==T_consumer){        
        Consumer* consumer=new Consumer(); 
        bool isLogIn=consumer->logIn(T_consumer,clntSock);
        if(isLogIn)
            consumer->showFuncTable(clntSock);
        delete consumer;
    }
    if(userType==T_merchant){
        Merchant* merchant=new Merchant();
        bool isLogIn=merchant->logIn(T_merchant,clntSock);
        if(isLogIn)
            merchant->showFuncTable(clntSock);
        delete merchant;
    }
    // cout<<"Turn to start menu"<<endl;
    // system("pause");
    // system("cls");

}
void MENU::signUpGuide(SOCKET clntSock)
{
    int userType;
    // cout<<"Sign Up :"<<endl;
    // cout<<"Please input your UserType:(1:consumer 2:merchant)"<<endl;
    string str;
    str+="Sign Up :\n";
    str+="Please input your UserType:(1:consumer 2:merchant)\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    //cin>>userType;
    RECVMESSAGE 
    userType = atoi(recvBuffer);    CLEARBUFF 
    if(userType==T_consumer){        //
        Consumer* consumer=new Consumer(); 
        bool isSignUp=consumer->signUp(T_consumer,clntSock);
        if(isSignUp)
            consumer->showFuncTable(clntSock);
        delete consumer;
    }
    if(userType==T_merchant){
        Merchant* merchant=new Merchant();
        bool isSignUp=merchant->signUp(T_merchant,clntSock);
        if(isSignUp)
            merchant->showFuncTable(clntSock);
        delete merchant;
    }
}
void MENU::showGoods(SOCKET clntSock)
{
    ifstream gifs;
    short type;
    string desc,belong;
    int num;
    double price,discount;
    gifs.open(GoodsFileName,ios::in);
    string str;
    if(!gifs.is_open()){
        //cout<<"Cann't find file"<<endl;
        str+="Cann't find file\n";
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
        // cout<<"\n\t\t\tShow goods"<<endl;
        // cout<<"\t\ttype:1:food,2:clothes,3:book"<<endl;
        // cout<<"Description\ttype\tprice\tnumber\tbelong\tdiscount\tprice_d"<<endl;
        str+="\n\t\t\tShow goods\n";
        str+="\t\ttype:1:food,2:clothes,3:book\n";
        str+="Description\ttype\tprice\tnumber\tbelong\tdiscount\tprice_d\n";
        for(int j=0;j<i;j++){
            gifs>>desc>>type>>price>>num>>belong>>discount;
            // cout<<desc<<"\t\t"<<type<<"\t"<<price<<"\t"<<num<<"\t"<<belong<<"\t"<<discount
            // <<"\t\t"<<discount * price<<endl;

            str=str+desc+"\t\t"+to_string(type)+"\t"+to_string(price)+"\t"
                +to_string(num)+"\t"+belong+"\t"+to_string(discount)+"\t"
                +to_string(discount * price)+"\n";
        }
    }
    // cout<<"\n";
    // cout<<"Turn to start menu"<<endl;
    str+="\nInput any to Turn to start menu\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
//     system("pause");
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



bool User::logIn(const int userType,SOCKET clntSock)
{
    string userName;
    int password;
    int tryTimes=0;
    this->m_type=userType;
    string str;
    if(userType==T_consumer)
        //cout<<"Log in as a consumer:"<<endl;
        str+="Log in as a consumer:\n";
    if(userType==T_merchant)
        //cout<<"Log in as a merchant:"<<endl;
        str+="Log in as a consumer:\n";
    //cout<<"Please input your username:"<<endl;
    str+="Please input your username:\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    //cin>>userName;
    RECVMESSAGE
    userName=recvBuffer;    CLEARBUFF
    //todo:在文件中查找用户名
    bool nameExist=false;//用户名是在文件的标志
    string fuserName;
    int fuserType,fpassword;
    double fbalance;//文件中存放的内容
    ifstream uifs;
    uifs.open(UserFileName,ios::in);
    if(!uifs.is_open()){
        //cout<<"Cann't find file"<<endl;
        str+="Cann't find file\n";
        uifs.close();
        str+="Input any to go on\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE CLEARBUFF
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
            //cout<<"Can't find this account"<<endl;
            str+="Can't find this account\n";
            str+="Input any to go on\n";
            MAKEMESSAGE SENDMESSAGE CLEARBUFF
            RECVMESSAGE CLEARBUFF
            return false;
        }
    }

    //cout<<"Please input your password"<<endl;
    str+="Please input your password\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF

    //cin>>password;
    RECVMESSAGE
    password = atoi(recvBuffer); 
    while(password!=fpassword&&tryTimes<3){
        tryTimes++;
        //cout<<"The password is wrong, please input the right one"<<endl;
        str+="The password is wrong, please input the right one\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        //cin>>password;
        RECVMESSAGE
        password = atoi(recvBuffer);  
    }
    if(tryTimes==3){
        //cout<<"Fail to log in!"<<endl;
        str+="Fail to log in!\nInput any to return\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE CLEARBUFF
        return false;
    }
    else{//将文件里该用户信息读入
        //cout<<"Success to log in!"<<endl;
        str+="Success to log in!\nInput any to go on";
        this->m_account=fuserName;
        this->m_password=fpassword;
        this->m_type=fuserType;
        this->m_balance=fbalance;
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE CLEARBUFF
        return true;
    }
}
bool User::signUp(const int userType,SOCKET clntSock)
{
    string str;
    string userName;
    int password;
    this->m_type=userType;
    if(userType==T_consumer)
        //cout<<"Sign up as a consumer:"<<endl;
        str+="Sign up as a consumer:\n";
    if(userType==T_merchant)
        //cout<<"Sign up as a merchant:"<<endl;
        str+="Sign up as a merchant:\n";
    //cout<<"Please input your username:"<<endl;
    str+="Please input your username:\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF

    //cin>>userName;
    RECVMESSAGE
    userName=recvBuffer;    CLEARBUFF
    //todo:遍历文件，查询用户名是否已经用过
    bool nameUsed=false;//用户名是否使用过的标志
    string fuserName;
    int fuserType,fpassword;
    double fbalance;//文件中存放的内容
    ifstream uifs;
    uifs.open(UserFileName,ios::in);
    if(!uifs.is_open()){
        //cout<<"Cann't find file"<<endl;
        str+="Cann't find file\n";
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
            //cout<<"This username is unavailable,turn to start menu and try again"<<endl;
            str+="This username is unavailable,Input any to turn to start menu and try again\n";
            MAKEMESSAGE SENDMESSAGE CLEARBUFF
            RECVMESSAGE CLEARBUFF
            return false;
        }
    } 
    this->m_account=userName;
    //cout<<"This username is available ,please set the password"<<endl;
    str+="This username is available ,please set the password\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF

    //cin>>password;
    RECVMESSAGE
    password=atoi(recvBuffer); CLEARBUFF   
    this->m_password=password;
    this->m_balance=0;//余额初始化为0
    //todo:将信息写入文件末尾
    ofstream uofs;
    uofs.open(UserFileName,ios::out|ios::app);
    uofs<<this->m_account<<"\t"<<this->m_type<<"\t"<<this->m_password<<"\t"<<this->m_balance<<endl;
    uofs.close();
    //cout<<"Success to create your account!\t"<<endl;
    str+="Success to create your account!\nInput any to return\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
    return true;
}
void User::logOut(SOCKET clntSock)
{
    string str;
    //cout<<"Log out!\t"<<endl;
    str+="log out!\nInput any to return\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
}
void User::dealBalance(User** All,const int accLine,SOCKET clntSock)
{
    string str;
    int op;
    double money;
    // cout<<"\nChoose what you want to do:"<<endl;
    // cout<<"1:Check the balance\t2:Recharge\t3:Consume"<<endl;
    str+="\nChoose what you want to do:\n";
    str+="1:Check the balance\t2:Recharge\t3:Consume\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    
    RECVMESSAGE
    //cin>>op;    CHECKCIN
    op= atoi(recvBuffer);   CLEARBUFF
    switch (op)
    {
    case 1:
        //cout<<"Your balance is:"<<this->m_balance<<endl;
        str+="Your balance is:"+to_string(this->m_balance)+"\n";
        break;
    case 2:
        //cout<<"Please input recharge amount"<<endl;
        str+="Please input recharge amount\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE 
        //cin>>money; CHECKCIN
        money=strtod(recvBuffer,NULL);  CLEARBUFF
        while(money<0){
            str+="Please input the right recharge amount\n";
            MAKEMESSAGE SENDMESSAGE CLEARBUFF
            RECVMESSAGE
            money=strtod(recvBuffer,NULL);  CLEARBUFF
        }
        this->m_balance+=money;
        All[accLine]->m_balance=this->m_balance;
        //cout<<"Success to recharge,your balance is now: "<<this->m_balance<<"\n"<<endl;
        str+="Success to recharge,your balance is now: "+to_string(this->m_balance)+"\n";
        break;
    case 3:
        //cout<<"Please input how much goods cost"<<endl;
        str+="Please input how much goods cost\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE 
        //cin>>money; CHECKCIN
        money=strtod(recvBuffer,NULL);  CLEARBUFF
        while(money<0){
            str+="Please input the right number\n";
            MAKEMESSAGE SENDMESSAGE CLEARBUFF
            RECVMESSAGE
            money=strtod(recvBuffer,NULL);  CLEARBUFF
        }
        if(money>this->m_balance){
            //cout<<"Sorry,you don't have enough money"<<"\n"<<endl;
            str+="Sorry,you don't have enough money\n\n";
            break;
        }
        else{
            this->m_balance-=money;
            All[accLine]->m_balance=this->m_balance;
            //cout<<"Success to consume,your balance is now: "<<this->m_balance<<endl;
            str+="Success to consume,your balance is now: "+to_string(this->m_balance)+"\n";
            break;
        }
        
    default:
        //cout<<"Error!"<<endl;
        str+="Error!\n";
        break;
    }
    str+="Input any to continue\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
}
void User::changePassword(User** All,const int accLine,SOCKET clntSock)
{
    string str;
    int password,tryTimes=0;
    //cout<<"Your username is:"<<this->m_account<<endl;
    //cout<<"Please input your old password:"<<endl;
    str+="Your username is:"+this->m_account+"\nPlease input your old password:\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE
    //cin>>password;  CHECKCIN
    password=atoi(recvBuffer);  CLEARBUFF
    while(password!=this->m_password&&tryTimes<3){
        //cout<<"Not the right password"<<endl;
        str+="Not the right password\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE
        //cin>>password;  CHECKCIN
        password=atoi(recvBuffer);  CLEARBUFF
        tryTimes++;
    }
    if(tryTimes==3){
        //cout<<"Fail to change password!"<<endl;
        str+="Fail to change password!\n";
    }
    else{
        //cout<<"Please input your new password:"<<endl;
        str+="Please input your new password:\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE
        //cin>>password;  CHECKCIN
        password=atoi(recvBuffer);  CLEARBUFF
        this->m_password=password;
        All[accLine]->m_password=password;
        //cout<<"Success to change password,your new password is: "<<All[accLine]->m_password<<endl;
        str+="Success to change password,your new password is: "+to_string(All[accLine]->m_password)+"\n";
        //system("pause");
    }
    str+="Input any to go on\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
}
void User::searchGoods(Goods** All,const int gAllSize,SOCKET clntSock)
{
    string str;
    string goodsName,desc,belong;
    int desLine=-1;
    int type,num;
    double price,discount;
    //cout<<"Please input the goods you want to find"<<endl;
    str+="Please input the goods you want to find\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE
    //cin>>goodsName;
    goodsName=recvBuffer;   CLEARBUFF
    for(int i=0;i<gAllSize;i++){
        desc=All[i]->m_description;
        type=All[i]->m_type;
        price=All[i]->m_price;
        num=All[i]->m_number;
        belong=All[i]->m_belong;
        discount=All[i]->discount;
        if(desc==goodsName){
            //cout<<"\nFind it!"<<endl;
            //cout<<"\ttype:1:food,2:clothes,3:book\n"<<endl;
            //cout<<"Description\ttype\tprice\tnumber\tbelong\tdiscount\tprice_d"<<endl;
            str+="\nFind it!\n\ttype:1:food,2:clothes,3:book\n\n";
            str+="Description\ttype\tprice\tnumber\tbelong\tdiscount\tprice_d\n";
            desLine=i;
            // cout<<All[i]->m_description<<"\t\t"<<All[i]->m_type<<"\t"<<
            //     All[i]->m_price<<"\t"<<All[i]->m_number<<"\t"<<All[i]->m_belong
            //     <<"\t"<<All[i]->discount<<"\t\t"<<All[i]->getPrice()<<endl;
            str+=All[i]->m_description+"\t\t"+to_string(All[i]->m_type)+"\t";
            str+=to_string(All[i]->m_price)+"\t"+to_string(All[i]->m_number)+"\t"+All[i]->m_belong;
            str+="\t"+to_string(All[i]->discount)+"\t\t"+to_string(All[i]->getPrice())+"\n";
            break;
        }
    }
    if(desLine<0){
        //cout<<"Sorry,no such goods"<<endl;
        str+="Sorry,no such goods\n";    
    }
    str+="Input any to go on\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
    return;
}
void User::showGoods(Goods** All,const int gAllSize,SOCKET clntSock)
{
    string str;
    //cout<<"\n\t\t\tShow goods"<<endl;
    //cout<<"\t\ttype:1:food,2:clothes,3:book"<<endl;
    str+="\n\t\t\tShow goods\n\t\ttype:1:food,2:clothes,3:book\n";
    //cout<<"Description\ttype\tprice\tnumber\tbelong\tdiscount\tprice_d"<<endl;
    str+="Description\ttype\tprice\tnumber\tbelong\tdiscount\tprice_d\n";
    for(int i=0;i<gAllSize;i++){
        // cout<<All[i]->m_description<<"\t\t"<<All[i]->m_type<<"\t"<<
        //     All[i]->m_price<<"\t"<<All[i]->m_number<<"\t"<<All[i]->m_belong
        //     <<"\t"<<All[i]->discount<<"\t\t"<<All[i]->getPrice()<<endl;
        str+=All[i]->m_description+"\t\t"+to_string(All[i]->m_type)+"\t";
            str+=to_string(All[i]->m_price)+"\t"+to_string(All[i]->m_number)+"\t"+All[i]->m_belong;
            str+="\t"+to_string(All[i]->discount)+"\t\t"+to_string(All[i]->getPrice())+"\n";
    }
    //cout<<"\n";
    str+="\nInput any to go on\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF

}
const int User:: getTxtSize(SOCKET clntSock)
{
    string str;
    ifstream uifs;
    int lineNum=0;
    uifs.open(UserFileName,ios::in);
    if(!uifs.is_open()){
        //cout<<"Cann't find file"<<endl;
        str+="Cann't find file\n";
        str+="Input any to go on\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE CLEARBUFF
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
void User:: getAll(User** All,int& acc,const int allSize,SOCKET clntSock)
{
    string str;
    ifstream uifs;
    int lineNum=0;
    string account;
    int type,password;
    double balance;
    uifs.open(UserFileName,ios::in);
    if(!uifs.is_open()){
        //cout<<"Cann't find file"<<endl;
        str+="Cann't find file\n";
        str+="Input any to go on\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE CLEARBUFF
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
const int User::getGoodSize(SOCKET clntSock)
{
    string str;
    ifstream gifs;
    int lineNum=0;
    gifs.open(GoodsFileName,ios::in);
    if(!gifs.is_open()){
        //cout<<"Cann't find file"<<endl;
        str+="Cann't find file\n";
        str+="Input any to go on\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE CLEARBUFF
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
void User::getAll(Goods** All,const int gAllsize,SOCKET clntSock)
{
    string str;
    ifstream gifs;
    int lineNum=0;
    short type;
    string desc,belong;
    int num;
    double price,discount;
    gifs.open(GoodsFileName,ios::in);
    if(!gifs.is_open()){
        //cout<<"Cann't find file"<<endl;
        str+="Cann't find file\n";
        str+="Input any to go on\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE CLEARBUFF
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


void Consumer::showFuncTable(SOCKET clntSock)
{
    string str;
    int choice;
    // cout<<"Choose what you want to do:"<<endl;
    // cout<<"1:change password\t2:manage balance\t3:show goods"<<endl;
    // cout<<"4:search goods\t5:shopping\t0:log out"<<endl;
    str+="Choose what you want to do:\n";
    str+="1:change password\t2:manage balance\t3:show goods\n";
    str+="4:search goods\t5:shopping\t\t0:log out\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE
    //cin>>choice;    CHECKCIN
    choice=atoi(recvBuffer);     CLEARBUFF
    while(choice){
        const int allSize=this->getTxtSize(clntSock);
        const int gAllSize=this->getGoodSize(clntSock);
        if(allSize<=0||gAllSize<=0){
            //cout<<"Error:cannot find User or Goods txt,please set user and goods in advance"<<endl;
            str+="Error:cannot find User or Goods txt,please set user and goods in advance\n";
            break;
        }
        User** all=new User*[allSize];
        Goods** gAll=new Goods*[gAllSize];
        int acc=0;
        this->getAll(all,acc,allSize,clntSock);
        this->getAll(gAll,gAllSize,clntSock);
        switch (choice)
        {
        case 1:
            this->changePassword(all,acc,clntSock);
            break;
        case 2:
            this->dealBalance(all,acc,clntSock);
            break;
        case 3:
            this->showGoods(gAll,gAllSize,clntSock);
            break;
        case 4:
            this->searchGoods(gAll,gAllSize,clntSock);
            break;
        case 5:
            this->shopping(all,allSize,gAll,gAllSize,clntSock);
            break;
        default:
            //cout<<"You got a wrong choice!\tInput a right one!"<<endl;
            str+="You got a wrong choice!\tInput a right one!\n";
            break;
        }
        this->setAll(all,allSize);
        delete []all;
        delete []gAll;
        // cout<<"Choose what you want to do else:"<<endl;
        // cout<<"1:change password\t2:manage balance\t3:show goods"<<endl;
        // cout<<"4:search goods\t5:shopping\t";
        // cout<<"0:log out"<<endl;
        str+="Choose what you want to do else:\n";
        str+="1:change password\t2:manage balance\t3:show goods\n";
        str+="4:search goods\t5:shopping\t\t0:log out\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE
        //cin>>choice;    CHECKCIN
        choice=atoi(recvBuffer); CLEARBUFF
    }
    if(choice==0){
        this->logOut(clntSock);
    }

    str+="Input any to go on\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
    return;
}
void Consumer::shopping(User** All,const int allSize,Goods** gAll,const int gAllsize,SOCKET clntSock)
{
    string str;
    //创建一个购物车
    //购物车是指向抽象类的指针向量，每个元素为具体的指向派生类的指针，
    //通过用户输入确定商品名和数量,price 仍为单价，
    //通过this指针来确定消费者账户，Goods自身的belong属性来确定消费者账户
    //添加商品时先在Goods** gAll中找到该商品，根据type，new相应的商品类，拷贝初始化，填入相应的type,name,price,belong,根据用户输入填入num,
    //价格计算：单价*数量*折扣
    int choice;
    vector<Goods*> shoppingCart;


/*******************/
    // Goods* test=NULL;
    // test=new Book(T_book,"rice",2,30,"rita",1);
    // shoppingCart.push_back(test);
    // cout<<"Here are your shoping Cart:"<<endl;
    // cout<<"type:"<<shoppingCart[0]->m_type<<"\tdesc:"<<shoppingCart[0]->m_description
    //     <<"\tprice:"<<shoppingCart[0]->m_price<<"\tnum:"<<shoppingCart[0]->m_number
    //     <<"\tdicount:"<<shoppingCart[0]->discount<<endl;
/***************/

    // cout<<"Shopping"<<endl;
    // cout<<"choose what you want to do"<<endl;
    // cout<<"1:Add goods to shoppingCart\t\t2:Remove goods from shoppingCart"<<endl;
    // cout<<"3:Change num of goods in shoppingCart\t\t4:Show all goods in e-shop"<<endl;
    // cout<<"5:Search goods in e-shop\t\t6:Generate order"<<endl;
    // cout<<"0:Drop out and return to Functable"<<endl;
    str+="Shopping\nchoose what you want to do\n";
    str+="1:Add goods to shoppingCart\t\t2:Remove goods from shoppingCart\n";
    str+="3:Change num of goods in shoppingCart\t\t4:Show all goods in e-shop\n";
    str+="5:Search goods in e-shop\t\t6:Generate order\n0:Drop out and return to Functable\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF

    RECVMESSAGE
    //cin>>choice;    CHECKCIN
    choice=atoi(recvBuffer);    CLEARBUFF
    while(choice){
        switch (choice)
        {
        case 1 :
            //Add goods to shoppingCart
            this->addToCart(shoppingCart,gAll,gAllsize,clntSock);
            break;
        case 2 :
            this->removeFromCart(shoppingCart,clntSock);
            //2:Remove goods from shoppingCart
            break;
        case 3 :
            //cout<<"type"<<shoppingCart[0]->m_type<<endl;
            this->changeInCart(shoppingCart,gAll,gAllsize,clntSock);
            //3:Change num of goods in shoppingCart
            break;
        case 4 :
            this->showGoods(gAll,gAllsize,clntSock);
            break; 
        case 5 :
            this->searchGoods(gAll,gAllsize,clntSock);
            break;
        case 6 :
            this->generateOrder(shoppingCart,gAll,gAllsize,All,allSize,clntSock);
            break;          
        default:
            //cout<<"You got a wrong choice,choose the right one!"<<endl;
            str+="You got a wrong choice,choose the right one!\n";
            break;
        }
        // cout<<"choose what you want to do else"<<endl;
        // cout<<"1:Add goods to shoppingCart\t\t2:Remove goods to shoppingCart"<<endl;
        // cout<<"3:Change num of goods in shoppingCart\t\t4:Show all goods in e-shop"<<endl;
        // cout<<"5:Search goods in e-shop\t\t6:Generate order"<<endl;
        // cout<<"0:Drop out and return to Functable"<<endl;
        str+="choose what you want to do else\n";
        str+="1:Add goods to shoppingCart\t\t2:Remove goods from shoppingCart\n";
        str+="3:Change num of goods in shoppingCart\t\t4:Show all goods in e-shop\n";
        str+="5:Search goods in e-shop\t\t6:Generate order\n0:Drop out and return to Functable\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE
        //cin>>choice;    CHECKCIN
        choice=atoi(recvBuffer);    CLEARBUFF
    }
    if(choice==0){
        // cout<<"Dear "<<this->m_account<<", you have "<<this->m_balance<<"RMB left"<<endl;
        // cout<<"Finish shopping!"<<endl;
        str+="Dear "+this->m_account+", you have "+to_string(this->m_balance)+"RMB left\n";
        str+="Finish shopping!\n";
    }
    str+="Input any to go on\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
    //system("pause");
    return ;
}
void Consumer::addToCart(vector<Goods*>& shoppingCart,Goods** gAll,const int gAllsize,SOCKET clntSock)
{
    string str;
    string name;
    int gline=-1;
    int bnum=0;
    //cout<<"please input goods name:"<<endl;
    str+="please input goods name:\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE
    //cin>>name;
    name=recvBuffer;   CLEARBUFF 
    gline=this->getGloca(gAll,gAllsize,name);
    if(gline>=0){
        // cout<<"Find it!"<<endl;
        // cout<<"please input number you want to buy"<<endl;
        str+="Find it!\nplease input number you want to buy\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE
        //cin>>bnum;  CHECKCIN
        bnum=atoi(recvBuffer);  CLEARBUFF
        while(bnum<0){
            str+="please input correct number\n";
            MAKEMESSAGE SENDMESSAGE CLEARBUFF
            RECVMESSAGE
            bnum=atof(recvBuffer); CLEARBUFF
        } 
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
            //cout<<"Success to add goods to your shoppingCart"<<endl;
            str+="Success to add goods to your shoppingCart\n";
        }
        else{
            //cout<<"Error:more than numbers on sale!";
            str+="Error:more than numbers on sale!\n";
        }
    }
    else{
        //cout<<"No such goods"<<endl;
        str+="No such goods\n";
    }
    //cout<<"\nHere are goods in your shoppingCart:"<<endl;
    str+="\nHere are goods in your shoppingCart:\n";
    str+="Input any to go on\n\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
    this->showCart(shoppingCart,clntSock);
}
void Consumer::removeFromCart(vector<Goods*>& shoppingCart,SOCKET clntSock)
{
    string str;
    int remove;
    // cout<<"Here are goods in your shoppingCart:"<<endl;
    // cout<<"shoppingCart.size:"<<shoppingCart.size()<<endl;
    str+="shoppingCart.size:"+to_string(shoppingCart.size())+"\n";
    str+="Here are goods in your shoppingCart:\n";
    str+="Input any to go on\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
    this->showCart(shoppingCart,clntSock);
    //cout<<"Choose which you want to remove(please input the number before description)"<<endl;
    str+="Choose which you want to remove(please input the number before description)\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE 
    //cin>>remove;    CHECKCIN
    remove=atoi(recvBuffer);    CLEARBUFF
    remove--;
    int i=0;
    if(remove<0||remove>=shoppingCart.size()){
        //cout<<"Wrong number!"<<endl;
        str+="Wrong number!\n";
        str+="Input any to go on\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE CLEARBUFF
        return;
    }
    for(auto it=shoppingCart.begin();it!=shoppingCart.end();it++){
        if(remove==i){
            it=shoppingCart.erase(it);
            if(it==shoppingCart.end())  
                break;
            //cout<<"Success to remove it\n"<<endl;
            str+="Success to remove it\n\n";

        }
        i++;
    }
    //cout<<"Here are goods in your shoppingCart:"<<endl;
    str+="Here are goods in your shoppingCart:\n";
    str+="Input any to go on\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
    this->showCart(shoppingCart,clntSock);
    //cout<<"\n";
}
void Consumer::changeInCart(vector<Goods*>& shoppingCart,Goods** gAll,const int gAllsize,SOCKET clntSock)
{
    string str;
    int i,cnum,cprice;
    int gline=-1;
    //cout<<"Here are goods in your shoppingCart:"<<endl;
    str+="Here are goods in your shoppingCart:\n";
    str+="Input any to go on\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
    this->showCart(shoppingCart,clntSock);
    //cout<<"Choose which you want to change the number(please input the number before description)"<<endl;
    str+="Choose which you want to change the number(please input the number before description)\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE
    //cin>>i; CHECKCIN
    i=atoi(recvBuffer); CLEARBUFF
    i--;
    if(i<0||i>=shoppingCart.size()){
        //cout<<"Error:no such goods"<<endl;
        str+="Error:no such goods\n";
        str+="Input any to go on\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE CLEARBUFF
        return ;
    }
    //cout<<"Please input the new number"<<endl;
    str+="Please input the new number\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE 
    //cin>>cnum;  CHECKCIN
    cnum=atoi(recvBuffer);  CLEARBUFF
    //cout<<shoppingCart[i]->m_description<<endl;
    str+=shoppingCart[i]->m_description+"\n";
    gline=this->getGloca(gAll,gAllsize,shoppingCart[i]->m_description);
    //cout<<"gline:"<<gline<<endl;
    
    if((gline>=0)&&(gAll[i]->m_number>=cnum)){
        shoppingCart[i]->m_number=cnum;//更新数量
        // cout<<"type:"<<shoppingCart[i]->m_type<<endl;
        // cout<<"Success to change number"<<endl;
        // cout<<"Here are goods in your shoppingCart:"<<endl;
        str+="type: "+to_string(shoppingCart[i]->m_type)+"\n";
        str+="Success to change number\n";
        str+="Here are goods in your shoppingCart:\n";
        str+="Input any to go on\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE CLEARBUFF
        this->showCart(shoppingCart,clntSock);
    }
    else{
        //cout<<"Error:more than numbers on sale or no such goods"<<endl;
        str+="Error:more than numbers on sale or no such goods\n";
        str+="Input any to go on\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE CLEARBUFF
    }
    
}
void Consumer::showCart(vector<Goods*> shoppingCart,SOCKET clntSock)
{
    string str;
    const int size=shoppingCart.size();
    for(int i=0;i<size;i++){
        // cout<<i+1<<"\tdescription:"<<shoppingCart[i]->m_description<<"\ttype:"<<shoppingCart[i]->m_type
        //     <<"\tprice:"<<shoppingCart[i]->m_price<<"\tbelong:"<<shoppingCart[i]->m_belong
        //     <<"\tnumber:"<<shoppingCart[i]->m_number<<"\tdiscount:"<<shoppingCart[i]->discount<<endl;
        str+=to_string(i+1)+"\tdescription:"+shoppingCart[i]->m_description+"\ttype:"+to_string(shoppingCart[i]->m_type)
            +"\tprice:"+to_string(shoppingCart[i]->m_price)+"\tbelong:"+shoppingCart[i]->m_belong
            +"\tnumber:"+to_string(shoppingCart[i]->m_number)+"\tdiscount:"+to_string(shoppingCart[i]->discount)+"\n";
    }
    str+="Input any to go on\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
}
void Consumer::generateOrder(vector<Goods*> &shoppingCart,Goods** gAll,const int gAllsize,User** All,const int Allsize,SOCKET clntSock)
{
    string str;
    //判断是否能成功生成订单
    //只有Goods** 中所有商品的数量均大于等于0，用户的余额够支付，才能生成订单
    double totalPrice=0;
    string gname;
    int size=shoppingCart.size();
    for(int i=0;i<size;i++){
        totalPrice+=shoppingCart[i]->getPrice()*shoppingCart[i]->m_number;
    }
    //cout<<"Total price is:"<<totalPrice<<endl;
    str+="Total price is:"+to_string(totalPrice)+"\n";
    if(totalPrice>this->m_balance){
        // cout<<"You have "<<this->m_balance<<"RMB"<<endl;       
        // cout<<"Fail to generate order: short of money"<<endl;
        str+="You have "+to_string(this->m_balance)+"RMB\n";
        str+="Fail to generate order: short of money\n";
        str+="Input any to go on\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE CLEARBUFF
        return;
    }
    //冻结商品
    //flag 超量标记
    //在内存中冻结：
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
        //cout<<"Fail to generate order: The number is over that on sale"<<endl;
        str+="Fail to generate order: The number is over that on sale\n";
        for(int i=0;i<size;i++){
            int gline;
            gname=shoppingCart[i]->m_description;
            gline=this->getGloca(gAll,gAllsize,gname);
            gAll[gline]->m_number+=shoppingCart[i]->m_number;
        }
        str+="Input any to go on\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE CLEARBUFF
        return ;
    }
    //在文件中冻结
    this->setAll(gAll,gAllsize);

    //订单支付：只有用户选择不买时失败
    int ifPay=0;
    //cout<<"Do you want to pay for these goods:(1:yes\tothernumber:no,and goback)"<<endl;
    str+="Do you want to pay for these goods:(1:yes\tothernumber:no,and goback)\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE
    //cin>>ifPay; CHECKCIN
    ifPay=atoi(recvBuffer); CLEARBUFF
    if(ifPay!=1){
        //cout<<"Fail to pay:Consumer choose to leave"<<endl;
        str+="Fail to pay\n";
        //还原goods内存中数量
        for(int i=0;i<size;i++){
            int gline;
            gname=shoppingCart[i]->m_description;
            gline=this->getGloca(gAll,gAllsize,gname);
            gAll[gline]->m_number+=shoppingCart[i]->m_number;
        }
    }
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
            //清空购物车
            this->clearCart(shoppingCart);
        } 
        
        str+="Success to pay.\n";
    }
    //支付失败，还原内存 或支付成功后或写回文件
    this->setAll(All,Allsize);
    this->setAll(gAll,gAllsize);
    str+="Input any to go on\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
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

void Merchant::showFuncTable(SOCKET clntSock)
{
    string str;
    int choice;
    // cout<<"Choose what you want to do:"<<endl;
    // cout<<"1:change password\t2:manage balance\t3:show goods"<<endl;
    // cout<<"4:search goods\t5:add goods\t6:revise goods' info\t7:activity discount"<<endl;
    // cout<<"0:log out"<<endl;

    str+="Choose what you want to do:\n";
    str+="1:change password\t2:manage balance\t3:show goods\n";
    str+="4:search goods\t5:add goods\t6:revise goods' info\t7:activity discount\n";
    str+="0:log out\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE 
    //cin>>choice;    CHECKCIN
    choice=atoi(recvBuffer);  CLEARBUFF
    while(choice){
        if(choice==5)
            this->addGoods(clntSock);//这里将add单独分出来是因为在没有txt文档时add功能也必须可用（创建该文档）,且add是不依赖原有txt文件内容
        const int allSize=this->getTxtSize(clntSock);
        const int gAllSize=this->getGoodSize(clntSock);
        if(allSize<=0||gAllSize<=0){
            //cout<<"Error:cannot find User or Goods txt,please set user and goods in advance"<<endl;
            str+="Error:cannot find User or Goods txt,please set user and goods in advance\n";
            break;
        }
        User** all=new User*[allSize];
        Goods** gAll=new Goods*[gAllSize];
        int acc=0;
        this->getAll(all,acc,allSize,clntSock);
        this->getAll(gAll,gAllSize,clntSock);
        switch (choice)
        {
        case 1:
            this->changePassword(all,acc,clntSock);
            break;
        case 2:
            this->dealBalance(all,acc,clntSock);
            break;
        case 3:
            this->showGoods(gAll,gAllSize,clntSock);
            break;
        case 4:
            this->searchGoods(gAll,gAllSize,clntSock);
            break;
        case 5:
            break;
        case 6:
            this->reviseGoodsInfo(gAll,gAllSize,clntSock);
            break;
        case 7:
            this->activity(gAll,gAllSize,clntSock);
            break;
        default:
            //cout<<"You got a wrong choice!\tInput a right one!"<<endl;
            str+="You got a wrong choice!\tInput a right one!\n";
            break;
        }
        this->setAll(all,allSize);
        delete []all;
        delete []gAll;
        // cout<<"Choose what you want to do else:"<<endl;
        // cout<<"1:change password\t2:manage balance\t3:show goods"<<endl;
        // cout<<"4:search goods\t5:add goods\t6:revise goods' info\t7:activity discount"<<endl;
        // cout<<"0:log out"<<endl;
        str+="Choose what you want to do else:\n";
        str+="1:change password\t2:manage balance\t3:show goods\n";
        str+="4:search goods\t5:add goods\t6:revise goods' info\t7:activity discount\n";
        str+="0:log out\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE
        //cin>>choice;    CHECKCIN
        choice=atoi(recvBuffer);  CLEARBUFF
    }
    if(choice==0){
        this->logOut(clntSock);
    }
    str+="Input any to go on\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
    return;
}
void Merchant::reviseGoodsInfo(Goods** All,const int gAllSize,SOCKET clntSock)
{
    string str;
    string goodsName,desc,belong;
    int desLine=-1;
    int type,num;
    double price;
    //cout<<"Please input the goods you want to change"<<endl;
    str+="Please input the goods you want to change\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE
    //cin>>goodsName;
    goodsName=recvBuffer;   CLEARBUFF
    for(int i=0;i<gAllSize;i++){
        desc=All[i]->m_description;
        type=All[i]->m_type;
        price=All[i]->m_price;
        num=All[i]->m_number;
        belong=All[i]->m_belong;
        if(desc==goodsName&&belong!=this->m_account)
            //cout<<"This is not your goods"<<endl;
            str+="This is not your goods\n";
        if(desc==goodsName&&belong==this->m_account){
            desLine=i;
            // cout<<"\nFind it!"<<endl;
            // cout<<"\ttype:1:food,2:clothes,3:book\n"<<endl;
            // cout<<"Description\ttype\tprice\tnumber\tbelong\tdiscount"<<endl;
            str+="\nFind it!\n\ttype:1:food,2:clothes,3:book\n\n";
            str+="Description\ttype\tprice\tnumber\tbelong\tdiscount\n";
            // cout<<All[i]->m_description<<"\t\t"<<All[i]->m_type<<"\t"<<
            //     All[i]->m_price<<"\t"<<All[i]->m_number<<"\t"<<All[i]->m_belong
            //     <<"\t"<<All[i]->discount<<endl;
            str+=All[i]->m_description+"\t\t"+to_string(All[i]->m_type)+"\t"+
                to_string(All[i]->m_price)+"\t"+to_string(All[i]->m_number)+"\t"+All[i]->m_belong
                +"\t"+to_string(All[i]->discount)+"\n";
            break;
        }
    }
    if(desLine<0){
        //cout<<"Sorry,no such goods"<<endl;
        str+="Sorry,no such goods\n";
        str+="Input any to go on\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE CLEARBUFF
        return;
    }
    else{
        int kind,cnumber;
        double cprice,cdiscount;
        // cout<<"\nWhat you want to deal with the goods"<<endl;
        // cout<<"1:change price\t2:change number\t3:change discount"<<endl;
        str+="\nWhat you want to deal with the goods\n";
        str+="1:change price\t2:change number\t3:change discount\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE
        //cin>>kind;  CHECKCIN
        kind=atoi(recvBuffer);  CLEARBUFF
        switch (kind)
        {
        case 1:
            //cout<<"Please input new price of "<<All[desLine]->m_description<<endl;
            str+="Please input new price of "+All[desLine]->m_description+"\n";
            MAKEMESSAGE SENDMESSAGE CLEARBUFF
            RECVMESSAGE
            //cin>>cprice;    CHECKCIN
            cprice=atof(recvBuffer);    CLEARBUFF
            All[desLine]->m_price=cprice;
            //cout<<"Success to change price"<<endl;
            str+="Success to change price\n";
            break;
        case 2:
            //cout<<"Please input new number of "<<All[desLine]->m_description<<endl;
            str+="Please input new number of "+All[desLine]->m_description+"\n";
            MAKEMESSAGE SENDMESSAGE CLEARBUFF
            RECVMESSAGE 
            //cin>>cnumber;   CHECKCIN
            cnumber=atoi(recvBuffer);   CLEARBUFF
            All[desLine]->m_number=cnumber;
            //cout<<"Success to change number"<<endl;
            str+="Success to change number\n";
            break;
        case 3:
            //cout<<"Please input new discount of "<<All[desLine]->m_description<<endl;
            str+="Please input new discount of "+All[desLine]->m_description+"\n";
            MAKEMESSAGE SENDMESSAGE CLEARBUFF
            RECVMESSAGE
            //cin>>cdiscount; CHECKCIN
            cdiscount=atof(recvBuffer); CLEARBUFF
            while(cdiscount<0){
                str+="please input correct price\n";
                MAKEMESSAGE SENDMESSAGE CLEARBUFF
                RECVMESSAGE
                cdiscount=atof(recvBuffer); CLEARBUFF
            } 
            All[desLine]->discount=cdiscount;
            //cout<<"Success to change discount"<<endl;
            str+="Success to change discount\n";
            break;
        default:
            //cout<<"You got a wrong choice"<<endl;
            str+="You got a wrong choice\n";
            break;
        }
    }
    this->setAll(All,gAllSize);
    str+="Input any to go on\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
}
void Merchant::addGoods(SOCKET clntSock)
{
    string str;
    short type;
    int num;
    double price,discount;
    string description;
    //cout<<"\nAdd Goods:"<<endl;
    //cout<<"Please input type:\n1:food\t2:clothes\t3:book"<<endl;
    str+="\nAdd Goods:\nPlease input type:\n1:food\t2:clothes\t3:book\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE
    //cin>>type;  CHECKCIN
    type=atoi(recvBuffer);  CLEARBUFF
    //cout<<"Please input description,price and number and discount"<<endl;
    str+="Please input description,price and number and discount\n";
    str+="description:\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE
    //cin>>description;
    description=recvBuffer; CLEARBUFF 
    str+="price:\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE
    //cin>>price;
    price=atof(recvBuffer); CLEARBUFF
    while(price<0){
        str+="please input correct price\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE
        price=atof(recvBuffer); CLEARBUFF
    }
    str+="number:\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE
    //cin>>num;
    num=atoi(recvBuffer);   CLEARBUFF
    while(num<0){
        str+="please input correct number\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE
        num=atoi(recvBuffer); CLEARBUFF
    }
    str+="discount:\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE
    //cin>>discount;
    discount=atof(recvBuffer); CLEARBUFF
    while(discount<0){
        str+="please input correct number\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE
        discount=atof(recvBuffer); CLEARBUFF
    } 
    //description>>price>>num>>discount; CHECKCIN
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
    //cout<<"Success to add goods"<<endl;
    str+="Success to add goods\n";
    str+="Input any to go on\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF

}
void Merchant::activity(Goods** All,const int gAllSize,SOCKET clntSock)
{
    string str;
    int type;
    double cdiscount;
    //cout<<"Set type discount\nPlease input type:"<<endl;
    str+="Set type discount\nPlease input type:\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE
    //cin>>type;  CHECKCIN
    type=atoi(recvBuffer);  CLEARBUFF
    //cout<<"Please input new discount"<<endl;
    str+="Please input new discount\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE
    //cin>>cdiscount; CHECKCIN
    cdiscount=atof(recvBuffer); CLEARBUFF
    while(cdiscount<0){
        str+="please input correct number\n";
        MAKEMESSAGE SENDMESSAGE CLEARBUFF
        RECVMESSAGE
        cdiscount=atof(recvBuffer); CLEARBUFF
    } 
    for(int i=0;i<gAllSize;i++){
        if(All[i]->m_belong==this->m_account&&All[i]->m_type==type){
            All[i]->discount=cdiscount;
        }
    }
    //cout<<"Success to change all type "<<type<<"'s discount"<<endl;
    str+="Success to change all type "+to_string(type)+"'s discount\n";
    this->setAll(All,gAllSize);
    str+="Input any to go on\n";
    MAKEMESSAGE SENDMESSAGE CLEARBUFF
    RECVMESSAGE CLEARBUFF
}


int main(){
    //初始化 DLL
    WSADATA wsaData;
    WSAStartup( MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //绑定套接字
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;  //使用IPv4地址
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    sockAddr.sin_port = htons(1234);  //端口

    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    //进入监听状态
    listen(servSock, 20);
    //接收客户端请求
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    cout<<"start working!\n\n"<<endl;
   
    /**********************************************************************************/
    while(1){
         
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
        cout<<"working!\n\n"<<endl;
        MENU f;
        f.getChoice(clntSock);
        cout<<"waiting.......\n"<<endl;
        closesocket(clntSock);
        
    }
    /*********************************************************************************/  
    closesocket(servSock);
    system("pause");
    //终止 DLL 的使用
    WSACleanup();
    return 0;
}

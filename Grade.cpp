#include <iostream>
#include <fstream>  //文件
#include <iomanip>  //setw()
#include <sstream>  //文件
#include <Windows.h> //system()
#include <algorithm>   //transform()
#include <cctype>     //tolower()
using namespace std;

//全局变量
struct student* stuhead = NULL;

//声明
void display();  //展示
int CountLines(ifstream& file);//文件行数统计
void  menu();//主菜单
void addstudent(string num, string name, double sc1, double sc2,double sc);//添加学生数据//链表
void add();//添加学生
void origrinstu();//录入原有学生
void avg();//计算成绩平均值
void proportion1();//及格和不及格占比
void proportion2(); //优良中占比
void edit();    //编辑学生信息
int save();    //保存到文件
bool isExist(string snum); //学号唯一性判断
void Delete(string n);  //删除功能
void search();  //搜索
void sort();    //排序

//主函数
int main()
{
    origrinstu();
    menu();
}

//类
struct student
{
public:
    string snum;
    string sname;
    double score1 =0;
    double score2 =0;
    double score = 0;
    struct student* stunext = NULL;
};

//函数
//主菜单
void menu()
{
    int a;
    while (1)
    {
      
        cout <<endl<< "----------------------------------------";
        cout <<endl<< "学生成绩管理系统" << endl;
        cout << "1.添加新学生成绩" << endl;
        cout << "2.学生成绩统计" << endl;
        cout << "3.学生成绩查看" << endl;
        cout << "4.编辑学生信息" << endl;
        cout << "5.删除学生信息" << endl;
        cout << "6.查找学生信息" << endl;
        cout << "0.退出系统" << endl;
        cout << "----------------------------------------" << endl;
        cout << "请输入选项："; 
        cin >> a;
        system("cls");
        if (a == 1)
        {
            add();
        }
        else if (a == 2)
        {
            int i = 0;
            // 修改子菜单结构
            do {  // 改为循环结构
                cout <<endl<< "----------------------------------------";
                cout <<endl<< "请选择统计内容（输入0返回）" << endl;
                cout << "1.平均分" << endl;
                cout << "2.不及格比例" << endl;
                cout << "3.优良比例" << endl;
                cout << "4.按从高到低排序" << endl;
                cout << "----------------------------------------" << endl;
                cin >> i;
                system("cls");
                if (i == 0)
                {
                    break;
                }
                if (i == 1)
                {
                    avg();
                }
                else if (i == 2)
                {
                    proportion1();
                }
                else if (i == 3)
                {
                    proportion2();
                }
                else if (i == 4)
                {
                    sort();
                    display();
                }
            } while (true);
        }
        else if (a == 3)
        {
            display();
        }
        else if (a == 4)
        {
            edit();
        }
        else if (a==5)
        {   
            string name;
            cout << "请输入要删除的学生信息(姓名或学号)：";
            cin >> name;
            Delete(name);
        }
        else if (a == 6)
        {
            search(); // 调用新增的查询函数
        }
        else if (a == 0) 
        {
            char s;
            cout << "是否保存学生信息?(y/n):";
            cin >> s;
            if (tolower(s) == 'n')   //全部转为小写
            {
                cout << "已退出，信息未保存";
                exit(0); 
            }
            else if (tolower(s) == 'y')  
            {
               if (save())
                {

                    exit(0);
                }
                else
                {
                    cout << "保存失败";
                    exit(0);
                }
               
            }
            
        }

    }
}


//添加学生数据//链表
void addstudent(string num, string name, double sc1, double sc2,double sc) 
{

    struct student* p = new student;
    p->snum = num;
    p->sname = name;
    p->score1 = sc1;
    p->score2 = sc2;
    p->score = sc;
    p->stunext = NULL;
    if (stuhead == NULL) {
        stuhead = p;
    }
    else {
        struct student* q = stuhead;
        while (q->stunext != NULL) {
            q = q->stunext;
        }
        q->stunext = p;
    }
}

//添加学生
void add()
{
    int a;
    string  snum;
    string sname;
    int choice;
    bool exists;
    double score1 = 0;
    double score2 = 0;
    double score = 0;
    cout << "请输入要添加的人数：(按0返回主菜单)";
    cin >> a;
    for (int i = 0; i < a; i++)
    {
        cout << "请输入学生姓名：";
        cin >> sname;
       
        do {
            exists = false;
            cout << "请输入学生学号：";
            cin >> snum;
            if (isExist(snum)) {
                exists = true;
                cout << "学号已存在，请选择操作：\n1.重新输入学号\n2.返回主菜单" << endl;
                cin >> choice;
                if (choice == 2) {
                    return;// 返回主菜单，结束添加
                    system("cls");
                }
            }
        } while (exists);
        cout << "请输入学生成绩1：";
        cin >> score1;
        cout << "请输入学生成绩2：";
        cin >> score2;
        score = score1 + score2;
        addstudent(snum, sname, score1, score2, score);
        cout << "添加完成"<<endl;
        display();
    }
   
}

//文件行数统计
int CountLines(ifstream& file)
{
    int n = 0;
    string tmp;
    file.seekg(0, ios::beg);    // 让指针在文件开头
    while (getline(file, tmp))
    {
        n++;
    }
    file.clear();               // 清除文件结束标志
    file.seekg(0, ios::beg);    // 重置指针到文件开头
    return n;
    file.close();
}

//录入原有学生
void origrinstu()
{
    string  snum;
    string sname;
    double score1 = 0;
    double score2 = 0;
    double score = 0;
    ifstream  file;
    file.open("sort.txt", ios::out | ios::in);
    if (file.fail())
    {
        cout << "sort.txt文件不存在，将跳过读入" << endl;
    }
    else
    {
        int lines = CountLines(file);
        int line = 0;
        while (line < lines)
        {

            file >> sname >> snum >> score1 >> score2 >>score;
            
            addstudent(snum, sname, score1, score2,score);
            line++;
        }
    }

}

//展示
void display()
{   
    cout << endl;
    struct student* p = stuhead;
    cout << setw(11) << left << "姓名 " << setw(11) << left << "学号 " 
        << setw(5) << left << "成绩1 " << setw(5) << left << "成绩2 " << setw(5) << left << "总成绩 " << endl;
    while (p != NULL)
    {

        cout << setw(10) << left << p->sname << " " << setw(10) << left << p->snum << " "
            << setw(5) << left << p->score1 << " " << setw(5) << left << p->score2 << setw(5) << left << p->score << endl;
        p = p->stunext;
    }
    cout << endl;
}

//计算成绩平均值
void avg()
{
    double sum = 0;
    double num=0;
    double res1 = 0;
    double res2=0;
    struct student* p = stuhead;
    while (p != NULL)
    {
        sum += p->score1;
        num++;
        p = p->stunext;
    }
    res1 = sum / num;
    p = stuhead;
    sum = 0;
    while (p != NULL)
    {
        sum += p->score2;
        p = p->stunext;
    }
    res2 = sum / num;
    cout << "score1科目的平均分是:" << res1 << endl;
    cout << "score2科目的平均分是:" << res2 << endl;
}

//及格和不及格占比
void proportion1()
{
    int n1=0,n2=0,num=0;    //人数
    struct student* p = stuhead;
    while (p!=NULL)
    {
        if (p->score1 > 60) 
        {
            n1++;
            if (p->score2 >60)
            {
                n2++;
            }
        }
        else if (p->score2 > 60)
        {
            n2++;
        }
        num++;
        p=p->stunext;
    }
    cout << "科目一的及格人数为：" << n1 << endl;
    cout << "及格比例为 " << (n1 * 1.00 / num)*100 << "%" << endl<<endl;
    cout << "科目二的及格人数为：" << n2 << endl;
    cout << "及格比例为：" <<( n2 * 1.00 / num) *100<< "%" << endl;

}

//优良中占比
void  proportion2()
{
    int n1 = 0, n2 = 0, n3 = 0, num = 0;    //人数
    struct student* p = stuhead;
    while (p != NULL)
    {
        if (p->score1 >= 90)
        {
            n1++;
        }
        else if (p->score1 >= 80 && p->score1 < 90)
        {
            n2++;
        }
        else if (p->score1 >= 60 && p->score1 < 80)
        {
            n3++;
        }
        num++;
        p = p->stunext;
    }
    cout << "科目一的优秀人数为：" << n1 << " " << "占比" << (n1 * 1.00 / num) * 100 << "%" << endl;
    cout << "科目一的良好人数为：" << n2 << " " << "占比" << (n2 * 1.00 / num) * 100 << "%" << endl;
    cout << "科目一的中等人数为：" << n3 << " " << "占比" << (n3 * 1.00 / num) * 100 << "%" << endl;
    n1 = 0;
    n2 = 0;
    n3 = 0;
    p = stuhead;
    while (p != NULL)
    {
        if (p->score2 >= 90)
        {
            n1++;
        }
        else if (p->score2 >= 80 && p->score2 < 90)
        {
            n2++;
        }
        else if (p->score2 >= 60 && p->score2 < 80)
        {
            n3++;
        }
        p = p->stunext;
    }
    cout << "科目二的优秀人数为：" << n1 << "占比" << (n1 * 1.00 / num) * 100 << "%" << endl;
    cout << "科目二的良好人数为：" << n2 << "占比" << (n2 * 1.00 / num) * 100 << "%" << endl;
    cout << "科目二的中等人数为：" << n3 << "占比" << (n3 * 1.00 / num) * 100 << "%" << endl;
}

//编辑
void edit()
{   
    int a;
    string snum;
    string sname;
    double s1=0;
    double s2=0;
    struct student* p = stuhead;
    do {
    re:
        system("cls");
        display();
        cout <<endl<< "----------------------------------------";
        cout <<endl<< "编辑模式（输入0返回）" << endl;
        cout << "请输入需要修改的内容：" << endl;
        cout << "1.姓名" << endl;
        cout << "2.学号" << endl;
        cout << "3.成绩" << endl;
        cin >> a;

        if (a == 0)
        {
            break;
        }
        else if (a == 1)
        {
            
            cout << "请输入要修改学生的学号:";
            cin >> snum;
            while (p != NULL && snum != p->snum)
            {
                p = p->stunext;
            
            }
            if (p == NULL)
            {
                cout << "没有该学生" << endl;
                goto re;
            }
            else
            {
                cout << "请输入新名字";
                cin >> sname;
                cout << setw(11) << left << "姓名 " << setw(11) << left << "学号 "
                    << setw(5) << left << "成绩1 " << setw(5) << left << "成绩2 " << setw(5) << left << "总成绩" << endl;
                cout << setw(10) << left << p->sname << " " << setw(10) << left << p->snum << " "
                    << setw(5) << left << p->score1 << " " << setw(5) << left << p->score2 <<setw(5) << left << p->score << endl;
                p->sname = sname;
                cout << setw(11) << left << "姓名 " << setw(11) << left << "学号 "
                    << setw(5) << left << "成绩1 " << setw(5) << left << "成绩2 " << setw(5) << left << "总成绩" << endl;
                cout << setw(10) << left << p->sname << " " << setw(10) << left << p->snum << " "
                    << setw(5) << left << p->score1 << " " << setw(5) << left << p->score2<<setw(5) << left << p->score << endl;
            }
            cout << "是否继续编辑？（1继续/0返回）"; // 新增提示
            cin >> a;
        }
        else if (a==2)
        {
           
            cout << "请输入要修改学生的姓名:";
            cin >> sname;    
            while (p != NULL && sname != p->sname)
            {
                p = p->stunext;

            }
            if (p == NULL)
            {
                cout << "没有该学生" << endl;
                goto re;
            }
            else
            {
                cout << "请输入新学号:";
                cin >> snum;
                if (isExist(snum))
                {
                    cout << "学号已存在" << endl;
                }
                else 
                {
                cout << setw(11) << left << "姓名 " << setw(11) << left << "学号 "
                    << setw(5) << left << "成绩1 " << setw(5) << left << "成绩2 " << setw(5) << left << "总成绩" << endl;
                cout << setw(10) << left << p->sname << " " << setw(10) << left << p->snum << " "
                    << setw(5) << left << p->score1 << " " << setw(5) << left << p->score2 << setw(5) << left << p->score << endl;
                p->snum = snum;
                cout << setw(11) << left << "姓名 " << setw(11) << left << "学号 "
                    << setw(5) << left << "成绩1 " << setw(5) << left << "成绩2 " << setw(5) << left << "总成绩" << endl;
                cout << setw(10) << left << p->sname << " " << setw(10) << left << p->snum << " "
                    << setw(5) << left << p->score1 << " " << setw(5) << left << p->score2 << setw(5) << left << p->score << endl;
                }
                
            }
            cout << "是否继续编辑？（1继续/0返回）"; // 新增提示
            cin >> a;
        }
        else if (a == 3)
        {
       
            cout << "请输入要修改学生的姓名:";
            cin >> snum;
            while (p != NULL && sname != p->sname)
            {
                p = p->stunext;

            }
            if (p == NULL)
            {
                cout << "没有该学生" << endl;
                goto re;
            }
            else
            {
                cout << "请分别输入两门成绩";
                cin >> s1 >> s2;
                cout << setw(11) << left << "姓名 " << setw(11) << left << "学号 "
                    << setw(5) << left << "成绩1 " << setw(5) << left << "成绩2 " << setw(5) << left << "总成绩" << endl;
                cout << setw(10) << left << p->sname << " " << setw(10) << left << p->snum << " "
                    << setw(5) << left << p->score1 << " " << setw(5) << left << p->score2 << setw(5) << left << p->score << endl;
                p->score1=s1;
                p->score2 = s2;
                p->score = s1 + s2;
                cout << setw(11) << left << "姓名 " << setw(11) << left << "学号 "
                    << setw(5) << left << "成绩1 " << setw(5) << left << "成绩2 " << setw(5) << left << "总成绩" << endl;
                cout << setw(10) << left << p->sname << " " << setw(10) << left << p->snum << " "
                    << setw(5) << left << p->score1 << " " << setw(5) << left << p->score2 << setw(5) << left << p->score << endl;
            }
            cout << "是否继续编辑？（1继续/0返回）"; // 新增提示
            cin >> a;
        }
        
    } while (a != 0); // 新增循环

}

//保存到文件
int save()
{
    sort();
    struct student* p=stuhead;
    ofstream  file;
    file.open("sort.txt", ios::out );
    if (file.fail())
    {
        cout << "sort.txt文件不存在，将重新创建" << endl;
        cout << "信息保存中" << endl;
        while (p != NULL)
        {
            file << setw(10) << left << p->sname << " " << setw(10) << left << p->snum << " "
                << setw(5) << left << p->score1 << " " << setw(5) << left << p->score2 << setw(5) << left << p->score << endl;
            p = p->stunext;
        }
        cout << "已退出，信息保存完成" << endl;
        file.close();
        return 1;
    }
    else if(!file.fail())
    {
        cout << "信息保存中" << endl;
        while (p != NULL)
        {
            file<<setw(10) << left << p->sname << " " << setw(10) << left << p->snum << " "
                << setw(5) << left << p->score1 << " " << setw(5) << left << p->score2 << setw(5) << left << p->score<<endl;
            p = p->stunext;
        }
        cout << "已退出，信息保存完成" << endl;
        file.close();
        return 1;
    }
    else
    {
        cout << "出现错误 保存失败" << endl;
        return 0;
    }
  
}

//学号查重
bool isExist(string snum) {
    struct student* p = stuhead;
    while (p != NULL) {
        if (p->snum == snum) {
            return true;
        }
        p = p->stunext;
    }
    return false;
}

//删除学生信息
void Delete(string n)
{
    bool found = false;
    struct student* p = stuhead;
    struct student* q = NULL;

    // 按姓名匹配删除
    while (p != NULL)
    {
        if (p->sname == n)
        {
            // 显示学生完整信息
            cout << "\n找到匹配姓名的学生：" << endl;
            cout << "----------------------------------------" << endl;
            cout << "姓名: " << p->sname << endl;
            cout << "学号: " << p->snum << endl;
            cout << "成绩1: " << p->score1 << endl;
            cout << "成绩2: " << p->score2 << endl;
            cout << "总成绩: " <<  p->score << endl;
            cout << "----------------------------------------" << endl;

            // 二次确认
            cout << "确认删除该学生？(y/n): ";
            char confirm;
            cin >> confirm;
            if (tolower(confirm) != 'y') {
                q = p;
                p = p->stunext;
                continue;
            }

            // 执行删除操作
            found = true;
            if (q == NULL) { // 头节点
                stuhead = p->stunext;
                delete p;
                p = stuhead;
            }
            else { // 中间/尾部节点
                q->stunext = p->stunext;
                delete p;
                p = q->stunext;
            }
            cout << "成功删除学生: " << n << endl;
        }
        else
        {
            q = p;
            p = p->stunext;
        }
    }

    // 姓名未找到时按学号匹配
    if (!found) {
        p = stuhead;
        q = NULL;
        while (p != NULL)
        {
            if (p->snum == n)
            {
                // 显示学生完整信息
                cout << "\n找到匹配学号的学生：" << endl;
                cout << "----------------------------------------" << endl;
                cout << "姓名: " << p->sname << endl;
                cout << "学号: " << p->snum << endl;
                cout << "成绩1: " << p->score1 << endl;
                cout << "成绩2: " << p->score2 << endl;
                cout << "----------------------------------------" << endl;
                // 二次确认
                cout << "确认删除该学生？(y/n): ";
                char confirm;
                cin >> confirm;
                if (tolower(confirm) != 'y') {
                    q = p;
                    p = p->stunext;
                    continue;
                }
                // 执行删除操作
                found = true;
                if (q == NULL) { // 头节点
                    stuhead = p->stunext;
                    delete p;
                    p = stuhead;
                }
                else { // 中间/尾部节点
                    q->stunext = p->stunext;
                    delete p;
                    p = q->stunext;
                }
                cout << "成功删除学号为 " << n << " 的学生" << endl;
            }
            else
            {
                q = p;
                p = p->stunext;
            }
        }
    }
    if (!found)
    {
        cout << "\n未找到姓名或学号为 [" << n << "] 的学生" << endl;
        cout << "请选择操作：" << endl;
        cout << "1. 重新输入查询条件" << endl;
        cout << "2. 返回主菜单" << endl;
        cout << "请输入选项：";

        int choice;
        cin >> choice;
        if (choice == 1)
        {
            cout << "请输入新的姓名/学号：";
            string newInput;
            cin >> newInput;
            Delete(newInput); // 递归调用
        }
        else
        {
            cout << "已取消删除操作" << endl;
        }
    }
    else
    {
        cout << "\n当前剩余学生信息：" << endl;
        display();
    }
}

// 模糊查询
void search() 
{
    string keyword;
    do {
        cout << "\n----------------------------------------";
        cout << "\n请输入要查找的学生姓名关键字（输入0返回主菜单）：";
        cin >> keyword;
        system("cls");
        if (keyword == "0") {
            system("cls");
            break;
        }
        // 将关键字转换为小写
        transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
        bool found = false;
        struct student* p = stuhead;
        int count = 0;
        while (p != NULL) 
        {
            // 将学生信息转换为小写
            string nameLower = p->sname;
            string numLower = p->snum;
            transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
            transform(numLower.begin(), numLower.end(), numLower.begin(), ::tolower);
            // 检查是否匹配
            if (nameLower.find(keyword) != string::npos ||numLower.find(keyword) != string::npos) 
            {
                if (!found) {
                    cout << "\n找到以下匹配结果：" << endl;
                    cout << "=================================================================" << endl;
                    cout << setw(15) << left << "姓名"
                        << setw(15) << left << "学号"
                        << setw(10) << left << "成绩1"
                        << setw(10) << left << "成绩2" <<setw(10) << left << "总成绩 " << endl;
                    found = true;
                }

                // 输出对齐的详细信息
                cout << setw(15) << left << p->sname
                    << setw(15) << left << p->snum
                    << setw(10) << left << p->score1
                    << setw(10) << left << p->score2
                    << setw(10) << left << p->score << endl;
                count++;
            }
            p = p->stunext;
        }
        if (found)
        {
            cout << "=================================================================" << endl;
            cout << "共找到 " << count << " 条匹配结果" << endl;
        }
        else {
            cout << "\n未找到包含 \"" << keyword << "\" 的学生信息" << endl;
        }
    } while (true);
}

// 排序
void sort() {
    bool swapped;
    struct student* p = NULL;
    struct student* q = NULL;
    struct student* next = NULL;
    struct student* head = NULL;
    do {
        swapped = false;
        p = NULL;
        q = stuhead;

        while (q->stunext != head) { // 每次循环减少遍历范围
            next = q->stunext;

            if (q->score < next->score) { // 降序排列
                // 交换q和next节点
                if (p != NULL) {
                    p->stunext = next;
                }
                else {
                    stuhead = next; // 更新头节点
                }
                q->stunext = next->stunext;
                next->stunext = q;

                // 更新p和q以继续遍历
                p = next;
                swapped = true;
            }
            else {
                p = q;
                q = q->stunext;
            }
        }
        head = q; // 最后交换的节点在正确位置，下次无需检查
    } while (swapped);
}
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;

// Выдаёт айди
int give_id();
// Проверяте наличие пользователя в файле
bool check_existence(string fname, string sname);
// Пустой файл?
bool is_empt(ifstream& pFile);
// Проверяет есть ли такой пользователь
bool checklogin(string login);
// Регистрация
int regis();
// Логин
int log_in();
// Список пользователей
void userlist();
// Основная функция друзей
void friends(int id);
// Добавить друга
void addfriend(int id);
// Удалить друга
void delfriend(int id);
// выдаёт айди ао логину
int getid(string user);
// Отправляет сообщения
void send_message(string filePath, int id);
// Основная функция сообщений
void messenger(int id);

int main()
{
    SetConsoleCP(1251);
    setlocale(LC_ALL, "Russian");
    int input, uid = -1;
    string log, pas;
    ofstream fout;
    while (uid == -1) {
        cout << "\n1. Зарегистрироваться\n2. Залогиниться\n3. Выйти из приложения\nВведите цифру для выбора действия\n";
        cin >> input;
        switch (input)
        {
        case 1:
            uid = regis();
            break;
        case 2:
            uid = log_in();
            break;
        case 3:
            return 0;
            break;
        default:
            cout << "Вы ввели некоректные данные, попробуйте еще раз\n";
            cin >> input;
            break;
        }
    }

    while (true) {
        int Input;
        cout << "\nВведите цифру для выбора действия\n1 - друзья\n2 - сообщения\n3 - пользователи\n4 - выход\n";
        cin >> Input;
        switch (Input)
        {
        case 1:
            friends(uid);
            break;
        case 2:
            messenger(uid);
            break;
        case 3:
            userlist();
            break;
        case 4:
            return 0;
            break;
        default:
            cout << "Вы ввели некоректные данные, попробуйте еще раз\n";
            break;
        }
    }
}

// Регистрация
int regis()
{
    string log, pas; int id;
    ofstream log_file("users.txt", ios_base::app);
    cout << "\nПридумайте имя пользователя\n";
    cin >> log;
    if (checklogin(log)) {
        id = give_id();
        cout << "\nПридумайте пароль\n";
    даун_норм_пароль_придумай:
        cin >> pas;
        if (log == pas) {
            cout << "Логин и пароль обязаны различаться, это увеличивает безопасность ваших личных данных,\nпридумайте другой пароль\n";
            goto даун_норм_пароль_придумай;
        }
        log_file << "\n" << log << "\n" << pas << "\n" << id;
        log_file.close();
        cout << "\nВы успешно зарегистрировались\n";
        return id;
    }
    else {
        cout << "\nТакой логин уже используется";
        return -1;
    }
    log_file.close();
}
// Логин
int log_in() {
    string ulogin, upasw, r;
    int f = 1;
    ifstream logpas("users.txt");
    cout << "Введите имя пользователя\n";
    cin >> ulogin;
    cout << "Введите пароль\n";
    cin >> upasw;
    while (!logpas.eof()) {
        getline(logpas, r);
        if (r == ulogin) {
            getline(logpas, r);
            if (r == upasw) {
                f = 0;
                getline(logpas, r);
                break;
            }
            else {
                cout << "Неверный пароль\n";
                f = 0;
                r = "-1";
                break;
            }
        }
    }
    logpas.close();
    if (f == 1) {
        cout << "Нет такого пользователя\n";
        r = "-1";
    }
    return stoi(r);
}
// Основная функция друзей
void friends(int id) {
    int input;
    string file_path;
    file_path = to_string(id) + "_friends.txt";
    ifstream friendlist(file_path);
    if (!friendlist or is_empt(friendlist)) {
        cout << "У вас еще нет друзей. Вы можете их добавить!\n";
    }
    else {
        cout << "Список ваших друзей:\n";
        cout << ifstream(file_path).rdbuf();
        friendlist.close();
    }
    cout << "\n1. Добавить друга\n2. Удалить друга\n3. Вернуться назад\nВведите цифру для выбора действия\n";
    cin >> input;
    switch (input) {
    case 1:
        addfriend(id);
        break;
    case 2:
        delfriend(id);
        break;
    case 3:
        break;
    }
}
// Добавить друга
void addfriend(int id) {
    string file_path, friendname;
    file_path = to_string(id) + "_friends.txt";
    ofstream friendlist(file_path, ios_base::app);
    cout << "Введите логин друга\n";
    cin >> friendname;
    if (!checklogin(friendname)) {
        cout << "У вас теперь на одного друга больше!\n";
        friendlist << friendname << " id:" << getid(friendname) << "\n";
        friendlist.close();
    }
    else {
        cout << "Пользователя с таким логином не существует\n";
    }
}
// Удалить друга
void delfriend(int id) {
    fstream ffriends, fnew;
    string fname, log, s, s1;
    fname = to_string(id) + "_friends.txt";
    ffriends.open(fname, ios::in);
    cout << "Введите логин пользователя, которого хотите удалить из друзей\n";
    cin >> log;
    if (!check_existence(fname, log)) {
        cout << "Такого пользователя нет у вас в друзьях\n";
    }
    else {
        fnew.open("Tfriends.txt", ios::out);
        while (getline(ffriends, s)) {
            s1 = s.substr(0, s.find(' '));
            if (!(s1 == log)) {
                fnew << s << endl;
            }
        }
        fnew.close();
        ffriends.close();
        fnew.open("Tfriends.txt", ios::in);
        ffriends.open(fname, ios::out);
        while (getline(fnew, s)) {
            ffriends << s << endl;
        }
        fnew.close();
        ffriends.close();
        cout << "У вас теперь на одного друга меньше";
    }
}
// Список пользователей
void userlist() {
    ifstream user("users.txt");
    string login, id, pas;
    int i;
    cout << "\nСписок пользователей:\n--------------------\n";
    getline(user, login);
    while (!user.eof()) {
        getline(user, login);
        getline(user, pas);
        getline(user, id);
        cout << "Имя пользователя: " << login << "\nid пользователя:" << id << "\n--------------------\n";
    }
}
// Проверяет есть ли такой пользователь
bool checklogin(string login)
{
    ifstream log_file("users.txt");
    string log;
    if (!log_file) {
        return 1;
    }
    while (!log_file.eof()) {
        getline(log_file, log);
        if (log == login) {
            return 0;
        }
    }
    log_file.close();
    return 1;
}
// Проверяте наличие пользователя в файле
bool check_existence(string fname, string sname) {
    fstream f;
    f.open(fname, ios::in);
    string s, s2, s3;
    while (!f.eof()) {
        f >> s;
        if (s == sname) {
            return true;
        }
    }
    f.close();
    return false;
}
// Выдаёт айди
int give_id() {
    ifstream idline("users.txt");
    string last_id = "";
    while (!idline.eof()) {
        getline(idline, last_id);
    }
    if (last_id == "") {
        return 1;
    }
    else
        return stoi(last_id) + 1;
    idline.close();
}
// Пустой файл?
bool is_empt(ifstream& pFile) {
    return pFile.peek() == ifstream::traits_type::eof();
}
// выдаёт айди ао логину
int getid(string user) {
    int userid;
    string data;
    ifstream file("users.txt");
    while (!file.eof()) {
        getline(file, data);
        if (data == user) {
            break;
        }
    }
    getline(file, data); getline(file, data);
    userid = stoi(data);
    file.close();
    return userid;
}
// Отправляет сообщения
void send_message(string filePath, int id) {
    ofstream chatfile(filePath, ios_base::app);
    char mes[1024];
    cin.ignore();
    cin.getline(mes, 1024);
    chatfile << "Пользователь '" << to_string(id) << "': " << mes << "\n";
    chatfile.close();
}
// Основная функция сообщений
void messenger(int id) {
    string chel, filePath;
    int chelid, mainid = id;
    cout << "C кем открыть диалог? Введите логин.\n";
    cin >> chel;
    if (!checklogin(chel)) {
        chelid = getid(chel);
        if (mainid > chelid) {
            swap(mainid, chelid);
        }
        filePath = "dialog_" + to_string(mainid) + "_" + to_string(chelid) + ".txt";
        ifstream chatfile(filePath);
        if (!chatfile or is_empt(chatfile)) {
            cout << "Диалога еще нет. Напишите первым!\n";
        }
        else {
            cout << "Ваш диалог:\n";
            cout << ifstream(filePath).rdbuf() << "\n";
        }
        cout << "Введите сообщение и нажмите 'enter'.\n";
        chatfile.close();
        send_message(filePath, id);
    }
    else {
        cout << "Такого пользователя не существует\n";
    }
}

#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <locale>
#include <string>
#include <atomic>
#ifdef _WIN32
#include <windows.h>
#endif
int getTerminalWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    return columns;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
#endif
}

void printCentered(const std::string& text) {
    int terminalWidth = getTerminalWidth();
    int textLength = text.length();
    int padding = (terminalWidth - textLength) / 2;
    if (padding > 0) {
        std::cout << std::string(padding, ' ') << text << std::endl;
    }
    else {
        std::cout << text << std::endl;
    }
}

using namespace std;

atomic<bool> stopFlag(false);

string reset = "\033[0m";
string red = "\033[31m";
string green = "\033[32m";
string blue = "\033[34m";

class Karakter {
public:
    int kimlik;
    float aclik;
    float susuzluk;
    float aclikLvl;
    float susuzlukLvl;
    float para;
    float uyku;
    float guc;
    bool uyudu;
    string isim;


    Karakter(int k, const string& i, float a, float s, float aLvl, float sLvl, float u, float p, float g)
        : kimlik(k), isim(i), aclik(a), susuzluk(s), aclikLvl(aLvl), susuzlukLvl(sLvl), uyku(u), uyudu(false), para(p), guc(g) {}

    void paraGonder(Karakter& alici, float miktar) {
        if (uyudu) {
            cout << isim << " suan" << red << " uyuyor." << reset << "Once uyandir." << endl;
            return;
        }
        else if (alici.uyudu) {
            cout << alici.isim << " suan" << red << " uyuyor." << reset << "Once uyandir." << endl;
            return;
        }

        if (para >= miktar) {
            para -= miktar;
            alici.para += miktar;
            cout << isim << " " << miktar << " $ gonderdi. Kalan para= " << para << endl;
        }
        else {
            cout << isim << " yeterli paraya sahip degil. Mevcut para= " << para << endl;
        }
    }

    void gucKontrolu(const Karakter& diger) const {
        if (guc > diger.guc) {
            cout << isim << " " << diger.isim << "'den daha guclu." << endl;
        }
        else if (guc < diger.guc) {
            cout << isim << " " << diger.isim << "'den daha zayif." << endl;
        }
        else {
            cout << isim << " ve " << diger.isim << " esit gucte." << endl;
        }
    }



    void hareketEt(const string& yon) {
        if (yon == "ileri" || yon == "geri" || yon == "sag" || yon == "sol") {
            cout << "Karakter " << kimlik << " " << yon << " yonune hareket ediyor." << endl;
        }
        else {
            cout << "Belirtilen yon gecersiz." << endl;
        }

    }

    void ic(const string& icecek) {
        if (uyudu) {
            cout << isim << " suan" << red << " uyuyor." << reset << "Once uyandir." << endl;
            return;
        }
        float miktar;
        string iIcecek = "";
        uyku = 100;

        if (icecek == "kola" || icecek == "k") {
            miktar = 53;
            iIcecek = "kola";
        }
        else if (icecek == "ayran" || icecek == "a") {
            miktar = 38;
            iIcecek = "ayran";
        }
        else if (icecek == "su" || icecek == "s") {
            miktar = 100;
            iIcecek = "su";
        }
        else {
            cout << blue << isim << reset << ": Dolapta sadece kola, ayran ve su var." << endl;
            return;
        }

        if (susuzluk > susuzlukLvl + 20) {
            cout << isim << " susamadi. Su anki susuzluk durumu: " << susuzluk << endl;
            return;
        }
        else if (susuzluk + miktar < susuzlukLvl) {

            susuzluk = min(susuzluk + miktar, 100.0f);
            cout << isim << " " << iIcecek << " icti. Ama hala biraz susuz. Su anki susuzluk durumu: " << susuzluk << endl;

        }
        else {
            susuzluk = min(susuzluk + miktar, 100.0f);
            cout << isim << " " << iIcecek << " icti, rahatladi. Su anki susuzluk durumu: " << susuzluk << endl;

        }
    }


    void sporYap(const string& arg) {
        if (uyudu) {
            cout << isim << " suan" << red << " uyuyor." << reset << "Once uyandir." << endl;
            return;
        }


        cout << isim << " spor yapti ve cok yoruldu. Guc: " << guc << green << "+5" << reset << endl;
        guc = guc + 5;
        uyku = 50;


    }

    void ye(const string& yemek) {
        if (uyudu) {
            cout << isim << " suan" << red << " uyuyor" << reset << ".Once uyandir." << endl;
            return;
        }
        float miktar;
        string iYemek = "";
        uyku = 100;

        if (yemek == "hamburger" || yemek == "h") {
            iYemek = "hamburger";
            miktar = 40;
        }
        else if (yemek == "dondurma" || yemek == "d") {
            iYemek = "dondurma";
            miktar = 30;
        }
        else if (yemek == "iskender" || yemek == "i") {
            iYemek = "iskender";
            miktar = 100;
        }
        else {
            cout << blue << isim << reset << ": Dolapta sadece dondurma, iskender ve hamburger var." << endl;
            return;
        }



        if (aclik > aclikLvl + 30) {
            cout << isim << " Suan ac hissetmiyor. Aclik durumu: " << aclik << endl;
            return;
        }
        else if (aclik + miktar < aclikLvl) {
            aclik = min(aclik + miktar, 100.0f);
            cout << isim << " " << iYemek << " yedi. Ama hala biraz ac. Aclik durumu: " << aclik << endl;

        }
        else {
            aclik = min(aclik + miktar, 100.0f);
            cout << isim << " " << iYemek << " yedi. Tika basa doydu. Aclik durumu: " << aclik << endl;

        }
    }

    void durumYazdir() const {
        bool ac;
        bool susuz;

        if (aclik < aclikLvl) {
            ac = true;
        }
        else {
            ac = false;
        }

        if (susuzluk < susuzlukLvl) {
            susuz = true;
        }
        else {
            susuz = false;
        }

        cout << blue << isim << "= " << reset << "Kimlik no= " << kimlik << ", Aclik durumu= " << (ac ? red : green) << (ac ? "Aç(" : "Tok(") << aclik << ")" << reset
            << ", Susuzluk durumu= " << (susuz ? red : green) << (susuz ? "Susuz kalmis(" : "Iyi(") << susuzluk << ")" << reset
            << ", Uyku durumu= " << (uyudu ? red : green) << (uyudu ? "Uyuyor" : "Uyanik") << reset << ", Parasi= " << para << "$ " << "Güc= " << guc << endl;

    }

    void uyandir(const string& arg) {
        uyku = 100;
        uyudu = false;
        if (aclik < aclikLvl && susuzluk < susuzlukLvl) {
            cout << isim << " uyandi ama" << red << " ac ve susuz" << reset << " gorunuyor. Bir seyler yedir ve icir." << endl;
        }
        else if (aclik < aclikLvl) {
            cout << isim << " uyandi ama" << red << " acikmis" << reset << " gorunuyor. Bir seyler yedir." << endl;
        }
        else if (susuzluk < susuzlukLvl) {
            cout << isim << " uyandi ama" << red << " susamis" << reset << " gorunuyor. Bir seyler icir." << endl;
        }
        else {
            cout << isim << green << " uyandi." << reset << endl;
        }
    }
};



void statuAzalt(Karakter& karakter1, Karakter& karakter2) {
    int i = 200;
    while (i >= 0 && !stopFlag) {
        if (karakter1.aclik > 3) {
            karakter1.aclik = karakter1.aclik - 3;
        }
        else {
            karakter1.aclik = 0;
        }
        if (karakter2.aclik > 2) {
            karakter2.aclik = karakter2.aclik - 2;
        }
        else {
            karakter2.aclik = 0;
        }

        if (karakter1.susuzluk > 4) {
            karakter1.susuzluk = karakter1.susuzluk - 4;
        }
        else {
            karakter1.susuzluk = 0;
        }
        if (karakter2.susuzluk > 3) {
            karakter2.susuzluk = karakter2.susuzluk - 3;
        }
        else {
            karakter2.susuzluk = 0;
        }

        if (!karakter1.uyudu) {
            if (karakter1.uyku > 1) {
                karakter1.uyku = karakter1.uyku - 2;
            }
            else {
                cout << red << karakter1.isim << " uyudu." << reset << endl;
                karakter1.uyudu = true;
                cout << "> ";
            }
        }

        if (!karakter2.uyudu) {
            if (karakter2.uyku > 1) {
                karakter2.uyku = karakter2.uyku - 2;
            }
            else {
                cout << red << karakter2.isim << " uyudu." << reset << endl;
                karakter2.uyudu = true;
                cout << "> ";
            }
        }

        if (i <= 1) {
            stopFlag = true;
            break;
        }

        this_thread::sleep_for(chrono::milliseconds(500));
        i -= 1;
    }


}




void yardim() {
    cout << "\nKOMUTLAR:" << endl
        << "1- hareketEt <kimlik> <yön> (ileri, geri, sag, sol)" << endl
        << "2- ye <kimlik> <yemek> (hamburger - h, dondurma - d, iskender - i)" << endl
        << "3- ic <kimlik> <içecek> (su - s, kola - k, ayran - a)" << endl
        << "4- paraVer <gönderenId> <miktar>" << endl
        << "5- gucKontrol <kimlik>" << endl
        << "6- sporYap <kimlik>" << endl
        << "7- durum" << endl
        << "8- uyan" << endl
        << "9- komutlar" << endl
        << "10- kapat" << endl;
}

void statuAzaltBaslat(thread& t1, Karakter& karakter1, Karakter& karakter2) {
    if (t1.joinable()) {
        stopFlag = true;
        t1.join();
    }
    stopFlag = false;
    t1 = thread(statuAzalt, ref(karakter1), ref(karakter2));
}



int main() {

    setlocale(LC_ALL, "Turkish");

    Karakter karakter1(1, "Turkay", 100.0f, 100.0f, 50.0f, 60.0f, 200.0f, 200.0f, 95.0f);
    Karakter karakter2(2, "Esma", 100.0f, 100.0f, 58.0f, 63.0f, 200.0f, 280.0f, 90.0f);

    string text = blue + "SUBU Metaverse evrenine hos geldin.\n" + reset;

    printCentered(text);

    karakter1.durumYazdir();
    karakter2.durumYazdir();
    yardim();

    thread t1;
    statuAzaltBaslat(t1, karakter1, karakter2);

    string komut;
    while (true) {
        cout << "> ";
        getline(cin, komut);
        stringstream ss(komut);
        string eylem, arg1, arg2, arg3;
        ss >> eylem >> arg1 >> arg2 >> arg3;


        if (eylem == "hareketEt") {
            if (arg1 == "1") karakter1.hareketEt(arg2);
            else if (arg1 == "2") karakter2.hareketEt(arg2);
        }
        else if (eylem == "ye") {
            statuAzaltBaslat(t1, karakter1, karakter2);
            if (arg1 == "1") karakter1.ye(arg2);
            else if (arg1 == "2") karakter2.ye(arg2);
        }
        else if (eylem == "sporYap") {
            if (arg1 == "1") karakter1.sporYap(arg2);
            else if (arg1 == "2") karakter2.sporYap(arg2);
        }
        else if (eylem == "gucKontrol") {
            if (arg1 == "1") karakter1.gucKontrolu(karakter2);
            else if (arg1 == "2") karakter2.gucKontrolu(karakter1);
        }
        else if (eylem == "uyan") {
            if (arg1 == "1") karakter1.uyandir(arg2);
            else if (arg1 == "2") karakter2.uyandir(arg2);
        }
        else if (eylem == "ic") {
            if (arg1 == "1") karakter1.ic(arg2);
            else if (arg1 == "2") karakter2.ic(arg2);
        }
        else if (eylem == "paraVer") {
            try {
                float miktar = stof(arg2);
                if (arg1 == "1") karakter1.paraGonder(karakter2, miktar);
                else if (arg1 == "2") karakter2.paraGonder(karakter1, stof(arg2));
            }
            catch (const std::exception& e) {
                cout << "Hatali komut. Sunu dene: paraVer <gönderenId> <miktar>" << endl;
            }

        }
        else if (eylem == "durum") {
            karakter1.durumYazdir();
            karakter2.durumYazdir();
        }
        else if (eylem == "komutlar") {
            yardim();
        }
        else if (eylem == "kapat") {
            stopFlag = true;
            break;
        }
        else {
            cout << "Bilinmeyen komut." << endl;
        }
    }

    if (t1.joinable()) {
        t1.join();
    }

    return 0;
}

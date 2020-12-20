#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;


class ChangeVec {
private:
    vector<pair<string, int>> changed;
    unsigned int startingPoint;      // next, previous page ����� �ʿ��� �ܾ� ������

public:
    ChangeVec() {
        this->setStartingPoint(0);
        this->setChanged(this->text_to_vector());
    }

    vector<pair<string, int>> text_to_vector() {
        // ofstream output("print.txt");
        ifstream readfile("test.txt");

        char text;                 // file���� ������ ���� 1��
        int byteCount = 0;             // 1 char = 1 byte
        vector<char> word;
        vector<pair<string, int>> wordWithSize;

        if (readfile.is_open()) {
            while (!readfile.eof()) {   // test.txt�� vector�� ��ȯ
                readfile.get(text);
                if ((int)text == 0x20) {
                    // space == 0x20(ascii), �ܾ��� ��
                    string s(word.begin(), word.end());
                    wordWithSize.push_back(make_pair(s, byteCount));
                    word.clear();
                    byteCount = 0;
                }
                else {
                    byteCount++;
                    word.push_back(text);
                }
            }
        }
        readfile.close();
        return wordWithSize;
    }

    vector<pair<string, int>>& getChanged() {
        return this->changed;
    }
    void setChanged(vector<pair<string, int>> newChanged) {
        this->changed = newChanged;
    }

    unsigned int getStartingPoint() {
        return this->startingPoint;
    }
    void setStartingPoint(unsigned int newStartingPoint) {
        this->startingPoint = newStartingPoint;
    }


};

class Page {
private:
    ChangeVec changeVec;

public:
    Page(){}
    Page(ChangeVec _changeVec) :changeVec(_changeVec) {

    }

    void print_page() { // ����� ���� methods
        // ChangeVec class���� ������ �����´�.
        vector<pair<string,int>>& givenVec = changeVec.getChanged();
        int byteCount = 0;
        int lineCount = 1;
        unsigned int startingPoint = changeVec.getStartingPoint();
 
        cout << "0" << lineCount << "| ";
        // 75 byte, 20 line�� ���� �ݺ���
        do {
            string word = givenVec[startingPoint].first;
            byteCount += givenVec[startingPoint].second + 1;        // ���� ����

            // 75 byte �˻�
            if (byteCount > 75) {
                cout << endl;
                lineCount++;
                if (lineCount < 10) {
                    cout << "0" << lineCount << "| ";
                }
                else if(lineCount <=20){
                    cout << lineCount << "| ";
                }
                // ���ο� line�� ���� �ʱ�ȭ
                byteCount = 0;
                byteCount += givenVec[startingPoint].second + 1;    // ���� ����
                
                // 20 line �˻�
                if (lineCount >20) {
                    break;
                }
            }
            cout << word << " ";
            startingPoint++;
        } while (startingPoint<givenVec.size());

    }

    void print_next() {
        // ChangeVec class���� ������ �����´�.
        vector<pair<string,int>>& givenVec = changeVec.getChanged();
        int byteCount = 0;
        int lineCount = 1;
        unsigned int startingPoint = changeVec.getStartingPoint();

        //���� �������� startingPoint�� ��� ���� �ݺ��� ��� 
        do {
            string word = givenVec[startingPoint].first;
            byteCount += givenVec[startingPoint].second+1;     

            // 75 byte �˻�
            if (byteCount > 75) {
                lineCount++;
                // ���ο� line�� ���� �ʱ�ȭ
                byteCount = 0;
                byteCount += givenVec[startingPoint].second+1;

                //20 line �˻�
                if (lineCount > 20) {
                    break;
                }
            }
            startingPoint++;
        } while (startingPoint < givenVec.size());

        changeVec.setStartingPoint(--startingPoint);
        // ���� setting�� startingPoint�� print()
        print_page();
    }

    void print_previous() {
        // ChangeVec class���� ������ �����´�.
        vector<pair<string, int>>& givenVec = changeVec.getChanged();
        int byteCount = 0;
        int lineCount = 1;
        unsigned int startingPoint = changeVec.getStartingPoint();

        //���� �������� startingPoint�� ��� ���� �ݺ��� ��� 
     
            do {
                startingPoint--;
                string word = givenVec[startingPoint].first;
                byteCount += givenVec[startingPoint].second + 1;


                // 75 byte �˻�
                if (byteCount > 75) {
                    lineCount++;
                    // ���ο� line�� ���� �ʱ�ȭ
                    byteCount = 0;
                    byteCount += givenVec[startingPoint].second + 1;

                    // 20 line �˻�
                    if (lineCount > 19) {
                        break;
                    }
                }
            } while (startingPoint >= 0);

            changeVec.setStartingPoint(startingPoint);

        // ���� setting�� startingPoint�� print()
        print_page();
    }

    ChangeVec getChangeVec() {
        return this->changeVec;
    }
};

class Modify {
private:
    ChangeVec changeVec;
    Page page;

public:
    Modify(){}
    Modify(ChangeVec _changeVec, Page _page) : changeVec(_changeVec), page(_page) {

    }
    bool insert_word(int _line, int _order, string givenWord) {
        vector<pair<string, int>>& givenVec = changeVec.getChanged();
        int lineCount = 1;
        int wordCount = 0;
        int byteCount = 0;
        int startingPoint = changeVec.getStartingPoint();
        int insertionPoint=0;     // ������ ���� index
        if (_line > 20) {
            return false;
        }
        else {
            
            do {
                wordCount++;
                string word = givenVec[startingPoint].first;
                byteCount += givenVec[startingPoint].second + 1;

                // 75 byte �˻�
                if (byteCount > 75) {
                    lineCount++;
                    // ���ο� line�� ���� �ʱ�ȭ
                    byteCount = 0;
                    wordCount = 0;
                    byteCount += givenVec[startingPoint].second + 1;
                    
                }
                // �ش� ���ο��� insertion �۾�
                if (lineCount == _line && wordCount == _order) {
                    insertionPoint = startingPoint+1;
                    break;
                }
                startingPoint++;
            } while (lineCount < 21);

            if (lineCount == 21 || insertionPoint == 0) {
                return false;
            }
            else {
                int sizeOfString = givenWord.length();
                pair<string, int> insertion = make_pair(givenWord, sizeOfString);
                vector<pair<string, int>>::iterator it = givenVec.begin();
                givenVec.insert(it + insertionPoint, insertion);

                changeVec.setChanged(givenVec); // insertion �� vector update
                Page newPage(changeVec);
                page.print_page();

                return true;
            }
            
        }


    }
    bool delete_word(int _line, int _order) {
        vector<pair<string, int>>& givenVec = changeVec.getChanged();
        int lineCount = 1;
        int wordCount = 0;
        int byteCount = 0;
        int startingPoint = changeVec.getStartingPoint();
        int deletePoint = 0;     // ������ ���� index
        if (_line > 20) {
            return false;
        }
        else {

            do {
                wordCount++;
                string word = givenVec[startingPoint].first;
                byteCount += givenVec[startingPoint].second + 1;

                // 75 byte �˻�
                if (byteCount > 75) {
                    lineCount++;
                    // ���ο� line�� ���� �ʱ�ȭ
                    byteCount = 0;
                    wordCount = 0;
                    byteCount += givenVec[startingPoint].second + 1;

                }
                // �ش� ���ο��� insertion �۾�
                if (lineCount == _line && wordCount == _order) {
                    deletePoint = startingPoint;
                    break;
                }
                startingPoint++;
            } while (lineCount < 21);

            if (lineCount == 21 || deletePoint < 0) {
                return false;
            }
            else {
                givenVec.erase(givenVec.begin()+deletePoint);

                changeVec.setChanged(givenVec); // insertion �� vector update
                Page newPage(changeVec);
                newPage.print_page();

                return true;
            }

        }

    }

    vector<pair<string, int>> change_word(string oldWord, string newWord) {
        vector<pair<string, int>>& givenVec = changeVec.getChanged();
        int index = 0;
        for (; index < givenVec.size(); index++) {
            if (givenVec[index].first == oldWord) {
                int newSize = newWord.length();
                givenVec[index].first=newWord;
                givenVec[index].second = newSize;
            }
        }
        changeVec.setChanged(givenVec);
        Page newPage(changeVec);
        newPage.print_page();
        return givenVec;
    }


    ChangeVec getChangeVec() {
        return this->changeVec;
    }
    Page getPage() {
        return this->page;
    }
};

class Seek : public Page{
private :
    ChangeVec changeVec;
    Page page;
public:
    Seek(){}
    Seek(ChangeVec _changeVec, Page _page) :changeVec(_changeVec), page(_page) {

    }

    bool seek_word(string givenWord) {
        vector<pair<string, int>>& givenVec = changeVec.getChanged();
        unsigned int startingPoint = -1;

        // search        
        for (int index = 0; index < givenVec.size(); index++) {
            if (givenVec[index].first == givenWord) {
                startingPoint = index;
                break;
           }
        }
        // not found
        if (startingPoint == -1) {
            return false;
        }
        else {
            changeVec.setStartingPoint(startingPoint);
            Page newPage(changeVec);
            page.print_page();
            return true;
        }
    }
    
    ChangeVec getChangeVec() {
        return this->changeVec;
    }

    Page getPage() {
        return this->page;
    }
};



void show_menu(string aMessage) {
    cout << "-----------------------------------------------------------------------------------------------" << endl;
    cout << "n:����������, p;����������, i:����, d:����, c:����, s:ã��, t:���� �� ����" << endl;
    cout << "-----------------------------------------------------------------------------------------------" << endl;
    cout << "(" << aMessage << ")" << endl;
    cout << "-----------------------------------------------------------------------------------------------" << endl;
    cout << "�Է� : ";
    
}

int main() {
    ChangeVec changeVec_;
    Page page_;
    Modify modify_(changeVec_, page_);
    Seek seek_(changeVec_, page_);
    Page newPage;
    ChangeVec newVec_;

    page_.print_page();        // �ʱ� ���
    show_menu("ó�� ����Դϴ�.");

        char menu;
        int lineOrder;
        int wordOrder;
        string search;
        string oldstr;
        string newstr;
        

        cin >> menu;
        do {
        switch (menu) {
        case 'n':
            page_.print_next();
            show_menu("���� �������Դϴ�.");
            break;

        case 'p':
            page_.print_previous();
            show_menu("���� �������Դϴ�.");
            break;

        case 'i':
            cout << "������ ������ �Է��Ͻÿ� : ";
            cin >> lineOrder;
            cout << "������ �ܾ��� ������ �Է��Ͻÿ� : ";
            cin >> wordOrder;
            cout << "������ �ܾ �Է��Ͻÿ� : ";
            cin >> oldstr;
            while(!modify_.insert_word(lineOrder, wordOrder, oldstr)) {
                cout << "�ùٸ��� ���� ���� �Է��߽��ϴ�. �ٽ� �Է��Ͻÿ�" << endl;
                cout << "������ ������ �Է��Ͻÿ� : ";
                cin >> lineOrder;
                cout << "������ �ܾ��� ������ �Է��Ͻÿ� : ";
                cin >> wordOrder;
                cout << "������ �ܾ �Է��Ͻÿ� : ";
                cin >> oldstr;
                
            }
            show_menu("�ܾ �����߽��ϴ�.");
            break;

        case 'd':
            cout << "������ �ܾ��� ������ �Է��Ͻÿ�: ";
            cin >> lineOrder;
            cout << "������ �ܾ��� ������ �Է��Ͻÿ�: ";
            cin >> wordOrder;
            while (!modify_.delete_word(lineOrder, wordOrder)) {
                cout << "�ùٸ��� ���� ���� �Է��߽��ϴ�. �ٽ� �Է��Ͻÿ�" << endl;

                cout << "������ �ܾ��� ������ �Է��Ͻÿ�: ";
                cin >> lineOrder;
                cout << "������ �ܾ��� ������ �Է��Ͻÿ�: ";
                cin >> wordOrder;
            }
            show_menu("�ܾ �����߽��ϴ�.");
            break;

        case 'c':
            cout << "���� �ܾ �Է��Ͻÿ�: " << endl;
            cin >> oldstr;
            cout << "���ο� �ܾ �Է��Ͻÿ�: " << endl;
            cin >> newstr;
            newVec_.setChanged(modify_.change_word(oldstr, newstr));
            show_menu("�ܾ �ٲپ����ϴ�.");
            break;

        case 's':
            cout << "ã�� �ܾ �Է��Ͻÿ�: ";
            cin >> search;
            if (!seek_.seek_word(search)) {
                show_menu("�ܾ ã�� ���߽��ϴ�.");
            }
            show_menu("ã�� �ܾ��Դϴ�.");
            break;

        default:
            break;
        }
        cin >> menu;
    } while (menu != 't');

    // ���� ���� �� ���� ����
    ofstream writeFile;
    writeFile.open("test.txt");    
    vector<pair<string, int>>& writeVec = newVec_.getChanged();

    if (writeFile.is_open()) {
        for (int i = 0; i < writeVec.size(); i++) {
            string str = writeVec[i].first;
            writeFile << str << " ";
        }
    }
    writeFile.close();


    cout <<"���α׷��� ����˴ϴ�."<< endl;

    return 0;
}

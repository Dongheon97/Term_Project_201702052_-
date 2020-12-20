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
    unsigned int startingPoint;      // next, previous page 실행시 필요한 단어 시작점

public:
    ChangeVec() {
        this->setStartingPoint(0);
        this->setChanged(this->text_to_vector());
    }

    vector<pair<string, int>> text_to_vector() {
        // ofstream output("print.txt");
        ifstream readfile("test.txt");

        char text;                 // file에서 가져온 글자 1개
        int byteCount = 0;             // 1 char = 1 byte
        vector<char> word;
        vector<pair<string, int>> wordWithSize;

        if (readfile.is_open()) {
            while (!readfile.eof()) {   // test.txt를 vector로 변환
                readfile.get(text);
                if ((int)text == 0x20) {
                    // space == 0x20(ascii), 단어의 끝
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

    void print_page() { // 출력을 위한 methods
        // ChangeVec class에서 정보를 가져온다.
        vector<pair<string,int>>& givenVec = changeVec.getChanged();
        int byteCount = 0;
        int lineCount = 1;
        unsigned int startingPoint = changeVec.getStartingPoint();
 
        cout << "0" << lineCount << "| ";
        // 75 byte, 20 line을 위한 반복문
        do {
            string word = givenVec[startingPoint].first;
            byteCount += givenVec[startingPoint].second + 1;        // 공백 포함

            // 75 byte 검사
            if (byteCount > 75) {
                cout << endl;
                lineCount++;
                if (lineCount < 10) {
                    cout << "0" << lineCount << "| ";
                }
                else if(lineCount <=20){
                    cout << lineCount << "| ";
                }
                // 새로운 line을 위한 초기화
                byteCount = 0;
                byteCount += givenVec[startingPoint].second + 1;    // 공백 포함
                
                // 20 line 검사
                if (lineCount >20) {
                    break;
                }
            }
            cout << word << " ";
            startingPoint++;
        } while (startingPoint<givenVec.size());

    }

    void print_next() {
        // ChangeVec class에서 정보를 가져온다.
        vector<pair<string,int>>& givenVec = changeVec.getChanged();
        int byteCount = 0;
        int lineCount = 1;
        unsigned int startingPoint = changeVec.getStartingPoint();

        //다음 페이지의 startingPoint를 얻기 위해 반복문 사용 
        do {
            string word = givenVec[startingPoint].first;
            byteCount += givenVec[startingPoint].second+1;     

            // 75 byte 검사
            if (byteCount > 75) {
                lineCount++;
                // 새로운 line을 위한 초기화
                byteCount = 0;
                byteCount += givenVec[startingPoint].second+1;

                //20 line 검사
                if (lineCount > 20) {
                    break;
                }
            }
            startingPoint++;
        } while (startingPoint < givenVec.size());

        changeVec.setStartingPoint(--startingPoint);
        // 새로 setting한 startingPoint로 print()
        print_page();
    }

    void print_previous() {
        // ChangeVec class에서 정보를 가져온다.
        vector<pair<string, int>>& givenVec = changeVec.getChanged();
        int byteCount = 0;
        int lineCount = 1;
        unsigned int startingPoint = changeVec.getStartingPoint();

        //이전 페이지의 startingPoint를 얻기 위해 반복문 사용 
     
            do {
                startingPoint--;
                string word = givenVec[startingPoint].first;
                byteCount += givenVec[startingPoint].second + 1;


                // 75 byte 검사
                if (byteCount > 75) {
                    lineCount++;
                    // 새로운 line을 위한 초기화
                    byteCount = 0;
                    byteCount += givenVec[startingPoint].second + 1;

                    // 20 line 검사
                    if (lineCount > 19) {
                        break;
                    }
                }
            } while (startingPoint >= 0);

            changeVec.setStartingPoint(startingPoint);

        // 새로 setting한 startingPoint로 print()
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
        int insertionPoint=0;     // 삽입을 위한 index
        if (_line > 20) {
            return false;
        }
        else {
            
            do {
                wordCount++;
                string word = givenVec[startingPoint].first;
                byteCount += givenVec[startingPoint].second + 1;

                // 75 byte 검사
                if (byteCount > 75) {
                    lineCount++;
                    // 새로운 line을 위한 초기화
                    byteCount = 0;
                    wordCount = 0;
                    byteCount += givenVec[startingPoint].second + 1;
                    
                }
                // 해당 라인에서 insertion 작업
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

                changeVec.setChanged(givenVec); // insertion 한 vector update
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
        int deletePoint = 0;     // 삭제을 위한 index
        if (_line > 20) {
            return false;
        }
        else {

            do {
                wordCount++;
                string word = givenVec[startingPoint].first;
                byteCount += givenVec[startingPoint].second + 1;

                // 75 byte 검사
                if (byteCount > 75) {
                    lineCount++;
                    // 새로운 line을 위한 초기화
                    byteCount = 0;
                    wordCount = 0;
                    byteCount += givenVec[startingPoint].second + 1;

                }
                // 해당 라인에서 insertion 작업
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

                changeVec.setChanged(givenVec); // insertion 한 vector update
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
    cout << "n:다음페이지, p;이전페이지, i:삽입, d:삭제, c:변경, s:찾기, t:저장 후 종료" << endl;
    cout << "-----------------------------------------------------------------------------------------------" << endl;
    cout << "(" << aMessage << ")" << endl;
    cout << "-----------------------------------------------------------------------------------------------" << endl;
    cout << "입력 : ";
    
}

int main() {
    ChangeVec changeVec_;
    Page page_;
    Modify modify_(changeVec_, page_);
    Seek seek_(changeVec_, page_);
    Page newPage;
    ChangeVec newVec_;

    page_.print_page();        // 초기 출력
    show_menu("처음 출력입니다.");

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
            show_menu("다음 페이지입니다.");
            break;

        case 'p':
            page_.print_previous();
            show_menu("이전 페이지입니다.");
            break;

        case 'i':
            cout << "삽입할 라인을 입력하시오 : ";
            cin >> lineOrder;
            cout << "삽입할 단어의 순서를 입력하시오 : ";
            cin >> wordOrder;
            cout << "삽입할 단어를 입력하시오 : ";
            cin >> oldstr;
            while(!modify_.insert_word(lineOrder, wordOrder, oldstr)) {
                cout << "올바르지 않은 값을 입력했습니다. 다시 입력하시오" << endl;
                cout << "삽입할 라인을 입력하시오 : ";
                cin >> lineOrder;
                cout << "삽입할 단어의 순서를 입력하시오 : ";
                cin >> wordOrder;
                cout << "삽입할 단어를 입력하시오 : ";
                cin >> oldstr;
                
            }
            show_menu("단어를 삽입했습니다.");
            break;

        case 'd':
            cout << "삭제할 단어의 라인을 입력하시오: ";
            cin >> lineOrder;
            cout << "삭제할 단어의 순서를 입력하시오: ";
            cin >> wordOrder;
            while (!modify_.delete_word(lineOrder, wordOrder)) {
                cout << "올바르지 않은 값을 입력했습니다. 다시 입력하시오" << endl;

                cout << "삭제할 단어의 라인을 입력하시오: ";
                cin >> lineOrder;
                cout << "삭제할 단어의 순서를 입력하시오: ";
                cin >> wordOrder;
            }
            show_menu("단어를 삭제했습니다.");
            break;

        case 'c':
            cout << "기존 단어를 입력하시오: " << endl;
            cin >> oldstr;
            cout << "새로운 단어를 입력하시오: " << endl;
            cin >> newstr;
            newVec_.setChanged(modify_.change_word(oldstr, newstr));
            show_menu("단어를 바꾸었습니다.");
            break;

        case 's':
            cout << "찾을 단어를 입력하시오: ";
            cin >> search;
            if (!seek_.seek_word(search)) {
                show_menu("단어를 찾지 못했습니다.");
            }
            show_menu("찾은 단어입니다.");
            break;

        default:
            break;
        }
        cin >> menu;
    } while (menu != 't');

    // 파일 저장 후 종료 과정
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


    cout <<"프로그램이 종료됩니다."<< endl;

    return 0;
}

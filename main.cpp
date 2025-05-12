#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
using namespace std;

enum action{
    Skip = 10, Reverse = 11, Draw2 = 12, Wild = 13, Wild4= 14
};
string actionToString(int acction){
    switch(acction) {
        case Skip: return "Skip";
        case Reverse: return "Reverse";
        case Draw2: return "Draw2";
        case Wild: return "Wild";
        case Wild4: return "Wild4";
        default: return "Unknown";
    }
}
enum color{
    Blue = 15, Red = 16, Yellow = 17, Green = 18, Black = 19
};
string colorToString(int color) {
    switch(color) {
        case Blue: return "Blue";
        case Red: return "Red";
        case Yellow: return "Yellow";
        case Green: return "Green";
        case Black: return "Black";
        default: return "Unknown";
    }
}

void toUpper(string& str){
    for(int i=0;i<str.size();i++){
        str.at(i)= toupper(str.at(i));
    }
}

class card{
public:
    int Color;
    int Value;
    void full(){
        int card;
        card = rand()%101;
        if (card < 87) {
            Value = rand() % 13;
            Color = rand() % 4 + 15;
        }
        else{
            Value = rand() % 2 + 13;
            Color = Black;
        }
    }
    void  print(){
        string valueStr;
        if (Value<10) {
            valueStr = to_string(Value);
        } else {
            valueStr = actionToString(Value);
        }
        cout << "[" << valueStr << " " << colorToString(Color) << "]";
    }
    void enter() {
        string color;
        string value;
        cout << "Enter the color of the card:(Red, Green, Blue, Yellow, Black)"; cin >> color;
        cout << "Enter the name of the card:(0-9, Skip, Reverse, Draw2, Wild, Wild4)"; cin >> value;
        toUpper(color);
        toUpper(value);

        if (color == "RED") {Color = Red;}
        else if (color == "GREEN") {Color = Green;}
        else if (color == "BLUE") {Color = Blue;}
        else if (color == "YELLOW") {Color = Yellow;}
        else if (color == "BLACK") {Color = Black;}
        else {
            cout << "Invalid Color! Try again.\n";
            enter();
            return;
        }
        if(value.size() == 1 && value[0] >= 48 && value[0] <= 57){
            Value=stoi(value);
        }else if(value == "SKIP"){ Value= Skip ;
        }else if(value == "REVERSE"){ Value= Reverse ;
        }else if(value == "DRAW2"){ Value= Draw2 ;
        }else if(value == "WILD"){ Value= Wild ;
        }else if(value == "WILD4"){ Value= Wild4 ;}
        else{
            cout<<"Invalid Value!Try again please.\n";
            enter();
        }
    }
};
class player{
public:
    int id;
    vector<card> hand;
    void printCards(){
        for (int i = 0; i < hand.size(); ++i) {
            hand.at(i).print();
            cout << "  |  " ;
        }
        cout<<endl;
    }
};

class Humanplayer : public player{
public:
    string name;
    void newcolor(card &top){
        cout << "Choose a new color (Red, Green, Blue, Yellow): ";
        string newColor;
        cin >> newColor;
        toUpper(newColor);
        if (newColor == "RED" ) {top.Color = Red;}
        else if (newColor == "GREEN") {top.Color = Green;}
        else if (newColor == "BLUE") {top.Color = Blue;}
        else if (newColor == "YELLOW") {top.Color = Yellow;}
        else {
            cout << "Invalid color! Try again." << endl;
            newcolor(top);
        }
    }
    void play(card &top,int &next){
        cout <<"id: " << id <<" your cards: ";
        printCards();

        card playCard;
        bool hasCard = 0;

        for (int i = 0; i < hand.size(); ++i) {
            if (hand.at(i).Color == Black || hand.at(i).Value == top.Value || top.Color == hand.at(i).Color) {
                hasCard = 1;
            }
        }
        if (hasCard == 1) {
            while (true) {
                playCard.enter();
                bool truCard = 0;
                int indexPlayCard = 0;
                for (int i = 0; i < hand.size(); i++) {
                    if (playCard.Value == hand.at(i).Value && playCard.Color == hand.at(i).Color) {
                        if (playCard.Color == top.Color || playCard.Value == top.Value || playCard.Color == Black) {
                            truCard = 1;
                            indexPlayCard = i;
                        }
                    }
                }
                if (truCard==0) {
                    cout << "Wrong Card! Try again please."<< endl;
                    continue;
                }
                hand.erase(hand.begin()+indexPlayCard);
                if(playCard.Color==Black){newcolor(playCard);}
                top=playCard;
                break;
            }
        }else{
            playCard.full();
            hand.push_back(playCard);
            if (playCard.Color == top.Color || playCard.Value == top.Value || playCard.Color == Black) {
                while(true){
                    printCards();
                    card C;
                    C.enter();
                    if(C.Value != playCard.Value || C.Color != playCard.Color){
                        cout << "Wrong Card! Try again please."<< endl;
                        continue;
                    }
                    hand.erase(hand.begin()+hand.size()-1);
                    if(playCard.Color==Black){newcolor(playCard);}
                    top=playCard;
                }
            }
            if (top.Value == 10 || top.Value == 11 || top.Value == 12 || top.Value == 14){
                next = 1;
            }
        }
    }
};
class Botplayer : public player{
public:
    void play(card &top,int i,int &next){
        cout <<"id: " << id << "  bot"<<i<<" cards:";
        printCards();

        vector <card> playCards;
        card playCard;

        for (int i = 0; i < hand.size(); ++i) {
            if (hand.at(i).Value == top.Value || hand.at(i).Color == top.Color || hand.at(i).Color == Black ){
                playCards.push_back (hand.at(i));
            }
        }

        if(playCards.size()>0){
            int indexPlayCard = rand()%(playCards.size());
            top=playCards.at(indexPlayCard);
            hand.erase(hand.begin() + indexPlayCard);
            cout << "bot played : ";
            top.print();
            if(top.Color==Black){
                int newColor = rand() % 4 + 15;
                top.Color = newColor;
                cout << "\nBot" << i << " played wild and choose color: " << colorToString(newColor) << endl;
            }
            return;
        }

        playCard.full();
        if(playCard.Value == top.Value || playCard.Color == top.Color || playCard.Color == Black ){
            top=playCard;
            cout << "bot played : ";
            top.print();
            if(top.Color==Black){
                int newColor = rand() % 4 + 15;
                top.Color = newColor;
                cout << "\nBot" << i << " played wild and choose color: " << colorToString(newColor) << endl;
            }
        }else{
            hand.push_back(playCard);
        }
        if (top.Value == 10 || top.Value == 11 || top.Value == 12 || top.Value == 14){
            next = 1;
        }
    }
};
class GameManager{
public:
    bool Clockwise = 1;
    card Topcard;
    int NextTurnindex = 0;
    Humanplayer player1;
    Botplayer bots[3];
    void distribute() {
        cout << "Enter your name : " << endl;
        cin>>player1.name;
        player1.id = rand();
        card C;
        for (int i = 0; i < 7; ++i) {
            C.full();
            player1.hand.push_back(C);
        }
        for (int i = 0; i < 3; ++i) {
            bots[i].id = rand();
            for (int j = 0; j < 7; ++j) {
                C.full();
                bots[i].hand.push_back(C);
            }
        }
    }
    void ClockWies() {
        if (Clockwise == 1) {
            NextTurnindex++;
        } else {
            NextTurnindex--;
        }
    }
    void Turnindex() {
        if (NextTurnindex < 0) {
            NextTurnindex = -NextTurnindex % 4;
            NextTurnindex = -NextTurnindex;
        } else {
            NextTurnindex = NextTurnindex % 4;
        }
    }
    void giveCardToBot(int n, Botplayer &b){
        card C;
        for (int i = 0; i < n; ++i) {
            C.full();
            b.hand.push_back(C);
        }
    }
    void giveCardToHuman(int n, Humanplayer &h){
        card C;
        for (int i = 0; i < n; ++i) {
            C.full();
            h.hand.push_back(C);
        }
    }
    void DrawCard(int x) {
        Turnindex();
        if (NextTurnindex == 0) {
            giveCardToHuman(x,player1);
        } else if (NextTurnindex == 1 || NextTurnindex == -3) {
            giveCardToBot(x, bots[0]);
        } else if (NextTurnindex == 2 || NextTurnindex == -2) {
            giveCardToBot(x, bots[1]);
        } else if (NextTurnindex == 3 || NextTurnindex == -1) {
            giveCardToBot(x, bots[2]);
        }
    }
    void play() {
        distribute();
        Topcard.full();
        if (Topcard.Color == Black){
            cout << "Topcard: ";
            Topcard.print();
            cout << endl;
            cout <<"  your cards:";
            player1.printCards();
            while (true) {
                cout << "choose color (Yellow,Blue,Red,Green):\n";
                string color;
                cin >> color;
                toUpper(color);
                if (color == "RED") {
                    Topcard.Color = Red;
                    break;
                }else if (color == "BLUE"){
                    Topcard.Color = Blue;
                    break;
                }else if (color == "YELLOW"){
                    Topcard.Color = Yellow;
                    break;
                }else if (color == "GREEN"){
                    Topcard.Color = Green;
                    break;
                }else{
                    cout << "wrong color try again!" << endl;
                }
            }
        }
        if (Topcard.Value == 11) { Clockwise = 0; }
        else { Clockwise = 1; }
        if (Topcard.Value == 10) { ClockWies(); }
        while (true) {
            int next = 0;
            cout << "\nCards Count:\n";
            cout << "- You (" << player1.name << "): " << player1.hand.size() << " cards\n";
            cout << "- Bot1: " << bots[0].hand.size() << " cards\n";
            cout << "- Bot2: " << bots[1].hand.size() << " cards\n";
            cout << "- Bot3: " << bots[2].hand.size() << " cards\n";
            cout << "Topcard: ";
            Topcard.print();
            cout << endl;
            Turnindex();
            if (NextTurnindex == 0) {
                player1.play(Topcard, next);
                if (player1.hand.size() == 0) {
                    cout << "\nwinner" << endl;
                    return;
                }
                ClockWies();
            } else if (NextTurnindex == 1 || NextTurnindex == -3) {
                bots[0].play(Topcard, 1, next);
                if (bots[0].hand.size() == 0) {
                    cout << "\nBot " << bots[0].id << " wins!" << endl;
                    return;
                }
                ClockWies();
            } else if (NextTurnindex == 2 || NextTurnindex == -2) {
                bots[1].play(Topcard, 2, next);
                if (bots[1].hand.size() == 0) {
                    cout << "\nBot " << bots[1].id << " wins!" << endl;
                    return;
                }
                ClockWies();
            } else if (NextTurnindex == 3 || NextTurnindex == -1) {
                bots[2].play(Topcard, 3, next);
                if (bots[2].hand.size() == 0) {
                    cout << "\nBot " << bots[2].id << " wins!" << endl;
                    return;
                }
                ClockWies();
            }
            if (next == 0) {
                if (Topcard.Value == 10) {
                    ClockWies();
                }
                if (Topcard.Value == 11) {
                    if (Clockwise == 1) {
                        NextTurnindex -= 2;
                        Clockwise = 0;
                    } else {
                        NextTurnindex += 2;
                        Clockwise = 1;
                    }
                }
                if (Topcard.Value == 12) {
                    DrawCard(2);
                    ClockWies();
                }
                if (Topcard.Value == 14) {
                    DrawCard(4);
                    ClockWies();
                }
            }
        }
    }
};
int main() {
    srand(time(0));
    GameManager round1;
    round1.play();
    return 0;
}
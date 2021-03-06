#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <utility>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <sstream>
using namespace std;

int svm(const char* cmd) {
    char buffer[128];
    string result = "";
    shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    int x=0;
    stringstream convert(result);
    convert>>x;
    return x;
}

int win(const vector<int>& board){
    int wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},
                      {1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for(int i = 0; i < 8; ++i) {
        if(board[wins[i][0]] != 0 &&
           board[wins[i][0]] == board[wins[i][1]] &&
           board[wins[i][0]] == board[wins[i][2]])
            return board[wins[i][2]];
    }
    return 0;
}

bool over(const vector<int>& board){
    if(win(board)==0) {
      for (int i = 0; i < 9; i++) {
        if (board[i]==0) return false;
      }
    }
    return true;
}

int minimax(vector<int>& board, int player) {
    int winner = win(board);
    if(winner != 0) return winner*player;

    int move = -1;
    int score = -2;
    for(int i = 0; i < 9; ++i) {
        if(board[i] == 0) {
            board[i] = player;
            int thisScore = -minimax(board, player*-1);
            if(thisScore > score) {
                score = thisScore;
                move = i;
            }
            board[i] = 0;
        }
    }
    if(move == -1) return 0;
    return score;
}

int best_move(vector<int> a, bool mini){
  bool show=true;
  if (mini){
    int move = -1;
    int score = -2;
    for (int i = 0; i < 9; i++) {
      if(a[i] == 0) {
          a[i] = 1;
          int tempScore = -minimax(a, -1);
          a[i] = 0;
          if(tempScore > score) {
              score = tempScore;
              move = i;
          }
      }
    }
    if (show) cout<< "Computer's move:\t" << move << "\n\n";
    return move;
  }else{
    string input;
    for (int i = 0; i < a.size(); i++) {
      input.append(to_string(a[i]+1));
    }

    string command= "python SVM/playSVM.py '"+input+"'";
    cout << command << "\n";
    char cmd[32];
    for (size_t i = 0; i < 32; i++) {
      cmd[i]=command[i];
    }
    return svm(cmd);
  }
}

char getChar(int i){
  switch(i){
    case 0:
      return '_';
    case 1:
      return 'O';
    case -1:
      return 'X';
  }
}

void printBoard(const vector<int>& board){
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      cout<< getChar(board[i*3 + j]) << "\t";
    }
    cout<<"\n\n";
  }
}

vector<int> reverseBoard(vector<int> board){
  for(int i=0;i<9;i++){
    board[i]=board[i]*-1;
  }
  return board;
}

void makeMove(vector<int>& board, int playerNum, int move){
  board[move]=playerNum;
}

void demo(vector<int> a,bool againstUser,bool m=true){
  printBoard(a);
  for(int player=-1;!over(a);player=player*-1){
    if (player==1) {
      makeMove(a,1,best_move(a,m));
    }else{
      if(!againstUser){
        makeMove(a,-1,best_move(reverseBoard(a),m));
      }else{
        cout<< "Pick your move (0-8):\t";
        int move;
        cin >> move;
        if(a[move]==0) makeMove(a,-1,move);
      }

    }
    printBoard(a);
  }
}

int main(){
  //Change these booleans for demos
  //userInput decides whether demos is cpu v cpu or user v cpu
  bool userInput = true;
  //play against live minimax evaluation or pretrained svm
  bool minimax = true;
  vector<int> a;
  //written out for debugging to plug in starting state
  a.push_back(0);
  a.push_back(0);
  a.push_back(0);
  a.push_back(0);
  a.push_back(0);
  a.push_back(0);
  a.push_back(0);
  a.push_back(0);
  a.push_back(0);
  demo(a,userInput,minimax);

  return 0;
}

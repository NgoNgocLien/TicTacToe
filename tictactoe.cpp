#include <iostream>
#include <windows.h> 	// for TextColor function,system("cls"), Sleep(), Beep() 
#include <conio.h> 		// getch() and _getch()
#include <iomanip> 		// setw() for interface design
#include <cstdlib> 		// rand() for computer's choice and move suggestion
#include <ctime>
#include <fstream> 		// for signing up and logging in account 

using namespace std;

#define MAX 26

// define frequencies of musical notes (Hertz)
const int G = 392, A = 440, B = 494, C = 523, D = 587, E = 659, F = 698;
// define time duration for each note (millisecond)
const int S = 400, L = 800;

struct Board{
    char board[MAX][MAX];
    int size;
};

struct Player{
    char name[MAX];
    char marker;
    int won;
};

/// FUNCTIONS SUPPORTING INTERFACE ///
void TextColor(int color); 
void gotoXY (int col, int row);
void print_top_horizontal_line(Board B);
void print_bot_horizontal_line(Board B);
void print_vertical_line(Board B);
void print_alphabet_line(Board B);
void reset_board(Board &B);
void print_board(Board B, char marker1, char marker2);
void print_account_options();
int print_main_menu();
void print_tutorial();
void print_board_options(Board &B);
void print_Noel(int color);

/// FUNCTIONS SUPPORTING SOUND EFFECT ///
void print_error(); 
void sound_Noel();

/// FUNCTIONS SUPPORTING ACCOUNT ///
bool account_setting(Player P[]);

/// FUNCTIONS FOR GAME OPERATION ///
// FUNCTIONS SUPPORTING GAME'S BODY 
bool check_valid_position(Board B, int row, int col);
bool check_winner(Board B);
int change_xCor_into_col(int xCor);
int change_yCor_into_row(int yCor);
void move_suggestion(Board B);
bool move(Board &B, char marker);
// FUNCTIONS FOR GAME'S BODY, INCLUDING 2 MODES 
void pvp(Board B, Player P[]);
void pve(Board B, Player P[]);
// FUNCTIONS FOR GAME'S ENDING
void statistical_outcome(Player P[], int match, int mode, Board B);
bool replay_game();
// FUNCTION FOR ENTIRE GAME OPERATION
void start_game(Board &B, Player P[]);

/// MAIN ///
int main(){
    Board B;
    Player P[2];
    if (account_setting(P)){
        system("cls");
        start_game(B, P);
    }
    else
        return 0;
}

/// CHANGE TEXT AND BACK GROUND COLOR ///
void TextColor(int color){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
// Ref: Pham Nguyen Quang Nhan - a friend of mine

/// CURSOR SETTING ///
void gotoXY (int col, int row){
	COORD coord;
	coord.X = col;
	coord.Y = row;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}
// Ref: Pham Nguyen Quang Nhan - a friend of mine

/// PRINT BOARD - TOP HORIZONTAL LINE OF EACH SQUARE /// 
void print_top_horizontal_line(Board B){
    cout << "\t";
    for (int i = 0; i < B.size; i++){
        cout << " _______";
    }
    cout << "\n";
}
/// PRINT BOARD - BOTTOM HORIZONTAL LINE OF EACH SQUARE ///
void print_bot_horizontal_line(Board B){
    cout << "\t";
    for (int i = 0; i < B.size; i++){
        cout << "|_______";
    }
    cout << "|\n";
}
/// PRINT BOARD - VERTICAL LINE OF EVERY SINGLE LINE ///
void print_vertical_line(Board B){
    cout << "\t";
    for (int i = 0; i < B.size + 1; i++){
        cout << "|       ";
    } 
    cout << "\n";
}

/// PRINT COLUMN INDEX IN ALPHABETICAL ORDER ///
void print_alphabet_line(Board B){
    TextColor(14);
    cout << "\t    ";
    for (int i = 0; i < B.size; i++){
        cout << static_cast<char>(i + 'A') << "       ";
    }
    cout << "\n";
    TextColor(15);
}

/// CLEAR ALL MARKERS IN EACH SQUARE ///
void reset_board(Board &B){
    for (int i = 0; i < B.size; i++){
        for (int j = 0; j < B.size; j++){
            B.board[i][j] = ' ';
        }
    }
}

/// PRINT BOARD ///
void print_board(Board B, char marker1 = ' ', char marker2 = ' '){
    print_alphabet_line(B);
    print_top_horizontal_line(B);
    for (int k = 0; k < B.size; k++){
        for (int i = 0; i < 3; i++){
        if (i % 3 == 1){

            /// PRINT ROW INDEX IN NUMERICAL ORDER ///
            TextColor(14);
            cout << "\t\b\b\b" << setw(2) << k << " "; 
            TextColor(15);

            for (int j = 0; j < B.size; j++){
                cout << "|   ";
                if (marker1 != ' ' && B.board[k][j] == marker1) {
                    TextColor(11);
                    cout << B.board[k][j];
                }
                else if (marker2 != ' ' && B.board[k][j] == marker2) {
                    TextColor(10);
                    cout << B.board[k][j];
                } 
                else cout << B.board[k][j];
                TextColor(15);
                cout << "   "; // k = row, j = column
            }
            cout << "|\n";
        } 
        else if (i % 3 == 2){
            print_bot_horizontal_line(B);
        }
        else 
            print_vertical_line(B);
        }
    }
}

/// PRINT ACCOUNT OPTIONS : SIGN UP AND LOG IN ///
void print_account_options(){
    cout << "\n";
    cout << "\t============================================================\n";
    cout << "\t|"; 
    TextColor(14);
    cout << "                 WELCOME TO TIC TAC TOE!!!                ";
    TextColor(15);
    cout << "|\n";
    cout << "\t|----------------------------------------------------------|\n";
    cout << "\t|                                                          |\n";
    cout << "\t|"; 
    TextColor(13);
    cout << " PLEASE LOG IN TO PLAY                                    ";
    TextColor(15);
    cout << "|\n";
    TextColor(15);
    cout << "\t|                                                          |\n";
    cout << "\t| 1. No account yet? Sign up!                              |\n";
    cout << "\t| 2. Log in                                                |\n"; 
    cout << "\t| 3. Quit                                                  |\n";
    cout << "\t|                                                          |\n";
    cout << "\t============================================================\n";
}

/// PRINT MAIN MENU ///
/// AND GET MENU CHOICE ///
int print_main_menu(){
    int menu_choice;
    
    do {
        cout << "\n";
        cout << "\t============================================================\n";
        cout << "\t|"; 
        TextColor(14);
        cout << "                 WELCOME TO TIC TAC TOE!!!                ";
        TextColor(15);
        cout << "|\n";
        cout << "\t|----------------------------------------------------------|\n";
        cout << "\t|                                                          |\n";
        cout << "\t|"; 
        TextColor(13);
        cout << " MAIN MENU                                                ";
        TextColor(15);
        cout << "|\n";
        TextColor(15);
        cout << "\t|                                                          |\n";
        cout << "\t| 1. Single player game                                    |\n";
        cout << "\t| 2. Two players game                                      |\n"; 
        cout << "\t| 3. How to play                                           |\n";
        cout << "\t| 4. Quit                                                  |\n";
        cout << "\t============================================================\n";
        cout << "Enter your choice: ";
        cin >> menu_choice;

        if (menu_choice != 1 && menu_choice != 2 && menu_choice != 3 && menu_choice != 4){ 
            print_error();
            system("cls");
        }
        if (menu_choice == 3){
            system("cls");
            print_tutorial();
        }
    } while (menu_choice != 1 && menu_choice != 2 && menu_choice != 4);
    
    return menu_choice;
}

/// PRINT HOW TO PLAY ///
void print_tutorial(){
        cout << "\n";
        cout << "\t==============================================================================================================================\n";
        cout << "\t|"; 
        TextColor(14);
        cout << "                 WELCOME TO TIC TAC TOE!!!                                                                                  ";
        TextColor(15);
        cout << "|\n";
        cout << "\t|----------------------------------------------------------------------------------------------------------------------------|\n";
        cout << "\t|                                                                                                                            |\n";
        cout << "\t|"; 
        TextColor(13);
        cout << " HOW TO PLAY                                                                                                                ";
        TextColor(15);
        cout << "|\n";
        TextColor(15);

        cout << "\t|                                                                                                                            |\n";
        cout << "\t| ";
        TextColor(10);
        cout << "RULES:";
        TextColor(15);
        cout << "                                                                                                                     |\n";
        cout << "\t| - Players alternatively place their markers on the game board until someone wins or there are no more available moves.     |\n";
        cout << "\t| - Board 1x1: Neither player wins because this is an unfair game. "; 
        TextColor(14); 
        cout << "\\(0 o 0)/"; 
        TextColor(15); 
        cout << "                                                 |\n";
        cout << "\t| - Board 2x2: Be the first to get 2 in a row to win. This is also an unfair game but the first player is always the winner. |\n";
        cout << "\t| - Board 3x3: Be the first to get 3 in a row to win.                                                                        |\n";
        cout << "\t| - Board 4x4: Be the first to get 4 in a row to win.                                                                        |\n";
        cout << "\t| - Board 5x5 and above: Be the first to get 5 in a row to win.                                                              |\n";
        cout << "\t| - If all the squares are filled without a winner, the game is a draw.                                                      |\n";
        cout << "\t|                                                                                                                            |\n";
        cout << "\t| ";
        TextColor(11);
        cout << "CONTROL:";
        TextColor(15);
        cout << "                                                                                                                   |\n";
        cout << "\t| - To move around the board, use arrows: UP, DOWN, RIGHT, LEFT                                                              |\n";
        cout << "\t| - To fill a square with your marker, press ENTER.                                                                          |\n";
        cout << "\t| - To get HINT, press H in your turn.                                                                                       |\n";
        cout << "\t|                                                                                                                            |\n";
        cout << "\t==============================================================================================================================\n";
        cout << "Press any key to get to MAIN MENU.\n";
        getch();
        system("cls");
}

/// PRINT BOARD SIZE OPTIONS ///
/// AND GET BOARD SIZE ///
void print_board_options(Board &B){
    int board_choice;

    do{
        cout << "\n";
        cout << "\t============================================================\n";
        cout << "\t|"; 
        TextColor(14);
        cout << "                 WELCOME TO TIC TAC TOE!!!                ";
        TextColor(15);
        cout << "|\n";
        cout << "\t|----------------------------------------------------------|\n";
        cout << "\t|                                                          |\n";
        cout << "\t|"; 
        TextColor(13);
        cout << " CHOOSE BOARD SIZE                                        ";
        TextColor(15);
        cout << "|\n";
        TextColor(15);
        cout << "\t|                                                          |\n";
        cout << "\t| 1. 3x3                                                   |\n";
        cout << "\t| 2. 5x5                                                   |\n"; 
        cout << "\t| 3. 7x7                                                   |\n";
        cout << "\t| 4. Any size                                              |\n";
        cout << "\t============================================================\n";
        cout << "Enter your choice: ";
        cin >> board_choice;

        if (board_choice != 1 && board_choice != 2 && board_choice != 3 && board_choice != 4) {
            print_error();
            system("cls");
        }
    } while (board_choice != 1 && board_choice != 2 && board_choice != 3 && board_choice != 4);
    
    if (board_choice == 1) B.size = 3;
    if (board_choice == 2) B.size = 5;
    if (board_choice == 3) B.size = 7;
    if (board_choice == 4){
        cout << "Enter your size: ";
        cin >> B.size;
    }
}

// PRINT MERRY CHRISTMAS AND HAPPY NEW YEAR
void print_Noel(int color){
        system("cls");
        cout << "\n";
        TextColor(color);
        cout << "\t===========================================================\n";
        cout << "\t|    *     *     *     *     *     *     *     *     *    |\n";
        cout << "\t| *     *     *     *     *     *     *     *     *     * |\n";
        cout << "\t|    *     *     *     *     *     *     *     *     *    |\n";
        cout << "\t| *     *     *     *     *     *     *     *     *     * |\n";
        cout << "\t|    *     *     *  Merry Christmas and  *     *     *    |\n";
        cout << "\t| *     *     *     * Happy New Year! *     *     *     * |\n";
        cout << "\t|    *     *     *     *     *     *     *     *     *    |\n";
        cout << "\t| *     *     *     *     *     *     *     *     *     * |\n";
        cout << "\t|    *     *     *     *     *     *     *     *     *    |\n";
        cout << "\t===========================================================\n";
}

/// PRINT ERROR MESSAGE WITH ALARM ///
void print_error(){
    TextColor(12);
    cout << "Invalid option!\a\n";
    TextColor(15);
    getch();
}

/// PRINT MERRY CHIRSTMAS WITH SOUND ///
void sound_Noel(){
	print_Noel(1);
	Beep(G, L);
	print_Noel(3);
	Beep(C, L);
	print_Noel(4);
	Beep(C, S);
	print_Noel(5);
	Beep(D, S);
	print_Noel(6);
	Beep(C, S);
	print_Noel(7);
	Beep(B, S);
	print_Noel(8);
	Beep(A, L);
	print_Noel(9);
	Beep(A, L);
	print_Noel(10);
	
	Beep(A, L);
	print_Noel(11);
	Beep(D, L);
	print_Noel(12);
	Beep(D, S);
	print_Noel(13);
	Beep(E, S);
	print_Noel(14);
	Beep(D, S);
	print_Noel(15);
	Beep(C, S);
	print_Noel(1);
	Beep(B, L);
	print_Noel(2);
	Beep(G, L);
	print_Noel(3);
	
	Beep(G, L);
	print_Noel(4);
	Beep(E, L);
	print_Noel(5);
	Beep(E, S);
	print_Noel(6);
	Beep(F, S);
	print_Noel(7);
	Beep(E, S);
	print_Noel(8);
	Beep(D, S);
	print_Noel(9);
	Beep(C, L);
	print_Noel(10);
	Beep(A, L);
	print_Noel(11);
	
	Beep(G, S);
	print_Noel(12);
	Beep(A, S);
	print_Noel(13);
	Beep(A, L);
	print_Noel(14);
	Beep(D, L);
	print_Noel(15);
	Beep(B, L);
	print_Noel(2);
	Beep(C, L);
}

/// GET USER'S CHOICE FROM ACCOUNT OPTIONS ///
/// SIGN UP AND LOG IN FUNCTIONS ///
bool account_setting(Player P[]){
    int logIn_choice;
    fstream username, password;

    do {
        system("cls");
        print_account_options();
        cout << "Enter your choice: ";
        cin >> logIn_choice;
        cin.ignore();

        if (logIn_choice != 1 && logIn_choice != 2 && logIn_choice != 3){ 
            print_error();
            system("cls");
        }
        /// SIGN UP ///
        if (logIn_choice == 1){
            char inputU[MAX];
            char inputP[MAX];

            bool success = false;
            while (!success){
                system("cls");
                print_account_options();
                cout << "\n";
                TextColor(13);
                cout << "SIGN UP\n";
                TextColor(15);
                cout << "\n";

                cout << "- Enter new username: ";
                cin.getline(inputU, MAX);
                

                // Check if username already exists
                username.open("username.txt", ios::in);
                char temp[MAX];
                bool existU = false;
                while (username >> temp){
                    for (int i = 0; inputU[i] != '\0'; i++){
                        if (inputU[i] != temp[i]) break;
                        else if (inputU[i+1] == '\0') existU = true;
                    }
                }
                username.close();

                if (existU){
                    TextColor(12);
                    cout << "Username already exists. Choose another one.\a\n";
                    TextColor(15);
                    getch();
                }
                else{
                    cout << "- Enter password: ";
                    cin.getline(inputP, MAX);

                    username.open("username.txt", ios::out | ios::app);
                    password.open("password.txt", ios::out | ios::app);

                    username << inputU << endl;
                    password << inputP << endl;
                    
                    username.close();
                    password.close();

                    success = true;
                    cout << "\n";
                    TextColor(14);
                    cout << "Welcome ";
					TextColor(11);
					cout << inputU;
					TextColor(14);
					cout << ". You have signed up successfully!\n";
                    TextColor(15);
                    getch();
                }
            }
        } // Successful signing up brings user back to account options to choose logging in or quitting the program

    } while (logIn_choice != 2 && logIn_choice != 3);

    /// QUIT ///
    if (logIn_choice == 3){
        system("cls");
        for (int i = 9; i <= 15; i++){
            system("cls");
            cout << "\n";
            TextColor(i);
            cout << "\t===========================================================\n";
            cout << "\t|                                                         |\n";
            cout << "\t|                                                         |\n";
            cout << "\t|                      See you again!                     |\n";
            cout << "\t|                                                         |\n";
            cout << "\t|                                                         |\n";
            cout << "\t|                Designed by Ngo Ngoc Lien.               |\n";
            cout << "\t|                         12/2021                         |\n";
            cout << "\t|                                                         |\n";
            cout << "\t===========================================================\n";
            Sleep(1000);
        }
        return false;
    }
    
    /// LOG IN ///
    else if(logIn_choice == 2){
        char inputU[MAX];
        char inputP[MAX];

        bool success = false;
        while (!success){
            system("cls");
            print_account_options();
            cout << "\n";
            TextColor(13);
            cout << "LOG IN\n";
            TextColor(15);
            cout << "\n";

            cout << "- Enter username: ";
            cin.getline(inputU, MAX);
            cout << "- Enter password: ";
            cin.getline(inputP, MAX);
            
            // Check if username and password match
            char u[MAX];
            char p[MAX];

            bool correctU = false;
            bool correctP = false;

            username.open("username.txt", ios::in);
            password.open("password.txt", ios::in);

            while(username >> u && password >> p){
                for (int i = 0; inputU[i] != '\0'; i++){
                    if (inputU[i] != u[i]) break;
                    else if (inputU[i+1] == '\0') correctU = true;
                }
                for (int i = 0; inputP[i] != '\0'; i++){
                    if (inputP[i] != p[i]) break;
                    else if (inputP[i+1] == '\0') correctP = true;
                }
                if (correctU == true && correctP == true) break;
            }

            username.close();
            password.close(); 

            if (correctU == true && correctP == true) {
                success = true;
                cout << "\n";
                TextColor(14);
                cout << "Welcome back ";
                TextColor(11);
				cout << inputU;
				TextColor(14);
				cout << ". You have logged in successfully!\n";
                TextColor(15);
                memcpy(P[0].name, inputU, MAX);
                getch();
            }
            else{
            	TextColor(12);
            	cout << "\n";
                cout << "Wrong username or password.\a\n";
                cout << "Please try again!\n";
                TextColor(15);
                getch();
            }
        } 
    }
    return true;
}

/// CHECK IF MOVE IS VALID ///
bool check_valid_position(Board B, int row, int col){
    if (row > B.size || col > B.size){ 
        return false;
    }
    else if (B.board[row][col] != ' ') {
        return false;
    }
    else return true;
}

/// CHECK IF EITHER PLAYER WINS ///
bool check_winner(Board B){
    bool win = false;
    int count;
    char temp; // used for checking diagonal

    /// BOARD SIZE FROM 5 AND BELOW ///
    if (B.size == 1 || B.size == 2 || B.size == 3 || B.size == 4 || B.size == 5){
        /// CHECK ROW ///
        for (int i = 0; i < B.size; i++){
            count = 1;
            for (int j = 1; !win && j < B.size; j++){
                if (B.board[i][j] != ' ' && B.board[i][j] == B.board [i][j-1]) count++;
                else count = 1; // Reset count if count has't reached size

                if (count == B.size){
                    win = true;
                    break;
                }
            }
            if (win) break;
        }

        /// CHECK COLUMN ///
        for (int j = 0; !win && j < B.size; j++){
            count = 1;
            for (int i = 1; i < B.size; i++){
                if (B.board[i][j] != ' ' && B.board[i][j] == B.board [i-1][j]) count++;
                else count = 1; // Reset count if count has't reached size 

                if (count == B.size){
                    win = true;
                    break;
                }
            }
            if (win) break;
        }

        /// CHECK DIAGONAL ///
        /// left to right, downward
        temp = B.board[0][0];
        count = 1; 
        for (int i = 1; !win && i < B.size; i++){
            
            if (B.board[i][i] != ' ' && B.board[i][i] == temp) count++;
            else count = 1; // Reset count if count has't reached size

            if (count == B.size){
                win = true;
                break;
            }
        
            temp = B.board[i][i];
        }
        /// right to left, downward
        temp = B.board[0][B.size-1];
        count = 1; 
        for (int i = 1; !win && i < B.size; i++){
            if (B.board[i][B.size-1 - i] != ' ' && B.board[i][B.size-1 - i] == temp) count++;
            else count = 1; // Reset count if count has't reached size

            if (count == B.size){
                win = true;
                break;
            }
            
            temp = B.board[i][B.size-i-1];
        }
    }

    /// BOARD SIZE ABOVE 5 ///
    else {
        /// CHECK ROW ///
        for (int i = 0; i < B.size; i++){
            count = 1;
            for (int j = 1; !win && j < B.size; j++){
                if (B.board[i][j] != ' ' && B.board[i][j] == B.board [i][j-1]) count++;
                else count = 1; // Reset count if count has't reached size 

                if (count == 5){
                    win = true;
                    break;
                }
            }
            if (win) break;
        }

        /// CHECK COLUMN ///
        for (int j = 0; !win && j < B.size; j++){
            count = 1;
            for (int i = 1; i < B.size; i++){
                if (B.board[i][j] != ' ' && B.board[i][j] == B.board [i-1][j]) count++;
                else count = 1; // Reset count if count has't reached size 

                if (count == 5){
                    win = true;
                    break;
                }
            }
            if (win) break;
        }

        /// CHECK DIAGONAL ///
        /// LEFT TO RIGHT, downward

        // Start at each square on column 0 (column A)
        for (int i = B.size-5; !win && i >= 0 ; i--){
            temp = B.board[i][0];
            count = 1;

            int k; 
            for (k = 1; i + k < B.size; k++){ // run till bottom horizontal line
                if (B.board[i+k][0+k] != ' ' && B.board[i+k][0+k] == temp) count++;
                else count = 1;

                if (count == 5){
                    win = true;
                    break;
                }

                temp = B.board[i+k][0+k];
            }
            if (win) break;
        }
        // Start at each square on row 0, except square A0 (board[0][0])
        for (int j = 1; !win && j <= B.size-5 ; j++){
            temp = B.board[0][j];
            count = 1;

            int k, l;
            for (k = 1; j + k < B.size; k++){ // run till rightmost vertical line
                if (B.board[0+k][j+k] != ' ' && B.board[0+k][j+k] == temp) count++;
                else count = 1;

                if (count == 5){
                    win = true;
                    break;
                }

                temp = B.board[0+k][j+k];
            }
            if (win) break;
        } 

        /// RIGHT TO LEFT, downward

        // Start at each square on last column (size-1)
        for (int i = B.size-5; !win && i >= 0 ; i--){ // i runs from the square with biggest row index that is able to reach 5 from right to left
            temp = B.board[i][B.size-1]; 
            count = 1;

            int k; 
            for (k = 1; i + k < B.size; k++){ // run till bottom horizontal line
                if (B.board[i+k][B.size-1 - k] != ' ' && B.board[i+k][B.size-1 - k] == temp) count++;
                else count = 1;

                if (count == 5){
                    win = true;
                    break;
                }

                temp = B.board[i+k][B.size-1 - k];
            }
            if (win) break;
        }

        // Start at each square on row 0, except row [0][size-1] (rightmost square on row 0)
        for (int j = B.size-2; !win && j >= 4 ; j--){ // j is column
            temp = B.board[0][j]; 
            count = 1;

            int k, l;
            for (k = 1; j - k >= 0; k++){ // run till rightmost vertical line
                if (B.board[0+k][j-k] != ' ' && B.board[0+k][j-k] == temp) count++;
                else count = 1;

                if (count == 5){
                    win = true;
                    break;
                }

                temp = B.board[0+k][j-k];
            }
            if (win) break;
        }
    }
    return win;
}

/// CHANGE CONSOLE COORDINATE X INTO BOARD COLUMN INDEX ///
int change_xCor_into_col(int xCor){ 
    int col = -1;
    for (int i = 12; i <= xCor; i += 8) 
        col++;
    return col;
}

/// CHANGE CONSOLE COORDINATE Y INTO BOARD ROW INDEX ///
int change_yCor_into_row(int yCor){
    int row = -1;
    for (int i = 3; i <= yCor; i += 3) 
        row++;
    return row;
}

/// SUGGEST MOVE - HINT///
void move_suggestion(Board B){
    srand(time(0));
    int col, row;
    do{
        row = rand() % B.size;
        col = rand() % B.size;
    } while (!check_valid_position(B, row, col));
    
    TextColor(13);
    cout << "HINT: " << static_cast<char>(col + 'A') << row << endl;
    TextColor(15);
}

/// MOVE AROUND BOARD WITH ARROW KEYS ///
/// AND PUT CURRENT PLAYER'S MARKER INTO A SQUARE///
bool move(Board &B, char marker){
    int xCor = 12, yCor = 3;
    char but;
    while (true){
        gotoXY(xCor, yCor);
        but = _getch();

        if (but == 77){ // Left arrow
            xCor += 8;
            if (xCor > 12 + 8*(B.size-1)){
                xCor = 12 + 8*(B.size-1);
            }
        }
        else if (but == 75){ // Right arrow
            xCor += -8;
            if (xCor < 12){
                xCor = 12;
            }
        }
        else if (but == 72){ // Up arrow
            yCor += -3;
            if (yCor < 3){
                yCor = 3;
            }
        }
        else if (but == 80){ // Down arrow
            yCor += 3;
            if (yCor > 3 + 3*(B.size-1)){
                yCor = 3 + 3*(B.size-1);
            } 
        }
        else if (but == 'h' || but == 'H'){
            gotoXY(0,3 + 3*(B.size-1) + 4); // Print HINT under board, not right at the cursor
            move_suggestion(B);
        }
        else if (but == 13) // Enter button
            break;
	}
    
    int col = change_xCor_into_col(xCor);
    int row = change_yCor_into_row(yCor);
    
    if (!check_valid_position(B, row, col)){
        cout << "\a";
        return false;
    }
    else {
        B.board[row][col] = marker; // Put marker into chosen square
        return true;
    }
}

/// 2 GAME MODES ///
/// CHANGE PLAYER'S TURN CONSECUTIVELY ///

/* EVERY TURN INCLUDES : 
 *  PRINT BOARD, 
 *  INFORM WHOSE TURN IT IS, 
 *  PLAYER MOVING AROUND (CHECK VALID MOVE), 
 *  CHECK IF EITHER PLAYER WINS AND ANNOUNCE WINNER,
 *  INCREASE THE NUMBER OF TIMES A PLAYER WINS
 */

/// TWO PLAYERS MODE ///
void pvp(Board B, Player P[]){
    char col;
    int row;

    for (int i = 0; i < B.size*B.size; i++){
        if (!(i % 2)){
            print_board(B, P[0].marker, P[1].marker);
            TextColor(11);
            cout << P[0].name;
            TextColor(15);
            cout << "'s turn. Press H to get hint.\n";
            
            // Moving and check valid move are combined
            if (!move(B, P[0].marker)){
                system("cls");
                i--;
                continue;
            }
            
            if (check_winner(B)){
                P[0].won += 1;

                system("cls");
                print_board(B, P[0].marker, P[1].marker);
                cout << "\n";
                
				cout << "\t";
                for (int j = 0; j < B.size*8 + 1; j++){
                	cout << "=";
				}
                cout << "\n";
                cout << "\t ";
                TextColor(11);
                cout << P[0].name;
                TextColor(14);
                cout << " wins the game!\n";
                TextColor(15);
                cout << "\t";
                for (int j = 0; j < B.size*8 + 1; j++){
                	cout << "=";
				}
                cout << "\n";

                getch();
                return;
            }
        }
        else {
            print_board(B, P[0].marker, P[1].marker);
            TextColor(10);
            cout << P[1].name;
            TextColor(15);
            cout << "'s turn. Press H to get hint.\n";

            if (!move(B, P[1].marker)){
                system("cls");
                i--;
                continue;
            }

            if (check_winner(B)){
                P[1].won += 1;

                system("cls");
                print_board(B, P[0].marker, P[1].marker);
                cout << "\n";
                
				cout << "\t";
                for (int j = 0; j < B.size*8 + 1; j++){
                	cout << "=";
				}
                cout << "\n";
                cout << "\t ";
                TextColor(10);
                cout << P[1].name;
                TextColor(14);
                cout << " wins the game!\n";
                TextColor(15);
                cout << "\t";
                for (int j = 0; j < B.size*8 + 1; j++){
                	cout << "=";
				}
                cout << "\n";
                
                getch();
                return;
            }
        }
        system("cls");
    }
    // Check draw after all turns has been taken
    if (!check_winner(B)){
        system("cls");
        print_board(B, P[0].marker, P[1].marker);
        cout << "\n";
                
		cout << "\t";
        for (int j = 0; j < B.size*8 + 1; j++){
        	cout << "=";
		}
        cout << "\n";
        cout << "\t ";
        TextColor(14);
        cout << "Draw!\n";
        TextColor(15);
        cout << "\t";
        for (int j = 0; j < B.size*8 + 1; j++){
        	cout << "=";
		}
        cout << "\n";
        
        getch();
    }
    return;
}

/// SINGLE PLAYER MODE ///
void pve(Board B, Player P[]){
    char col;
    int row;

    for (int i = 0; i < B.size*B.size; i++){
        if (!(i % 2)){
            print_board(B, P[0].marker, P[1].marker);
            TextColor(11);
            cout << P[0].name;
            TextColor(15);
            cout << "'s turn. Press H to get hint.\n";
            
            if (!move(B, P[0].marker)){
                system("cls");
                i--;
                continue;
            }
            
            if (check_winner(B)){
                P[0].won += 1;

                system("cls");
                print_board(B, P[0].marker, P[1].marker);
                cout << "\n";
                
				cout << "\t";
		        for (int j = 0; j < B.size*8 + 1; j++){
		        	cout << "=";
				}
		        cout << "\n";
		        cout << "\t ";
                TextColor(11);
                cout << P[0].name;
                TextColor(14);
                cout << " wins the game!\n";
                TextColor(15);
                cout << "\t";
		        for (int j = 0; j < B.size*8 + 1; j++){
		        	cout << "=";
				}
		        cout << "\n";
                
                getch();
                return;
            }
        }
        else {
            print_board(B, P[0].marker, P[1].marker);
            TextColor(10);
            cout << "Computer";
            TextColor(15);
            cout << "'s turn...\n";

            // Computer making choice
            srand(time(0));
            row = rand() % B.size; 
            col = rand() % B.size;
            if (!check_valid_position(B, row, col)){
                system("cls");
                i--;
                continue;
            }
            else
                B.board[row][col] = P[1].marker;
            
            Sleep(1000);
            system("cls");
            print_board(B, P[0].marker, P[1].marker);
            TextColor(10);
            cout << "Computer";
            TextColor(15);
            cout << "'s turn...\n";
            
            if (check_winner(B)){
                P[1].won += 1;

                system("cls");
                print_board(B, P[0].marker, P[1].marker);
                cout << "\n";
                
				cout << "\t";
		        for (int j = 0; j < B.size*8 + 1; j++){
		        	cout << "=";
				}
		        cout << "\n";
		        cout << "\t ";
                TextColor(11);
                cout << P[0].name;
                TextColor(14);
                cout << " loses the game!\n";
                TextColor(15);
                cout << "\t";
		        for (int j = 0; j < B.size*8 + 1; j++){
		        	cout << "=";
				}
		        cout << "\n";
                
                getch();
                return;
            }
            cout << "Press any key to continue.\n";
            getch();
        }
        system("cls");
    }
    cin.ignore();
    if (!check_winner(B)){
        system("cls");
        print_board(B, P[0].marker, P[1].marker);
        cout << "\n";
                
		cout << "\t";
        for (int j = 0; j < B.size*8 + 1; j++){
        	cout << "=";
		}
        cout << "\n";
        cout << "\t ";
        TextColor(14);
        cout << "Draw!\n";
        TextColor(15);
        cout << "\t";
        for (int j = 0; j < B.size*8 + 1; j++){
        	cout << "=";
		}
        cout << "\n";
        
        getch();
    }
    return;
}

/// PRINT STATISTICAL OUTCOME ///
void statistical_outcome(Player P[], int match, int mode, Board B){ 
    int len0 = 0, len1 = 0;  
    for (int i = 0; P[0].name[i] != '\0'; i++){
        len0++; // 5 
    }
    if (len0%2) len0++;

    for (int i = 0; P[1].name[i] != '\0'; i++){
        len1++;
    }
    if (len1%2) len1++;

    cout << "\n";
    cout << "\t===========================================================\n";
    cout << "\t|";
    TextColor(13);
    cout << " Number of matches: ";
    TextColor(15);
    cout << setw(2) << match << "                                   |\n";
    cout << "\t| Mode: ";                                                                                      
    (mode == 1) ? cout << "Single Player Game                                |\n" : cout << "Two Players Game                                  |\n";
    cout << "\t| Size: " << setw(2) << B.size << "                                                |\n";
    cout << "\t|=========================================================|\n";

    cout << "\t| ";
    for (int i = 0; i < (26 - len0)/2; i++) cout << " ";
    TextColor(11);
    cout << setw(len0) << P[0].name;
    TextColor(15);
    for (int i = 0; i < (26 - len0)/2; i++) cout << " ";
    cout << " | ";
    for (int i = 0; i < (26 - len1)/2; i++) cout << " ";
    TextColor(10);
    cout << setw(len1) << P[1].name;
    TextColor(15);
    for (int i = 0; i < (26 - len1)/2; i++) cout << " ";
    cout << " |\n";
    cout << "\t|----------------------------|----------------------------|\n";
    cout << "\t|             " << setw(2) << P[0].won << "             |             " << setw(2) << P[1].won << "             |\n";
    cout << "\t===========================================================\n";
}

/// PRINT REPLAY OPTIONS AND GET USER'S CHOICE ///
bool replay_game(){
    int replay_choice;

    do {
        TextColor(14);
        cout << "Replay?\n";
        TextColor(15);
        cout << "\n";
        cout << "1. Yes\n";
        cout << "2. No\n";
        cout << "\n";
        cout << "Enter your choice: ";
        cin >> replay_choice;
        cin.ignore();
        if (replay_choice != 1 && replay_choice != 2){
            print_error();
            system("cls");
        }
    } while (replay_choice != 1 && replay_choice != 2);

    if (replay_choice == 1) return true;
    else return false;
}

/// START GAME ///
void start_game(Board &B, Player P[]){
    int menu_choice = print_main_menu();
    
    while (menu_choice != 4){
        bool play = true;

        int match = 1;
        P[0].won = 0;
        P[1].won = 0;
        
        system("cls");
        print_board_options(B);
        cin.ignore();
        system("cls");

        // Two players game
        if (menu_choice == 2) {
            TextColor(11);
            cout << "Player 1: "; 
            TextColor(15);
            cout << P[0].name << "\n";
            cout << "Choose your marker: "; 
            cin >> P[0].marker;
            cin.ignore();

            TextColor(10);
            cout << "Player 2:\n";
            TextColor(15);
            cout << "Enter your name: ";
            cin.getline(P[1].name, MAX);
            cout << "Choose your marker: ";
            cin >> P[1].marker;
            system("cls");
        }
        // Single player game
        else if (menu_choice == 1) {
            TextColor(11);
            cout << "Player: ";
            TextColor(15);
            cout << P[0].name << "\n";
            cout << "Choose your marker: ";
            cin >> P[0].marker;

            TextColor(10);
            cout << "Computer:\n";
            TextColor(15);
            memcpy(P[1].name, "Computer", MAX);
            cout << "Choose computer's marker: ";
            cin >> P[1].marker;
            system("cls");
        }
        
        // Game starts here
        while (play){
            system("cls");
            reset_board(B);
            if (menu_choice == 1){
                pve(B, P);
            }
            if (menu_choice == 2){
                pvp(B, P);
            }
            system("cls");
            statistical_outcome(P, match, menu_choice, B);
            // Ask user if he/she wants to replay
            play = replay_game();
            if (play) match++;
        }
        system("cls");
        menu_choice = print_main_menu();
    }
    
    /// QUIT (menu_choice == 4) ///
    /// GOODBYE MESSAGES ///
    for (int i = 9; i <= 15; i++){
        system("cls");
        cout << "\n";
        TextColor(i);
        cout << "\t===========================================================\n";
        cout << "\t|                                                         |\n";
        cout << "\t|                                                         |\n";
        cout << "\t|               Thanks for playing my game!               |\n";
        cout << "\t|                                                         |\n";
        cout << "\t|                                                         |\n";
        cout << "\t|                Designed by Ngo Ngoc Lien.               |\n";
        cout << "\t|                         12/2021                         |\n";
        cout << "\t|                                                         |\n";
        cout << "\t===========================================================\n";
        Sleep(1000);
    }
    sound_Noel();
}

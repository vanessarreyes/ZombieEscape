#include "ZombieEscape.hpp"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <stdio.h>
using std::flush;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

// function to print a message as if it is being typed
// slow_print(parse, 30);
void slow_print(const string& message, unsigned int millis_per_char) {
  for (const char c: message) {
    cout << c << flush;
    sleep_for(milliseconds(millis_per_char));
  }
  cout << endl;
}

// function to print out menu
void Menu() {
  slow_print("----- Menu -----", 30);
  slow_print("1. Instructions", 30);
  slow_print("2. Begin Survival", 30);
  slow_print("3. High Scores", 30);
  slow_print("4. Resume Saved Game", 30);
}

void noSaveMenu() {
  slow_print("----- Menu -----", 30);
  slow_print("1. Instructions", 30);
  slow_print("2. Begin Survival", 30);
  slow_print("3. High Scores", 30);
}

// function that outputs the game instructions
void Instructions() {
  cout << endl;
  string message = "----- Instructions -----";
  slow_print(message, 30);
  message = "1. When menu is presented, insert 2 to begin gameplay";
  slow_print(message, 30);
  message = "2. At the beginning of the game, you will chose a character";
  slow_print(message, 30);
  message = "   - each character's information will be presented";
  slow_print(message, 30);
  message = "   - each character has a weapon and two strengths and weaknesses";
  slow_print(message, 30);
  message = "   - each character's strengths and weaknesses will contribute to your gameplay";
  slow_print(message, 30);
  message = "3. After you have chosen a character, the game will begin";
  slow_print(message, 30);
  message = "4. During the gameplay, you will be given options to choose from to survive the Zombie Apocalypse. Choose which option you prefer and see how long you will survive.";
  slow_print(message, 30);
  message = "5. If you want to save your game to play later, press 0 when asked an option during a game level.";
  slow_print(message, 30);
  message = "6. Best of luck!";
  slow_print(message, 30);
  cout << endl;
}

// constructor
CharacterStory::CharacterStory() {
  head = NULL;
  numCharacters = 0;
  currentCharacter = 1;
  endGame = false;
  win = false;
  save = false;
}

// function that deletes the binary tree for linked list
// recursive function
void CharacterStory::deleteTree(StoryTree* Tnode) {
  // if that cheks if node is NULL
  if (Tnode != NULL)	{
    // call recursion on left node
    deleteTree(Tnode->leftChoice);
    // call recusrions on right node
    deleteTree(Tnode->rightChoice);
    // delete node
    delete Tnode;
  }
}

// destructor
CharacterStory::~CharacterStory() {
  // pointer to head of linked list
  LLCharacters* node = head;
  // while loop that checks if node is NULL
  while (node != NULL) {
    // pointer to next node in linked lsit
    LLCharacters* temp = node->next;
    // if that checks if root(parent) of node's binary tree is NULL
    if (node->parent == NULL) {
      // delete the node's binary tree
      deleteTree(node->parent);
    }
    // delete node
    delete node;
    // node is now set to next node in linked list
    node = temp;
  }
}

// function that takes user information from ifstream and parses it to create/insert new node in linked list
void CharacterStory::newCharacter(string line, int count) {
  LLCharacters* node = new LLCharacters;
  // set line to stringstream
  string parse;
  stringstream ss(line);
  while(getline(ss, parse, ',')) {
    // pointer to a new node being created
    switch (count) {
      // case to insert the name of character
      case 1: {
        node->name = parse;
        break;
      }
      // case to insert the profession of character
      case 2: {
        node->profession = parse;
        break;
      }
      // case to insert the weapon of character
      case 3: {
        node->weapon = parse;
        break;
      }
      // case to insert the first strength of character
      case 4: {
        node->strengths[0] = parse;
        break;
      }
      // case to insert the second strength of character
      case 5: {
        node->strengths[1] = parse;
        break;
      }
      // case to insert the first weakness of character
      case 6: {
        node->weaknesses[0] = parse;
        break;
      }
      // case to insert the second weakness of character
      case 7: {
        node->weaknesses[1] = parse;
        break;
      }
    }
    // if that checks if count is 7
    // put new node into list, update current node, reset count
    if (count == 7) {
      LLCharacters* current = head;
      if (head == NULL) {
        head = node;
        head->prev = NULL;
      }
      else {
        while (current->next != NULL) {
          current = current->next;
        }
        current->next = node;
        current->next->prev = current;
      }
    }
    // else add one to count
    else {
      count++;
    }
    // reset parse
    parse = "";
  }
}

// function that inserts information for each character in linked lists
void CharacterStory::importCharacters(string filename) {
  // ifstream to stream file containing characters
  ifstream fileCharacters;
  fileCharacters.open(filename);

  // declare variables
  string line;
  int count = 1;
  int num = 0;
  // pointer to head of linked list
  while(getline(fileCharacters, line)) {
    // call function to add character to linked list
    newCharacter(line, count);
    // add one to number of characters imported
    num++;
  }
  // set number of characters to num
  numCharacters = num;

  LLCharacters* node = head;
  num = 0;
  while(node != NULL) {
    num++;
    node->characterNum = num;
    node = node->next;
  }
}

// function to return the number of characters in game
int CharacterStory::getNumCharacters() {return numCharacters;}

// function to read saved game file and insert data in queue
void CharacterStory::uploadSaveGame(queue<char>* q) {
  string line, parse;

  // read text file
  ifstream myFile;
  // open text file
  myFile.open("save_game.txt");
  if (myFile.is_open()) {
    // while loop that reads each line
    while(getline(myFile, line)) {
        // stringstream that represents each line
        stringstream ss(line);
        // while loop that parses each line
        while(getline(ss, parse, ',')) {
          // push each parse into queue (represents player choices)
          q->push(parse[0]);
      }
    }
  }
}

// helper function to return the head of the linked list
LLCharacters* CharacterStory::getHeadLL() {return head;}

// helper function to create new node with message in tree
StoryTree* newStoryNode(string message) {
  StoryTree* node = new StoryTree;
  node->message = message;

  node->leftChoice = NULL;
  node->rightChoice = NULL;
  return node;
}

// function to insert the messages from vector that holds messages in text file
// into a binary tree using levelOrder insert
StoryTree* insertNodeInTree(StoryTree* root, string message, queue<StoryTree*>& q) {
  StoryTree* newNode = newStoryNode(message);
  if (root == NULL) {
    root = newNode;
  }
  // the left child of the current Node is used if available
  else if (q.front()->leftChoice == NULL) {
    q.front()->leftChoice = newNode;
  }
  // the right child of the current Node is used if available
  // Since the left child of this Node is has already been used, the Node is popped
  // from the queue fter using its right child
  else {
    q.front()->rightChoice = newNode;
    q.pop();
  }

  // whenever a new Node is added to the tree, its address is pushed into the queue.
  // So that its children Nodes can be used later
  q.push(newNode);
  return root;
}

// function to print out level order traverse
void levelOrder(StoryTree* node) {
  if (node == NULL) return;
  queue<StoryTree*> q;
  q.push(node);
  while (!q.empty()) {
    cout << q.front()->message;
    cout << endl;
    if (q.front()->leftChoice != NULL) q.push(q.front()->leftChoice);
    if (q.front()->rightChoice != NULL) q.push(q.front()->rightChoice);
    q.pop();
  }

}

// function to import the story of each character
void CharacterStory::importCharacterStory(LLCharacters* character, string filename) {
  // ifstream to stream file containing characters
  ifstream fileCharacters;
  fileCharacters.open(filename);

  // declare variables
  string line;
  string parse;
  int numCharacters = 1;
  // vector that holds all the text for each story, parsed
  vector<string> Story;
  // queue for insertNodeInTree function
  queue<StoryTree*> q;
  // pointer to head of linked list
  while(getline(fileCharacters, line)) {
    // stringstream
    stringstream ss(line);
    // while loop that parses the story text file
    while (getline(ss, parse, '~')) {
      // insert the parsed text into the vector Story
      Story.push_back(parse);
    }
  }

  // for loop that inserts each of the messages in vector Story into a binary tree by level order
  for (int i = 0; i < Story.size(); i++) {
    character->parent = insertNodeInTree(character->parent, Story[i], q);
  }
  // function to print out binary tree by level order
  // levelOrder(character->parent);
}

// function to print a chracter's information to console
void printCharacterInfo(LLCharacters* node) {
  string message;

  message = "Name: " + string(node->name);
  slow_print(message, 30);
  message = "Profession: " + string(node->profession);
  slow_print(message, 30);
  message = "Weapon: " + string(node->weapon);
  slow_print(message, 30);
  message = "Strengths: ";
  slow_print(message, 30);
  message = " - " + string(node->strengths[0]);
  slow_print(message, 30);
  message = " - " + string(node->strengths[1]);
  slow_print(message, 30);
  message = "Weaknesses: ";
  slow_print(message, 30);
  message = " - " + string(node->weaknesses[0]);
  slow_print(message, 30);
  message = " - " + string(node->weaknesses[1]);
  slow_print(message, 30);
  cout << endl;
}

// function that allows the user to pick their chracter for game
LLCharacters* CharacterStory::chooseCharacter() {
  // pointers to the head of linked list and previous node from head
  LLCharacters* node = head;
  LLCharacters* prev = head->prev;

  // display messages
  cout << endl;
  string message;
  message =  "----- Choose a Character -----";
  slow_print(message, 30);
  message = "Below is the list of characters to choose from. Characters will be displayed in order with details about each character.";
  slow_print(message, 30);
  message = "When viewing a character, if you want to begin the game with the current character you are viewing just input the number presented on the screen.";
  slow_print(message, 30);
  message = "If you want to view the next/previous character just input the number presented on the screen.";
  slow_print(message, 30);
  message = "List of characters:";
  slow_print(message, 30);
  message = " - Alek Ramirez";
  slow_print(message, 30);
  message = " - Andrea Pack";
  slow_print(message, 30);
  message = " - Vincente Ripp";
  slow_print(message, 30);
  cout << endl;

  // declare variables
  bool correct = false;
  bool begin = false;
  string input = "";
  int num = -1;
  int numCharacters = 1;
  // while that checks if the user has not chosen a character
  while (begin == false) {
    message = "--- Current Character ---";
    slow_print(message, 30);
    // display the current character's information
    printCharacterInfo(node);

    // give player options
    message = "1. Begin Game as " + string(node->name);
    slow_print(message, 30);
    // if that checks if the number of characters is 1
    // displays selected menu
    if (numCharacters == 1) {
      message = "2. Next Character";
      slow_print(message, 30);
    }
    // if that checks if the number of characters is 4
    // displays selected menu
    else if (numCharacters == 3) {
      message = "2. Previous Character";
      slow_print(message, 30);
    }
    // all else
    else {
      message = "2. Next Character";
      slow_print(message, 30);
      message = "3. Previous Character";
      slow_print(message, 30);
    }

    // while loop that checks if the user entered in a valid input
    while (correct == false) {
      // get user input
      getline(cin, input);
      int count = 0;
      // while loop that checks if the string is a digit
      while (isdigit(input[count])) {
        // add one to count
        count++;
      }
      // if that checks if count is the length of the user input and the input is less than 3 and greater than 0
      if (count == input.length() && (num = stoi(input)) < 4 && (num = stoi(input)) > 0) {
        // if that checks if the input is greater than 2 and number of characters is 1
        // also checks if input is greater than 2 and number of characters is 4
        // displays message
        if ((numCharacters == 1 && (num = stoi(input)) > 2) || (numCharacters == 4 && (num = stoi(input)) > 2)) {
            slow_print("Please enter in a valid input.", 30);
        }
        // all else
        else {
          // set correct to true
          correct = true;
        }
      }
      // all else
      else {
        slow_print("Please enter in a valid input.", 30);
      }
    }
    // set correct to false to prevent forever loop
    correct = false;

    // if that checks if num is 1
    // calls function with given character node
    if (num == 1) {
      // set begin to true
      begin = true;
      // return the node of the choosen character
      return node;
    }
    // if that checks if num is 2
    if (num == 2) {
      // if that checks if the number of characters is 4
      // based on selected menu, if true will update to previous character in list
      if (numCharacters == 3) {
        // set current node to prev
        node = prev;
        // set prev node to previous node
        prev = prev->prev;
        // subtract one from number of characters
        numCharacters--;
      }
      // all else
      // go to next character in list
      else {
        // prev node is current node
        prev = node;
        // current node is next node
        node = node->next;
        // add one to character
        numCharacters++;
      }
    }
    // all else
    else {
      // set node to previous node
      node = prev;
      // if that checks if the previous node is the head of LL
      if (prev != head) {
        // make prev to be previous node before it
        prev = prev->prev;
      }
      // subtract one from the number of characters
      numCharacters--;
    }
    cout << endl;
  }
  return node;
}

// function that allows the user to chose which option from each story tree node
StoryTree* playerChoice(StoryTree* node, queue<char>& q, bool& end, bool& win, bool& save) {
  stringstream ss(node->message);
  string parse;

  while(getline(ss, parse, '/')) {
    // CHANGE PACE FROM 10 TO 35
    // if parse has # at the end then player lost game
    if (parse[parse.length()-1] == '#') {
      // remove last character in parse
      parse.pop_back();
      // slow print parse
      slow_print(parse, 30);
      // end game is true
      end = true;
      // return the current node
      return node;
    }
    // else if that checks if parse has $ at end then player won game
    else if (parse[parse.length()-1] == '$') {
      // remove last character in parse
      parse.pop_back();
      // slow print parse
      slow_print(parse, 30);
      // win game is true
      win = true;
      // return current node
      return node;
    }
    else {
      // slow print the parse
      slow_print(parse, 30);
    }
  }

  bool correct = false;
  string input;
  int num;
  // while loop that checks if the user entered in a valid input
  while (correct == false && end == false && win == false) {
    // get user input
    getline(cin, input);
    int count = 0;
    // while loop that checks if the string is a digit
    while (isdigit(input[count])) {
      // add one to count
      count++;
    }
    // if that checks if count is the length of the user input and the input is less than 3
    if (count == input.length() && (num = stoi(input)) < 3 && (num = stoi(input)) > -1) {
      // set correct to true
      correct = true;
    }
    // all else
    else {
      slow_print("Please enter in a valid input.", 30);
    }
  }
  cout << endl;

  // if that checks if player choice is 1
  if (num == 1) {
    // push L to represent leftChoice
    q.push('L');
    // return the left node from current node
    return node->leftChoice;
  }
  // else if player choice is 2
  else if (num == 2) {
    // oush R to represent rightChoice
    q.push('R');
    // return the right node from current node
    return node->rightChoice;
  }
  // else if the user chose 0 (save the game)
  else {
    // save is true
    save = true;
    // return the current node
    return node;
  }
}

// function that begins the game with the given character
// function takes StoryTree node if game is started from saved game
bool CharacterStory::startGame(LLCharacters* character) {
  cout << endl;
  cout << "Your Journey" << endl;
  cout << "------------------------" << endl;
  cout << endl;
  bool stopGame;
  // queue that holds player's choices
  queue<char> q;
  StoryTree* node;
  // if that checks if the player is starting game from beginning
  // if character is NULL then starting from saved game
  if (character == NULL) {
    // slow print message
    slow_print("Uploading saved game: ", 30);
    // queue that holds saved game info
    queue<char> saved_q;
    int characterValue, count = 1;
    // call function to get saved game info
    uploadSaveGame(&saved_q);
    // while loop that checks if the saved queue is empty
    while (!saved_q.empty()) {
      // if count is 1 then queue position holds interger value
      if (count == 1) {
        // push value from saved_q to new queue
        q.push(saved_q.front());
        // convert char to integer (-48 bcs of ASCii value)
        characterValue = int(saved_q.front()) - 48;
        // pop value from queue
        saved_q.pop();

        // pointer to head of linked list
        LLCharacters* temp = head;
        int num = 1;
        // while loop that runs through linked list until character is found
        while (num != characterValue) {
          // add one to num
          num++;
          // update temp to next node in list
          temp = temp->next;
        }
        // set temp node to character
        character = temp;
        // set node to the parent of the binary tree of character
        node = character->parent;
        // add one to count
        count++;
      }
      // else queue holds player choices
      else {
        // push value from saved_q to new queue
        q.push(saved_q.front());
        // if that checks if the value from saved_q
        if (saved_q.front() == 'L') {
          // update current node to leftChoice
          node = node->leftChoice;
        }
        // all else
        else {
          // update current node to rightChoice
          node = node->rightChoice;
        }
        // pop value from saved q
        saved_q.pop();
      }
    }
    // print character info
    printCharacterInfo(character);
    // slow print message
    slow_print("Saved Story: ", 30);
    cout << endl;
  }
  // else starting from beginning
  else {
    node = character->parent;
  }

  // while loop that check if the game is still continuing
  while (endGame == false && win == false && save == false) {
    // call playerChoice to update current node to continue story
    node = playerChoice(node, q, endGame, win, save);
  }

  // if that checks if endGame is true
  // lost game
  if (endGame == true) {
    // slow print message
    slow_print("You lost.", 30);
    cout << endl;
    // call gameOver to see if player wants to play again
    return stopGame = gameOver();
  }
  // else if that checks if player won
  else if (win == true){
    // slow print message
    slow_print("You won!", 30);
    cout << endl;
    // if that checks if player received new high score to be set in text file
    getWinScore(character, &q);
    // call game over to see if player wants to play again
    return stopGame = gameOver();
  }
  // all else
  else {
    // user wants to save the game
    slow_print("Your game has been saved.", 30);
    // call saveGame to save player game info
    saveGame(character->characterNum, q);
    // return true to end game
    return true;
  }
}

// function that asks the user if they want to start another game or exit
bool CharacterStory::gameOver() {
  slow_print("Would you like to play another game?", 30);
  slow_print("1. Yes", 30);
  slow_print("2. No", 30);

  // declare variables
  bool correct = false;
  string input = "";
  int num = -1;
  // while loop that checks if the user entered in a valid input
  while (correct == false) {
    // get user input
    getline(cin, input);
    int count = 0;
    // while loop that checks if the string is a digit
    while (isdigit(input[count])) {
      // add one to count
      count++;
    }
    // if that checks if count is the length of the user input and the input is less than 3
    if (count == input.length() && (num = stoi(input)) < 3) {
      // set correct to true
      correct = true;
    }
    // all else
    else {
      slow_print("Please enter in a valid input.", 30);
    }
  }
  // if that checks if num is 1 (play again)
  if (num == 1) return false;
  // all else (don't play again)
  return true;
}

// function that saves the player's game in a text file
void CharacterStory::saveGame(int characterValue, queue<char>& q) {
  // ofstream to output to text file
  ofstream toFile;
  // open file to output to
  toFile.open("save_game.txt");
  // output character value to text file
  toFile << characterValue << ",";
  // while loop that outputs players's choices from queue to file to save game
  while (!q.empty()) {
    toFile << q.front() << ",";
    q.pop();
  }
  // close file
  toFile.close();
}

// function to change save file game depending if user saved game or not
void CharacterStory::checkSaveFile() {
  // if save is true
  if (save == false) {
    // write to file
    ofstream File;
    File.open("save_game.txt");
    File << "NO";
  }
}

// function to reset game varaibles if playter plays again
void CharacterStory::resetGameVariables() {
  endGame = false;
  win = false;
  save = false;
}

// function that takes queue that holds player's choices
// counts player's moves and puts them in high score text file if new high score
void CharacterStory::getWinScore(LLCharacters* character, queue<char>* q) {
  // declare variables
  int score, newScore, count = 0, playerNum = character->characterNum;
  string line, parse, updatedScore, name;
  bool updated = false;
  string scores[numCharacters*3];

  // set new score to player's number of choices
  newScore = (q->size());
  // if stream to read text file
  ifstream scoreFile;
  // open text file
  scoreFile.open("highscores.txt");
  // while that reads text file
  while (getline(scoreFile, line)) {
    // string stream of text line
    stringstream ss(line);
    // while that parses line
    while (getline(ss, parse, ',')) {
      // insert parse into array
      scores[count] = parse;
      count++;
    }
  }
  // close text file
  scoreFile.close();

  // switch case that sets place to correct value based on character number
  int place;
  switch(playerNum) {
    case 1: {
      place = 0;
      break;
    }
    case 2: {
      place = 3;
      break;
    }
    case 3: {
      place = 6;
      break;
    }
  }

  // set score to the current highest score in array for character
  score = stoi(scores[place + 2]);
  // if that checks if the new score is less than or there is no high score
  if (newScore < score || score == -1) {
    // set updated score to string
    updatedScore = to_string(newScore);
    // get players name
    slow_print("Congrats you reached a new high score! Please enter in your name.", 30);
    getline(cin, name);
    cout << endl;
    // set high score player name to new name
    scores[place] = name;
    // update the high score
    scores[place + 2] = updatedScore;

    // declare variables
    count = 0;
    int space = 1;
    // output to text file
    ofstream toFile;
    // open text file
    toFile.open("highscores.txt");
    // for loop that runs through array and outputs to text file
    for (int i = 0; i < numCharacters*3; i++) {
      if (space == 3) {
        if (i == (numCharacters*3 - 1)) {
          toFile << scores[i] << ",";
        }
        else {
          toFile << scores[i] << "," << endl;
          space = 1;
        }
      }
      else {
        toFile << scores[i] << ",";
        space++;
      }
    }
    // close text file
    scoreFile.close();
  }
}

void CharacterStory::getHighScores() {
  string message;
  message = "---- High Scores ----";
  slow_print(message, 30);
  message = "The high scores represent the least amount of choices it took the player to survive a game.";
  slow_print(message, 30);
  cout << endl;
  // declare variables
  string line, parse, name, character, score;
  int count = 1;
  // read text file
  ifstream myFile;
  // open text file
  myFile.open("highscores.txt");
  // while that reads each line
  while (getline(myFile, line)) {
    // stringstream
    stringstream ss(line);
    // parse each line
    while(getline(ss, parse, ',')) {
      // switch that sets parse to correct string value
      switch(count) {
        case 1: {
          name = parse;
          break;
        }
        case 2: {
          character = parse;
          break;
        }
        case 3: {
          score = parse;
          break;
        }
      }
      // add one to count
      count++;
    }
    // display message
    message = "High Score for " + character + ":";
    slow_print(message, 30);
    if (name == "NAME") {
      message = "There is no high score for this character.";
      slow_print(message, 25);
      cout << endl;
    }
    else {
      message = " - Player name, Score: " + name + ", " + score;
      slow_print(message, 30);
      cout << endl;
    }
    count = 1;
  }

}

int main(int argc, char* argv[]) {
  // create object of class
  CharacterStory ZombieGame;
  // import the information of the characters
  ZombieGame.importCharacters(argv[1]);
  // pointer to the head of the linked list
  LLCharacters* node = ZombieGame.getHeadLL();
  // int that represents the files sent through terminal
  int importedFile = 2;
  int numCharacters = ZombieGame.getNumCharacters();
  for (int i = 0; i < numCharacters; i++) {
    // import the character's story for all characters
    ZombieGame.importCharacterStory(node, argv[importedFile]);
    importedFile++;
    node = node->next;

  }

  cout << "=======================================================================================" << endl;
  cout << "                                     Zombie Escape                                     " << endl;
  cout << "=======================================================================================" << endl;
  cout << endl;

  bool quit = false;
  // while that checks if the game ends
  while (quit == false) {
    // decalre variables
    bool no_save = false;
    string Fline = "";
    // if stream to read save game file
    ifstream myFile;
    // open file
    myFile.open("save_game.txt");
    // while that reads the text file
    while(getline(myFile, Fline)) {
      // if the text file line has NO
      if (Fline == "NO") {
        // there is no save
        no_save = true;
      }
    }

    // if there is not save, print out no save Menu
    if (no_save == true) {
      noSaveMenu();
    }
    // else print regular menu
    else {
      Menu();
    }

    // declare variables
    bool correct = false;
    string input = "";
    int num = -1;
    // while loop that checks if the user entered in a valid input
    while (correct == false) {
      // get user input
      getline(cin, input);
      int count = 0;
      // while loop that checks if the string is a digit
      while (isdigit(input[count])) {
        // add one to count
        count++;
      }
      // if that checks if count is the length of the user input and the input is less than 3
      if (count == input.length() && (num = stoi(input)) < 5 && (num = stoi(input)) > 0) {
        if ((num = stoi(input)) == 4 && no_save == true) {
          slow_print("Please enter in a valid input.", 30);
        }
        else {
          // set correct to true
          correct = true;
        }
      }
      // all else
      else {
        slow_print("Please enter in a valid input.", 30);
      }
    }
    correct = false;

    // can be 1-4 representing the characters
    int characterNum = -1;
    string filename;
    // ifs that check value of num and call correct function with number value
    // display instructions
    if (num == 1) {Instructions();}
    // calls function to allow user to choose character
    else if (num == 2) {
      // call chooseCharacter function
      LLCharacters* character = ZombieGame.chooseCharacter();
      quit = ZombieGame.startGame(character);
    }
    // calls function to see high scores
    else if (num == 3) {
      ZombieGame.getHighScores();
    }
    // else game is going to be uploaded from saved game
    else {
      quit = ZombieGame.startGame(NULL);
    }
    // call function to see if the user saved game or not
    ZombieGame.checkSaveFile();

    // if that checks if game variables need to be rest if player wants to play another game
    if (quit == false) {
      ZombieGame.resetGameVariables();
    }
  }
  return 0;
}

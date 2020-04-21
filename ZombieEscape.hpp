#include <iostream>
#include <string>
#include <queue>
using namespace std;


struct StoryTree {
  string message;                 // message displayed to the user at location

  StoryTree* leftChoice;          // pointer to the left choice of game
  StoryTree* rightChoice;         // pointer to the right choice of game
};

// linked list that holds all the characters info
// each character has own story line
struct LLCharacters {
  string name;                    // name of the character
  string profession;
  string weapon;                  // character's weapon
  string strengths[2];            // charcater's strengths
  string weaknesses[2];           // character's weaknesses
  int characterNum;               // character's number value

  StoryTree* parent;              // pointer to the binary tree's parent/character's story
  LLCharacters* next;             // pointer to the next character in the linked list
  LLCharacters* prev;
};


class CharacterStory {
  private:
    LLCharacters* head;           // pointer to the head of linked list (characters)
    int numCharacters;            // int that represents number of characters in game
    int currentCharacter;         // int that represents chosen character
    bool endGame;                 // bool that represents if game needs to end
    bool win;                     // bool that represents if player has won
    bool save;                    // bool that represents if player wants to save game and end

  public:
    CharacterStory();                           // constructor
    ~CharacterStory();                          // deconstructor
    void deleteTree(StoryTree*);                // function to delete binary tree for destructor
    void newCharacter(string line, int count);  // function to add new node to linked list/character
    void importCharacters(string file);         // function to import characters
    int getNumCharacters();                     // function to return number of chracters in linked list
    void uploadSaveGame(queue<char>* q);        // function that uploads saved game for player to play
    LLCharacters* getHeadLL();                  // function to get the head of the Linked List
    void importCharacterStory(LLCharacters* node, string file);  // function to import a character's story
    LLCharacters* chooseCharacter();            // function to choose character
    bool startGame(LLCharacters* character);    // function to start the game
    bool gameOver();                            // function to end game
    void saveGame(int characterValue, queue<char>& q);   // function that sends game info to text file to save
    void checkSaveFile();                       // function to check if player saved game or not
    void resetGameVariables();                  // function that resets the classes private varaibles
    void getWinScore(LLCharacters* node, queue<char>* q); // function that takes winning score and inserts into text file
    void getHighScores();                       // function to display high scores
};

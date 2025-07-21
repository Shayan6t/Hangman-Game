#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

class WordManager
{
private:
    char words[3][5][20];
    const char* categories[3] = { "Fruits", "Clothes", "Animals" };
    int categoryIndex;
public:
    WordManager();
    void loadWords();
    void setCategory(int index);
    const char* getCategoryHint() const;
    const char* getRandomWord() const;
};

class Game
{
private:
    int lives;
    char currentWord[20];
    char guessedWord[20];
    int currentWordLength;
    int guessedWordLength;
    WordManager wordManager;
    int alphabetStatus[26];
    int score;
    sf::Texture hangmanTextures[7];
    sf::Sprite hangmanSprite;
    void handleInput(char letter);
    bool isWordGuessed() const;
    void update(sf::RenderWindow& window);
public:
    Game();
    void resetGame();
    void start();
    void render(sf::RenderWindow& window);
};
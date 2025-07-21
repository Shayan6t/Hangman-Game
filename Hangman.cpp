#include "Hangman.h"
#include <fstream>
#include <iostream>
using namespace std;

WordManager::WordManager() : categoryIndex(0)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            words[i][j][0] = '\0';
        }
    }
    srand(static_cast<unsigned>(time(0)));
}

void WordManager::loadWords()
{
    const char* filenames[3] = { "fruits.txt", "clothes.txt", "animals.txt" };
    for (int i = 0; i < 3; i++)
    {
        ifstream file(filenames[i]);
        if (!file.is_open())
        {
            cout << "Error: Could not open file " << filenames[i] << endl;
            continue;
        }
        for (int j = 0; j < 5 && file.getline(words[i][j], 20); j++);
        file.close();
    }
}

void WordManager::setCategory(int index)
{
    categoryIndex = index;
}

const char* WordManager::getCategoryHint() const
{
    return categories[categoryIndex];
}

const char* WordManager::getRandomWord() const
{
    return words[categoryIndex][rand() % 5];
}

Game::Game() : lives(6), currentWordLength(0), guessedWordLength(0), score(0)
{
    wordManager.loadWords();
    for (int i = 0; i < 26; i++)
    {
        alphabetStatus[i] = 0;
    }
    //Randomly Select Category
    int randomCategoryIndex = rand() % 3;
    wordManager.setCategory(randomCategoryIndex);
    const char* word = wordManager.getRandomWord();
    int i = 0;
    while (word[i] != '\0' && i < 20)
    {
        if (word[i] >= 'a' && word[i] <= 'z')
        {
            currentWord[i] = word[i] - ('a' - 'A');
        }
        else
        {
            currentWord[i] = word[i];
        }
        i++;
    }
    currentWordLength = i;
    currentWord[i] = '\0';
    for (int j = 0; j < currentWordLength; j++)
    {
        guessedWord[j] = '_';
    }
    guessedWord[currentWordLength] = '\0';
    for (int i = 0; i <= 6; i++)
    {
        char filename[20];
        sprintf_s(filename, sizeof(filename), "Pictures/%dwrong.png", i);
        hangmanTextures[i].loadFromFile(filename);
    }
    hangmanSprite.setTexture(hangmanTextures[0]);
    hangmanSprite.setPosition(50, 200);
}

void Game::handleInput(char letter)
{
    if (letter >= 'a' && letter <= 'z')
    {
        letter = letter - ('a' - 'A');
    }
    cout << "User guessed: " << letter << endl;
    cout << "Current word: " << currentWord << endl;
    bool found = false;
    for (int i = 0; i < currentWordLength; i++)
    {
        if (currentWord[i] == letter && guessedWord[i] == '_')
        {
            guessedWord[i] = letter;
            found = true;
        }
    }
    int index = letter - 'A';
    if (found)
    {
        alphabetStatus[index] = 1;
    }
    else if (alphabetStatus[index] == 0)
    {
        alphabetStatus[index] = -1;
        lives--;
        if (lives >= 0)
        {
            hangmanSprite.setTexture(hangmanTextures[6 - lives]);
        }
    }
}

bool Game::isWordGuessed() const
{
    for (int i = 0; i < currentWordLength; i++) 
    {
        if (guessedWord[i] == '_')
            return false;
    }
    return true;
}

void Game::update(sf::RenderWindow& window)
{
    // Game update logic can go here if needed
}

void Game::render(sf::RenderWindow& window)
{
    window.clear();
    sf::Font font;
    font.loadFromFile("Fonts/main.ttf");
    sf::Font font1;
    font1.loadFromFile("Fonts/main2.otf");
    sf::Font font2;
    font2.loadFromFile("Fonts/main1.ttf");

    sf::Text titleText;
    titleText.setFont(font1);
    titleText.setCharacterSize(150);
    titleText.setFillColor(sf::Color::Red);
    titleText.setString("HANGMAN");
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setPosition((1020 - titleBounds.width) / 2, 0);
    window.draw(titleText);

    window.draw(hangmanSprite);

    sf::Text scoreText;
    scoreText.setFont(font1);
    scoreText.setCharacterSize(80);
    char scoreStr[20];
    sprintf_s(scoreStr, sizeof(scoreStr), "Score: %d", score);
    scoreText.setString(scoreStr);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setPosition(50, 30);
    window.draw(scoreText);

    sf::Text savehim;
    savehim.setFont(font1);
    savehim.setCharacterSize(50);
    savehim.setString("!! PLEASE SAVE ME !!");
    savehim.setFillColor(sf::Color::Red);
    sf::FloatRect savehimBounds = savehim.getLocalBounds();
    savehim.setPosition(1020 - savehimBounds.width - 20, 50);
    window.draw(savehim);

    sf::Text hintLabelText;
    hintLabelText.setFont(font1);
    hintLabelText.setCharacterSize(80);
    hintLabelText.setString("Hint: ");
    hintLabelText.setFillColor(sf::Color::White);
    sf::FloatRect hintLabelBounds = hintLabelText.getLocalBounds();
    hintLabelText.setPosition(1020 - hintLabelBounds.width - 190, 150);
    window.draw(hintLabelText);

    sf::Text hintText;
    hintText.setFont(font1);
    hintText.setCharacterSize(80);
    hintText.setString(wordManager.getCategoryHint());
    sf::FloatRect hintBounds = hintText.getLocalBounds();
    hintText.setPosition(1020 - hintBounds.width - 40, 150);
    window.draw(hintText);

    sf::Text wordText;
    wordText.setFont(font1);
    wordText.setCharacterSize(100);
    wordText.setString(guessedWord);
    sf::FloatRect wordBounds = wordText.getLocalBounds();
    wordText.setPosition(1020 - wordBounds.width - 300, 250);
    window.draw(wordText);

    sf::Text livesText;
    livesText.setFont(font1);
    livesText.setCharacterSize(70);
    char livesStr[10];
    sprintf_s(livesStr, sizeof(livesStr), "Lives: %d", lives);
    livesText.setString(livesStr);
    sf::FloatRect livesBounds = livesText.getLocalBounds();
    livesText.setPosition(1020 - livesBounds.width - 50, 370);
    window.draw(livesText);

    sf::Text guide;
    guide.setFont(font);
    guide.setCharacterSize(25);
    guide.setString("!!Save me through your Keyboard!!");
    guide.setFillColor(sf::Color::White);
    sf::FloatRect guideBounds = guide.getLocalBounds();
    guide.setPosition(30, 530);
    window.draw(guide);

    const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    sf::Text letterText;
    letterText.setFont(font2);
    letterText.setCharacterSize(100);
    float xPosition = 175;
    float yPosition = 550;
    for (int i = 0; i < 10; i++)
    {
        letterText.setString(alphabet[i]);
        if (alphabetStatus[i] == 1)
        {
            letterText.setFillColor(sf::Color::Green);//Correctly guessed
        }
        else if (alphabetStatus[i] == -1)
        {
            letterText.setFillColor(sf::Color::Red);//Incorrectly guessed
        }
        else
        {
            letterText.setFillColor(sf::Color::White);//Not guessed yet
        }
        letterText.setPosition(xPosition, yPosition);
        window.draw(letterText);
        xPosition += 70;
    }

    xPosition = 220;
    yPosition += 70;
    for (int i = 10; i < 19; i++)
    {
        letterText.setString(alphabet[i]);
        if (alphabetStatus[i] == 1)
        {
            letterText.setFillColor(sf::Color::Green);//Correctly guessed
        }
        else if (alphabetStatus[i] == -1)
        {
            letterText.setFillColor(sf::Color::Red);//Incorrectly guessed
        }
        else
        {
            letterText.setFillColor(sf::Color::White);//Not guessed yet
        }
        letterText.setPosition(xPosition, yPosition);
        window.draw(letterText);
        xPosition += 70;
    }

    xPosition = 290;
    yPosition += 70;
    for (int i = 19; i < 26; i++)
    {
        letterText.setString(alphabet[i]);
        if (alphabetStatus[i] == 1)
        {
            letterText.setFillColor(sf::Color::Green);//Correctly guessed
        }
        else if (alphabetStatus[i] == -1)
        {
            letterText.setFillColor(sf::Color::Red);//Incorrectly guessed
        }
        else
        {
            letterText.setFillColor(sf::Color::White);//Not guessed yet
        }
        letterText.setPosition(xPosition, yPosition);
        window.draw(letterText);
        xPosition += 70;
    }
    window.display();
}

void Game::resetGame()
{
    hangmanSprite.setTexture(hangmanTextures[0]);
    hangmanSprite.setPosition(50, 200);
    lives = 6;
    guessedWordLength = 0;
    for (int i = 0; i < 26; i++)
    {
        alphabetStatus[i] = 0;
    }
}

void Game::start()
{
    sf::RenderWindow window(sf::VideoMode(1020, 800), "Hangman Game");
    while (window.isOpen())
    {
        lives = 6;
        guessedWordLength = 0;
        int randomCategoryIndex = rand() % 3;
        wordManager.setCategory(randomCategoryIndex);
        const char* word = wordManager.getRandomWord();
        int i = 0;
        while (word[i] != '\0' && i < 20)
        {
            if (word[i] >= 'a' && word[i] <= 'z')
            {
                currentWord[i] = word[i] - ('a' - 'A');
            }
            else
            {
                currentWord[i] = word[i];
            }
            i++;
        }
        currentWordLength = i;
        currentWord[i] = '\0';
        for (int j = 0; j < currentWordLength; j++)
        {
            guessedWord[j] = '_';
        }
        guessedWord[currentWordLength] = '\0';
        for (int i = 0; i < 26; i++)
        {
            alphabetStatus[i] = 0;
        }
        while (window.isOpen() && lives > 0 && !isWordGuessed())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                else if (event.type == sf::Event::TextEntered)
                {
                    char letter = static_cast<char>(event.text.unicode);
                    if (letter >= 'a' && letter <= 'z')
                    {
                        handleInput(letter);
                    }
                    else if (event.text.unicode == 27)
                    {
                        window.clear();
                        sf::Font font;
                        font.loadFromFile("Fonts/main.ttf");
                        sf::Text endText;
                        endText.setFont(font);
                        endText.setCharacterSize(100);
                        endText.setFillColor(sf::Color::Red);
                        endText.setString("! Game Over !");
                        sf::FloatRect endTextBounds = endText.getLocalBounds();
                        endText.setPosition((1020 - endTextBounds.width) / 2, 240);
                        window.draw(endText);

                        sf::Text emText;
                        emText.setFont(font);
                        emText.setCharacterSize(80);
                        emText.setFillColor(sf::Color::White);
                        emText.setString("! Exiting to Menu !");
                        sf::FloatRect emTextBounds = emText.getLocalBounds();
                        emText.setPosition((1020 - emTextBounds.width) / 2, 350);
                        window.draw(emText);

                        window.display();
                        sf::sleep(sf::seconds(5));
                        window.close();
                    }
                }
            }
            update(window);
            render(window);
        }
        if (lives > 0 && isWordGuessed())
        {
            score += 2 * lives;
            window.clear();
            sf::Font font;
            font.loadFromFile("Fonts/main.ttf");
            sf::Font font1;
            font1.loadFromFile("Fonts/main2.otf");
            window.clear();

            sf::Text scoreText;
            scoreText.setFont(font1);
            scoreText.setCharacterSize(100);
            char scoreStr[20];
            sprintf_s(scoreStr, sizeof(scoreStr), "Your Score: %d", score);
            scoreText.setString(scoreStr);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(30, 30);
            window.draw(scoreText);

            sf::Text endText;
            endText.setFont(font);
            endText.setCharacterSize(100);
            endText.setFillColor(sf::Color::Red);
            endText.setString("! You Won !");
            sf::FloatRect endTextBounds = endText.getLocalBounds();
            endText.setPosition((1020 - endTextBounds.width) / 2, 250);
            window.draw(endText);

            sf::Text ex;
            ex.setFont(font);
            ex.setCharacterSize(50);
            ex.setFillColor(sf::Color::White);
            ex.setString("! Press ENTER to Continue !");
            sf::FloatRect exBounds = ex.getLocalBounds();
            ex.setPosition((1020 - exBounds.width) / 2, 370);
            window.draw(ex);

            sf::Text exit;
            exit.setFont(font);
            exit.setCharacterSize(50);
            exit.setFillColor(sf::Color::Red);
            exit.setString("! Press ESC to Exit !");
            sf::FloatRect exitBounds = exit.getLocalBounds();
            exit.setPosition((1020 - exitBounds.width) / 2, 450);
            window.draw(exit);

            window.display();

            bool waitingForInput = true;
            while (waitingForInput)
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    {
                        window.close();
                        waitingForInput = false;
                    }
                    else if (event.type == sf::Event::TextEntered)
                    {
                        if (event.text.unicode == 27)
                        {
                            window.clear();
                            sf::Font font;
                            font.loadFromFile("Fonts/main.ttf");
                            sf::Text endText;
                            endText.setFont(font);
                            endText.setCharacterSize(100);
                            endText.setFillColor(sf::Color::Red);
                            endText.setString("! Game Over !");
                            sf::FloatRect endTextBounds = endText.getLocalBounds();
                            endText.setPosition((1020 - endTextBounds.width) / 2, 240);
                            window.draw(endText);

                            sf::Text emText;
                            emText.setFont(font);
                            emText.setCharacterSize(80);
                            emText.setFillColor(sf::Color::White);
                            emText.setString("! Exiting to Menu !");
                            sf::FloatRect emTextBounds = emText.getLocalBounds();
                            emText.setPosition((1020 - emTextBounds.width) / 2, 350);
                            window.draw(emText);

                            window.display();
                            sf::sleep(sf::seconds(5));
                            window.close();
                            waitingForInput = false;
                        }
                        else if (event.text.unicode == 13)
                        {
                            waitingForInput = false;
                            resetGame();
                        }
                    }
                }
            }
        }
        else if (lives == 0)
        {
            window.clear();
            sf::Font font;
            font.loadFromFile("Fonts/main.ttf");
            sf::Text endText;
            endText.setFont(font);
            endText.setCharacterSize(100);
            endText.setFillColor(sf::Color::Red);
            endText.setString("! You Lost !");
            sf::FloatRect endTextBounds = endText.getLocalBounds();
            endText.setPosition((1020 - endTextBounds.width) / 2, 240);
            window.draw(endText);

            sf::Text emText;
            emText.setFont(font);
            emText.setCharacterSize(80);
            emText.setFillColor(sf::Color::White);
            emText.setString("! Exiting to Menu !");
            sf::FloatRect emTextBounds = emText.getLocalBounds();
            emText.setPosition((1020 - emTextBounds.width) / 2, 350);
            window.draw(emText);

            window.display();
            sf::sleep(sf::seconds(5));
            window.close();
        }
    }
}


int main()
{
    Game hangman;
    hangman.start();
    return 0;
}

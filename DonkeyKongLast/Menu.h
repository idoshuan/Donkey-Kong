#include <iostream>
#include <string>

class Menu {
public:
    void displayMenu() {
        int choice;
        do {
            clearScreen(); // Call system-specific clear screen (e.g., system("clear") or system("cls"))
            std::cout << "===========================\n";
            std::cout << "     Donkey Kong Game      \n";
            std::cout << "===========================\n";
            std::cout << "1. Start Game\n";
            std::cout << "2. Instructions\n";
            std::cout << "3. Exit\n";
            std::cout << "===========================\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            handleChoice(choice);
        } while (choice != 3);
    }

private:
    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void handleChoice(int choice) {
        switch (choice) {
        case 1:
            break;
        case 2:
            displayInstructions();
            break;
        case 3:
            std::cout << "Exiting the game. Goodbye!\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    }
    void startGame() {
    
    }

    void displayInstructions() {
        clearScreen();
        std::cout << "===========================\n";
        std::cout << "       Instructions        \n";
        std::cout << "===========================\n";
        std::cout << "Use arrow keys to move.\n";
        std::cout << "Avoid obstacles and reach the top.\n";
        std::cout << "Good luck!\n";
        std::cout << "===========================\n";
        std::cout << "Press Enter to return to the menu.\n";
        std::cin.ignore();
        std::cin.get();
    }
};

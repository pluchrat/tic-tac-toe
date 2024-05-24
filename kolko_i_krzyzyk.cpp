#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

// Struktura reprezentująca gracza
struct Player {
    int index;
    char sign;
    int wins;
    int draws;
    int loses;
    int points;
};

// Funkcja do wczytania danych o graczach z pliku players.txt
std::vector<Player> loadPlayers(const std::string& filename) {
    std::vector<Player> players;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        return players;
    }

    Player player;
    while (file >> player.index >> player.sign) {
        player.wins = 0;
        player.draws = 0;
        player.loses = 0;
        player.points = 0;
        players.push_back(player);
    }
    file.close();

    return players;
}
// Funkcja sprawdzająca, czy jest zwycięzca
char checkWinner(const char t[3][3]) {
    // Sprawdzanie poziomych linii
    for (int i = 0; i < 3; ++i) {
        if (t[i][0] == t[i][1] && t[i][1] == t[i][2] && t[i][0] != ' ')
            return t[i][0];
    }

    // Sprawdzanie pionowych linii
    for (int i = 0; i < 3; ++i) {
        if (t[0][i] == t[1][i] && t[1][i] == t[2][i] && t[0][i] != ' ')
            return t[0][i];
    }

    // Sprawdzanie przekątnych
    if (t[0][0] == t[1][1] && t[1][1] == t[2][2] && t[0][0] != ' ')
        return t[0][0];

    if (t[0][2] == t[1][1] && t[1][1] == t[2][0] && t[0][2] != ' ')
        return t[0][2];

    // Jeśli nikt nie wygrał
    return ' ';
}

void loadResults(const std::string& results, std::vector<Player>& players) {
    std::ifstream file(results);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << results << std::endl;
        return;
    }

    char line;
    // Tablica do przechowywania pola gry 3x3 jako znaków
    char t[3][3];
    int licznik = 0;
    int i = 0;
    int j = 0;
    int test;
    std::vector<char> participatingSigns; // Przechowuje znaki uczestniczących graczy
    while (file >> line) {
        if (line != ' ' && licznik <= 8) {
            t[i][j] = line;
            licznik++;
            // Dodajemy znaki graczy, którzy uczestniczyli w grze
            if (std::find(participatingSigns.begin(), participatingSigns.end(), line) == participatingSigns.end()) {
                participatingSigns.push_back(line);
            }
        }

        if (i == 0 && j != 2) {
            i = 0;
            j++;
        }
        else {
            if (i == 0 && j == 2) {
                i = 1;
                j = 0;
            }
            else {
                if (i == 1 && j != 2) {
                    i = 1;
                    j++;
                }
                else {
                    if (i == 1 && j == 2) {
                        i = 2;
                        j = 0;
                    }
                    else {
                        if (i == 2 && j != 2) {
                            i = 2;
                            j++;
                        }
                        else {
                            if (i == 2 && j == 2) {
                                i = 0;
                                j = 0;
                            }
                        }
                    }
                }
            }
        }
        if (licznik == 9) {
            // Sprawdź, kto wygrał
            char winner = checkWinner(t);
            if (winner != ' ')
            {
                // Znaleziono zwycięzcę, zaktualizuj wyniki graczy
                for (Player& player : players) {
                    if (player.sign == winner) {
                        player.wins++;
                        player.points += 3; // Zwycięzca otrzymuje 3 punkty
                        test = 1;
                    }
                    else if (std::find(participatingSigns.begin(), participatingSigns.end(), player.sign) != participatingSigns.end() && test == 1) {
                        // Gracz był uczestnikiem gry, ale nie jest zwycięzcą
                        player.loses++;
                    }
                }
            }
            if (winner == ' ')
            {
                // Jeśli jest remis, ale nie ma zwycięzcy, dodaj remis dla uczestniczących graczy
                for (Player& player : players) {
                    if (std::find(participatingSigns.begin(), participatingSigns.end(), player.sign) != participatingSigns.end()) {
                        player.draws++;
                        player.points++; // Każdy gracz otrzymuje 1 punkt za remis
                    }
                    else {
                        player.loses++; // Dodaj przegraną dla wszystkich graczy, którzy nie byli uczestnikami danej gry
                    }
                }
            }


            // Wyzeruj zmienne
            i = 0;
            j = 0;
            licznik = 0;
            test = 0;
            participatingSigns.clear(); // Wyczyść listę znaków uczestniczących graczy po każdej grze


        }
    
    }
    file.close();
}





// Funkcja porównująca graczy do sortowania
bool comparePlayers(const Player& p1, const Player& p2) {
    // Sortowanie malejąco po punktach
    if (p1.points != p2.points)
        return p1.points > p2.points;
    // Jeśli punkty są takie same, sortujemy alfabetycznie po znaku gracza
    return p1.sign < p2.sign;
}

// Funkcja wyświetlająca tabelę wyników
void showTable(const std::vector<Player>& players) {
    std::cout << "ID\tSign\tWins\tDraws\tLoses\tPoints" << std::endl;
    for (const Player& player : players) {
        std::cout << player.index << "\t" << player.sign << "\t" << player.wins << "\t" << player.draws << "\t" << player.loses << "\t" << player.points << std::endl;
    }
}

int main() {
    std::string wybor;
    bool exitRequested = false;

    while (!exitRequested) {
        std::cout << "1 - show table" << std::endl;
        std::cout << "2 - show results" << std::endl;
        std::cout << "3 - show player stats" << std::endl;
        std::cout << "4 - exit" << std::endl;
        std::cout << "Choose option:";
        std::cin >> wybor;

        if (wybor == "1") {
            std::vector<Player> players = loadPlayers("C:/Users/dpluc/OneDrive/Pulpit/studia_2023_2024/s2/infa/tic-tac_toe/players.txt");
            loadResults("C:/Users/dpluc/OneDrive/Pulpit/studia_2023_2024/s2/infa/tic-tac_toe/tictactoe.txt", players);
            std::sort(players.begin(), players.end(), comparePlayers);
            showTable(players);
        }
        else if (wybor == "2") {
            std::ifstream resultsFile("C:/Users/dpluc/OneDrive/Pulpit/studia_2023_2024/s2/infa/tic-tac_toe/tictactoe.txt");
            if (!resultsFile.is_open()) {
                std::cerr << "Error: Unable to open file tictactoe.txt" << std::endl;
                return 1;
            }

            std::string line;
            while (std::getline(resultsFile, line)) {
                if (!line.empty()) {
                    std::cout << line << std::endl;
                }
            }

            resultsFile.close();
        }
        else if (wybor == "3") {
            std::vector<Player> players = loadPlayers("C:/Users/dpluc/OneDrive/Pulpit/studia_2023_2024/s2/infa/tic-tac_toe/players.txt");
            loadResults("C:/Users/dpluc/OneDrive/Pulpit/studia_2023_2024/s2/infa/tic-tac_toe/tictactoe.txt", players);

            int playerIndex;
            std::cout << "Enter player index: ";
            std::cin >> playerIndex;

            // Szukamy gracza o podanym indeksie
            Player selectedPlayer;
            bool found = false;
            for (const Player& player : players) {
                if (player.index == playerIndex) {
                    selectedPlayer = player;
                    found = true;
                    break;
                }
            }

            if (found) {
                std::cout << "Player stats for index " << selectedPlayer.index << ":" << std::endl;
                std::cout << "Sign: " << selectedPlayer.sign << std::endl;
                std::cout << "Wins: " << selectedPlayer.wins << std::endl;
                std::cout << "Draws: " << selectedPlayer.draws << std::endl;
                std::cout << "Loses: " << selectedPlayer.loses << std::endl;
                std::cout << "Points: " << selectedPlayer.points << std::endl;
            }
            else {
                std::cout << "Player with index " << playerIndex << " not found." << std::endl;
            }
        }
        else if (wybor == "4") {
            exitRequested = true; // Ustaw flagę wyjścia na true, aby wyjść z pętli
        }
        else {
            std::cout << "Invalid option!" << std::endl;
        }
    }

    return 0;
}

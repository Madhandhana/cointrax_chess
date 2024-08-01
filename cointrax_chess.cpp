#include <iostream>
#include <vector>
#include <unordered_map>
#include <utility>
#include <SFML/Graphics.hpp>

using namespace std;

class Position {
public:
    int row, col;
    
    Position(int r, int c) : row(r), col(c) {}

    bool isValid() const {
        return (row >= 0 && row < 8 && col >= 0 && col < 8);
    }

    string key() const {
        return to_string(row) + "," + to_string(col);
    }
};

class ChessGraph {
public:
    unordered_map<string, vector<Position>> knightMoves;
    unordered_map<string, vector<Position>> rookMoves;
    unordered_map<string, vector<Position>> bishopMoves;
    unordered_map<string, vector<Position>> queenMoves;
    unordered_map<string, vector<Position>> kingMoves;
    unordered_map<string, vector<Position>> whitePawnMoves;
    unordered_map<string, vector<Position>> blackPawnMoves;

    void addEdge(unordered_map<string, vector<Position>>& adjList, const Position& from, const Position& to) {
        if (from.isValid() && to.isValid()) {
            adjList[from.key()].push_back(to);
        }
    }

    vector<Position> getMoves(const Position& pos, int pieceType) {
        switch (pieceType) {
            case 1: return knightMoves[pos.key()];
            case 2: return rookMoves[pos.key()];
            case 3: return bishopMoves[pos.key()];
            case 4: return queenMoves[pos.key()];
            case 5: return kingMoves[pos.key()];
            case 6: return whitePawnMoves[pos.key()];
            case 7: return blackPawnMoves[pos.key()];
            default: return {};
        }
    }

    void initializeGraph() {
        // Knight moves
        const int knightOffsets[8][2] = {{2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Position pos(r, c);
                for (auto& offset : knightOffsets) {
                    addEdge(knightMoves, pos, Position(r + offset[0], c + offset[1]));
                }
            }
        }

        // Rook moves
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Position pos(r, c);
                for (int i = 0; i < 8; ++i) {
                    if (i != r) addEdge(rookMoves, pos, Position(i, c)); // Vertical moves
                    if (i != c) addEdge(rookMoves, pos, Position(r, i)); // Horizontal moves
                }
            }
        }

        // Bishop moves
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Position pos(r, c);
                for (int i = -7; i <= 7; ++i) {
                    if (i != 0) {
                        addEdge(bishopMoves, pos, Position(r + i, c + i));
                        addEdge(bishopMoves, pos, Position(r + i, c - i));
                    }
                }
            }
        }

        // Queen moves
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Position pos(r, c);
                for (int i = 0; i < 8; ++i) {
                    if (i != r) addEdge(queenMoves, pos, Position(i, c)); // Vertical moves
                    if (i != c) addEdge(queenMoves, pos, Position(r, i)); // Horizontal moves
                }
                for (int i = -7; i <= 7; ++i) {
                    if (i != 0) {
                        addEdge(queenMoves, pos, Position(r + i, c + i));
                        addEdge(queenMoves, pos, Position(r + i, c - i));
                    }
                }
            }
        }

        // King moves
        const int kingOffsets[8][2] = {{1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Position pos(r, c);
                for (auto& offset : kingOffsets) {
                    addEdge(kingMoves, pos, Position(r + offset[0], c + offset[1]));
                }
            }
        }

        // White pawn moves
        for (int r = 1; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Position pos(r, c);
                addEdge(whitePawnMoves, pos, Position(r - 1, c)); // Single move forward
                if (r == 6) addEdge(whitePawnMoves, pos, Position(r - 2, c)); // Double move forward from starting position
                // Capture moves
                addEdge(whitePawnMoves, pos, Position(r - 1, c - 1));
                addEdge(whitePawnMoves, pos, Position(r - 1, c + 1));
            }
        }

        // Black pawn moves
        for (int r = 0; r < 7; ++r) {
            for (int c = 0; c < 8; ++c) {
                Position pos(r, c);
                addEdge(blackPawnMoves, pos, Position(r + 1, c)); // Single move forward
                if (r == 1) addEdge(blackPawnMoves, pos, Position(r + 2, c)); // Double move forward from starting position
                // Capture moves
                addEdge(blackPawnMoves, pos, Position(r + 1, c - 1));
                addEdge(blackPawnMoves, pos, Position(r + 1, c + 1));
            }
        }
    }
};

void printColoredText(const string& text, const string& colorCode) {
    cout << "\033[" << colorCode << "m" << text << "\033[0m";
}

int main() {

    string text = R"(
 ░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░▒▓███████▓▒░▒▓████████▓▒░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░       ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░░▒▓███████▓▒░▒▓███████▓▒░ 
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░   ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░     ░▒▓█▓▒░        
░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░   ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░     ░▒▓█▓▒░        
░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░   ░▒▓███████▓▒░░▒▓████████▓▒░░▒▓██████▓▒░       ░▒▓█▓▒░      ░▒▓████████▓▒░▒▓██████▓▒░  ░▒▓██████▓▒░░▒▓██████▓▒░  
░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░   ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░             ░▒▓█▓▒░     ░▒▓█▓▒░ 
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░   ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░             ░▒▓█▓▒░     ░▒▓█▓▒░ 
 ░▒▓██████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░ ░▒▓█▓▒░   ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░       ░▒▓██████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓███████▓▒░▒▓███████▓▒░  
                                                                                                                                                                         
                                                                                                                                                                         
)";

    string colorCode1 = "31"; // Red
    string colorCode2 = "32"; // Green
    string colorCode3 = "33"; // Yellow
    string colorCode4 = "34"; // Blue
    string colorCode5 = "35"; // Magenta

    printColoredText(text, colorCode3);

    ChessGraph chessGraph;
    chessGraph.initializeGraph();

    while (true) {
        int choice;
        int row, col;

        cout << "Choose a chess piece:\n";
        cout << "1. Knight\n";
        cout << "2. Rook\n";
        cout << "3. Bishop\n";
        cout << "4. Queen\n";
        cout << "5. King\n";
        cout << "6. White Pawn\n";
        cout << "7. Black Pawn\n";
        cout << "Enter your choice (or 'exit' to quit): ";
        string input;
        cin >> input;

        if (input == "exit") {
            break;
        }

        try {
            choice = stoi(input);
        } catch (...) {
            cout << "Invalid input. Please enter a valid number or 'exit' to quit.\n";
            continue;
        }

        cout << "Enter the position of the piece (row col): ";
        cin >> row >> col;

        Position pos(row, col);
        vector<Position> moves = chessGraph.getMoves(pos, choice);

        sf::RenderWindow window(sf::VideoMode(800, 800), "Chess Board");

        sf::Texture boardTexture, highlightTexture;
        if (!boardTexture.loadFromFile("chessboard.png")) {
            cout << "Failed to load chessboard texture!" << endl;
            return 1;
        }
        if (!highlightTexture.loadFromFile("highlight.png")) {
            cout << "Failed to load highlight texture!" << endl;
            return 1;
        }

        sf::Sprite boardSprite(boardTexture);
        vector<sf::Sprite> highlightSprites;
        for (const auto& move : moves) {
            if (move.isValid()) {
                sf::Sprite highlight(highlightTexture);
                highlight.setPosition(move.col * 105, move.row * 85);
                highlightSprites.push_back(highlight);
            }
        }

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(boardSprite);
            for (const auto& highlight : highlightSprites) {
                window.draw(highlight);
            }
            window.display();
        }
    }

    return 0;
}

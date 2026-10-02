#include <iostream>
#include <string>
#include <random>

using namespace std;

struct Property {
    string name;
    int cost;
    string owner;
};

struct Node {
    Property data;
    Node* next;

    Node(const Property& property) : data(property), next(nullptr) {}
};

class CircularLinkedList {
private:
    Node* head;
    int size;

public:
    CircularLinkedList() : head(nullptr), size(0) {}

    ~CircularLinkedList() {
        clear();
    }

    void add(const Property& property) {
        Node* newNode = new Node(property);

        if (head == nullptr) {
            head = newNode;
            newNode->next = head;
        } else {
            Node* tail = head;

            while (tail->next != head) {
                tail = tail->next;
            }

            tail->next = newNode;
            newNode->next = head;
        }

        size++;
    }

    Node* search(const string& propertyName) const {
        if (head == nullptr) {
            return nullptr;
        }

        Node* current = head;

        do {
            if (current->data.name == propertyName) {
                return current;
            }

            current = current->next;
        } while (current != head);

        return nullptr;
    }

    bool remove(const string& propertyName) {
        if (head == nullptr) {
            return false;
        }

        Node* current = head;
        Node* previous = nullptr;

        do {
            if (current->data.name == propertyName) {
                if (current == head && current->next == head) {
                    head = nullptr;
                } else if (current == head) {
                    Node* tail = head;

                    while (tail->next != head) {
                        tail = tail->next;
                    }

                    head = head->next;
                    tail->next = head;
                } else {
                    previous->next = current->next;
                }

                delete current;
                size--;
                return true;
            }

            previous = current;
            current = current->next;

        } while (current != head);

        return false;
    }

    Node* getHead() const {
        return head;
    }

    int getSize() const {
        return size;
    }

    void print() const {
        if (head == nullptr) {
            cout << "The list is empty." << endl;
            return;
        }

        Node* current = head;

        do {
            cout << current->data.name
                 << " ($" << current->data.cost << ", owner: "
                 << (current->data.owner.empty()
                         ? "Unowned"
                         : current->data.owner)
                 << ") -> ";

            current = current->next;

        } while (current != head);

        cout << "back to " << head->data.name << endl;
    }

    void clear() {
        if (head == nullptr) {
            return;
        }

        Node* current = head->next;

        while (current != head) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }

        delete head;
        head = nullptr;
        size = 0;
    }
};

struct Player {
    string name;
    int money;
    Node* position;
};

void movePlayer(Player& player, int spaces) {
    for (int i = 0; i < spaces; i++) {
        player.position = player.position->next;
    }
}

void playTurn(Player& player, int turn, int roll) {
    movePlayer(player, roll);

    Property& property = player.position->data;

    cout << "Turn " << turn << ": "
         << player.name << " rolled " << roll
         << " and landed on " << property.name << "." << endl;

    if (property.owner.empty()) {
        if (player.money >= property.cost) {
            player.money -= property.cost;
            property.owner = player.name;

            cout << player.name << " bought "
                 << property.name << " for $"
                 << property.cost << "." << endl;
        } else {
            cout << player.name
                 << " cannot afford this property." << endl;
        }
    } else if (property.owner == player.name) {
        cout << property.name
             << " is already owned by "
             << player.name << "." << endl;
    } else {
        cout << property.name
             << " is owned by "
             << property.owner << "." << endl;
    }

    cout << player.name << " has $"
         << player.money << " remaining." << endl << endl;
}

int main() {
    cout << "LINKED LIST TESTS" << endl;

    CircularLinkedList testList;

    testList.add({"Test Property 1", 100, ""});
    testList.add({"Test Property 2", 200, ""});
    testList.add({"Test Property 3", 300, ""});

    cout << "After adding three properties: ";
    testList.print();

    cout << "Searching for Test Property 2: "
         << (testList.search("Test Property 2") != nullptr
                 ? "Found"
                 : "Not found")
         << endl;

    cout << "Removing Test Property 2: "
         << (testList.remove("Test Property 2")
                 ? "Success"
                 : "Failed")
         << endl;

    cout << "After removal: ";
    testList.print();

    cout << endl;

    CircularLinkedList board;

    board.add({"Boardwalk", 400, ""});
    board.add({"Park Place", 350, ""});
    board.add({"Pennsylvania Avenue", 320, ""});
    board.add({"North Carolina Avenue", 300, ""});
    board.add({"Pacific Avenue", 300, ""});
    board.add({"Marvin Gardens", 280, ""});
    board.add({"Atlantic Avenue", 260, ""});
    board.add({"Ventnor Avenue", 260, ""});
    board.add({"Water Works", 150, ""});
    board.add({"Electric Company", 150, ""});

    cout << "INITIAL CIRCULAR BOARD" << endl;
    board.print();

    cout << "Number of properties: "
         << board.getSize() << endl << endl;

    Player alice{"Alice", 1500, board.getHead()};
    Player bob{"Bob", 1500, board.getHead()};

    mt19937 generator(42);
    uniform_int_distribution<int> dice(1, 6);

    cout << "MONOPOLY SIMULATION" << endl << endl;

    for (int turn = 1; turn <= 10; turn++) {
        Player& currentPlayer =
            (turn % 2 == 1) ? alice : bob;

        int roll = dice(generator);

        playTurn(currentPlayer, turn, roll);
    }

    cout << "FINAL BOARD" << endl;
    board.print();

    cout << endl << "FINAL PLAYER STATUS" << endl;
    cout << alice.name << ": $" << alice.money << endl;
    cout << bob.name << ": $" << bob.money << endl;

    return 0;
}

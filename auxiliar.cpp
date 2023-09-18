#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

void geradorMatrizes(int l, int c, const char* filename) {
    ofstream file(filename);

    for (int i = 0; i < l; ++i) {
        for (int j = 0; j < c; ++j) {
            file << rand() % 10 << " "; // Gere números inteiros aleatórios de 0 a 99
        }
        file << endl;
    }

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cout << "forma correta: " << argv[0] << " l1 c1 l2 c2" << endl;
        return 1;
    }

    int l1 = atoi(argv[1]);
    int c1 = atoi(argv[2]);
    int l2 = atoi(argv[3]);
    int c2 = atoi(argv[4]);

    // Inicialize a semente do gerador de números aleatórios com o tempo atual
    srand(static_cast<unsigned int>(time(nullptr)));

    geradorMatrizes(l1, c1, "./matrizes/m1.txt");
    geradorMatrizes(l2, c2, "./matrizes/m2.txt");

    cout << "Matrizes M1 e M2 geradas e salvas em M1.txt e M2.txt." << endl;

    return 0;
}

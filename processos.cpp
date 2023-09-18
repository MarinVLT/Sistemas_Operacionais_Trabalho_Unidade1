#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <unistd.h>
#include <vector>
#include <iomanip>

using namespace std;

chrono::steady_clock::time_point inicio;
chrono::steady_clock::time_point fim;
int P;
vector<vector<int>> matriz1;
vector<vector<int>> matriz2;

double soma_tempo = 0;
//double media_tempo = 0;



vector<vector<int>> reescreverMatriz(string arq) {

  ifstream arquivo_matriz(arq);

  // int linhas=0, colunas=0;
  string l;

  vector<vector<int>> matriz;

  // pega os numeros das linhas e colunas
  while (getline(arquivo_matriz, l)) {
    vector<int> linhas_int;
    for (char c : l) {
      if (c != ' ') {
        string aux;
        aux[0] = c;

        linhas_int.push_back(stoi(aux));
        // colunas++;
      }
    }
    // linhas++;
    matriz.push_back(linhas_int);
  }

  arquivo_matriz.close();

  return matriz;
}

// multiplica as matrizes
vector<int> multiplicarMatriz(const vector<vector<int>> &matriz1,
                              const vector<vector<int>> &matriz2, int P,
                              int posicao) {
  int linha_i = posicao / matriz2[0].size();
  int coluna_i = posicao % matriz2[0].size();
  int linha_f = min(linha_i + P, static_cast<int>(matriz1.size()));
  int coluna_f = matriz2[0].size();

  int contador = 0;

  vector<int> resultado;

  for (int i = linha_i; i < linha_f; i++) {
    for (int j = coluna_i; j < coluna_f; j++) {
      int soma = 0;
      for (int k = 0; k < matriz1[i].size(); k++) {
        soma += matriz1[i][k] * matriz2[k][j];
      }
      resultado.push_back(soma);
      contador++;

      if (contador == P)
        break;
    }
    coluna_i = 0;
    if (contador == P)
      break;
  }

  return resultado;
}

// escreve os resultados e tempo no arquivo novo
void escreverResultado(const vector<int> &resultado, int processo_atual) {
  ofstream arquivo_processo("./saida_processos/processo_" + to_string(processo_atual) + ".txt");

  int linha_i = processo_atual * P / matriz2[0].size();
  int coluna_i = processo_atual * P % matriz2[0].size();

  for (int i = 0; i < resultado.size(); i++) {
    arquivo_processo << "c" << linha_i + 1 << coluna_i + 1 << " "
                     << resultado[i] << endl;
    coluna_i++;
    if (coluna_i == matriz2[0].size()) {
      coluna_i = 0;
      linha_i++;
    }
  }

  fim = chrono::steady_clock::now();
  int tempo = chrono::duration_cast<chrono::milliseconds>(fim - inicio).count();
  arquivo_processo << tempo;
  

  //cout << setprecision(15) << tempo << endl;
  
  arquivo_processo.close();
}

int main(int argc, char *argv[3]) {
  matriz1 = reescreverMatriz(argv[1]);
  matriz2 = reescreverMatriz(argv[2]);
  P = atoi(argv[3]); // Quantidade de elementos por processo.
  int n_processos =
      (matriz1.size() * matriz2[0].size()) / P; // Quantidade de processos.
  if ((matriz1.size() * matriz2[0].size()) % P != 0) {
    n_processos++;
  }

  inicio = chrono::steady_clock::now();

  for (int i = 0; i < n_processos; i++) {
    pid_t pid = fork();
    if (pid == 0) {
      vector<int> resultado = multiplicarMatriz(matriz1, matriz2, P, P * i);
      escreverResultado(resultado, i);
      exit(0);
    }
  }

  for (int i = 0; i < n_processos; i++) {
    wait(NULL);
  }

  cout << "Multiplicação concluída. Resultado salvo em ./saida_processos/" << endl;

  //media_tempo = soma_tempo/8;
  //cout << "media do tempo: " << setprecision(15) << media_tempo << endl;

  return 0;
}

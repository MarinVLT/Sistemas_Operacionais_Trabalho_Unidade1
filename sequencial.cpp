#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <chrono>

using namespace std;

vector<vector<int>> reescreverMatriz(string arq){
  
  ifstream arquivo_matriz(arq);
  
  //int linhas=0, colunas=0;
  string l;

  vector<vector<int>> matriz;

  //pega os numeros das linhas e colunas
    while(getline(arquivo_matriz, l)){
      vector<int> linhas_int;
      for(char c : l){
        if(c !=  ' '){
          string aux;
          aux[0] = c;
          
          linhas_int.push_back(stoi(aux));
          //colunas++;
        }
      }
      //linhas++;
      matriz.push_back(linhas_int);
    }
  

  arquivo_matriz.close();

  return matriz;
}

//retorna o numero de colunas
int getcoluna(string arq){
  ifstream arquivo_matriz(arq);
  int c=0, aux=0;
  string linha;

  while(getline(arquivo_matriz, linha)){
    aux++;
    if(aux == 1){
      for(char item : linha){
        if(item != ' ') c++;
      }
      break;
    }  
  }
  
  arquivo_matriz.close();
  return c;
}

//retorna o numero de linhas
int getlinha(string arq){
  ifstream arquivo_matriz(arq);
  int l=0;
  string linha;
  
  while(getline(arquivo_matriz, linha)){
    l++;  
  }
  
  arquivo_matriz.close();
  
  return l;
}

//multiplica as matrizes
vector<vector<float>> multiplicarMatrizes(const vector<vector<int>>& matriz1, const vector<vector<int>>& matriz2) {
    int linhas1 = matriz1.size();
    int colunas1 = matriz1[0].size();
    int linhas2 = matriz2.size();
    int colunas2 = matriz2[0].size();

    //Verifique a compatibilidade das matrizes para a multiplicação
    if (colunas1 != linhas2) {
        cerr << "Erro: As matrizes não são compatíveis para multiplicação." << endl;
        exit(1);
    }

    // Crie a matriz de resultado
    vector<vector<float>> resultado(linhas1, vector<float>(colunas2, 0));

    // Realize a multiplicação das matrizes
    for (int i = 0; i < linhas1; i++) {
        for (int j = 0; j < colunas2; j++) {
            for (int k = 0; k < colunas1; k++) {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    return resultado;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Uso: " << argv[0] << " arquivo_matriz1 arquivo_matriz2 arquivo_saida" << endl;
        return 1;
    }

    // Abra os arquivos de entrada
    ifstream arquivoMatriz1(argv[1]);
    ifstream arquivoMatriz2(argv[2]);

    if (!arquivoMatriz1 || !arquivoMatriz2) {
        cerr << "Erro ao abrir os arquivos de entrada." << endl;
        return 1;
    }

    // Leia as matrizes dos arquivos de entrada
    auto matriz1 = reescreverMatriz(argv[1]);
    auto matriz2 = reescreverMatriz(argv[2]); 

    //multiplicação
    chrono::steady_clock::time_point inicio = chrono::steady_clock::now();
    vector<vector<float>> resultado = multiplicarMatrizes(matriz1, matriz2);
    chrono::steady_clock::time_point fim = chrono::steady_clock::now();

    int duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio).count(); 
    //cout << "t1: " << duracao << endl;

    //linhas-colunas da matriz1
    int n_linhas1 = getlinha(argv[1]);
    int n_coluna1 = getcoluna(argv[1]);

    // cout << "l:" << n_linhas1 << endl;
    // cout << "c:" << n_coluna1 << endl;
    
    //linhas-colunas da matriz2
    int n_linhas2 = getlinha(argv[2]);
    int n_coluna2 = getcoluna(argv[2]);

    //salvar no arquivo de saida
    ofstream arquivoSaida(argv[3]);
    if (!arquivoSaida) {
        cerr << "Erro ao abrir o arquivo de saída." << endl;
        return 1;
    }

    arquivoSaida << n_linhas1 << " " << n_coluna1 << endl;

    for (int i = 0; i < n_linhas1; i++) {
        for (int j = 0; j < n_coluna2; j++) {
            arquivoSaida << "c" << i+1 << j+1 << ' ';
            arquivoSaida << resultado[i][j] << endl;
        }
    }

    arquivoSaida << duracao << endl;

    
    cout << "Multiplicação concluída. Resultado salvo em " << argv[3] << endl;

    return 0;
    
  
}
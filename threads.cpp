#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <vector>
#include <pthread.h>
#include <iomanip>

using namespace std;

chrono::steady_clock::time_point inicio;
chrono::steady_clock::time_point fim;
int P;
vector<vector<int>> matriz1;
vector<vector<int>> matriz2;


//para gerar os graficos
double soma_tempo = 0;
double media_tempo = 0;

int n_linhas_m1=0;
int n_colunas_m1=0;
int n_linhas_m2=0;
int n_colunas_m2=0;

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
vector<int> multiplicarMatriz(const vector<vector<int>>& matriz1, const vector<vector<int>>& matriz2, int P, int posicao) {
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

            if(contador == P) break;
        }
        coluna_i = 0;
        if(contador == P) break;
    }

    return resultado;
}

//escreve os resultados e tempo no arquivo novo
void escreverResultado(const vector<int>& resultado, long thread_id) {
    ofstream arquivo_thread("./saida_threads/thread_" + to_string(thread_id) + ".txt");

    int elementos_por_thread = (matriz1.size() * matriz2[0].size()) / P;
    int inicio_posicao = thread_id * elementos_por_thread;
    
    arquivo_thread << n_linhas_m2  << " " << n_colunas_m2 << endl;

    for (int i = 0; i < resultado.size(); i++) {
        int linha = (inicio_posicao + i) / matriz2[0].size();
        int coluna = (inicio_posicao + i) % matriz2[0].size();
        arquivo_thread << "c" << linha + 1 << coluna + 1 << " " << resultado[i] << endl;
    }

    fim = chrono::steady_clock::now();
    int tempo = chrono::duration_cast<chrono::milliseconds>(fim - inicio).count();
    arquivo_thread << tempo;
    
    //cout << "t: "<< tempo << ";" <<endl;
    soma_tempo += tempo;
    arquivo_thread.close();
}

//função chamada para calcular e escrever no arquivo
void* calcularMatriz(void* arg) {
    long thread_id = (long)arg;
    int elementos_por_thread = (matriz1.size() * matriz2[0].size()) / P;
    int inicio_posicao = thread_id * elementos_por_thread;
  
    vector<int> resultado = multiplicarMatriz(matriz1, matriz2, P, inicio_posicao);
    escreverResultado(resultado, thread_id);
    pthread_exit(NULL);
}



int main(int argc, char* argv[3]) {
    matriz1 = reescreverMatriz(argv[1]);
    matriz2 = reescreverMatriz(argv[2]);

    P = atoi(argv[3]); // Quantidade de elementos por thread.
    
    n_linhas_m1 = getlinha(argv[1]);
    n_colunas_m1 = getcoluna(argv[1]);

    n_linhas_m2 = getlinha(argv[2]);
    n_colunas_m2 = getcoluna(argv[2]);

    int n_threads = (matriz1.size() * matriz2[0].size()) / P; // Quantidade de threads.
    if ((matriz1.size() * matriz2[0].size()) % P != 0) {
        n_threads++;
    }


    inicio = chrono::steady_clock::now();

    pthread_t threads[n_threads];
    for (long i = 0; i < n_threads; i++) {
        pthread_create(&threads[i], NULL, calcularMatriz, (void*)i);
    }

    for (long i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    media_tempo = soma_tempo/6;
    //cout << "media do tempo: " << setprecision(10) << media_tempo << endl;
    cout << "Multiplicação concluída. Resultado salvo em ./saida_threads/" << endl;

    return 0;
}

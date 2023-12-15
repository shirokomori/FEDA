#include <iostream>
#include<vector>
#include<string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <stdio.h>
#include<limits>
using namespace std;
//Clase creada para implementar el metodo clasico de distancia, fue sacado de la practica 16 del ramo con una pequeña modificacion
class Solution1{
public:
    vector<vector<int>> dp;                                     // se crea la matriz sobre la cual se guardaran las medidas obtenidas

    int clasica(string word1, string word2) {                         //funcion creada para llamar a la funion recursiva para obtener la medida
        dp.assign(word1.size(), vector<int>(word2.size(), -1));
        return solve(0, 0, word1, word2);
    }
    int solve(int i, int j, const string &word1, const string &word2) {     //funcion que calcula la medida con el metodo clasico
        if (i >= word1.size() || j >= word2.size())
            return (word2.size() - j) + (word1.size() - i);
        if(i == word1.size()) return word2.size() - j;
        if(j == word2.size()) return word1.size() - i;

        if (dp[i][j] != -1) return dp[i][j];

        if (word1[i] == word2[j])
            return dp[i][j] = solve(i+1, j+1, word1, word2);

        int insert = 1 + solve(i, j+1, word1, word2);
        int dlete = 1 + solve(i+1, j, word1, word2);

        return dp[i][j] = min(insert, dlete);
    }
};

//Clase creada para implementar el metodo adaptativo de distancia, fue inspirado de 
class Solution2{
public:
    int adaptiva(string word1, string word2){
         if (word1.size()<word2.size())
         {
            string word=word1;
            word1=word2;
            word2=word;
         }
         int D=1;
         int t=solve(word1, word2,D);
         while (t==-1)
         {
            D=2*D;
            t=solve(word1, word2,D);
         }
         return t;
    }

    int solve(const string &word1, const string &word2,int D) {
        int n = word1.size();
        int m = word2.size();
        
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, INT_MAX));
        for (int i = 0; i <= min(D + 1, n); i++){
            dp[i][0] = i;
        }
        for (int j = 0; j <= min(D + 1, m); j++){
            dp[0][j] = j;
        }

        for (int s = 1; s <= m; s++)
        {
            for (int i = max(1, s - D); i < s; i++)
            {
                if (word1[i - 1] == word2[s - 1]){
                    dp[i][s] = dp[i - 1][s - 1];
                }
                else{
                    dp[i][s] = 1 + min(dp[i - 1][s], dp[i][s - 1]);
                }
            }
            for (int j = max(1, s - D); j < s; j++)
            {
                if (word1[s - 1] == word2[j - 1]){
                    dp[s][j] = dp[s - 1][j - 1];
                }
                else{
                    dp[s][j] = 1 + min(dp[s - 1][j], dp[s][j - 1]);
                }
            }
            if (word1[s - 1] == word2[s - 1]){
                dp[s][s] = dp[s - 1][s - 1];
            }
            else{
                dp[s][s] = 1 + min(dp[s - 1][s], dp[s][s - 1]);
            }
        }
        for (int s = m + 1; s <= n; s++)
        {
            for (int j = max(1, m - D); j <= m; j++)
            {
                if (j == max(1, m - D))
                    dp[s][j] = 1 + dp[s - 1][j];
                else
                {
                    if (word1[s - 1] == word2[j - 1]){
                        dp[s][j] = dp[s - 1][j - 1];
                    }
                    else{
                        dp[s][j] = 1 + min(dp[s - 1][j], dp[s][j - 1]);
                    }
                }
            }
        }

        if (dp[n][m] <= D)
        {
            return dp[n][m];
        }
        else
        {
          return -1;
        }


    }
};


string borrar(string word1,int d){
    int a = rand() % word1.size();
    for (int i = 0; i < d; i++)
    {
        word1.erase(a,1);
        a = rand() % word1.size();
    }
    return word1;
    
};
string insertar(string word1,int d){
    int a = rand() % word1.size();
    for (int i = 0; i < d; i++)
    {
        char ch = 'a' + rand() % 26;
        word1.insert(a, 1, ch);
        a = rand() % word1.size();
    }
    return word1;
    
};
string modificaciones(string word1,int d){
    for (int i = 0; i < d; i++)
    {
        int b= rand()%2;
        if (b==1)
        {
            word1=borrar(word1,1);
        }
        else
        {
            word1=insertar(word1,1);
        }
        
    }
    return word1;
    
};


int main()
{   
    cout<<"Distintas Palabras"<<endl;
    vector<int> lineass={10,50,100}; 
    for (int j = 0; j < 3; j++)
    {
        ifstream Libro1;
        ifstream Libro2;
        Libro1.open("71035-0.txt",ios::in); 
        Libro2.open("71039-0.txt",ios::in);
        cout<<"-------------------"<<lineass[j]<<"-------------------"<<endl;
        int lineas=lineass[j];
        ostringstream ss1;
        for (int i = 0; i < lineas; i++)
        {
            string palabra;
            getline(Libro1,palabra);
            ss1<<palabra<<" ";
        }
        string palabra1=ss1.str();
        ostringstream ss2;
        for (int i = 0; i < lineas; i++)
        {
            string palabra;
            getline(Libro2,palabra);
            ss2<<palabra<<" ";
        }
        string palabra2=ss2.str();

        int sum_c=0;
        int distancia1;
        for (int k = 0; k < 50; k++)
        {
            auto start_time = std::chrono::high_resolution_clock::now(); 
            Solution1 min1;
            distancia1=min1.clasica(palabra1,palabra2);
            auto end_time = std::chrono::high_resolution_clock::now();
            sum_c+=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        }

        int sum_a=0;
        int distancia2;
        for (int k = 0; k < 50; k++)
        {
            auto start_time2 = std::chrono::high_resolution_clock::now(); 
            Solution2 min2;
            distancia2=min2.adaptiva(palabra1,palabra2);
            auto end_time2 = std::chrono::high_resolution_clock::now();
            sum_a+=std::chrono::duration_cast<std::chrono::microseconds>(end_time2 - start_time2).count();
        }
        cout<<"Clasica"<<" ";
        cout<<"Adaptativa"<<endl;
        cout<<distancia1<<" ";
        cout<<distancia2<<endl;
        cout<<sum_c/50<<" microseconds"<<" ";
        cout<<sum_a/50<<" microseconds"<<endl;
        Libro1.close();
        Libro2.close();

    }
    cout<<"Igual Palabras"<<endl;
    for (int j = 0; j < 3; j++)
    {
        ifstream Libro1;
        ifstream Libro2;
        Libro1.open("71035-0.txt",ios::in); 
        cout<<"-------------------"<<lineass[j]<<"-------------------"<<endl;
        int lineas=lineass[j];
        ostringstream ss1;
        for (int i = 0; i < lineas; i++)
        {
            string palabra;
            getline(Libro1,palabra);
            ss1<<palabra<<" ";
        }
        string palabra1=ss1.str();

        int sum_c=0;
        int distancia1;
        for (int k = 0; k < 50; k++)
        {
            auto start_time = std::chrono::high_resolution_clock::now(); 
            Solution1 min1;
            distancia1=min1.clasica(palabra1,palabra1);
            auto end_time = std::chrono::high_resolution_clock::now();
            sum_c+=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        }

        int sum_a=0;
        int distancia2;
        for (int k = 0; k < 50; k++)
        {
            auto start_time2 = std::chrono::high_resolution_clock::now(); 
            Solution2 min2;
            distancia2=min2.adaptiva(palabra1,palabra1);
            auto end_time2 = std::chrono::high_resolution_clock::now();
            sum_a+=std::chrono::duration_cast<std::chrono::microseconds>(end_time2 - start_time2).count();
        }
        cout<<"Clasica"<<" ";
        cout<<"Adaptativa"<<endl;
        cout<<distancia1<<" ";
        cout<<distancia2<<endl;
        cout<<sum_c/50<<" microseconds"<<" ";
        cout<<sum_a/50<<" microseconds"<<endl;
        Libro1.close();

    }

    cout<<"Distintas Palabras de distintos tamaños"<<endl;
    vector<int> lineass1={10,50,100};
    vector<int> lineass2={5,10,15};  
    for (int j = 0; j < 3; j++)
    {
        ifstream Libro1;
        ifstream Libro2;
        Libro1.open("71035-0.txt",ios::in); 
        Libro2.open("71039-0.txt",ios::in);
        cout<<"-------------------"<<lineass1[j]<<" "<<lineass2[j]<<"-------------------"<<endl;
        int lineas1=lineass1[j];
        ostringstream ss1;
        for (int i = 0; i < lineas1; i++)
        {
            string palabra;
            getline(Libro1,palabra);
            ss1<<palabra<<" ";
        }
        string palabra1=ss1.str();
        ostringstream ss2;
        int lineas2=lineass2[j];
        for (int i = 0; i < lineas2; i++)
        {
            string palabra;
            getline(Libro2,palabra);
            ss2<<palabra<<" ";
        }
        string palabra2=ss2.str();

        int sum_c=0;
        int distancia1;
        for (int k = 0; k < 50; k++)
        {
            auto start_time = std::chrono::high_resolution_clock::now(); 
            Solution1 min1;
            distancia1=min1.clasica(palabra1,palabra2);
            auto end_time = std::chrono::high_resolution_clock::now();
            sum_c+=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        }

        int sum_a=0;
        int distancia2;
        for (int k = 0; k < 50; k++)
        {
            auto start_time2 = std::chrono::high_resolution_clock::now(); 
            Solution2 min2;
            distancia2=min2.adaptiva(palabra1,palabra2);
            auto end_time2 = std::chrono::high_resolution_clock::now();
            sum_a+=std::chrono::duration_cast<std::chrono::microseconds>(end_time2 - start_time2).count();
        }
        cout<<"Clasica"<<" ";
        cout<<"Adaptativa"<<endl;
        cout<<distancia1<<" ";
        cout<<distancia2<<endl;
        cout<<sum_c/50<<" microseconds"<<" ";
        cout<<sum_a/50<<" microseconds"<<endl;
        Libro1.close();
        Libro2.close();
    }
    cout<<"modificar palabras palabras"<<endl;
    vector<int> modif={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
    for (int j = 0; j < modif.size(); j++)
    {
        ifstream Libro1;
        ifstream Libro2;
        Libro1.open("71035-0.txt",ios::in); 
        Libro2.open("71035-0.txt",ios::in);
        cout<<"-------------------"<<modif[j]<<"-------------------"<<endl;
        ostringstream ss1;
        for (int i = 0; i < 10; i++)
        {
            string palabra;
            getline(Libro1,palabra);
            ss1<<palabra<<" ";
        }
        string palabra1=ss1.str();
        ostringstream ss2;
        for (int i = 0; i < 10; i++)
        {
            string palabra;
            getline(Libro2,palabra);
            ss2<<palabra<<" ";
        }
        string palabra2=ss2.str();
        palabra2=modificaciones(palabra2,modif[j]);
        int sum_c=0;
        int distancia1;
        for (int k = 0; k < 50; k++)
        {
            auto start_time = std::chrono::high_resolution_clock::now(); 
            Solution1 min1;
            distancia1=min1.clasica(palabra1,palabra2);
            auto end_time = std::chrono::high_resolution_clock::now();
            sum_c+=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        }

        int sum_a=0;
        int distancia2;
        for (int k = 0; k < 50; k++)
        {
            auto start_time2 = std::chrono::high_resolution_clock::now(); 
            Solution2 min2;
            distancia2=min2.adaptiva(palabra1,palabra2);
            auto end_time2 = std::chrono::high_resolution_clock::now();
            sum_a+=std::chrono::duration_cast<std::chrono::microseconds>(end_time2 - start_time2).count();
        }
        cout<<"Clasica"<<" ";
        cout<<"Adaptativa"<<endl;
        cout<<distancia1<<" ";
        cout<<distancia2<<endl;
        cout<<sum_c/50<<" microseconds"<<" ";
        cout<<sum_a/50<<" microseconds"<<endl;
        Libro1.close();
        Libro2.close();

    }




    return 0;
}

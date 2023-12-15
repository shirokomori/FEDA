#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>

using namespace std;

// Asignamiento de nodos a los autores
// Autores->nodos
map<string,int> asignacion( vector<string> &A){
    map<string,int> nodos;
    int j=0;

    for (int i = 0; i < A.size(); i++)
    {
        if (A[i]!="t")
        {
            if (!nodos.count(A[i]))
            {                
                nodos[A[i]]=j;
                j+=1;
            }
            
        }
        
    }
    return nodos;
}

//Cracion de diccionario para buscar autores segun el nodo
//Es necesario tener un diccionario de autores a nodos

map<int,string> asignacion_n( map<string,int> &A){
    map<int,string> nodos_n;
    for (pair<string, int> it : A) {
        nodos_n[it.second]=it.first;
    }
    return nodos_n;
}


//Creacion de lista de adyacencia 
//Se debe tener la informacion de los autores por publicacion y el diccionario de con los nodos correspondiente a cada autor

map<int,map<int,int>> lista_adyacencia(vector<string>A,map<string,int>& N){
    map<int,map<int,int>> lista_ady;
    for (int i = 0; i < N.size(); i++)
    {
        lista_ady[i];
    }

    vector<int> l;
    for (int i = 0; i < A.size(); i++)
    {
        if (A[i]!="t")
        {
            l.push_back(N[A[i]]);
        }
        else
        {
            if (l.size()>1)
            {
                for (int j = 0; j < l.size(); j++)
                {
                    for (int k = 0; k < l.size(); k++)
                    {
                        if (j!=k)
                        {
                            if (!lista_ady[l[j]].count(l[k]))
                            {                
                                lista_ady[l[j]][l[k]]=1;
                            }
                            else
                            {
                                lista_ady[l[j]][l[k]]+=1;
                            }
                        }
                    }
                    
                }
            }
            l.clear();
            
        }
        
    }
    return lista_ady;    
}


//Creacion Matriz de adyacencia CSR
//Se crea a partir de la lista de adyacencia
vector<vector<int>> Matriz_CSR(map<int,map<int,int>>&la){
    vector<int> V;
    vector<int> J;
    vector<int> I;
    int c=0;
    for (int i = 0; i < la.size(); i++)
    {
        int t_l=la[i].size();
        if (t_l>0)
        {
            I.push_back(c);
            for (pair<int, int> it : la[i])
            {
                J.push_back(it.first);
                V.push_back(it.second);
                c+=1;
            }
            
        }
        else
        {
            I.push_back(c);
        }
        
        
    }
    I.push_back(c);
    vector<vector<int>> M={V,J,I};
    return M;
}

//Colusion en lista de adyacencia
string colucion_lista(map<int,map<int,int>>&la){
    int par=0;
    int tot=0;
    for (int i = 0; i < la.size(); i++)
    {
        int t_l=la[i].size();
        if (t_l>0)
        {
            for (pair<int, int> it : la[i])
            {   
                tot+=1;
                if(it.first>i)
                {
                    if (it.second%2==0)
                    {
                        par+=1;
                    }
                }
                
            }
            
        }
        
    }
        
    int impar=tot-par;
    if (par>impar)
    {
        return "Hay colusiones";
    }
    else
    {
        return "No hay colusiones";
    }
}

//Colusion en matriz crs
string colucion_matriz(vector<int>&V,vector<int>&J,vector<int>&I){
    int par=0;
    int tot=0;
    for (int i = 1; i < I.size(); i++)
    {
        for (int j = I[i-1]; j < I[i]; j++)
        {
            if(J[j]>i-1)
            {
                tot+=1;
                if (V[j]%2==0)
                {
                    par+=1;    
                }
            }
            
        }
        
        
    }
    int impar=tot-par;
    if (par>impar)
    {
        return "Hay colusiones";
    }
    else
    {
        return "No hay colusiones";
    }
    
    
}

int main()
{
    //Lectura de archivo y creacion de vector para facilitar el trabajo
    ifstream info ;
    info.open("info.txt",ios::in);
     string autor;
    vector<string> autores;
    while (getline(info,autor))
    {
        autores.push_back(autor);
    }
    info.close();

    int rep=1;
    // Asignamiento de nodos a los autores
    // Autores->nodos
    int tim_autores=0;
    for (int i = 0; i < rep; i++)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        map<string,int> nodos=asignacion(autores);
        auto end_time = std::chrono::high_resolution_clock::now();
        tim_autores+=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    }
    cout<<"Asignamiento de nodos a los autores: "<<tim_autores/rep<<" seg"<<endl;
    
    //Calculo espacio de memoria
    map<string,int> nodos=asignacion(autores);
    cout<<"Espacio de memoria: "<<sizeof(nodos) + nodos.size() * sizeof(map<string,int>)<<" bytes"<<endl;

    // Asignamiento de autores a los nodos
    // nodos->autores
    int tim_autores_n=0;
    for (int i = 0; i < rep; i++)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        map<int,string> nodos_n=asignacion_n(nodos);
        auto end_time = std::chrono::high_resolution_clock::now();
        tim_autores_n+=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    }
    cout<<"Asignamiento de autores a los nodos: "<<tim_autores_n/rep<<" seg"<<endl;
    
    //Calculo espacio de memoria
    map<int,string> nodos_n=asignacion_n(nodos);
    cout<<"Espacio de memoria: "<<sizeof(nodos_n) + nodos_n.size() * sizeof(map<int,string>)<<" bytes"<<endl;

    //Lista de adyacencia
    int tim_lista=0;
    for (int i = 0; i < rep; i++)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        map<int,map<int,int>> lista_ady=lista_adyacencia(autores,nodos);
        auto end_time = std::chrono::high_resolution_clock::now();
        tim_lista+=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    }
    cout<<"Lista de adyacencia: "<<tim_lista/rep<<" seg"<<endl;
    
    //Calculo espacio de memoria
    map<int,map<int,int>> lista_ady=lista_adyacencia(autores,nodos);;
    cout<<"Espacio de memoria: "<<sizeof(lista_ady) + lista_ady.size() * sizeof(int)+lista_ady.size() * sizeof(map<int,int>)<<" bytes"<<endl;


    //Matriz de adyacencia CSR
    int tim_matriz=0;
    for (int i = 0; i < rep; i++)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        vector<vector<int>> m_crs=Matriz_CSR(lista_ady);
        auto end_time = std::chrono::high_resolution_clock::now();
        tim_matriz+=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    }
    cout<<"Matriz de adyacencia CSR: "<<tim_matriz/rep<<" seg"<<endl;
    
    //Calculo espacio de memoria
    vector<vector<int>> m_crs=Matriz_CSR(lista_ady);
    vector<int> V=m_crs[0];
    vector<int> J=m_crs[1];
    vector<int> I=m_crs[2];
    cout<<"Espacio de memoria: "<<sizeof(V) + V.size() * sizeof(int)+sizeof(I) + I.size() * sizeof(int)+sizeof(J) + J.size() * sizeof(int)<<" bytes"<<endl;

    //Colusion en Lista de adyasencia
    int tim_cl=0;
    for (int i = 0; i < rep; i++)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        string colusion_l=colucion_lista(lista_ady);
        auto end_time = std::chrono::high_resolution_clock::now();
        tim_cl+=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    }
    cout<<"Colusion en lista: "<<tim_cl/rep<<" seg"<<endl;
    
    //Calculo espacio de memoria
    string colusion_l=colucion_lista(lista_ady);
    cout<<"Espacio de memoria: "<<sizeof(colusion_l)<<" bytes"<<endl;


    //Colusion en Matriz de adyasencia
    int tim_cm=0;
    for (int i = 0; i < rep; i++)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        string colusion_m=colucion_matriz(V,J,I);
        auto end_time = std::chrono::high_resolution_clock::now();
        tim_cm+=std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    }
    cout<<"Colusion en Matriz: "<<tim_cm/rep<<" seg"<<endl;
    
    //Calculo espacio de memoria
    string colusion_m=colucion_matriz(V,J,I);
    cout<<"Espacio de memoria: "<<sizeof(colusion_m)<<" bytes"<<endl;
    cout<<colusion_m.size();




    return 0;
}


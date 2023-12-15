#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>

using namespace std;




int main()
{
    ifstream info ;
    info.open("info.txt",ios::in);

    string autor;
    vector<string> autores;
    while (getline(info,autor))
    {
        autores.push_back(autor);
    }

        

    info.close();

    // Asignamiento de nodos a los autores
    // Autores->nodos
    auto start_time = std::chrono::high_resolution_clock::now();
    map<string,int> nodos;
    int j=0;

    for (int i = 0; i < autores.size(); i++)
    {
        if (autores[i]!="t")
        {
            if (!nodos.count(autores[i]))
            {                
                nodos[autores[i]]=j;
                j+=1;
            }
            
        }
        
    }
    //cout<<"# de autores: "<<nodos.size()<<endl;
    auto end_time = std::chrono::high_resolution_clock::now();
    cout<<"Nodos: "<<std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count()<<" seg"<<endl;
    cout<<sizeof(nodos) + nodos.size() * sizeof(map<string,int>)<<" bytes"<<endl;

    // Nodos->Autores
    map<int,string> nodos_n;
    for (pair<string, int> it : nodos) {
        nodos_n[it.second]=it.first;
    }
    //cout<<"# de autores: "<<nodos_n.size()<<endl;

    // Lista de adyacencia
    auto start_time_l = std::chrono::high_resolution_clock::now();
    map<int,map<int,int>> lista_ady;
    for (int i = 0; i < 3371649; i++)
    {
        lista_ady[i];
    }

    //cout<<"tamano lista ady: "<<lista_ady.size()<<endl;

    vector<int> l;
    for (int i = 0; i < autores.size(); i++)
    {
        if (autores[i]!="t")
        {
            l.push_back(nodos[autores[i]]);
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
    //cout<<"tamano primer elemento de la lsita de ady:"<<lista_ady[0].size()<<endl;
    auto end_time_l = std::chrono::high_resolution_clock::now();
    cout<<"Lista: "<<std::chrono::duration_cast<std::chrono::microseconds>(end_time_l - start_time_l).count()<<" seg"<<endl;
    cout<<sizeof(lista_ady) + lista_ady.size() * sizeof(int)+lista_ady.size() * sizeof(map<int,int>)<<" bytes"<<endl;


    // Matriz de adyacencia CSR
    auto start_time_m = std::chrono::high_resolution_clock::now();
    vector<int> V;
    vector<int> J;
    vector<int> I;
    int c=0;
    for (int i = 0; i < lista_ady.size(); i++)
    {
        int t_l=lista_ady[i].size();
        if (t_l>0)
        {
            I.push_back(c);
            for (pair<int, int> it : lista_ady[i])
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
    //cout<<"tamano vector I"<<size(I)<<endl;
    auto end_time_m = std::chrono::high_resolution_clock::now();
    cout<<"Matriz: "<<std::chrono::duration_cast<std::chrono::microseconds>(end_time_m - start_time_m).count()<<" seg"<<endl;
    cout<<sizeof(V) + V.size() * sizeof(int)+sizeof(I) + I.size() * sizeof(int)+sizeof(J) + J.size() * sizeof(int)<<" bytes"<<endl;

    // Par Lista Adyacencia
    auto start_time_pl = std::chrono::high_resolution_clock::now();
    vector<pair<int,int>> colusion_l;
    for (int i = 0; i < lista_ady.size(); i++)
    {
        int t_l=lista_ady[i].size();
        if (t_l>0)
        {
            for (pair<int, int> it : lista_ady[i])
            {
                if(it.first>i)
                {
                    if (it.second%2==0)
                    {
                        pair<int,int> cl;
                        cl.first=i;
                        cl.second=it.first;
                        colusion_l.push_back(cl);
                    }
                }
                
            }
            
        }
        
    }
    //cout<<"Tamano colusion lista"<<colusion_l.size()<<endl;
    //pair<int,int> dd=colusion_l[0];
    //cout<<dd.first<<" "<<dd.second<<endl;
    auto end_time_pl = std::chrono::high_resolution_clock::now();
    cout<<"Colusion Lista: "<<std::chrono::duration_cast<std::chrono::microseconds>(end_time_pl - start_time_pl).count()<<" seg"<<endl;
    cout<<sizeof(colusion_l) + colusion_l.size() * sizeof(pair<int,int>)<<" bytes"<<endl;

    // Par matriz Adyacencia
    auto start_time_pm = std::chrono::high_resolution_clock::now();
    vector<pair<int,int>> colusion_m;
    for (int i = 1; i < I.size(); i++)
    {
        for (int j = I[i-1]; j < I[i]; j++)
        {
            if(J[j]>i-1)
            {
                if (V[j]%2==0)
                {
                    pair<int,int> cl;
                    cl.first=i-1;
                    cl.second=J[j];
                    colusion_m.push_back(cl);    
                }
            }
            
        }
        
        
    }
    //cout<<"Tamano colusion matriz"<<colusion_m.size()<<endl;
    //pair<int,int> dd_m=colusion_m[0];
    //cout<<dd_m.first<<" "<<dd_m.second<<endl;
    auto end_time_pm = std::chrono::high_resolution_clock::now();
    cout<<"Colusion Matriz: "<<std::chrono::duration_cast<std::chrono::microseconds>(end_time_pm - start_time_pm).count()<<" seg"<<endl;
    cout<<sizeof(colusion_m) + colusion_m.size() * sizeof(pair<int,int>)<<" bytes"<<endl;


    return 0;

}

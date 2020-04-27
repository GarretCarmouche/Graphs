//Garret Carmouche
//Section 1
//4/22/2020

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <list> 
#include <istream>
#include <string>
#include <new>
#include <iomanip>
#include <map>

using namespace std;

struct edge{
    struct node *one = NULL;
    struct node *two = NULL;
    int weight;
};

struct node{
    list<struct edge *> connections = {};
    char character;
};

int Display(list<struct node *> *graph){
    list<struct node *> :: iterator it;
    list<struct node *> :: iterator it1;
    graph->sort();
    cout<<"  ";
    for(it = graph->begin(); it != graph->end(); ++it){

        //Output row/column labels
        if(it != graph->begin())
            cout<<(*it)->character<< " ";
        else{
            for(it1 = graph->begin(); it1 != graph->end(); ++it1){
                cout<<(*it1)->character<<"   ";
            }
            cout<<endl;
            cout<<(*it)->character<< " ";
            
        }
        
        //Output weights
        for(it1 = graph->begin(); it1 != graph->end(); ++it1){
            list<struct edge *> :: iterator it2;
            bool found = false;
            cout<<setw(4)<<left;
            for(it2 = (*it1)->connections.begin(); it2 != (*it1)->connections.end(); it2++){
                if((*it2)->one->character == (*it1)->character && (*it2)->two->character == (*it)->character){
                    cout<<(*it2)->weight;
                    found = true;
                    break;
                }
                else if((*it2)->one->character == (*it)->character && (*it2)->two->character == (*it1)->character){
                    cout<<left<<(*it2)->weight;
                    found = true;
                    break;
                }
            }
            if(!found)
                cout<<"0 ";
        }
        cout<<endl;
    }
}

int Create_Graph(list<struct node *> *graph){
    string line;
    string lastline;
	ifstream infile;

    list<struct node *> :: iterator it;
	infile.open ("data.csv");
    while(1)
    {
        getline(infile,line);
        if(line.compare(lastline)== 0){
            break;
        }
        lastline = line;

        char char1 = line[0];
        char char2 = line[3];

        struct node *node1 = NULL;
        struct node *node2 = NULL;

        //Check first character in graph
        if((*graph).size() > 0)
            for(it = (*graph).begin(); it != (*graph).end(); ++it){
                if((*it)->character == char1){
                    node1 = *it;
                    break;
                }
            }
        if(node1 == NULL){
            //node1 = (struct node *)malloc(sizeof(struct node));
            node1 = new struct node;
            node1->connections = {};
            node1->character = char1;
            (*graph).push_front(node1);
        }

        //Check second character in graph
        if((*graph).size() > 0)
        for(it = (*graph).begin(); it != (*graph).end(); ++it){
            if((*it)->character == char2){
                node2 = *it;
                break;
            }
        }

        if(node2 == NULL){
            //node2 = (struct node *)malloc(sizeof(struct node));
            node2 = new struct node;
            node2->character = char2;
            (*graph).push_front(node2);
            
        }

        //Create edge
        string numString = line.substr(5,line.size());
        int weight = stoi(numString);

        struct edge *newEdge = new struct edge;
        //struct edge *newEdge = (struct edge *)malloc(sizeof(struct edge));
        newEdge->one = node1;
        newEdge->two = node2;
        newEdge->weight = weight;

        
        node1->connections.push_front(newEdge);
        node2->connections.push_front(newEdge);
        
    }
	infile.close();
    return 1;
}

int Shortest_Distance(list<struct node *> *graph, char nodeName){
    list<struct node *> :: iterator it;
    struct node * startNode = NULL;
    map<char, int> distances;
    list<struct node *> checkedNodes = {};
    list<struct node *> uncheckedNodes = {};

    for(it = graph->begin(); it != graph->end(); it++){
        if((*it)->character == nodeName){
            startNode = *it;
        }
        distances.insert(pair<char,int>((*it)->character,99999999));
        uncheckedNodes.push_back(*it);
    }

    if(startNode == NULL)
        return 0;

    distances.erase(startNode->character);
    distances.insert(pair<char,int>(startNode->character, 0));
    while(!uncheckedNodes.empty()){

        //Get node with shortest distance
        int shortestDistance = 199999999;
        struct node *shortestNode;
        for(it = uncheckedNodes.begin(); it != uncheckedNodes.end(); it++){
            int dist = distances.at((*it)->character);
            if(dist < shortestDistance){
                shortestDistance = dist;
                shortestNode = *it;
            }
        }
        uncheckedNodes.remove(shortestNode);

        checkedNodes.push_front(shortestNode);

        list<struct edge *> :: iterator it1;
        for(it1 = shortestNode->connections.begin(); it1 != shortestNode->connections.end(); it1++){
            
            if((*it1)->one->character == shortestNode->character){
                int checkWeight = (*it1)->weight + distances.at((*it1)->one->character);
                int ogWeight = distances.at((*it1)->two->character);
                if(checkWeight < ogWeight){
                    distances.erase((*it1)->two->character);
                    distances.insert(pair<char,int>((*it1)->two->character,checkWeight));
                }
            }
            else if((*it1)->two->character == shortestNode->character){
                int checkWeight = (*it1)->weight + distances.at((*it1)->two->character);
                int ogWeight = distances.at((*it1)->one->character);
                if(checkWeight < ogWeight){
                    distances.erase((*it1)->one->character);
                    distances.insert(pair<char,int>((*it1)->one->character,checkWeight));
                }
            }
        }
    }
    map<char,int> :: iterator it2;
    for(it2 = distances.begin(); it2 != distances.end(); it2++){
        cout<<nodeName<<":"<<it2->first<< " "<<it2->second<<endl;
    }
}

int main(){
    list<struct node *> graph = {};

    int option = 0;
    while(option != 4){
        cout<<"1: Build graph"<<endl;
        cout<<"2: Display graph"<<endl;
        cout<<"3: Find shortest path to node"<<endl;
        cout<<"4: End"<<endl;
        cin>>option;

        switch(option){
            case 1:
                Create_Graph(&graph);
                break;
            case 2:
                Display(&graph);
                break;
            case 3:
                char node;
                cout<<"Input source node"<<endl;
                cin>>node;
                Shortest_Distance(&graph,node);
                break;
        }
    }
    return 1;
}
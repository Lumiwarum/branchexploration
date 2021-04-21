#include <iostream>
#include <vector>
#include <memory>
using namespace std;
enum colors{
    white,
    gray,
    black,
    red
};
template <typename T>
class vertex {
public:
    int id;
    colors color=white;
    T storedObject;
    vertex()=default;
    vertex(T object){
        storedObject=object;
    }
    void replaceObject(T replaceTo){
        storedObject=replaceTo;
    }
    bool equalTo(vertex<T> compareWith){
        if (compareWith.storedObject==storedObject){ return true;}
        return false;
    }
};
template<typename T,typename W>
class edge{
public:
    vertex<T>* from;
    vertex<T>* to;
    W weight;
    edge(vertex<T>* from,vertex<T>* to,W weight){
        this->from=from;
        this->to=to;
        this->weight=weight;
    }
};

template<typename V,typename W>
class graphADT{
public:
virtual vertex<V>* addVertex(V value){};
virtual void removeVertex(vertex<V>* vertex){};
virtual edge<V,W>* addEdge(vertex<V>* from,vertex<V>* to,W weight){};
virtual void removeEdge(edge<V,W>* edge){};
virtual vector<edge<V,W>> edgesFrom(vertex<V>* Vertex){};
virtual vector<edge<V,W>> edgesTo(vertex<V>* Vertex){};
virtual vertex<V>* findVertex(V value){};
virtual edge<V,W>* findEdge(V valueFrom,V valueTo){};
virtual bool hasEdge(vertex<V>* from,vertex<V>* to){};
};

template<typename V,typename W>
class  adjacencyMatrixGraph: public graphADT<V,W>{
private:
    vector<vertex<V>> vertexes;
    vector<vector<edge<V,W>*>> matrix;
public:
    adjacencyMatrixGraph(){
    }
    void changeIds(){
        for (int i=0;i<vertexes.size();i++){
            vertexes[i].id=i;
        }
    }

    vertex<V>* addVertex(V value) override {
        vertexes.push_back(vertex<V>(value));
        for (int i=0;i<matrix.size();i++){
            matrix[i].push_back(nullptr);
        }
        vector<edge<V,W>*> vectorToAdd;
        for (int i = 0; i< vertexes.size();i++){
            vectorToAdd.push_back(nullptr);
        }
        matrix.push_back(vectorToAdd);
        changeIds();
    }

    void removeVertex(vertex<V>* vertex) override{
        int remove;
        for (int i=0;i<vertexes.size();i++){
            if (vertex->equalTo(vertexes[i])){
                remove=i;
            }
        }
        int n=matrix.size();
        matrix.erase(matrix.begin()+remove);
        for (int i=0;i<n-1;i++){
            matrix[i].erase(matrix[i].begin()+remove);
        }
        vertexes.erase(vertexes.begin()+remove);
        changeIds();
    }

    edge<V,W>* addEdge(vertex<V>* from,vertex<V>* to,W weight) override{
        auto edgeToAdd = new edge<V,W>(from,to,weight);
        matrix[from->id][to->id] = edgeToAdd;
        return matrix[from->id][to->id];
    }

    void removeEdge(edge<V,W>* edge) override {
        matrix[edge->from->id][edge->to->id]= nullptr;
        delete matrix[edge->from->id][edge->to->id];
    }

    vector<edge<V,W>> edgesFrom(vertex<V>* Vertex) override {
        vector<edge<V,W>> returnVector;
        for (int i=0;i<matrix[Vertex->id].size();i++){
            if (matrix[Vertex->id][i]!= nullptr){
                returnVector.push_back(*matrix[Vertex->id][i]);
            }
        }
        return returnVector;
    }

    vector<edge<V,W>> edgesTo(vertex<V>* Vertex) override {
        vector<edge<V,W>> returnVector;
        for (int i=0;i<matrix.size();i++){
            if (matrix[i][Vertex->id]!= nullptr){
                returnVector.push_back(*matrix[i][Vertex->id]);
            }
        }
        return returnVector;
    }

    vertex<V>* findVertex(V value) override {
        for (int i=0;i<vertexes.size();i++){
            if (vertexes[i].storedObject==value){
                return &vertexes[i];
            }
        }
        return nullptr;
    }

    edge<V,W>* findEdge(V valueFrom,V valueTo) override {
        vertex<V>* from=findVertex(valueFrom);
        vertex<V>* to=findVertex(valueTo);
        if ((from != nullptr)&&(to!= nullptr)){
            return matrix[from->id][to->id];
        }
        return nullptr;
    }

    bool hasEdge(vertex<V>* from,vertex<V>* to) override {
        bool hasEdge=false;
        if (matrix[from->id][to->id]!= nullptr){hasEdge= true;}
        return hasEdge;
    }
    void cycleColoring(vertex<V>* vertex){
        vertex->color=red;
        vector<edge<V,W>> edges=edgesFrom(vertex);
        for (int j=0;j<edges.size();j++){
            if (edges[j].to->color==gray){
               cycleColoring(edges[j].to);
                break;
            }
            if (edges[j].to->color==red){
                break;
            }
        }
        return;
    }

    bool DFS(vertex<V>* vertex){
        if(vertex->color!=black){
        vertex->color=gray;
        vector<edge<V,W>> edges=edgesFrom(vertex);
        for (int j=0;j<edges.size();j++){
            if (edges[j].to->color==gray){
               cycleColoring(edges[j].to);
                return  false ;
            }
            if (edges[j].to->color==white){
                if (!DFS(edges[j].to)){
                    return false;
                }
            }
        }
        vertex->color=black;
        }
        return true;
    }

    vector<vertex<V>> isAcyclic(){
        bool acyclic=true;
        W weight;
        for (int i=0;i<vertexes.size();i++){
            acyclic=DFS(&vertexes[i]);
            if (!acyclic){break;}
        }
        vector<vertex<V>> cycle;
        for (int i=0;i<vertexes.size();i++){
            if (vertexes[i].color==red){
                cycle.push_back(vertexes[i]);
            }
            vertexes[i].color=white;
        }
        return cycle;
    }
    void transpose(){
        edge<V,W>* buffer; vertex<V>* vertexBuffer;
        for (int i=1;i<matrix.size();i++){
            for (int j=0;j<matrix[i].size();j++){
                if (j<i){
                    buffer=matrix[i][j];
                    matrix[i][j]=matrix[j][i];
                    matrix[j][i]=buffer;
                    if (matrix[j][i]!= nullptr){
                    vertexBuffer= matrix[j][i]->from;
                    matrix[j][i]->from=matrix[j][i]->to;
                    matrix[j][i]->to=vertexBuffer;}
                    if (matrix[i][j]!= nullptr){
                        vertexBuffer= matrix[i][j]->from;
                        matrix[i][j]->from=matrix[i][j]->to;
                        matrix[i][j]->to=vertexBuffer;
                    }
                }
            }
        }
    }
};

vector<string> split(string str){
    int i=0;
    vector<string> vector;
    string stringToPush;
    while ((str[i]!=' ')&&(i<str.size())){
        stringToPush=stringToPush+str[i];
        i++;
    }
    i++;
    vector.push_back(stringToPush);
    stringToPush="";
    while ((str[i]!=' ')&&(i<str.size())){
        stringToPush=stringToPush+str[i];
        i++;
    }
    i++;
    vector.push_back(stringToPush);
    stringToPush="";
    while ((str[i]!=' ')&&(i<str.size())){
        stringToPush=stringToPush+str[i];
        i++;
    }
    i++;
    vector.push_back(stringToPush);
    stringToPush="";
    while ((str[i]!=' ')&&(i<str.size())){
        stringToPush=stringToPush+str[i];
        i++;
    }
    i++;
    vector.push_back(stringToPush);
    stringToPush="";
    return vector;
}

int main() {
    adjacencyMatrixGraph<string,int> matrix;
    string strIn;
    vector<string> command;
    vector<string> output;
    getline(cin,strIn);
    while (strIn!="1"){
        command=split(strIn);
        if (command[0]=="ADD_VERTEX"){
            matrix.addVertex(command[1]);
        }
        if (command[0]=="REMOVE_VERTEX"){
            matrix.removeVertex(matrix.findVertex(command[1]));
        }
        if (command[0]=="ADD_EDGE"){
            matrix.addEdge(matrix.findVertex(command[1]),matrix.findVertex(command[2]),stoi(command[3]));
        }
        if (command[0]=="REMOVE_EDGE"){
            matrix.removeEdge(matrix.findEdge(command[1],command[2]));
        }
        if (command[0]=="HAS_EDGE"){
            if (matrix.hasEdge(matrix.findVertex(command[1]),matrix.findVertex(command[2]))){
                output.push_back("TRUE");
            } else {
                output.push_back("FALSE");
            }
        }
        if (command[0]=="IS_ACYCLIC"){
            vector<vertex<string>> cycle=matrix.isAcyclic();
            if (cycle.size()==0){
                output.push_back("ACYCLIC");
            } else {
                string answer=" ";
                for (int i=0;i<cycle.size();i++){
                    answer=answer+cycle[i].storedObject+" ";
                }
                output.push_back(answer);
            }
        }
        if (command[0]=="TRANSPOSE"){
            matrix.transpose();
        }
        getline(cin,strIn);
    }
    for (int i=0;i<output.size();i++){
       cout <<output[i]<<endl;
    }
    return 0;
}
/*ADD_VERTEX S
ADD_VERTEX A
ADD_VERTEX B
ADD_VERTEX C
ADD_VERTEX D
ADD_VERTEX E
ADD_EDGE S A 5
ADD_EDGE A B 3
ADD_EDGE A C 8
ADD_EDGE B C 1
ADD_EDGE B D 4
ADD_EDGE B E 11
IS_ACYCLIC
ADD_EDGE E A 0
IS_ACYCLIC
TRANSPOSE
IS_ACYCLIC
1*/
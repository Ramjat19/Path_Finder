#include<bits/stdc++.h>
using namespace std;

class MetroStop;
class MetroLine;
class AVLNode;

vector<MetroLine*> lines;

class MetroStop {
private:
    string stopName;
    MetroStop* nextStop;
    MetroStop* prevStop;
    MetroLine* line;
    int fare;
public:
    MetroStop(string name, MetroLine* metroLine, int Fare) : stopName(name), nextStop(nullptr), prevStop(nullptr), line(metroLine), fare(Fare) {}

    string getStopName() const {return stopName;}
    MetroStop* getNextStop() const {return nextStop;}
    MetroStop* getPrevStop() const {return prevStop;}
    MetroLine* getLine() const {return line;}
    int getFare() const {return fare;}

    void setNextStop(MetroStop* next) {nextStop = next;}
    void setPrevStop(MetroStop* prev) {prevStop = prev;}
};

class MetroLine {
private:
    string lineName;
    MetroStop* node;
public:
    MetroLine(string name) : lineName(name), node(nullptr) {}

    string getLineName() const {return lineName;}
    MetroStop* getNode() const {return node;}

    void setNode(MetroStop* Node) {node = Node;}
    void populateLine(string filename){
        ifstream file(filename);

        string line;
        vector<MetroStop*> nodes;
        while(getline(file, line)){
            for(int i = line.size()-1; i >= 0; i--){
                if(line[i] == ',') line.pop_back();
                else break;
            }
            int fare = 0;
            string x = line;
            for(int i = line.size()-1; i >= 0; i--){
                if(line[i] != ' '){
                    int n = 1;
                    for(int j = 0; j < (line.size()-(i+1)); j++) n *= 10;
                    fare += n*(line[i]-'0');
                    x.pop_back();
                }
                else{
                    x.pop_back();
                    break;
                }
            }
            line = x;
            MetroStop* stop = new MetroStop(line, this, fare);
            nodes.push_back(stop);
        }
        for(int i = 0; i < nodes.size(); i++){
            if(i != nodes.size()-1) nodes[i]->setNextStop(nodes[i+1]);
            else nodes[i]->setNextStop(nullptr);
            if(i != 0) nodes[i]->setPrevStop(nodes[i-1]);
            else nodes[i]->setPrevStop(nullptr);
        }
        setNode(nodes[0]);

        file.close();
    }
    void printLine() const{
        MetroStop* stop = node;
        while (stop != nullptr) {
            cout << stop->getStopName() << endl;
            stop = stop->getNextStop();
        }
    }
    int getTotalStops() const{
        MetroStop* stop = node;
        int ans = 0;
        while(stop != nullptr){
            ans++;
            stop = stop->getNextStop();
        }
        return ans;
    }
};

class AVLNode {
private:
    string stopName;
    vector<MetroStop*> stops;
    AVLNode* left;
    AVLNode* right;
public:
    AVLNode(string name) : stopName(name), left(nullptr), right(nullptr) {}

    string getStopName() const {return stopName;}
    const vector<MetroStop*>& getStops() const {return stops;}
    AVLNode* getLeft() const {return left;}
    AVLNode* getRight() const {return right;}

    void addMetroStop(MetroStop* metroStop) {stops.push_back(metroStop);}
    void setLeft(AVLNode* left) {this->left = left;}
    void setRight(AVLNode* right) {this->right = right;}
};

class AVLTree {
private:
    AVLNode* root;
public:
    AVLTree() : root(nullptr) {}

    AVLNode* getRoot() const {return root;}
    void setRoot(AVLNode* root) {this->root = root;}

    int height(AVLNode* node){
        if(node == nullptr) return 0;
        return 1 + max(height(node->getLeft()), height(node->getRight()));
    }
    int balanceFactor(AVLNode* node) {return (height(node->getLeft()) - height(node->getRight()));}
    AVLNode* rotateLeft(AVLNode* node){
        AVLNode *x = node->getRight();
        AVLNode *y = x->getLeft();
        x->setLeft(node);
        node->setRight(y);
        return x;
    }
    AVLNode* rotateRight(AVLNode* node){
        AVLNode *x = node->getLeft();
        AVLNode *y = x->getRight();
        x->setRight(node);
        node->setLeft(y);
        return x;
    }
    AVLNode* balance(AVLNode* node){
        int x = balanceFactor(node);
        if(x > 1){
            if(balanceFactor(node->getLeft()) < 0) node->setLeft(rotateLeft(node->getLeft()));
            return rotateRight(node);
        }
        else if(x < -1){
            if(balanceFactor(node->getRight()) > 0) node->setRight(rotateRight(node->getRight()));
            return rotateLeft(node);
        }
        return node;
    }
    int stringCompare(string s1, string s2){
        if(s1 == s2) return 0;
        else{
            if(s1.size() == s2.size()){
                int n = s1.size();
                for(int i = 0; i < n; i++){
                    if(s1[i] != s2[i]){
                        if(s1[i] > s2[i]) return 1;
                        else return -1;
                    }
                }
            }
            else{
                if(s1.size() > s2.size()) return 1;
                else return -1;
            }
        }
        return 0;
    }
    AVLNode* insert(AVLNode* node, MetroStop* metroStop){
        if(node == nullptr){
            AVLNode* newNode = new AVLNode(metroStop->getStopName());
            newNode->addMetroStop(metroStop);
            return newNode;
        }
        string key = metroStop->getStopName(), nodeValue = node->getStopName();
        if(key == nodeValue) node->addMetroStop(metroStop);
        else{
            if(stringCompare(key, nodeValue) < 0) node->setLeft(insert(node->getLeft(), metroStop));
            else node->setRight(insert(node->getRight(), metroStop));
        }
        return balance(node);
    }
    void populateTree(MetroLine* metroLine){
        MetroStop* stop = metroLine->getNode();
        while(stop != nullptr){
            setRoot(insert(root, stop));
            stop = stop->getNextStop();
        }
    }
    void inOrderTraversal(AVLNode* node){
        if(node == nullptr) return;
        inOrderTraversal(node->getLeft());
        cout << node->getStopName() << endl;
        inOrderTraversal(node->getRight());
    }
    int getTotalNodes(AVLNode* node){
        if(node == nullptr) return 0;
        return 1 + getTotalNodes(node->getLeft()) + getTotalNodes(node->getRight());
    }
    AVLNode* searchStop(string stopName){
        if(root == nullptr) return nullptr;
        AVLNode* node = root;
        while(node != nullptr){
            string key = stopName, nodeValue = node->getStopName();
            if(key == nodeValue) return node;
            else{
                if(stringCompare(key, nodeValue) < 0) node = node->getLeft();
                else node = node->getRight();
            }
        }
        return nullptr;
    }
};

class Trip {
private:
    MetroStop* node;
    Trip* prev;
public:
    Trip(MetroStop* metroStop, Trip* previousTrip) : node(metroStop), prev(previousTrip) {}

    MetroStop* getNode() const {return node;}
    Trip* getPrev() const {return prev;}
};

class Exploration {
private:
    queue<Trip*> trips;
public:
    Exploration() {
        if(trips.empty()) return;
        else while(!trips.empty()) trips.pop();
    }
    queue<Trip*> getTrips() const {return trips;}

    void enqueue(Trip* trip) {trips.push(trip);}
    Trip* dequeue(){
        if (trips.empty()) return nullptr;
        Trip* trip = trips.front();
        trips.pop();
        return trip;
    }
    bool isEmpty() const {return trips.empty();}
};

class Path {
private:
    vector<MetroStop*> stops;
    int totalFare;
public:
    Path() : totalFare(0) {}

    vector<MetroStop*> getStops() const {return stops;}
    int getTotalFare() const {return totalFare;}

    void addStop(MetroStop* stop) {stops.push_back(stop);}
    void setTotalFare(int fare) {totalFare = fare;}

    void printPath() const {
        if(stops.empty()) cout << "No possible path" << endl;
        else for (auto stop : stops) cout << stop->getStopName() << endl;
    }
};

class PathFinder {
private:
    AVLTree* tree;
    vector<MetroLine*> Lines;
public:
    PathFinder(AVLTree* avlTree, const vector<MetroLine*>& metroLines) : tree(avlTree), Lines(metroLines) {}

    void createAVLTree(){
        for(MetroLine* i : Lines){
            if(tree->getRoot() == nullptr) tree->setRoot(new AVLNode(i->getNode()->getStopName()));
            tree->populateTree(i);
        }
    }
    Path* findPath(string origin, string destination){
        Exploration exploration;
        MetroStop* oStop = nullptr;
        MetroStop* dStop = nullptr;
        set<MetroLine*> traversedLines;
        for (MetroLine* line : Lines) {
            MetroStop* stop = line->getNode();
            while (stop != nullptr) {
                if (stop->getStopName() == origin) oStop = stop;
                if (stop->getStopName() == destination) dStop = stop;
                stop = stop->getNextStop();
            }
        }
        if(oStop == nullptr || dStop == nullptr) return nullptr;

        exploration.enqueue(new Trip(oStop, nullptr));
        set<MetroStop*> visitedStops;

        while(!exploration.isEmpty()){
            Trip* currentTrip = exploration.dequeue();
            MetroStop* currentNode = currentTrip->getNode();
            MetroLine* currentLine = currentNode->getLine();
            if(currentNode->getStopName() == destination){
                Path* path = new Path();
                Trip* trip = currentTrip;
                int fare = 0;
                while(trip->getNode()->getStopName() != origin){
                    path->addStop(trip->getNode());
                    if(trip->getNode()->getLine() == trip->getPrev()->getNode()->getLine())
                        fare += abs(trip->getNode()->getFare() - trip->getPrev()->getNode()->getFare());
                    else{
                        if(trip->getPrev()->getNode()->getStopName() == trip->getNode()->getPrevStop()->getStopName())
                            fare += abs(trip->getNode()->getFare() - trip->getNode()->getPrevStop()->getFare());
                        else fare += abs(trip->getNode()->getFare() - trip->getNode()->getNextStop()->getFare());
                    }
                    trip = trip->getPrev();
                }
                path->addStop(oStop);
                path->setTotalFare(fare);
                return path;
            }
            visitedStops.insert(currentNode);
            AVLNode* avlNode = tree->searchStop(currentNode->getStopName());
            if (avlNode != nullptr && avlNode->getStops().size() > 1){
                for (MetroStop* i : avlNode->getStops()){
                    MetroLine* line = i->getLine();
                    if (line != currentLine && traversedLines.find(line) == traversedLines.end()){
                        MetroStop* stop = line->getNode();
                        while(stop != nullptr){
                            if(stop->getStopName() == currentNode->getStopName()) break;
                            stop = stop->getNextStop();
                        }
                        if(visitedStops.find(stop->getNextStop()) == visitedStops.end() && stop->getNextStop() != nullptr){
                            exploration.enqueue(new Trip(stop->getNextStop(), currentTrip));
                            if(traversedLines.find(line) == traversedLines.end()) traversedLines.insert(line);
                        }
                        if(visitedStops.find(stop->getPrevStop()) == visitedStops.end() && stop->getPrevStop() != nullptr){
                            exploration.enqueue(new Trip(stop->getPrevStop(), currentTrip));
                            if(traversedLines.find(line) == traversedLines.end()) traversedLines.insert(line);
                        }
                    }
                }
            }
            MetroStop* nextStop = currentNode->getNextStop();
            MetroStop* prevStop = currentNode->getPrevStop();
            if(nextStop != nullptr && visitedStops.find(nextStop) == visitedStops.end()) exploration.enqueue(new Trip(nextStop, currentTrip));
            if(prevStop != nullptr && visitedStops.find(prevStop) == visitedStops.end()) exploration.enqueue(new Trip(prevStop, currentTrip));
        }
        return nullptr;
    }

    AVLTree* getTree() const {return tree;}
    const vector<MetroLine*>& getLines() const {return Lines;}
};
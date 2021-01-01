// CSE 178 Lab 1
// Alina Gutierrez
// AES encryption: encrypting a 16 char message using 128 bit key

#include<bits/stdc++.h>
#include<iostream>
#include<cmath>
#include<stdio.h>
#include<time.h>
using namespace std;
#define M 128
#define N 8
#define S 16
const int SIZE = 4;
const int ROUNDS = 9;

class Data{
    private:
        char **contents;
        std::bitset<M> data;
        std::bitset<M> *key;
        int **storage;
        std::bitset<N> **sBox;
        std::bitset<N> **hx;
        vector<vector<int>> mix;

    public:
        Data();
        ~Data();
        void setData(std::bitset<M> dat);
        void setContents();
        void printContents();
        void charToBit(char** message);
        void bitsToString();
        void bitsToHex();
        void bitsToInts();
        std::bitset<N> decToBit(int decimal);
        std::bitset<M> decToKey(int decimal);
        void insertHexTable(int row, int column, std::bitset<N> bit);
        void populateHexTable();
        void printHexTable();
        void printStorage();
        int** returnStorage();
        char** returnContents();
        void generateRoundKeys();
        void printRoundKeys();
        void generateSBox();
        void displaySBox();
        void byteSubstitution();
        void shiftRows();
        void shiftBy(int row, int shift); //   use in shift rows: need to perform circular shfit to left by 1, 2, and 3 for rows 2, 3, and 4 
        void createMixMatrix();
        void mixColumns(); //   matrix multiplication 
        void addRoundKey(int round);
        void runAES();
        void hexToData();
        void dataToChar();
};

void Data::setContents(){  
    cout << "\nEnter contents..."<< endl;
    for(int row = 0; row < SIZE; row++){
        // cout << "row complete..."<< endl;
        for(int column = 0; column < SIZE; column++){
           cin >> contents[row][column];
        }
    } 
    cout << "Content accepted..."<< endl;
}
void Data::printContents(){
    cout<< "\nprinting data..." << endl;
    for(int row = 0; row < SIZE; row++){
        for(int column = 0; column < SIZE; column++){
            cout << contents[row][column] << " ";
        }
        cout << "\t";
    }
    cout <<  endl;
}
void Data::printStorage(){
    cout<< "\nprinting storage..." << endl;
    for(int row = 0; row < SIZE; row++){
        for(int column = 0; column < SIZE; column++){
            cout << storage[row][column] << " ";
        }
    }
    cout << endl;
}
void Data::charToBit(char** message){
    cout<< "\nconverting chars to bit... ";
    std::bitset<M> foo;
    for(int row = 0; row < SIZE; row++){
        for(int column = 0; column < SIZE; column++){
            // cout << "foo: " << endl << foo << endl;
            std::bitset<M> temp(message[row][column]);
            // cout << "temp: "<< endl << temp << endl;
            foo <<= N;
            foo ^= temp;
        }
    }
    cout<< "bits converted..." << endl;
    setData(foo);
    foo.reset();
}
void Data::bitsToString(){
    cout << data << endl;
}
// std::string* Data::bitsToHex(){
void Data::bitsToHex(){
    std::bitset<M> divider(0b0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000011111111);
    std::bitset<M> foo;
    std::bitset<M> mask = data;
    std::stringstream **ss = new std::stringstream*[SIZE];
    // std::string *hx = new std::string[16];
    cout << "\nextracting bits... ";
    // cout << "storage contains... " << endl;
    // cout << "hex value\tdecimal value\n";
    for(int row = 0; row < SIZE; row++){
        ss[row] = new stringstream[SIZE];
        for(int column = 0; column < SIZE; column++){
            foo = mask&divider;
            // cout << data << endl << foo << endl << endl;
            storage[row][column] = foo.to_ullong();
            foo.reset();
            // ss[row][column]  << std::hex << storage[row][column];
            // hx[cycle] = ss[cycle].str();
            mask >>= N;
            // cout  << ss[row][column].str()<< "\t\t" << storage[row][column]  << endl;
        }
    }
    foo.reset();
    mask.reset();
    divider.reset();
    cout << "extraction complete..." << endl << endl;
}
void Data::bitsToInts(){
    for(int rows = 0; rows <SIZE; rows++){
        for(int columns = 0; columns <SIZE; columns++){
            storage[rows][columns]= hx[rows][columns].to_ullong();
        }
    }
}
std::bitset<N> Data::decToBit(int decimal){
    std::bitset<N> deci(decimal);
    return deci;
}
std::bitset<M> Data::decToKey(int decimal){
    std::bitset<M> deci(decimal);
    return deci;
}
void Data::insertHexTable(int row, int column, std::bitset<N> bit){
    hx[row][column] = bit;
}
void Data::populateHexTable(){
    int reorder = SIZE-1;
    for(int row = 0; row < SIZE; row++){
        for(int column = 0; column < SIZE; column++){
            insertHexTable(row, column, decToBit(storage[reorder-row][reorder-column]));
        }
    }
}
void Data::printHexTable(){
    cout << "\nprinting Hex Table..."<< endl;
    for(int row = 0; row < SIZE; row++){
        for(int column = 0; column < SIZE; column++){
            cout << hx[row][column] << " ";
        }
        cout << endl;
    }
    cout << endl << endl;
}
void Data::setData(std::bitset<M> dat){
    data = dat;
}
char** Data::returnContents(){
    return contents;
}
int ** Data::returnStorage(){
    return storage;
}
void Data::generateRoundKeys(){
    cout << "generating round keys... " << endl;
    srand (time(NULL));
    std::bitset<M> rKey;
    char generate [] = "abcabcdefghijklmnopqrstuvwxyz!@-*&._";
    char temp;
    for(int roundKey = 0; roundKey < ROUNDS; roundKey++){
        // int limit = int(pow(2.0,(M-1)));
        for(int element = 0; element < S; element++){
            rKey <<= N;
            temp = generate[rand()%33];
            std::bitset<M> mask(temp);
            rKey ^= mask;
        }
        key[roundKey] = rKey;
        rKey.reset();
    }
}
void Data::printRoundKeys(){
    for(int element = 0; element < ROUNDS; element++){
        cout<< ("round %d", element+1) << ":\t" << key[element] << endl;
    }
}
void Data::generateSBox(){
    cout<< "generating S-Box... ";
    srand (time(NULL));
    int sBit;
    for(int row = 0; row < S; row++){
        sBox[row] = new std::bitset<N>[S];
        for(int column = 0; column < S; column++){
            sBit = rand() % 255;
            sBox[row][column] = decToBit(sBit);
        }
    }
    cout << "S-Box generated..." << endl;
}
void Data::displaySBox(){
    cout << "printing sBox... " << endl;
    for(int row = 0; row < S; row++){
        for(int column = 0; column < S; column++){
            cout << sBox[row][column] << " ";
        }
        cout << endl;
    }
    cout << "sBox printed..." << endl;
}
void Data::byteSubstitution(){
    cout<< "begin byte substitution..." << endl;
    int sRow, sColumn;
    std::bitset<N> mask(15);
    for(int row = 0; row < SIZE; row++){
        for(int column = 0; column < SIZE; column++){
            sRow = int((hx[row][column]>>SIZE).to_ullong());
            sColumn = int((hx[row][column]&mask).to_ullong());
            // cout<< sRow << " " << sColumn << endl;
            // cout << hx[row][column] << " ";
            hx[row][column] = sBox[sRow][sColumn];
            // cout << hx[row][column] <<"\t"<< sBox[sRow][sColumn] << endl; 
        }
    }
    mask.reset();
}
void Data::shiftBy(int row, int shift){
    cout << "begin shift ... ";
    std::bitset<N> temp, temp2, temp3;
    // printHexTable();
    switch(shift){
        case 1:
            // cout<< "shifting row 1" << endl;
            temp = hx[row][0];
            for(int column = 0; column < SIZE-shift; column++){
                hx[row][column] = hx[row][column+shift];
            }
            hx[row][SIZE-shift] = temp;
            break;
        case 2:
            // cout<< "shifting row 2" << endl;
            temp= hx[row][0];
            temp2 = hx[row][1];
            for(int column = 0; column < SIZE-shift; column++){
                // temp = hx[row][column];
                hx[row][column] = hx[row][column+shift];
                // hx[row][SIZE-shift] = temp;
            }
            hx[row][SIZE-shift] = temp;
            hx[row][SIZE-shift+1] = temp2;
            break;
        case 3:
            // cout<< "shifting row 3" << endl;
            temp= hx[row][0];
            temp2 = hx[row][1];
            temp3 = hx[row][2];
            for(int column = 0; column < SIZE-shift; column++){
                hx[row][column] = hx[row][column+shift];
            }
            hx[row][SIZE-shift] = temp;
            hx[row][SIZE-shift+1] = temp2;
            hx[row][SIZE-shift+2] = temp3;
            break;
    }
    // printHexTable();
    temp.reset();
    temp2.reset();
    temp3.reset();
}
void Data::shiftRows(){
    for(int rows = 1; rows < SIZE; rows++){
        shiftBy(rows, rows);
    }
}
void Data::createMixMatrix(){
    mix =  {
        {2,3,1,1}, 
        {1,2,3,1},
        {1,1,2,3},
        {3,1,1,2}} ;
}
void Data::mixColumns(){
    cout << "mixing columns..." <<endl;
    createMixMatrix();
    bitsToInts();
    // printHexTable();

    for(int rows = 0; rows <SIZE; rows++){
        int sum = 0;
        for(int columns = 0; columns <SIZE; columns++){
            for(int complete = 0; complete < SIZE; complete++){
                sum += mix[rows][complete]*storage[complete][columns];
            }
           hx[rows][columns] = decToBit(sum);
        } 
    }
    // printHexTable();
}
void Data::addRoundKey(int round){
    cout << "adding round keys...";
    std::bitset<M> rKey = key[round];
    std::bitset<M> mask(15);
    std::bitset<M> foo;
    std::bitset<N> xAdd;
    int temp;
    
    for(int columns = 0; columns <SIZE; columns++){
        for(int rows = 0; rows <SIZE; rows++){
            foo = mask&rKey;
            temp = foo.to_ullong();
            xAdd = decToBit(temp);
            hx[rows][columns] ^= xAdd;
            foo.reset();
            xAdd.reset();
        }
    }
    rKey.reset();
    mask.reset();
    foo.reset();
    xAdd.reset();

}
void Data::hexToData(){
    std::bitset <M> foo;
    std::bitset<M> mask;
    for(int row = 0; row < SIZE; row++){
        for(int column = 0; column < SIZE; column++){
            foo <<= N;
            mask = decToKey(hx[row][column].to_ullong());
            foo ^= mask;
        }
    }
    setData(foo);
    foo.reset();
    mask.reset();
}
void Data::dataToChar(){
    for(int cycle = 0; cycle < S; cycle++){
        
    }
    // unsigned long i = mybits.to_ulong();
    // char c;
    // if (i <= CHAR_MAX) 
    //     c = static_cast<char>( i );

}
Data::Data(void){
    cout << "Object is being created... ";
    // setSize(SIZE);
    contents = new char*[SIZE];
    storage = new int*[SIZE];
    hx = new std::bitset<N>*[SIZE];
    key = new std::bitset<M>[ROUNDS];
    sBox = new std::bitset<N>*[S];
    // vector <std::bitset<N>> hx;
    for (int row = 0; row < SIZE; row++){
        contents[row] = new char[SIZE];
        hx[row] = new std::bitset<N>[SIZE];
        storage[row] = new int[SIZE];
    }
    cout << "Object complete..." << endl;
}
Data::~Data(void){
    cout<< endl << "data being deleted... ";
    delete [] contents, storage, key, hx, sBox;
    data.reset();
    cout<< "deleted complete..."<< endl;
}
void Data::runAES(){
    setContents();
    printContents();
    charToBit(contents);
    bitsToHex();
    populateHexTable();
    generateSBox();
    displaySBox();
    generateRoundKeys();
    for(int round = 0; round < 9; round++){
        cout << endl << endl << "Begining Round "<< (" %d", round+1)<< endl;
        printHexTable();
        byteSubstitution();
        shiftRows();
        if(round != 8){
            mixColumns();
        }
        addRoundKey(round);
    }
    hexToData();
}
int main(){

    // each char datatype has size 8 bits (1 byte)
    // with block size of 128 bits we can use 16 bytes --> 16 char
    // each message must have max length of 16 characters

    // BEGIN AES STEPS:
    //  we need a 4x4 matrix to store 16 characters
    // each entry in the matrix should be of size 1 byte (8 bits)
    // we will use the left and right most bits to map to the S-box
    // std::bitset<M> message;
    Data data;
    
    data.runAES();
    data.printContents();
    data.printHexTable();
    // message.reset();
    return 0;
}

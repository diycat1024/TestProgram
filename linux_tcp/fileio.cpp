#include <fstream>
#include <iostream>

void writefile()
{
    char data[100];
    std::ofstream outfile;
    outfile.open("text.txt", std::ios::app);
    std::cout << "Writing to file" << std::endl;
    std::cout << "enter your name:";
    std::cin.getline(data,100);
    outfile << data << std::endl;
    std::cout << "Enter you age:";
    std::cin >> data;
    std::cin.ignore();
    outfile << data << std::endl;
    outfile.close();
}

void readfile()
{
    char data[100];
    std::ifstream infile;
    infile.open("text.txt");
    std::cout << "reading file: ";
    infile >> data;
    std::cout << data << std::endl;
    infile >> data;
    std::cout << data << std::endl;
    infile.close();
}
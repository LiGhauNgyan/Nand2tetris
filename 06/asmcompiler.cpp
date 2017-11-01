#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int label_hash(string x,bool o){
	static string a[32771] = { "" };
	int hsh=0;
	for (int i = 0; i < x.size(); i++){
		hsh = (hsh << 8) % 32771;
		hsh += x[i];
	}
	hsh = hsh % 32771;
	if (o){
		for (; a[hsh] != ""&&a[hsh] != x; hsh = (hsh + 1) % 32771);
		a[hsh] = x;
	}
	else{
		for (; a[hsh] != ""&&a[hsh] != x; hsh = (hsh + 1) % 32771);
		if (a[hsh] == "")return -1;
	}
	return hsh;
}
int variable_hash(string x,bool o){
	static string a[32771] = { "" };
	int hsh = 0;
	for (int i = 0; i < x.size(); i++){
		hsh = (hsh << 8) % 32771;
		hsh += x[i];
	}
	hsh = hsh % 32771;
	if (o){
		for (; a[hsh] != ""&&a[hsh] != x; hsh = (hsh + 1) % 32771);
		a[hsh] = x;
	}
	else{
		for (; a[hsh] != ""&&a[hsh] != x; hsh = (hsh + 1) % 32771);
		if (a[hsh] == "")return -1;
	}
	return hsh;
}
bool isnum(string a){
	bool f = 1;
	for (int i = 0; i < a.size(); i++)f &= (a[i] >= '0'&&a[i] <= '9');
	return f;
}
string ainstr(string ai){//A instruction
	string as = "0000000000000000";
	ai.erase(0, 1);
	int adr = stoi(ai);
	for (int i = 1; i <= 15; i++){
		if (adr&(1 << (15 - i)))as[i] = '1';
	}
	return as;
}
string cinstr(string ci){//C instruction
	string cs = "1110000000000000";
	string::size_type semicolon;
	string::size_type equal_sign;
	equal_sign = ci.find("=");
	semicolon = ci.find(";");
	string jump;
	string comp;
	string dest;
	if (semicolon != ci.npos){
		jump = ci.substr(semicolon + 1);
		ci.erase(semicolon);
	}
	if (equal_sign != ci.npos){
		comp = ci.substr(equal_sign + 1);
		ci.erase(equal_sign);
		dest = ci;
	}
	else{
		comp = ci;
	}
	if (jump == "JMP")cs.replace(13, 3, "111");
	else if (jump == "JLE")cs.replace(13, 3, "110");
	else if (jump == "JNE")cs.replace(13, 3, "101");
	else if (jump == "JLT")cs.replace(13, 3, "100");
	else if (jump == "JGE")cs.replace(13, 3, "011");
	else if (jump == "JEQ")cs.replace(13, 3, "010");
	else if (jump == "JGT")cs.replace(13, 3, "001");
	else;
	if (dest == "AMD")cs.replace(10, 3, "111");
	else if (dest == "AD")cs.replace(10, 3, "110");
	else if (dest == "AM")cs.replace(10, 3, "101");
	else if (dest == "A")cs.replace(10, 3, "100");
	else if (dest == "MD")cs.replace(10, 3, "011");
	else if (dest == "D")cs.replace(10, 3, "010");
	else if (dest == "M")cs.replace(10, 3, "001");
	else;
	if (comp == "0")cs.replace(3, 7, "0101010");
	else if (comp == "1")cs.replace(3, 7, "0111111");
	else if (comp == "-1")cs.replace(3, 7, "0111010");
	else if (comp == "D")cs.replace(3, 7, "0001100");
	else if (comp == "A")cs.replace(3, 7, "0110000");
	else if (comp == "!D")cs.replace(3, 7, "0011101");
	else if (comp == "!A")cs.replace(3, 7, "0110001");
	else if (comp == "-D")cs.replace(3, 7, "0001111");
	else if (comp == "-A")cs.replace(3, 7, "0110011");
	else if (comp == "D+1")cs.replace(3, 7, "0011111");
	else if (comp == "A+1")cs.replace(3, 7, "0110111");
	else if (comp == "D-1")cs.replace(3, 7, "0001110");
	else if (comp == "A-1")cs.replace(3, 7, "0110010");
	else if (comp == "D+A")cs.replace(3, 7, "0000010");
	else if (comp == "D-A")cs.replace(3, 7, "0010011");
	else if (comp == "A-D")cs.replace(3, 7, "0000111");
	else if (comp == "D&A")cs.replace(3, 7, "0000000");
	else if (comp == "D|A")cs.replace(3, 7, "0010101");
	else if (comp == "M")cs.replace(3, 7, "1110000");
	else if (comp == "!M")cs.replace(3, 7, "1110001");
	else if (comp == "-M")cs.replace(3, 7, "1110011");
	else if (comp == "M+1")cs.replace(3, 7, "1110111");
	else if (comp == "M-1")cs.replace(3, 7, "1110010");
	else if (comp == "D+M")cs.replace(3, 7, "1000010");
	else if (comp == "D-M")cs.replace(3, 7, "1010011");
	else if (comp == "M-D")cs.replace(3, 7, "1000111");
	else if (comp == "D&M")cs.replace(3, 7, "1000000");
	else if (comp == "D|M")cs.replace(3, 7, "1010101");
	return cs;
}
int main()
{
	string filename;
	getline(cin, filename);
	ifstream fin1(filename + ".asm");
	if (!fin1)return 0;
	ofstream fout1(filename + "LL.asm");
	string line;
	string::size_type blank;
	string::size_type note;
	int lhash[32771], vhash[32771], lh, vh, ln = 0;
	memset(lhash, -1, 32771); memset(vhash, -1, 32771);
	for (getline(fin1, line); !fin1.eof(); getline(fin1, line)){
		note = line.find("/");
		if (note != line.npos)line.erase(note);
		for (blank = line.find(" "); blank != line.npos; blank = line.find(" "))line.erase(blank, 1);
		if (line == "")continue;
		if (line[0] == '('){
			line.erase(line.size() - 1);
			line.erase(0, 1);
			lh=label_hash(line,1);
			lhash[lh] = ln;
		}
		else { fout1 << line << endl; ln++; }
	}
	fin1.close(); fout1.close();//ß^žV¿Õ¸ñÅc×¢áŒ
	ifstream fin2(filename + "LL.asm");
	ofstream fout2(filename + "L.asm");
	ln = 0; int vn = 16;
	vhash[variable_hash("SP", 1)] = 0;
	vhash[variable_hash("LCL", 1)] = 1;
	vhash[variable_hash("ARG", 1)] = 2;
	vhash[variable_hash("THIS", 1)] = 3;
	vhash[variable_hash("THAT", 1)] = 4;
	vhash[variable_hash("SCREEN", 1)] = 16384;
	vhash[variable_hash("KBD", 1)] = 24576;
	for (int i = 0; i <= 15; i++){
		vh = variable_hash("R" + to_string(i), 1);
		vhash[vh] = i;
	}
	for (getline(fin2, line); !fin2.eof(); getline(fin2, line)){
		if (line[0] == '@'){
			fout2 << "@";
			line.erase(0, 1);
			if (isnum(line)) fout2 << line ;
			else if (label_hash(line, 0) > 0)fout2 << lhash[label_hash(line, 0)];
			else { 
				vh = variable_hash(line, 1); 
				if (vhash[vh]<0)vhash[vh] = vn++;
				fout2 << vhash[vh];
			}
			fout2 << endl;
		}
		else fout2 << line << endl;
	}
	fin2.close(); fout2.close();//ÞD“QÌøÞD˜ËÓ›Åc×ƒÁ¿Ãûžé”µ×Ö
	ifstream fin3(filename + "L.asm");
	ofstream fout3(filename + ".hack");
	for (getline(fin3, line); !fin3.eof(); getline(fin3, line)){
		if (line[0] == '@')fout3 << ainstr(line) << endl;
		else fout3 << cinstr(line) << endl;
	}
	fin3.close();fout3.close();//ÞD™CÆ÷´a
	return 0;
}
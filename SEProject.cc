#include <stdio.h> 
#include "tinyxml2.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <list>

using namespace tinyxml2;
using namespace std;

#ifndef XMLCheckResult
    #define XMLCheckResult(a_eResult) if(a_eResult != XML_SUCCESS){printf("Error: %i\n", a_eResult);return NULL;}
#endif

class TakenClass {
    private:
        string courseCode = "";
        string title = "";
        string grade = "";

    public: 
        TakenClass(string courseC, string t, string g) {
            courseCode = courseC;
            title = t;
            grade = g;
        }

        string toString() {
            return "CourseCode: " + courseCode + "; title: " + title + "; grade: " + grade;
        }
};

list<TakenClass> loadTranscriptFile(string filename) {
    list <TakenClass> classes = list<TakenClass>();
    // Load XML file 
    XMLDocument doc;
   
    XMLError err = doc.LoadFile(filename.c_str());
    
    if (err != XML_SUCCESS) {
        cout << "Error " << err << " Loading File";
        return classes; 
    }
    
    XMLNode *root = doc.FirstChildElement("Transcript");
    if (root == NULL) {
        cout << "Failed to Load Transcript File" << endl;
        return classes;
    }
    
    XMLNode* term = root->FirstChild();
    if (term == NULL) {
        cout << "Failed to Find Term Element" << endl;
        return classes;
    }

    // create list of classes to return
    // Populate all courses in class list for each term
    if (term != NULL) {
        do {
            XMLElement* courses = term->FirstChildElement()->NextSiblingElement();
            if (courses != NULL) {
                XMLElement* curClass = courses->FirstChildElement("class");
                // Populate all courses listed in a term
                if (curClass != NULL) {
                    do {
                        TakenClass tmpClass(curClass->FirstChildElement("courseCode")->GetText(), 
                                            curClass->FirstChildElement("title")->GetText(), 
                                            curClass->FirstChildElement("grade")->GetText());
                        classes.push_back(tmpClass);
                        curClass = curClass->NextSiblingElement("class");
                    } while(curClass != NULL);
                }
            }
            term = term->NextSiblingElement();
        }
        while(term != NULL);
    }
    return classes;

}

int main()
{  
    

    list<TakenClass> takenClasses = loadTranscriptFile("SEtranscript.xml");
    ofstream newfile("readout2.xml");
    for(list<TakenClass>::iterator it = takenClasses.begin(); 
        it != takenClasses.end(); ++it) {
            newfile<< it->toString() << endl;
    }
    newfile.close();

/*XMLDocument doc;
XMLError eResult = doc.LoadFile("SEtranscript.xml");
XMLCheckResult(eResult);
XMLNode *pRoot = doc.FirstChild();
if(pRoot == nullptr) return XML_ERROR_FILE_READ_ERROR;
XMLElement *term = pRoot->FirstChildElement("term");
if (term == nullptr) return XML_ERROR_PARSING_ELEMENT;
const char *text = (*term).GetText();
cout << text;
//const char *term = pElement->GetText();
static const char* xml = "<term>";
doc.Parse(xml);
//XMLElement* titleElement = doc.FirstChildElement("Term: Fall Term 2018")->FirstChildElement("Subject	course");
//const char *title = titleElement->GetText();
//doc.Print();
*/cout<<"Ran to end\n";
//eResult = doc.SaveFile("readout2.xml");
//XMLCheckResult(eResult);

return 0;
}

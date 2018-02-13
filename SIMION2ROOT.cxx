#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "TTree.h"
#include "TFile.h"

using namespace std;

TTree *paramTree = new TTree("SimionTree","Tree with parameters of ions");
char ChFile[100], delimiter;
TString fName;

void readSIMION(istream &SIMIONFile, vector<string> &variables, vector< vector<float> > &output, int &nvar, char delimiter)
{
   string SIMIONLine, dummy;

	getline(SIMIONFile, dummy);
	cout <<"Added header"<<endl;
	cout<< dummy.c_str()<<endl; 
	while (dummy.compare(0,6,"------") != 0){ // searching for ------ Begin Next Fly'm ------
		getline(SIMIONFile, dummy);
		cout<< dummy.c_str()<<endl;
	}
	cout <<"SIMION header"<<endl;
	for (int i=0; i<9; i++){
			getline(SIMIONFile, dummy);
			cout<< dummy.c_str()<<endl;
	}	


	getline(SIMIONFile, dummy);
	
	cout << dummy<<endl;
    istringstream ParamStream(dummy);
    

	string SIMIONElementStr;
	
	cout << "Simion parameters' names >>>>>"<<endl;
		nvar=0;
		while( getline(ParamStream, SIMIONElementStr, delimiter) )
		{
			if ((SIMIONElementStr.end()-SIMIONElementStr.begin())>10){
				cout<<"Something is wrong - probably wrong delimiter"<<endl<<flush;
				exit(0);
			}
			SIMIONElementStr.erase(std::remove(SIMIONElementStr.begin(), SIMIONElementStr.end(), '\"'), SIMIONElementStr.end());
			SIMIONElementStr.erase(std::remove(SIMIONElementStr.begin(), SIMIONElementStr.end(), ' '), SIMIONElementStr.end());
			nvar++;
			cout << SIMIONElementStr <<endl;
			variables.push_back(SIMIONElementStr);
		}
		cout<<"Names loaded "<<endl;

	getline(SIMIONFile, dummy); // next line is empty	

	float SIMIONElement;
	vector<float> SIMIONRaw;

    while( getline(SIMIONFile, SIMIONLine) )
    {
           istringstream SIMIONStream(SIMIONLine);
           //vector<float> SIMIONRaw;

            // read every element from the line that is seperated by commas
            // and put it into the vector or strings

            while( getline(SIMIONStream, dummy, delimiter) )
            {
					SIMIONElement = strtof((dummy).c_str(),0);
					//SIMIONElement = strtof(dummy);
					//cout<< SIMIONElement <<" ";

                    SIMIONRaw.push_back(SIMIONElement);
            }
            //cout<<endl;
            
            //cout <<"SIMIONRaw.size()="<<SIMIONRaw.size()<<endl;
            
      /*     
        for (int i = 0; i < SIMIONRaw.size(); i++) {
            cout << SIMIONRaw[i] << " ";
        }
        cout << endl;
        */
        
            output.push_back(SIMIONRaw);
            if (SIMIONRaw.size()>1) SIMIONRaw.clear();
    }
    output.pop_back(); // last vector element is empty
    cout<<"end of the readSIMION"<<endl;
    
}


int main(int argc, char** argv)
{
	
	if ((argc<2)||(argc>3)) { cout<<" The use of SIMION2ROOT: \nSIMION2ROOT <file path/file> <delimiter in simion file ex: ,>\n if delimiter not present it will be considered as TAB"<<endl; 
			return EXIT_FAILURE;
	}
	
	int nvar, cnt;
    string dummy;
    fstream file(argv[1], ios::in);
    
    strcpy(ChFile, argv[1]);
    if (argc==3) delimiter=argv[2][0];
    else delimiter = '\t';
    
    if (delimiter!='\t') cout <<"delimiter=\'"<<delimiter<<"\'"<<endl;
    else cout << "delimiter = TAB"<<endl;


    fName =  ChFile;
    fName.Append (".root");
    
    cout<<"File open: "<<fName<<endl;
    
    TFile *f = new TFile(fName, "recreate");

    if(!file.is_open())
    {
           cout << "File not found!\n";
            return 1;
    }

    // typedef to save typing for the following object
    typedef vector< vector<float> > SIMIONFloatVector;
    typedef vector<string> SIMIONStringVector;

    SIMIONFloatVector SIMIONData;
	SIMIONStringVector SIMIONOutputVariables;

    readSIMION(file, SIMIONOutputVariables, SIMIONData, nvar, delimiter);
    // make a root file
    
//cout <<"nvar="<<nvar<<endl;
    
	float var[nvar];
	//cout<<"here"<<flush<<endl;
	paramTree = new TTree("paramTree","Tree with parameters of ions");
	cnt=0;
	//cout<<"here";
//	for(vector<string>::iterator j = SIMIONOutputVariables.begin(); j != SIMIONOutputVariables.end(); ++j)
	float priv;
	for(int i=0; i<SIMIONOutputVariables.size(); i++)
    {
		//cout<<"SIMIONOutputVariables.size()="<<SIMIONOutputVariables.size()<<endl;
		dummy = SIMIONOutputVariables[i];
		paramTree -> Branch(const_cast<char*>(dummy.c_str()) , var+cnt , "var[1]/F");//Form("var[%i]/F", cnt)); //(var+cnt) ,"var[cnt]/F");
		cnt++;
	}

	
	
	//var[0]=120.; var[1]=1.0;var[2]=0.1;
	//		paramTree->Fill();
	//	var[0]=10.; var[5]=1.0;var[6]=1000.;
	//		paramTree->Fill();				
	//f->Save();
	//f->Write();
	//f->Close();

    //for( SIMIONFloatVector::iterator i = SIMIONData.begin(); i != SIMIONData.end(); ++i)
    for (int i=0; i<SIMIONData.size() ; i++)
    {
		cnt=0;

            //for(vector<float>::iterator j = i->begin(); j != i->end(); ++j)
            for (int j=0; j<SIMIONData[0].size(); j++)
            {
          		var[cnt]= SIMIONData[i][j];
          		//cout<<var[cnt]<<" ";
          		//cout<<"i="<<i<<", j="<<j<<endl;
				cnt++;
 			}
 		//cout<<endl;

		paramTree->Fill();

	}
	
f->Write();
f->Close();

}

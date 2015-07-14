#include <string>
#include <fstream>

#include <map>
#include <vector>

#include "ConfigFileParser.h"
#include "FileStream.h"
using namespace std;

void main()
{
	ConfigFileParser cConfig("JudgeSentence-config.ini", "=");

	string strTestPath = cConfig.GetValueAsString("IN_TestSen_FILE");
	string strOriPath = cConfig.GetValueAsString("IN_OriSen_FILE");
	string strResultPath = cConfig.GetValueAsString("OUT_Result_FILE");
	
	//CFileStream<string, string> CReadOri;
	//CFileStream<string, string> CReadTest;
	//CReadTest.ReadBFSFileData(strTestPath);
	//CReadOri.ReadBFSFileData(strTestPath);
	fstream fTest, fOri;
	fTest.open(strTestPath, ios_base::in);
		if (!fTest.is_open())
		{
			cout << "Test file error!" << endl;
			//文件读取失败，返回false
			system("pause");
		}

	fOri.open(strOriPath, ios_base::in);
		if (!fOri.is_open())
		{
			cout << "Ori file error!" << endl;
			//文件读取失败，返回false
			system("pause");
		}
	fstream fResult(strResultPath, ios_base::out);

	//读入句子编号及CRF分词内容
	string strLine;
	string strOriNum;
	string strTestNum;     
	string strOriSen;
	string strTestSen;

	vector<string> vecTestWord;
	vector<string> vecOriWord;
	
	double iAllWord = 0; 
	double iAllCorrectRec = 0;
	double iAllRec = 0;
	double doAllReCall = 0;
	double doAllPre = 0;
	double doAllF1 = 0;

	while(getline(fTest, strLine))
	{
		double doReCall = 0;
		double doPre = 0;
		double doF1 = 0;
		double iTotalWord = 0;
		double iCorrectRec = 0;
		double iRec = 0;

		StringSpliter cSplitTest(strLine, "	", false);
		strTestNum = cSplitTest.GetSubString(true);
		strTestSen = cSplitTest.GetSubString(false);
		
		vecTestWord.clear();
		vecOriWord.clear();

		strTestSen += " ";
		StringTokenizer cTokenTest(strTestSen, " ", false);
		while (cTokenTest.HasMoreTokens())
		{
			vecTestWord.push_back(cTokenTest.NextToken());
		}


		//get ori text
		getline(fOri, strLine);
		StringSpliter cSplit(strLine, "	", false);
		strOriNum = cSplit.GetSubString(true);
		strOriSen = cSplit.GetSubString(false);

		//句子的序号一致则比较
		if(strTestNum == strOriNum)
	
		{
			strOriSen += " ";
			StringTokenizer cToken(strOriSen, " ", false);
			while (cToken.HasMoreTokens())
			{
				vecOriWord.push_back(cToken.NextToken());
			}
			vector<string>::iterator itervecTest = vecTestWord.begin();
			vector<string>::iterator itervecTestTemp;
			vector<string>::iterator itervecOri = vecOriWord.begin();
			for(; itervecOri != vecOriWord.end(); itervecOri++)
			{
				iTotalWord++;
				string strOri = *itervecOri;
				
				if(itervecTest == vecTestWord.end());
					
				else
				{
				string strTest = *itervecTest;
				int nOrder = strOri.find("^");
				int nCiXing = strOri.rfind("/");
				strOri = strOri.substr(nOrder+1, nCiXing-nOrder-1);
				nOrder = strTest.find("^");
				strTest = strTest.substr(nOrder+1);
				

				string strTemp0;
				string strTemp1;
				string strTemp2;
				string strTemp3;

				//itervecTestTemp = itervecTest;
				if(itervecTest != vecTestWord.begin())//find before words
				{
					
					strTemp0 = *(itervecTest-1);
					nOrder = strTemp0.find("^");
					strTemp0 = strTemp0.substr(nOrder+1);
				}

				if(itervecTest != vecTestWord.begin()&&itervecTest-1 != vecTestWord.begin())//find before words
				{
					
					strTemp1 = *(itervecTest-2);
					nOrder = strTemp0.find("^");
					strTemp1 = strTemp1.substr(nOrder+1);
				}
				

				if(itervecTest+1 != vecTestWord.end())//find after words
				{
					
					strTemp2 = *(itervecTest+1);
					nOrder = strTemp1.find("^");
					strTemp2 = strTemp2.substr(nOrder+1);
				}
				
				
				if(itervecTest+1 != vecTestWord.end()&& itervecTest+2 != vecTestWord.end())//find after words
				{
					
					strTemp3 = *(itervecTest+2);
				nOrder = strTemp1.find("^");
					strTemp3 = strTemp3.substr(nOrder+1);
				}
				

				if(strTest == strOri)
					iCorrectRec++;
				else if(strOri == strTemp0)
					iCorrectRec++;
				else if(strOri == strTemp1)
					iCorrectRec++;
				else if(strOri == strTemp2)
					iCorrectRec++;
				else if(strOri == strTemp3)
					iCorrectRec++;
				iRec++;
				itervecTest++;
				}
				
			}
		}
		
		doReCall = iCorrectRec/iTotalWord;
		doPre = iCorrectRec/iRec;
		doF1 = 2*doReCall*doPre/(doReCall+doPre);
		fResult << strOriNum << "	" << "Recall:" << doReCall << "	Precision:" <<  doPre << "	F1:" << doF1 << endl;
		iAllWord += iTotalWord;
		iAllCorrectRec += iCorrectRec;
		iAllRec += iRec;	
	}
	doAllReCall = iAllCorrectRec/iAllWord;
	doAllPre = iAllCorrectRec/iAllRec;
	doAllF1 = 2*doAllReCall*doAllPre/(doAllReCall+doAllPre);
	fResult << "平均值"<< "	" << "Recall:" << doAllReCall << "	Precision:" <<  doAllPre << "	F1:" << doAllF1 << endl;
	
	fResult.close();
	fTest.close();
	fOri.close();

}
	



